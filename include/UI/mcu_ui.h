#ifndef MCU_UI_GUARD
#define MCU_UI_GUARD

#include "lvgl.h"
#include "ui.h"
#include "Devices/ft6236.h"
#include "Devices/display.h"
#include "Devices/networking.h"
#include "vector"


void drawDisplay( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void readTouchpad(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);
void launchUi(void);
void setDisplayBrightness(const uint8_t value);
void drawWiFiMenu(WifiScanData data);

#endif