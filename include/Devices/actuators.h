#ifndef ACTUATORS_GUARD
#define ACTUATORS_GUARD

#define UVA_RELAY_GPIO 4
#define UVB_RELAY_GPIO 5
#define PLANTS_RELAY_GPIO 6
#define DIMMER_GPIO 7
#define BUTTON_GPIO 16

/* Functions */
// BUTTON
void configButtonGPIO(void);

// Relays
void configRelayGPIO(void);
void switchUVARelay(const bool value);
void switchUVBRelay(const bool value);
void switchPlantsRelay(const bool value);

// Dimmer
void configDimmerGPIO(void);
void setPWM(const int pwmvalue);

#endif