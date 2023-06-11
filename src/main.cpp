#include "UI/mcu_ui.h"
#include "RTOS/tasks.h"

TaskHandle_t lvglHandler = NULL;

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
  launchUi(); 
  xTaskCreatePinnedToCore(runUI ,"UITask",4096,NULL,1,&lvglHandler,1);
  // while screenloading == screen_actual
  while (lv_scr_act() == ui_loadingscreen) {
    wifi_data = getWiFiSSIDs();
    String ssid = readSSIDFlash();
    lv_label_set_text(ui_loadingscreenssid, ssid.c_str());
    delay(200);
    drawWiFiMenu(wifi_data);
    delay(100);
  }
  lv_obj_clean(ui_loadingscreen);
 
  createTasks();
  
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