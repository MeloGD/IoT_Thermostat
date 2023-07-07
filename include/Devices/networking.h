/**
 * @brief Fichero que define las funciones encargadas de gestionar la 
 * comunicación via WiFi, el protocolo MQTT, etc.
 * 
 * @file networking.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

#ifndef NETWORING_GUARD
#define NETWORING_GUARD

#include "UI/ui.h"
#include "WiFi.h"
#include "vector"
#include "Preferences.h"
#include "time.h"
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

/**
 * @brief Definición de una matriz de strings que contiene la información 
 * obtenida al realizar el escaneo de redes WiFi.
*/
typedef std::vector<std::vector<String>> WifiScanData;

/* Variables */

/**
 * @brief Tiempo máximo permitido para intectar conectarse a una red WiFi.
*/
extern const ulong wifi_timeout;
/**
 * @brief Objeto que contiene los datos de los crendenciales WiFi.
*/
extern Preferences device_data;
/**
 * @brief Matriz de strings que contiene la información obtenida al realizar
 * el escaneo de redes WiFi.
*/
extern WifiScanData wifi_data;
/**
 * @brief Usado principalmente para debugging, devuelve la cadena de datos
 * recibida por el servidor.
*/
extern String payload;
/**
 * @brief Objeto que contiene la IP bróker MQTT
*/
extern IPAddress MQTT_SERVER;
/**
 * @brief Instancia del protocolo MQTT en el ESP32
*/
extern AsyncMqttClient mqtt_client;
// Timers
/**
 * @brief Temporizador utilizado para controlar la llamada de la función que
 * reconecta con el bróker MQTT.
*/
extern TimerHandle_t mqtt_reconnect_timer;
/**
 * @brief Temporizador utilizado para controlar la llamada de la función que
 * reconecta con el WiFi.
*/
extern TimerHandle_t wifi_reconnect_timer;

// Functions
/**
 * @brief Escanea las redes WiFi disponibles.
 * @return Devuelve una matriz de strings con la información de todas las redes
 * obtenidas.
*/
WifiScanData getWiFiSSIDs(void);
/**
 * @brief Inicializa la memoria flash
*/
void initFlashMemmory(void);
/**
 * @brief Escribe el SSID en la memoria flash.
*/
void writeSSIDFlash(const char *ssid);
/**
 * @brief Escribe el password en la memoria flash.
*/
void writePasswordFlash(const char *password);
/**
 * @brief Lee de la memoria flash el SSID.
 * @return Devuelve el SSID.
*/
const String readSSIDFlash(void);
/**
 * @brief Lee de la memoria flash el password.
 * @return Devuelve el password.
*/
const String readPasswordFlash(void);
/**
 * @brief Función que prueba los credenciales introducidos para comprobar si
 * se ha podido conectar con éxito. Si es así, escribe los credenciales en
 * la memoria flash.
 * @param ssid El SSID que identifica la red WiFi.
 * @param password El password que autoriza el acceso a la red WiFi.
 * @return Devuelve true si se ha podido conectar, false si no.
*/
bool tryWiFi(const char *ssid, const char *password);
/**
 * @brief Función llamada en el main.cpp, se utiliza para iniciar 
 * los timers que empezarán a funcionar en caso de que el WiFi esté conectado o no.
 *  -   Si se detecta que se ha obtenido una IP, se llama a la función connectMQTT()
 *      para establecer una conexión con el bróker MQTT. Además, se activa un switch en 
 *      la GUI para notificar que el WiFi está conectado, se para el timer de reconección
 *      wifi_reconnect_timer y se activa el timer para reconectar con el bróker MQTT.
 *  -   Si se detecta que no se ha desconectado del WiFi, se para el timer de reconección
 *      al bróker MQTT y se activa el timer de reconección WiFi para intentar recuperar
 *      la conexión. Además, se desactiva un switch gráfica para notificar al usuario que
 *      el WiFi no está en funcionamiento.
 * @param event El SSID que identifica la red WiFi.
*/
void wifiEvent(WiFiEvent_t event);
/**
 * @brief Función llamada en el main.cpp, se utiliza para establecer una
 * conexión WiFi, con los datos almacenados en la memoria flash.
*/
void connectWiFi(void);
/**
 * @brief Función llamada por el timer wifi_reconnect_timer que intenta restablecer
 * la conexión previa.
*/
void wifiReconnect(void);
/**
 * @brief Función que crea los timers mqtt_reconnect_timer, wifi_reconnect_timer y fija
 * todos los comportamientos a realizar cuando se conecta, desconecta, suscribe a un tema,
 * publica, etc. Finalmente, configura el bróker MQTT con su IP y puerto de destino.
*/
void configMQTT(void);
/**
 * @brief Conecta con el bróker MQTT.
*/
void connectMQTT();
/**
 * @brief En caso de establecer una conexión con el bróker MQTT, intenta suscribirse
 * a un tópico con subcribeMQTT().
 * @param status true si se ha conectado, false si no.
*/
void onMQTTConnect(bool status);
/**
 * @brief En caso de perder la conexión con el bróker MQTT, comprueba primero si el
 * WiFi está conectado y si es así, inicia el timer mqtt_reconnect_timer.
 * @param reason Motivo de la desconexión, que pueden ser los siguientes:
 *  - TCP_DISCONNECTED
 *  - MQTT_UNACCEPTABLE_PROTOCOL_VERSION
 *  - MQTT_IDENTIFIER_REJECTED
 *  - MQTT_SERVER_UNAVAILABLE
 *  - MQTT_MALFORMED_CREDENTIALS
 *  - MQTT_NOT_AUTHORIZED
 *  - ESP8266_NOT_ENOUGH_SPACE
 *  - TLS_BAD_FINGERPRINT
*/
void onMQTTDisconnect(AsyncMqttClientDisconnectReason reason);
/**
 * @brief Para debugging. Cuando se suscribe correctamente a un tema publica
 * información relevante por el serial.
 * @param packer_id Identificador del paquete enviado
 * @param qos Nivel de calidad de servicio establecido 
*/
void onMQTTSubscribe(uint16_t packet_id, uint8_t qos);
/**
 * @brief Para debugging. Cuando no se puede suscribir a un tema publica
 * información relevante por el serial.
 * @param packer_id Identificador del paquete enviado
*/
void onMQTTUnsubscribe(uint16_t packet_id);
/**
 * @brief Para debugging. Cuando el mensaje llega correctamente al bróker MQTT, se 
 * publica información relevante por el serial en relación al conteido enviado, en este
 * caso, la información de temperatura/humedad de los sensores.
 * @param topic Tópico 
 * @param payload Mensaje enviado correctamente al servidor
 * @param properties Parámetro especifico de la librería.
 * @param size_len Logitud total del mensaje
 * @param index Parámetro especifico de la librería.
 * @param total Parámetro especifico de la librería.
*/
void onMQTTReceived(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
/**
 * @brief Para debugging. Una vez publicada la información correctamente en el servidor,
 * muestra información relevante por el serial.
 * @param packet_id Tópico 
*/
void onMQTTPublish(uint16_t packet_id);
/**
 * @brief Se suscribe a un tema para luego ser gestionado por el bróker MQTT.
 * @param packet_id Tópico 
*/
void subscribeMQTT(void);
/**
 * @brief Obtiene el contenido del payload enviado al bróker MQTT, iterando el payload 
 * como un array de chars y devolviendo la string formada tras las iteraciones.
 * @param data Contenido del mensaje o payload
 * @param len Longitud del mensaje o payload
 * @return Devuelve como string el contenido del mensaje o payload.
*/
String getPayloadContent(char* data, size_t len);

#endif