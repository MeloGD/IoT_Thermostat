#ifndef NETWORING_GUARD
#define NETWORING_GUARD

#include "WiFi.h"
#include "vector"
#include "Preferences.h"
#include "time.h"


typedef std::vector<std::vector<String>> WifiScanData;

static Preferences device_data;

static bool wifi_status = false;

// 10 segundos
const ulong wifi_timeout = 10 * 1000;

// Functions
WifiScanData getWiFiSSIDs(void);
void initFlashMemmory(void);
void writeSSIDFlash(const char *ssid);
void writePasswordFlash(const char *password);
const String readSSIDFlash(void);
const String readPasswordFlash(void);
bool tryWiFi(const char *ssid, const char *password);
bool connectWiFi(void);


#endif