#ifndef ACTUATORS_GUARD
#define ACTUATORS_GUARD

#define DIMMER_PIN 555555
#define UVA_RELAY_PIN 6
#define UVB_RELAY_PIN 4
#define PLANTS_RELAY_PIN 5

// Dimmer
void setPWM(const int pwmvalue);

// Relays
void switchUVARelay(const bool value);
void switchUVBRelay(const bool value);
void switchPlantsRelay(const bool value);


#endif