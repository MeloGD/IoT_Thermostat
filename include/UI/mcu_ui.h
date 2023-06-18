#ifndef MCU_UI_GUARD
#define MCU_UI_GUARD

#include "lvgl.h"
#include "ui.h"
#include "Devices/ft6236.h"
#include "Devices/display.h"
#include "Devices/networking.h"
#include "vector"

/* Variables */
// Menu management variables
extern bool eeprom_message_box_drawn;
extern bool eeprom_message_box_requested;
extern bool credentials_message_box_drawn;
extern bool credentials_message_box_requested;
extern int current_found_networks;
extern int last_found_networks;
extern const char *selected_ssid;
extern const char *selected_password;
// Screen variables
extern const uint32_t screen_width;
extern const uint32_t screen_height;
extern lv_disp_draw_buf_t draw_display_buffer;
extern lv_color_t display_buffer;
extern Display display;
extern FT6236 ts;

/* Functions */
void drawStartWiFiMenuScreen(void);
void drawDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void readTouchpad(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void configLVGLUI(void);
void setDisplayBrightness(const uint8_t value);
void drawCleanEEPROMMessageBox(void);
void uiEventCloseWiFiWindow( lv_event_t *e);
void uiEventCleanEEPROM( lv_event_t *e);
void uiEventWriteWiFiCredentials( lv_event_t * e);
void drawWiFiMenu(WifiScanData data);

#endif