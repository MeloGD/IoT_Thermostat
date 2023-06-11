#include "UI/mcu_ui.h"
#include "RTOS/tasks.h"


// setup y main
void setup(void) {
  Serial.begin(115200);

  
  pinMode(8, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);  
  pinMode(9, INPUT);
  pinMode(46, INPUT);
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);
  
  initFlashMemmory(); 
  createTasks();
  launchUi(); 
  
  WiFi.onEvent(wifiEvent);
  wifi_data = getWiFiSSIDs();
  configMQTT();
  connectWiFi();
  

  Serial.println( "Setup done" );
}

//int value = 0;
void loop() {
  // put your main code here, to run repeatedly:

  //delay(15000);
}