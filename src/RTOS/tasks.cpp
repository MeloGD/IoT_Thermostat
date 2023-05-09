#include "RTOS/tasks.h"
#include "UI/mcu_ui.h"
#include "Devices/sensor.h"

TaskHandle_t updateBrightnessTaskHandler = NULL;
TaskHandle_t writeTempUITaskHandler = NULL;

//
int last_state = HIGH;
int current_state;

static void updateScreenBrightnessTask(void *pv_parameters) {
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

//
SensorDS18B20 warm_hide_sensor(8);
SensorDS18B20 cold_hide_sensor(8);

static void writeTemperatureUI(void *args) {
  float value;
  char tempvalue[5];
  while (1) {
    value = warm_hide_sensor.readTemp(0);
    dtostrf(value,5,2,tempvalue);
    lv_label_set_text(ui_warmtemp, tempvalue);
    value = cold_hide_sensor.readTemp(1);
    dtostrf(value,5,2,tempvalue);
    lv_label_set_text(ui_coldtemp, tempvalue);
    vTaskDelay(100/portTICK_RATE_MS);
  }
}


static void  update1(void *args) {
  while (1) {
    Serial.print("Update1");
    vTaskDelay(100/portTICK_RATE_MS);
  }
} 
static void  update2(void *args) {
  while (1) {
    Serial.print("Update2");
    vTaskDelay(100/portTICK_RATE_MS);
  }
} 


// 1 funciona con 2, 1 funciona con 3, pero 2 con 3 no
void createTasks(void) {
  xTaskCreate(updateScreenBrightnessTask,"UpdateScreenBrightnessTask",2048,NULL,1,&updateBrightnessTaskHandler);
  xTaskCreate(writeTemperatureUI,"UpdateWarhHideTemperatureTask",2048,NULL,2,&writeTempUITaskHandler);
  //xTaskCreate(updateColdHideTemperature,"UpdateColdHideTemperatureTask",2048,NULL,3,&updateColdTempTaskHandler);
  xTaskCreate(update1,"Update1",2048,NULL,2,&writeTempUITaskHandler);
  xTaskCreate(update2,"Update2",2048,NULL,2,&writeTempUITaskHandler);
}