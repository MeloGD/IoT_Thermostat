/**
 * @brief Fichero que define las tareas que ejecuta el sistema.
 * 
 * @file tasks.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

#ifndef TASKS_GUARD
#define TASKS_GUARD

#include "UI/mcu_ui.h"
#include "Devices/sensor.h"
#include "Devices/rtc.h"
#include "Devices/actuators.h"
#include "Devices/networking.h"

// Handlers
/**
 * @brief Controlador de la tarea que ejecuta la GUI.
*/
extern TaskHandle_t lvglHandler; 
/**
 * @brief Controlador de la tarea que ejecuta el encendido/apagado de la pantalla.
*/
extern TaskHandle_t updateBrightnessTaskHandler;
/**
 * @brief Controlador de la tarea que escribe la información de los sensores 
 * en la GUI.
*/
extern TaskHandle_t writeSensorsDataUITaskHandler;
/**
 * @brief Controlador de la tarea que escribe el tiempo reportado por el 
 * reloj en la GUI.
*/
extern TaskHandle_t runClockUITaskHandler;
/**
 * @brief Controlador de la tarea que muestra el tiempo de actividad del
 * bombillo UVA en la GUI.
*/
extern TaskHandle_t updateActiveTimeUVATaskHandler;
/**
 * @brief Controlador de la tarea que muestra el tiempo de actividad del
 * bombillo UVB en la GUI.
*/
extern TaskHandle_t updateActiveTimeUVBTaskHandler;
/**
 * @brief Controlador de la tarea que muestra el tiempo de actividad del
 * bombillo Plants en la GUI.
*/
extern TaskHandle_t updateActiveTimePlantsTaskHandler;
/**
 * @brief Controlador de la tarea que fija la temperatura máxima a la que
 * puede llegar el bombillo UVA.
*/
extern TaskHandle_t setMaxTemperatureUVATaskHandler;
/**
 * @brief Controlador de la tarea controla el sistema de luces.
*/
extern TaskHandle_t lightSystemTaskHandler;/**
 * @brief Controlador de la tarea escanea redes WiFi.
*/
extern TaskHandle_t wifiScannerTaskHandler;
/**
 * @brief Controlador de la tarea dibuja la pantalla de bienvenida para
 * configurar el WiFi.
*/
extern TaskHandle_t drawWiFiMenuTaskHandler;/**
/**
 * @brief Controlador de la tarea que se encarga de publicar los datos de
 * los sensores a Mosquitto, el bróker MQTT.
*/
extern TaskHandle_t publishMQTTBrokerTaskHandler;/**


/* Functions */
/**
 * @brief Crea la tarea encargada de lanzar el supervidor o handler de LVGL
*/
void initUITask(void);
/**
 * @brief Crea todas las tareas del sistema encargadas de leer la temperatura,
 * comunicar datos, actualizar el reloj de la pantalla, etc. 
*/
void createTasks(void);
/**
 * @brief Calcula cuanto tiempo estará activo un bombillo en función de las horas
 *  especficadas en los rollers de tiempo, con el fin de mostrar esa información
 * en la GUI.
 * @param hours_remaining Número de horas en las que el bombillo estará encendido
 * @param minutes_remaining Número de minutos en las que el bombillo estará encendido
 * @param start_hour Hora a la que se activará el bombillo
 * @param start_minute Minuto en el que se activará el bombillo
 * @param finish_hour Hora a la que se desactivará el bombillo
 * @param finish_minute Minuto en el que se desactivará el bombillo
 * 
*/
void getTimeDifference(char* hours_remaining, char* minutes_remaining, 
                      const char* start_hour, const char* start_minute, 
                      const char* finish_hour, const char* finish_minute);
/**
 * @brief Obtiene la string de un roller que representa la hora para poder obtener
 * por separado la hora y los mintuos.  Todos los parámetros son punteros de arrays de chars.
 * @param roller_time Contiene la hora del roller.
 * @param hour Especifica solo la parte de la hora obtenida del roller_time
 * @param minutes Especficia solo la parte de los minutos obtenidos del roller time
 * 
*/
void getUIRollerTime(const char* roller_time, char* hour, char* minutes);

// Tasks functions
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * qie controla la GUI del dispositivo
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void runUI (void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que controla el brillo de la pantalla.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void updateScreenBrightnessTask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que escribe los datos de los sensores en la pantalla.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void writeSensorsDataUITask(void* args) ;
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que escribe la hora en la pantalla.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void runClockUITask(void* args) ;
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que lee los tiempos de actividad del bombillo UVA.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void updateActiveTimeUVATask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que lee los tiempos de actividad del bombillo UVB.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void updateActiveTimeUVBTask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que lee los tiempos de actividad del bombillo Plants.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void updateActiveTimePlantsTask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que controla la temperatura máxima del bombillo UVA.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void setMaxTemperatureUVATask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que controla el sistema de luces.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void controlLightsSystemTask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * escenea las redes WiFi.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void wifiScannerTask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * que dibuja la pantalla de bienvenida y así poder gestionar las redes WiFi.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void drawWiFiMenuTask(void* args);
/**
 * @brief Función llamada por createTasks(), se encarga de gestionar la tarea
 * qie publica los datos de los sensores a Mosquitto, el bróker de MQTT.
 * @param args Esta especficación hace posible que la función puede ser llamada
 * por su definición a la hora de crear la tarea en freeRTOS.
 * 
*/
static void publishMQTTBrokerTask(void* args);

#endif