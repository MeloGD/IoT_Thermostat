#include "Devices/actuators.h"
#include "Arduino.h"

// Dimmer
void setPWM(const int pwmvalue) {
  analogWrite(DIMMER_PIN, pwmvalue);
}

// Relay
void switchUVARelay(const bool value) {
  if (value == 1) {
    digitalWrite(UVA_RELAY_PIN, HIGH);
  } else {
    digitalWrite(UVA_RELAY_PIN, LOW);
  }
}

void switchUVBRelay(const bool value) {
  if (value == 1) {
    digitalWrite(UVB_RELAY_PIN, HIGH);
  } else {
    digitalWrite(UVB_RELAY_PIN, LOW);
  }
}

void switchPlantsRelay(const bool value) {
  if (value == 1) {
    digitalWrite(PLANTS_RELAY_PIN, HIGH);
  } else {
    digitalWrite(PLANTS_RELAY_PIN, LOW);
  }
}
