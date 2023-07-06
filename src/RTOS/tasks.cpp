#include "RTOS/tasks.h"

// Handlers
TaskHandle_t lvglHandler = NULL;
TaskHandle_t updateBrightnessTaskHandler = NULL;
TaskHandle_t writeSensorsDataUITaskHandler = NULL;
TaskHandle_t runClockUITaskHandler = NULL;
TaskHandle_t updateActiveTimeUVATaskHandler = NULL;
TaskHandle_t updateActiveTimeUVBTaskHandler = NULL;
TaskHandle_t updateActiveTimePlantsTaskHandler = NULL;
TaskHandle_t setMaxTemperatureUVATaskHandler = NULL;
TaskHandle_t lightSystemTaskHandler = NULL;
TaskHandle_t wifiScannerTaskHandler = NULL;
TaskHandle_t drawWiFiMenuTaskHandler = NULL;
TaskHandle_t publishMQTTBrokerTaskHandler = NULL;

/* Functions */
void initUITask(void) {
  xTaskCreatePinnedToCore(runUI ,"UITask",
                          4096,NULL,3,&lvglHandler,1);
}

void createTasks(void) {
  xTaskCreatePinnedToCore(updateScreenBrightnessTask, "UpdateScreenBrightnessTask",
                          2048,NULL,1,&updateBrightnessTaskHandler,1);
  xTaskCreatePinnedToCore(writeSensorsDataUITask,"UpdateDataUITask",
                          2048,NULL,3,&writeSensorsDataUITaskHandler,1);
  xTaskCreatePinnedToCore(runClockUITask,"RunClockUITask",
                          2048,NULL,3,&runClockUITaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimeUVATask,"UpdateActiveTimeUVA",
                          2048,NULL,3,&updateActiveTimeUVATaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimeUVBTask,"UpdateActiveTimeUVB",
                          2048,NULL,3,&updateActiveTimeUVBTaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimePlantsTask,"UpdateActiveTimePlants",
                          2048,NULL,3,&updateActiveTimePlantsTaskHandler,1);
  xTaskCreatePinnedToCore(setMaxTemperatureUVATask,"SetMaxTempUVA",
                          2048,NULL,3,&setMaxTemperatureUVATaskHandler,1);
  xTaskCreatePinnedToCore(controlLightsSystemTask,"ControlLightsSystem",
                          2048,NULL,3,&lightSystemTaskHandler,1);
  xTaskCreatePinnedToCore(publishMQTTBrokerTask,"ConnectMQTT",
                          4096,NULL,3,&publishMQTTBrokerTaskHandler,0);
  
}

// Calculates the reamaining time betweeen "on hour" and "off hour" 
void getTimeDifference(char* hours_remaining, char* minutes_remaining, 
                      const char* start_hour, const char* start_minute, 
                      const char* finish_hour, const char* finish_minute) {
    int hour_difference = 0, minute_difference = 0;
    int on_hour = atoi(start_hour);
    int on_minute = atoi(start_minute);
    int off_hour = atoi(finish_hour);
    int off_minute = atoi(finish_minute);
    
    if (on_hour == off_hour) {
      if (on_minute > off_minute) {
        hour_difference = 23;
        minute_difference = 60 - (on_minute - off_minute);
      } else if (on_minute == off_minute) {
        minute_difference = 0;
      } else {
        minute_difference = off_minute - on_minute;
      }
    } else if (on_hour > off_hour ) {
      hour_difference = 24 - (on_hour - off_hour);
      if (on_minute > off_minute) {
        minute_difference = 60 - (on_minute - off_minute);
      } else if (on_minute == off_minute) {
        minute_difference = 0;
      } else {
        minute_difference = off_minute - on_minute;
      }
    } else if (off_hour > on_hour) {
      hour_difference =  off_hour - on_hour;
      if (on_minute > off_minute) {
        hour_difference--;
        minute_difference = 60 - (on_minute - off_minute);
      } else if (on_minute == off_minute) {
        minute_difference = 0;
      } else {
        minute_difference = off_minute - on_minute;
      }
    }
    sprintf(hours_remaining, "%02d", hour_difference);
    sprintf(minutes_remaining, "%02d", minute_difference);
}

void getUIRollerTime(const char* roller_time, char* hour, char* minutes) {
  hour[0] = roller_time[0];
  hour[1] = roller_time[1];
  minutes[0] = roller_time[3];
  minutes[1] = roller_time[4];
}


/* Tasks */
static void runUI (void *args) {
  while (1) {
    lv_timer_handler();
    vTaskDelay(5 / portTICK_RATE_MS);
  }
}

// Brightness control
static void updateScreenBrightnessTask(void *args) {
  int last_state = HIGH;
  int current_state;
  while (1) {
    current_state = digitalRead(BUTTON_GPIO);
    if (last_state == HIGH && current_state == LOW ) {
      setDisplayBrightness(0);
      last_state = LOW;
    } else if (last_state == LOW && current_state == LOW) {
      setDisplayBrightness(255);
      last_state = HIGH;
    }
    vTaskDelay(140/portTICK_RATE_MS);
  }
}

// Read&Write temps/humidity UI
static void writeSensorsDataUITask(void *args) {
  float value;
  char formatted_value[8];
  prepareSensors();
  while (1) {
    warm_hide_temp = readTemperatures(0);
    cold_hide_temp = readTemperatures(1);
    environment_temp = readEnvironmentTemp();
    environment_hum = readEnvironmentHum();
    humid_hide_temp = readHumidHideTemp();
    humid_hide_hum = readHumidHideHum();
    // Primer sensor DS18B20 - Cueva caliente
    dtostrf(warm_hide_temp , 5 , 2 , formatted_value);
    lv_label_set_text(ui_warmtemp, formatted_value);
    // Segundo sensor DS18B20 - Cueva fría
    dtostrf(cold_hide_temp , 5 , 2 , formatted_value);
    lv_label_set_text(ui_coldtemp , formatted_value);
    // Primer sensor AM2301 - Cueva húmeda
    dtostrf(humid_hide_temp, 5 , 2 , formatted_value);
    lv_label_set_text(ui_moisttemp , formatted_value);
    dtostrf(humid_hide_hum, 5 , 2 , formatted_value);
    lv_label_set_text(ui_moistrelativehum , formatted_value); 
    // Segundo sensor AM2301 - Entorno
    dtostrf(environment_temp , 5 , 2 , formatted_value);
    lv_label_set_text( ui_environmenttemp, formatted_value);
    dtostrf(environment_hum, 5 , 2 , formatted_value);
    lv_label_set_text( ui_environmentrelativehum, formatted_value);
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

// Updates screens clock
static void runClockUITask(void *args) {
  configRTC();
  char rtc_time[8];
  char format[] = "hh:mm";
  while (1) {
    strcpy(rtc_time, format);
    reportTime().toString(rtc_time);
    lv_label_set_text(ui_clock , rtc_time);
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

static void updateActiveTimeUVATask(void *args) {
  char on_roller_uva[10], off_roller_uva[10] ;
  char hours_remaining [10], minutes_remaining [10];
  char on_hours[3], on_minutes[3], off_hours[3], off_minutes[3]; 
  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_screen2 != lv_scr_act() &&
        ui_screen4 != lv_scr_act() && ui_screen5 != lv_scr_act()) {
      
      lv_roller_get_selected_str(ui_onrollerscreen3, on_roller_uva, 10);
      lv_roller_get_selected_str(ui_offrollerscreen3, off_roller_uva, 10);
      
      getUIRollerTime(on_roller_uva, on_hours, on_minutes);
      getUIRollerTime(off_roller_uva, off_hours, off_minutes);

      getTimeDifference(hours_remaining, minutes_remaining, 
                        on_hours, on_minutes, 
                        off_hours, off_minutes);
      lv_label_set_text(ui_hours3 , hours_remaining);
      lv_label_set_text(ui_minutes3 , minutes_remaining);
    }

    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

static void updateActiveTimeUVBTask(void *args) {
  char on_roller_uvb[10], off_roller_uvb[10], hours_remaining [10];
  char minutes_remaining [10];
  char on_hour[3], on_minutes[3], off_hours[3], off_minutes[3]; 
  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_screen2 != lv_scr_act() &&
        ui_screen3 != lv_scr_act() && ui_screen5 != lv_scr_act()) {
      
      lv_roller_get_selected_str(ui_onrollerscreen4, on_roller_uvb, 10);
      lv_roller_get_selected_str(ui_offrollerscreen4, off_roller_uvb, 10);
      on_hour[0] = on_roller_uvb[0];
      on_hour[1] = on_roller_uvb[1];
      off_hours[0] = off_roller_uvb[0];
      off_hours[1] = off_roller_uvb[1];
      on_minutes[0] = on_roller_uvb[3];
      on_minutes[1] = on_roller_uvb[4];
      off_minutes[0] = off_roller_uvb[3];
      off_minutes[1] = off_roller_uvb[4];
      getTimeDifference(hours_remaining, minutes_remaining,
                        on_hour, on_minutes,
                        off_hours, off_minutes);
      lv_label_set_text(ui_hours4 , hours_remaining);
      lv_label_set_text(ui_minutes4 , minutes_remaining);
    }
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

static void updateActiveTimePlantsTask(void *args) {
  char on_roller_plants[10], off_roller_plants[10], hours_remaining [10];
  char minutes_remaining [10];
  char on_hour[3], on_minutes[3], off_hours[3], off_minutes[3]; 
  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_screen2 != lv_scr_act() &&
        ui_screen4 != lv_scr_act() && ui_screen3 != lv_scr_act()) {
    
      lv_roller_get_selected_str(ui_onrollerscreen5, on_roller_plants, 10);
      lv_roller_get_selected_str(ui_offrollerscreen5, off_roller_plants, 10);
      on_hour[0] = on_roller_plants[0];
      on_hour[1] = on_roller_plants[1];
      off_hours[0] = off_roller_plants[0];
      off_hours[1] = off_roller_plants[1];
      on_minutes[0] = on_roller_plants[3];
      on_minutes[1] = on_roller_plants[4];
      off_minutes[0] = off_roller_plants[3];
      off_minutes[1] = off_roller_plants[4];
      getTimeDifference(hours_remaining, minutes_remaining,
                        on_hour, on_minutes, 
                        off_hours, off_minutes);
      lv_label_set_text(ui_hours5 , hours_remaining);
      lv_label_set_text(ui_minutes5 , minutes_remaining);
    } 
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

// Sets the maximum temperature that the UVA bulb would target
static void setMaxTemperatureUVATask(void *args) {
  char max_temp_slider_value[8];
  int value;
  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_screen2 != lv_scr_act() &&
        ui_screen4 != lv_scr_act() && ui_screen5 != lv_scr_act()) { 
          
      value = lv_slider_get_value(ui_tempslider3);
      sprintf(max_temp_slider_value, "%02d", value);
      lv_label_set_text(ui_targettemp3, max_temp_slider_value);
    }
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

// Lights handling
static void controlLightsSystemTask(void *args) {
  char on_roller_uva[10], off_roller_uva[10] ;
  char on_roller_uvb[10], off_roller_uvb[10] ;
  char on_roller_plants[10], off_roller_plants[10] ;
  char on_hours_uva[3], on_minutes_uva[3], off_hours_uva[3], off_minutes_uva[3]; 
  char on_hours_uvb[3], on_minutes_uvb[3], off_hours_uvb[3], off_minutes_uvb[3]; 
  char on_hour_plants[3], on_minutes_plants[3], off_hours_plants[3], off_minutes_plants[3]; 
  digitalWrite(UVA_RELAY_GPIO, LOW);
  digitalWrite(UVB_RELAY_GPIO, LOW);
  digitalWrite(PLANTS_RELAY_GPIO, LOW);
  while (1) {
    lv_roller_get_selected_str(ui_onrollerscreen3, on_roller_uva, 10);
    lv_roller_get_selected_str(ui_offrollerscreen3, off_roller_uva, 10);
    getUIRollerTime(on_roller_uva, on_hours_uva, on_minutes_uva);
    getUIRollerTime(off_roller_uva, off_hours_uva, off_minutes_uva);
    controlUVALight(on_hours_uva, on_minutes_uva, off_hours_uva, off_minutes_uva);

    lv_roller_get_selected_str(ui_onrollerscreen4, on_roller_uvb, 10);
    lv_roller_get_selected_str(ui_offrollerscreen4, off_roller_uvb, 10);
    getUIRollerTime(on_roller_uvb, on_hours_uvb, on_minutes_uvb);
    getUIRollerTime(off_roller_uvb, off_hours_uvb, off_minutes_uvb);
    controlUVBLight(on_hours_uvb, on_minutes_uvb, off_hours_uvb, off_minutes_uvb);
    
    lv_roller_get_selected_str(ui_onrollerscreen5, on_roller_plants, 10);
    lv_roller_get_selected_str(ui_offrollerscreen5, off_roller_plants, 10);
    getUIRollerTime(on_roller_plants, on_hour_plants, on_minutes_plants);
    getUIRollerTime(off_roller_plants, off_hours_plants, off_minutes_plants);
    controlPlantsLight(on_hour_plants, on_minutes_plants, off_hours_plants, off_minutes_plants);    

    vTaskDelay(1000 / portTICK_RATE_MS);
  }
  
}

// WIFI Manager
static void wifiScannerTask(void *args) {
  while (1) {
    wifi_data = getWiFiSSIDs();
    vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

static void drawWiFiMenuTask(void *args) {
  while (1) {
    drawWiFiMenu(wifi_data);
    vTaskDelay(100 / portTICK_RATE_MS);
    }
}

static void publishMQTTBrokerTask(void *args) {
  while (1) {
    String publish_data = "";
    StaticJsonDocument<300> json_data;
    json_data["warm_hide_temp"] = warm_hide_temp;
    json_data["cold_hide_temp"] = cold_hide_temp;  
    json_data["environment_temp"] = environment_temp;
    json_data["environment_humidity"] = environment_hum;
    json_data["humid_hide_temp"] = humid_hide_temp;
    json_data["humid_hide_humidity"] = humid_hide_hum;
    serializeJson(json_data , publish_data);
    mqtt_client.publish("iot-esp32/tfg", 0, true, (char*)publish_data.c_str());
    vTaskDelay(5000 / portTICK_RATE_MS);
  }
}