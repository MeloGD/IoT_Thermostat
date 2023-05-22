#ifndef TASKS_GUARD
#define TASKS_GUARD

// Functions
void getDifference(char* result, const char* num1, const char* num2);

// Tasks functions
static void runUI (void *args);
static void updateScreenBrightnessTask(void *args);
static void writeSensorsDataUI(void *args) ;
static void runClockUI(void *args) ;
static void updateActiveTimeUVA(void *args);
static void updateActiveTimeUVB(void *args);
static void updateActiveTimePlants(void *args);
static void setMaxTemperatureUVA(void *args);
void createTasks(void);

#endif