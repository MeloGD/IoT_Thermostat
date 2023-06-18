#ifndef NETWORING_GUARD
#define NETWORING_GUARD

#include "UI/ui.h"
#include "WiFi.h"
#include "vector"
#include "Preferences.h"
#include "time.h"
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

typedef std::vector<std::vector<String>> WifiScanData;

/* Variables */
extern bool wifi_status;
extern const ulong wifi_timeout;
extern Preferences device_data;
extern WifiScanData wifi_data;
extern String payload;
extern IPAddress MQTT_SERVER;
extern AsyncMqttClient mqtt_client;
// Timers
extern TimerHandle_t mqtt_reconnect_timer;
extern TimerHandle_t wifi_reconnect_timer;

// Functions
WifiScanData getWiFiSSIDs(void);
void initFlashMemmory(void);
void writeSSIDFlash(const char *ssid);
void writePasswordFlash(const char *password);
const String readSSIDFlash(void);
const String readPasswordFlash(void);
bool tryWiFi(const char *ssid, const char *password);
void wifiEvent(WiFiEvent_t event);
void connectWiFi(void);
void wifiReconnect(void);
void configMQTT(void);
void connectMQTT();
void onMQTTConnect(bool status);
void onMQTTDisconnect(AsyncMqttClientDisconnectReason reason);
void onMQTTSubscribe(uint16_t packet_id, uint8_t qos);
void onMQTTUnsubscribe(uint16_t packet_id);
void onMQTTReceived(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMQTTPublish(uint16_t packet_id);
void publishMQTT(unsigned long data);
void subscribeMQTT(void);
String getPayloadContent(char* data, size_t len);

#endif