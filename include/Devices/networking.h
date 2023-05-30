#ifndef NETWORING_GUARD
#define NETWORING_GUARD

#include "WiFi.h"
#include "vector"
#include "Preferences.h"

typedef std::vector<std::vector<String>> WifiScanData;
static Preferences device_data;

static const char *ssid = "ssid";
static const char *passowrd = "password";
// Functions
WifiScanData getWiFiSSIDs(void);
void initFlashMemmory(void);
void writeSSIDFlash(const char *ssid);
void writePasswordFlash(const char *password);
const String readSSIDFlash(void);
const String readPasswordFlash(void);


#endif