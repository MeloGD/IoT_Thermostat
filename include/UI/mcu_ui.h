#ifndef MCU_UI_GUARD
#define MCU_UI_GUARD

#include "lvgl.h"
#include "ui.h"
#include "Devices/ft6236.h"
#include "Devices/display.h"
#include "Devices/networking.h"
#include "vector"

static bool eeprom_message_box_drawn = false;
static bool eeprom_message_box_requested = false;
static bool credentials_message_box_drawn = false;
static bool credentials_message_box_requested = false;
static int current_found_networks = 0;
static int last_found_networks = 0;
static const char *selected_ssid = "";
static const char *selected_password = "";


void drawDisplay( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void readTouchpad(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void launchUi(void);
void setDisplayBrightness(const uint8_t value);
void uiEventCloseWiFiWindow( lv_event_t *e);
void drawCleanEEPROMMessageBox(void);
void uiEventCleanEEPROM( lv_event_t *e);
void drawWiFiMenu(WifiScanData data);

#endif