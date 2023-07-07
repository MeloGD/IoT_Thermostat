#include "Devices/actuators.h"
#include "Arduino.h"

// Button
void configButtonGPIO(void) {
  pinMode(BUTTON_GPIO, INPUT_PULLUP);
}

// Relay
void configRelayGPIO(void) {
  pinMode(UVB_RELAY_GPIO, OUTPUT);
  pinMode(PLANTS_RELAY_GPIO, OUTPUT);
}

void switchUVBRelay(const bool value) {
  if (value == 1) {
    digitalWrite(UVB_RELAY_GPIO, HIGH);
  } else {
    digitalWrite(UVB_RELAY_GPIO, LOW);
  }
}

void switchPlantsRelay(const bool value) {
  if (value == 1) {
    digitalWrite(PLANTS_RELAY_GPIO, HIGH);
  } else {
    digitalWrite(PLANTS_RELAY_GPIO, LOW);
  }
}

void controlUVALight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute) {
  DateTime now = rtc.now();
  int up_time = atoi(on_hour) * 100 + atoi(on_minute);
  int current_time = (int)now.hour() * 100 + (int)now.minute();
  int down_time = atoi(off_hour) * 100 + atoi(off_minute);
    
  if( up_time < current_time && current_time < down_time) {  
    setPWM(1020);
    lv_obj_add_state(ui_uvaswitch, LV_STATE_CHECKED);
    
  } else {
    setPWM(1);
    lv_obj_clear_state(ui_uvaswitch, LV_STATE_CHECKED);
  }
  
}

void controlUVBLight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute) {
  DateTime now = rtc.now();
  int up_time = atoi(on_hour) * 100 + atoi(on_minute);
  int current_time = (int)now.hour() * 100 + (int)now.minute();
  int down_time = atoi(off_hour) * 100 + atoi(off_minute);
  
  if( up_time < current_time && current_time < down_time) {  
    switchUVBRelay(false);  
    lv_obj_add_state(ui_uvbswitch, LV_STATE_CHECKED);
  } else {
    switchUVBRelay(true);
    lv_obj_clear_state(ui_uvbswitch, LV_STATE_CHECKED);
  }
  
}

void controlPlantsLight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute) {
  DateTime now = rtc.now();
  int up_time = atoi(on_hour) * 100 + atoi(on_minute);
  int current_time = (int)now.hour() * 100 + (int)now.minute();
  int down_time = atoi(off_hour) * 100 + atoi(off_minute);
  
  if( up_time < current_time && current_time < down_time) {   
    switchPlantsRelay(false);
    lv_obj_add_state(ui_plantsswitch, LV_STATE_CHECKED);
  } else {
    switchPlantsRelay(true);
    lv_obj_clear_state(ui_plantsswitch, LV_STATE_CHECKED);
  }
}
// Dimmer
void configDimmerGPIO(void) {
  ledcSetup(1,60, 10);
  ledcAttachPin(DIMMER_GPIO, 1);
}

void setPWM(const int pwm_value) {
  ledcWrite(1, pwm_value);
}
