#ifndef TASKS_GUARD
#define TASKS_GUARD



static void updateScreenBrightnessTask(void *pv_parameters);
static void updateWarmHideTemperature(void *pv_parameters) ;
static void updateColdHideTemperature(void *pv_parameters) ;
void createTasks(void);

#endif