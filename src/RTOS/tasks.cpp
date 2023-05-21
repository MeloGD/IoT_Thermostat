#include "RTOS/tasks.h"
#include "UI/mcu_ui.h"
#include "Devices/sensor.h"
#include "Devices/rtc.h"

TaskHandle_t updateBrightnessTaskHandler = NULL;
TaskHandle_t writeSensorsDataUITaskHandler = NULL;
TaskHandle_t runClockUITaskHandler = NULL;
TaskHandle_t lvglHandler = NULL;

// 
static SemaphoreHandle_t mutex = NULL;
static int lock;
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

//

static void runClockUI(void *args) {
  configRTC();
  char rtc_time[8];
  char format[] = "hh:mm";
  while (1) {
    strcpy(rtc_time, format);
    Serial.print("\nHora: ");
    Serial.print(reportTime().hour());
    Serial.print("\nMinutos: ");
    Serial.print(reportTime().minute());
    reportTime().toString(rtc_time);
    
    xSemaphoreTake(mutex, portMAX_DELAY);
    lv_label_set_text(ui_clock , rtc_time);
    xSemaphoreGive(mutex);
    
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

static void runUI (void *args) {
  while (1) {
    lv_timer_handler();
    vTaskDelay(5 / portTICK_RATE_MS);
  }
  
}

// 1 funciona con 2, 1 funciona con 3, pero 2 con 3 no
void createTasks(void) {
  mutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(runUI ,"UITask",4096,NULL,1,&lvglHandler,1);
  xTaskCreatePinnedToCore(updateScreenBrightnessTask,"UpdateScreenBrightnessTask",2048,NULL,2,&updateBrightnessTaskHandler,1);
  xTaskCreatePinnedToCore(writeSensorsDataUI,"UpdateDataUITask",2048,NULL,3,&writeSensorsDataUITaskHandler,1);
  xTaskCreatePinnedToCore(runClockUI,"RunClockUITask",2048,NULL,3,&runClockUITaskHandler,1);
  //vTaskStartScheduler();
}