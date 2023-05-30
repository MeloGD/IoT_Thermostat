#ifndef TASKS_GUARD
#define TASKS_GUARD

#include "UI/mcu_ui.h"
#include "Devices/sensor.h"
#include "Devices/rtc.h"
#include "Devices/actuators.h"
#include "Devices/networking.h"


// Functions
void createTasks(void);
void getDifference(char* result1, char* result2, const char* hour1, const char* min1, const char* hour2, const char* min2);

// Tasks functions
static void runUI (void *args);
static void updateScreenBrightnessTask(void *args);
static void writeSensorsDataUITask(void *args) ;
static void runClockUITask(void *args) ;
static void updateActiveTimeUVATask(void *args);
static void updateActiveTimeUVBTask(void *args);
static void updateActiveTimePlantsTask(void *args);
static void setMaxTemperatureUVATask(void *args);
static void drawWiFiMenuTask(void *args);
static void connectWiFiMenuTask(void *args);

#endif