#ifndef NETWORING_GUARD
#define NETWORING_GUARD

#include "UI/ui.h"
#include "WiFi.h"
#include "vector"
#include "Preferences.h"
#include "time.h"

#include <AsyncMqttClient.h>


typedef std::vector<std::vector<String>> WifiScanData;

static Preferences device_data;

static bool wifi_status = false;


extern WifiScanData wifi_data;

void plsWiFiWork(void);

// 10 segundos
const ulong wifi_timeout = 10 * 1000;

static bool once = false;

// Functions
WifiScanData getWiFiSSIDs(void);
void initFlashMemmory(void);
void writeSSIDFlash(const char *ssid);
void writePasswordFlash(const char *password);
const String readSSIDFlash(void);
const String readPasswordFlash(void);
bool tryWiFi(const char *ssid, const char *password);

void connectWiFi(void);
void wifiEvent(WiFiEvent_t event);
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