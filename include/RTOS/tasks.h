#ifndef TASKS_GUARD
#define TASKS_GUARD

#include "UI/mcu_ui.h"
#include "Devices/sensor.h"
#include "Devices/rtc.h"
#include "Devices/actuators.h"
#include "Devices/networking.h"

/* Variables */
// Handlers
extern TaskHandle_t lvglHandler;
extern TaskHandle_t updateBrightnessTaskHandler;
extern TaskHandle_t writeSensorsDataUITaskHandler;
extern TaskHandle_t runClockUITaskHandler;
extern TaskHandle_t updateActiveTimeUVATaskHandler;
extern TaskHandle_t updateActiveTimeUVBTaskHandler;
extern TaskHandle_t updateActiveTimePlantsTaskHandler;
extern TaskHandle_t setMaxTemperatureUVATaskHandler;
extern TaskHandle_t lightSystemTaskHandler;
extern TaskHandle_t wifiScannerTaskHandler;
extern TaskHandle_t drawWiFiMenuTaskHandler;
extern TaskHandle_t connectWiFiTaskHandler;
extern TaskHandle_t connectMQTTBrokerTaskHandler;
extern TaskHandle_t maintainMQTTTaskHandler;
//Mutex
extern SemaphoreHandle_t mutex;
extern SemaphoreHandle_t mutex2;

/* Functions */
void initUITask(void);
void createTasks(void);
void getTimeDifference(char* hours_remaining, char* minutes_remaining,
                      const char* hour1, const char* minute_1,
                      const char* hour2, const char* minute_2);
// Tasks functions
static void runUI (void *args);
static void updateScreenBrightnessTask(void *args);
static void writeSensorsDataUITask(void *args) ;
static void runClockUITask(void *args) ;
static void updateActiveTimeUVATask(void *args);
static void updateActiveTimeUVBTask(void *args);
static void updateActiveTimePlantsTask(void *args);
static void setMaxTemperatureUVATask(void *args);
static void controlLightsSystemTask(void *args);
static void wifiScannerTask(void *args);
static void drawWiFiMenuTask(void *args);
static void connectWiFiMenuTask(void *args);
static void connectMQTTBrokerTask(void *args);
static void maintainMQTTBrokerTask(void *args);

#endif