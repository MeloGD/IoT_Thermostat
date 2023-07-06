#ifndef ACTUATORS_GUARD
#define ACTUATORS_GUARD

#include "Devices/rtc.h"
#include "Devices/sensor.h"
#include "UI/mcu_ui.h"

#define UVA_RELAY_GPIO 5
#define UVB_RELAY_GPIO 4
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
void controlUVALight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute);
void controlUVBLight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute);
void controlPlantsLight(const char* on_hour, const char* on_minute,
                     const char* off_hour, const char* off_minute);
                     
// Dimmer
void configDimmerGPIO(void);
void setPWM(const int pwmvalue);

#endif