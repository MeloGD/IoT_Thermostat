#ifndef RTC_GUARD
#define RTC_GUARD

#include "RTClib.h"
#include "Wire.h"

#define RTC_SDA 11
#define RTC_SCL 17


void configRTC(void);
DateTime reportTime(void);

#endif