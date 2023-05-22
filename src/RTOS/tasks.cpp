#include "RTOS/tasks.h"
#include "UI/mcu_ui.h"
#include "Devices/sensor.h"
#include "Devices/rtc.h"

// Handlers
TaskHandle_t lvglHandler = NULL;
TaskHandle_t updateBrightnessTaskHandler = NULL;
TaskHandle_t writeSensorsDataUITaskHandler = NULL;
TaskHandle_t runClockUITaskHandler = NULL;
TaskHandle_t updateActiveTimeUVATaskHandler = NULL;
TaskHandle_t updateActiveTimeUVBTaskHandler = NULL;
TaskHandle_t updateActiveTimePlantsTaskHandler = NULL;
TaskHandle_t setMaxTemperatureUVATaskHandler = NULL;

// Mutex
static SemaphoreHandle_t mutex = NULL;
static int lock;

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
static void writeSensorsDataUI(void *args) {
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

static void runClockUI(void *args) {
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

void getDifference(char* result, const char* num1, const char* num2) {
    // De array de chars a numeros
    int number1 = atoi(num1);
    int number2 = atoi(num2);
    // Calculamos la diferencia y reportamos el resultado con sprintf 
    int difference = number1 - number2;
    if (difference < 0) {
      difference *= -1;
    }
    sprintf(result, "%02d", difference);
};

static void updateActiveTimeUVA(void *args) {
  char roller_option1[10], roller_option2[10], result1 [10], result2 [10];
  char hours1[3], mins1[3], hours2[3], mins2[3]; 

  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_Screen2 != lv_scr_act() &&
        ui_Screen4 != lv_scr_act() && ui_Screen5 != lv_scr_act()) {
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

      getDifference(result1, hours1, hours2);
      getDifference(result2, mins1, mins2);

      lv_label_set_text(ui_hours3 , result1);
      lv_label_set_text(ui_minutes3 , result2);
      xSemaphoreGive(mutex);
    }
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

static void updateActiveTimeUVB(void *args) {
  char roller_option1[10], roller_option2[10], result1 [10], result2 [10];
  char hours1[3], mins1[3], hours2[3], mins2[3]; 

  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_Screen2 != lv_scr_act() &&
        ui_Screen3 != lv_scr_act() && ui_Screen5 != lv_scr_act()) {
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

      getDifference(result1, hours1, hours2);
      getDifference(result2, mins1, mins2);

      lv_label_set_text(ui_hours4 , result1);
      lv_label_set_text(ui_minutes4 , result2);
      xSemaphoreGive(mutex);
    }
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

static void updateActiveTimePlants(void *args) {
  char roller_option1[10], roller_option2[10], result1 [10], result2 [10];
  char hours1[3], mins1[3], hours2[3], mins2[3]; 

  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_Screen2 != lv_scr_act() &&
        ui_Screen4 != lv_scr_act() && ui_Screen3 != lv_scr_act()) {
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

      getDifference(result1, hours1, hours2);
      getDifference(result2, mins1, mins2);

      lv_label_set_text(ui_hours5 , result1);
      lv_label_set_text(ui_minutes5 , result2);
      xSemaphoreGive(mutex);
    }
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

// Sets the maximum temperature that the UVA bulb would target
static void setMaxTemperatureUVA(void *args) {
  char max_temp_slider_value[8];
  int value;
  while(1) {
    if (ui_screen1 != lv_scr_act() && ui_Screen2 != lv_scr_act() &&
        ui_Screen4 != lv_scr_act() && ui_Screen5 != lv_scr_act()) { 
      xSemaphoreTake(mutex, portMAX_DELAY);
      value = lv_slider_get_value(ui_tempslider3);
      sprintf(max_temp_slider_value, "%02d", value);
      lv_label_set_text(ui_targettemp3, max_temp_slider_value);
      xSemaphoreGive(mutex);
    }
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}
// 1 funciona con 2, 1 funciona con 3, pero 2 con 3 no
void createTasks(void) {
  mutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(runUI ,"UITask",4096,NULL,1,&lvglHandler,1);
  xTaskCreatePinnedToCore(updateScreenBrightnessTask,"UpdateScreenBrightnessTask",2048,NULL,2,&updateBrightnessTaskHandler,1);
  xTaskCreatePinnedToCore(writeSensorsDataUI,"UpdateDataUITask",2048,NULL,2,&writeSensorsDataUITaskHandler,1);
  xTaskCreatePinnedToCore(runClockUI,"RunClockUITask",2048,NULL,3,&runClockUITaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimeUVA,"UpdateActiveTimeUVA",2048,NULL,3,&updateActiveTimeUVATaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimeUVB,"UpdateActiveTimeUVB",2048,NULL,3,&updateActiveTimeUVBTaskHandler,1);
  xTaskCreatePinnedToCore(updateActiveTimePlants,"UpdateActiveTimePlants",2048,NULL,3,&updateActiveTimePlantsTaskHandler,1);
  xTaskCreatePinnedToCore(setMaxTemperatureUVA,"SetMaxTempUVA",2048,NULL,3,&setMaxTemperatureUVATaskHandler,1);
  //vTaskStartScheduler();
}