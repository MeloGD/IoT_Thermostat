#ifndef NETWORING_GUARD
#define NETWORING_GUARD

#include "WiFi.h"
#include "vector"

typedef std::vector<std::vector<String>> WifiScanData;

// Functions
WifiScanData getWiFiSSIDs(void);

#endif