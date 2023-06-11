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
TaskHandle_t connectWiFiTaskHandler = NULL;
TaskHandle_t connectMQTTBrokerTaskHandler = NULL;
TaskHandle_t maintainMQTTTaskHandler = NULL;

// Mutex
static SemaphoreHandle_t mutex = NULL;
static SemaphoreHandle_t mutex2 = NULL;

// Array of arways containing their differents networks and their data
WifiScanData wifi_data;

/// Tasks ///
// MCU lvgl UI orchestator
static void runUI (void *args) {
  while (1) {
    lv_timer_handler();
    vTaskDelay(5 / portTICK_RATE_MS);
  }
}

// Brightness control
int last_state = HIGH;
int current_state;

static void updateScreenBrightnessTask(void *args) {
  while (1) {
    current_state = digitalRead(16);
    if (last_state == HIGH && current_state == LOW ) {
      setDisplayBrightness(0);
      last_state = LOW;
      //Serial.print("last_state = low");
    } else if (last_state == LOW && current_state == LOW) {
      //display.setBrightness(255);
      setDisplayBrightness(255);
      last_state = HIGH;
      //Serial.print("last_state = high");
    }
    vTaskDelay(140/portTICK_RATE_MS);
  }
}

// Write temps/humidity UI
static void writeSensorsDataUITask(void *args) {
  float value;
  char formatted_value[8];
  prepareSensors();
  while (1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    // Primer sensor DS18B20 - Cueva caliente
    dtostrf((double)readTemperatures(0) , 5 , 2 , formatted_value);
    lv_label_set_text(ui_warmtemp, formatted_value);
    // Segundo sensor DS18B20 - Cueva fría
    dtostrf((double)readTemperatures(1) , 5 , 2 , formatted_value);
    lv_label_set_text(ui_coldtemp , formatted_value);
    // Primer sensor AM2301 - Cueva húmeda
    dtostrf((double)readHumidHideTemp(), 5 , 2 , formatted_value);
    lv_label_set_text(ui_environmenttemp , formatted_value);
    dtostrf((double)readHumidHideHum(), 5 , 2 , formatted_value);
    lv_label_set_text(ui_environmentrelativehum , formatted_value); 
    // Segundo sensor AM2301 - Entorno
    dtostrf((double)readEnvironmentTemp() , 5 , 2 , formatted_value);
    lv_label_set_text(ui_moisttemp , formatted_value);
    dtostrf((double)readEnvironmentHum() , 5 , 2 , formatted_value);
    lv_label_set_text(ui_moistrelativehum , formatted_value);
    //Serial.print("Sensors");
    xSemaphoreGive(mutex);
    
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
    
    xSemaphoreTake(mutex, portMAX_DELAY);
    lv_label_set_text(ui_clock , rtc_time);
    xSemaphoreGive(mutex);
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

// Shows the difference betweeen, for example, "on hour" and "off hour"
void getDifference(char* result1, char* result2, const char* number1, const char* number2, const char* number3, const char* number4) {
    char hours_remaining[3], minutes_remaining[3];
    int hour_difference = 0, minute_difference = 0;
    // De array de chars a numeros
    int hour1 = atoi(number1);
    int minute_1 = atoi(number2);
    int hour2 = atoi(number3);
    int minute_2 = atoi(number4);
    // Calculamos la diferencia y reportamos el resultado con sprintf 
    if (hour1 == hour2) {
      if (minute_1 > minute_2) {
        hour_difference = 23;
        minute_difference = 60 - (minute_1 - minute_2);
      } else if (minute_1 == minute_2) {
        minute_difference = 0;
      } else {
        minute_difference = minute_2 - minute_1;
      }
    } else if (hour1 > hour2 ) {
      hour_difference = 24 - (hour1 - hour2);
      if (minute_1 > minute_2) {
        minute_difference = 60 - (minute_1 - minute_2);
      } else if (minute_1 == minute_2) {
        minute_difference = 0;
      } else {
        minute_difference = minute_2 - minute_1;
      }
    } else if (hour2 > hour1) {
      hour_difference =  hour2 - hour1;
      if (minute_1 > minute_2) {
        hour_difference--;
        minute_difference = 60 - (minute_1 - minute_2);
      } else if (minute_1 == minute_2) {
        minute_difference = 0;
      } else {
        minute_difference = minute_2 - minute_1;
      }
    }

    sprintf(result1, "%02d", hour_difference);
    sprintf(result2, "%02d", minute_difference);
};

static void updateActiveTimeUVATask(void *args) {
  char roller_option1[10], roller_option2[10], result1 [10], result2 [10];
  char hours1[3], mins1[3], hours2[3], mins2[3]; 

  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_screen2 != lv_scr_act() &&
        ui_screen4 != lv_scr_act() && ui_screen5 != lv_scr_act()) {
      xSemaphoreTake(mutex, portMAX_DELAY);
      lv_roller_get_selected_str(ui_onrollerscreen3, roller_option1, 10);
      lv_roller_get_selected_str(ui_offrollerscreen3, roller_option2, 10);

      hours1[0] = roller_option1[0];
      hours1[1] = roller_option1[1];
      hours2[0] = roller_option2[0];
      hours2[1] = roller_option2[1];
      mins1[0] = roller_option1[3];
      mins1[1] = roller_option1[4];
      mins2[0] = roller_option2[3];
      mins2[1] = roller_option2[4];

      getDifference(result1, result2, hours1, mins1, hours2, mins2);

      lv_label_set_text(ui_hours3 , result1);
      lv_label_set_text(ui_minutes3 , result2);
      xSemaphoreGive(mutex);
    }
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

static void updateActiveTimeUVBTask(void *args) {
  char roller_option1[10], roller_option2[10], result1 [10], result2 [10];
  char hours1[3], mins1[3], hours2[3], mins2[3]; 

  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_screen2 != lv_scr_act() &&
        ui_screen3 != lv_scr_act() && ui_screen5 != lv_scr_act()) {
      xSemaphoreTake(mutex, portMAX_DELAY);
      lv_roller_get_selected_str(ui_onrollerscreen4, roller_option1, 10);
      lv_roller_get_selected_str(ui_offrollerscreen4, roller_option2, 10);

      hours1[0] = roller_option1[0];
      hours1[1] = roller_option1[1];
      hours2[0] = roller_option2[0];
      hours2[1] = roller_option2[1];
      mins1[0] = roller_option1[3];
      mins1[1] = roller_option1[4];
      mins2[0] = roller_option2[3];
      mins2[1] = roller_option2[4];

      getDifference(result1, result2, hours1, mins1, hours2, mins2);

      lv_label_set_text(ui_hours4 , result1);
      lv_label_set_text(ui_minutes4 , result2);
      xSemaphoreGive(mutex);
    }
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

static void updateActiveTimePlantsTask(void *args) {
  char roller_option1[10], roller_option2[10], result1 [10], result2 [10];
  char hours1[3], mins1[3], hours2[3], mins2[3]; 

  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_screen2 != lv_scr_act() &&
        ui_screen4 != lv_scr_act() && ui_screen3 != lv_scr_act()) {
      xSemaphoreTake(mutex, portMAX_DELAY);
      lv_roller_get_selected_str(ui_onrollerscreen5, roller_option1, 10);
      lv_roller_get_selected_str(ui_offrollerscreen5, roller_option2, 10);

      hours1[0] = roller_option1[0];
      hours1[1] = roller_option1[1];
      hours2[0] = roller_option2[0];
      hours2[1] = roller_option2[1];
      mins1[0] = roller_option1[3];
      mins1[1] = roller_option1[4];
      mins2[0] = roller_option2[3];
      mins2[1] = roller_option2[4];

      getDifference(result1, result2, hours1, mins1, hours2, mins2);

      lv_label_set_text(ui_hours5 , result1);
      lv_label_set_text(ui_minutes5 , result2);
      xSemaphoreGive(mutex);
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
      xSemaphoreTake(mutex, portMAX_DELAY);
      value = lv_slider_get_value(ui_tempslider3);
      sprintf(max_temp_slider_value, "%02d", value);
      lv_label_set_text(ui_targettemp3, max_temp_slider_value);
      xSemaphoreGive(mutex);
    }
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

// Lights handling
static void controlLightsSystemTask(void *args) {
  digitalWrite(UVA_RELAY_PIN, LOW);
  digitalWrite(UVB_RELAY_PIN, LOW);
  digitalWrite(PLANTS_RELAY_PIN, LOW);
  while (1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    if (lv_obj_has_state(ui_uvaswitch, LV_STATE_CHECKED)) {
      switchUVARelay(0);
    } else {
      switchUVARelay(1);
    }
    if (lv_obj_has_state(ui_uvbswitch, LV_STATE_CHECKED)) {
      switchUVBRelay(0);
    } else {
      switchUVBRelay(1);
    }
    if (lv_obj_has_state(ui_plantsswitch, LV_STATE_CHECKED)) {
      switchPlantsRelay(0);
    } else {
      switchPlantsRelay(1);
    }
    xSemaphoreGive(mutex);

    vTaskDelay(100 / portTICK_RATE_MS);
  }
  
}

// WIFI Manager
static void wifiScannerTask(void *args) {
  while (1) {
    xSemaphoreTake(mutex2, portMAX_DELAY);
    wifi_data = getWiFiSSIDs();
    xSemaphoreGive(mutex2);
    vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

static void drawWiFiMenuTask(void *args) {
  while (1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    //xSemaphoreTake(mutex2, portMAX_DELAY);
    
    drawWiFiMenu(wifi_data);

    //xSemaphoreGive(mutex2);
    xSemaphoreGive(mutex);
    vTaskDelay(100 / portTICK_RATE_MS);
    }
}

static void connectWiFiMenuTask(void *args) {
  while (1) {
    xSemaphoreTake(mutex2, portMAX_DELAY);
    connectWiFi();
    xSemaphoreGive(mutex2);

    vTaskDelay(5000 / portTICK_RATE_MS);
  }
}

int value = 0;  
static void connectMQTTBrokerTask(void *args) {
  while (1) {
    //xSemaphoreTake(mutex2, portMAX_DELAY);
    publishMQTT(value);
    value += 1;
    //xSemaphoreGive(mutex2);
    vTaskDelay(5000 / portTICK_RATE_MS);
  }
  
}


void createTasks(void) {
  mutex = xSemaphoreCreateMutex();
  mutex2 = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(runUI ,"UITask",4096,NULL,1,&lvglHandler,1);
  xTaskCreatePinnedToCore(updateScreenBrightnessTask,"UpdateScreenBrightnessTask",2048,NULL,2,&updateBrightnessTaskHandler,1);
  xTaskCreatePinnedToCore(writeSensorsDataUITask,"UpdateDataUITask",2048,NULL,2,&writeSensorsDataUITaskHandler,1);
  xTaskCreatePinnedToCore(runClockUITask,"RunClockUITask",2048,NULL,3,&runClockUITaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimeUVATask,"UpdateActiveTimeUVA",2048,NULL,3,&updateActiveTimeUVATaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimeUVBTask,"UpdateActiveTimeUVB",2048,NULL,3,&updateActiveTimeUVBTaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimePlantsTask,"UpdateActiveTimePlants",2048,NULL,3,&updateActiveTimePlantsTaskHandler,1);
  xTaskCreatePinnedToCore(setMaxTemperatureUVATask,"SetMaxTempUVA",2048,NULL,3,&setMaxTemperatureUVATaskHandler,1);
  xTaskCreatePinnedToCore(controlLightsSystemTask,"ControlLightsSystem",2048,NULL,3,&lightSystemTaskHandler,1);
  //xTaskCreatePinnedToCore(drawWiFiMenuTask,"DrawWifiMenu",4096,NULL,4,&drawWiFiMenuTaskHandler,1);
  //xTaskCreatePinnedToCore(wifiScannerTask,"WifiScanner",4096,NULL,2,&wifiScannerTaskHandler,0);
  //xTaskCreatePinnedToCore(connectWiFiMenuTask,"ConnectWifi",4096,NULL,2,&connectWiFiTaskHandler,0);
  xTaskCreatePinnedToCore(connectMQTTBrokerTask,"ConnectMQTT",4096,NULL,3,&connectMQTTBrokerTaskHandler,0);
  
}