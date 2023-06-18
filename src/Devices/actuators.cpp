#include "Devices/actuators.h"
#include "Arduino.h"

// Button
void configButtonGPIO(void) {
  pinMode(BUTTON_GPIO, INPUT_PULLUP);
}

// Relay
void configRelayGPIO(void) {
  pinMode(UVA_RELAY_GPIO, OUTPUT);
  pinMode(UVB_RELAY_GPIO, OUTPUT);
  pinMode(PLANTS_RELAY_GPIO, OUTPUT);
}

void switchUVARelay(const bool value) {
  if (value == 1) {
    digitalWrite(UVA_RELAY_GPIO, HIGH);
  } else {
    digitalWrite(UVA_RELAY_GPIO, LOW);
  }
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

// Dimmer
void configDimmerGPIO(void) {
  pinMode(DIMMER_GPIO, OUTPUT);
}

void setPWM(const int pwmvalue) {
  analogWrite(DIMMER_GPIO, pwmvalue);
}
