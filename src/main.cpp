#include "UI/mcu_ui.h"
#include "RTOS/tasks.h"

void setup(void) {
  Serial.begin(115200);
  
  configButtonGPIO();
  configRelayGPIO();
  configDimmerGPIO();
  configSensorsGPIO();
  initFlashMemmory(); 
  configLVGLUI(); 
  initUITask();
  drawStartWiFiMenuScreen();
  createTasks();
  
  WiFi.onEvent(wifiEvent);
  configMQTT();
  connectWiFi();

  Serial.println( "Setup done" );
}


void loop() {
  vTaskDelete(NULL);
}