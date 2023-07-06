#include "UI/mcu_ui.h"
#include "UI/ui_helpers.h"

/* Variables */
// Menu management variables
bool eeprom_message_box_drawn = false;
bool eeprom_message_box_requested = false;
bool credentials_message_box_drawn = false;
bool credentials_message_box_requested = false;
int current_found_networks = 0;
int last_found_networks = 0;
const char *selected_ssid = "";
const char *selected_password = "";
// Screen variables
const uint32_t screen_width  = 480;
const uint32_t screen_height = 320;
lv_disp_draw_buf_t draw_buffer;
lv_color_t buffer[screen_width * screen_height / 2];
Display display;
FT6236 ts = FT6236();

/* Functions */
void drawStartWiFiMenuScreen(void) {
  wifi_data = getWiFiSSIDs();
  String ssid = readSSIDFlash();
  lv_label_set_text(ui_loadingscreenssid, ssid.c_str());
  const ulong start_time = millis();
  while (lv_scr_act() == ui_loadingscreen && (millis() - start_time) < 20000) {
    drawWiFiMenu(wifi_data);
  }
  lv_obj_clean(ui_loadingscreen);
  _ui_screen_change( ui_screen1, LV_SCR_LOAD_ANIM_NONE, 0, 0);
}

void drawDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );
  display.startWrite();
  display.setAddrWindow( area->x1, area->y1, w, h );
  display.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  display.endWrite();
  lv_disp_flush_ready( disp );
}

void readTouchpad(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
  if(ts.touched()){
    data->state = LV_INDEV_STATE_PR;
    TS_Point p = ts.getPoint();
    data->point.x = p.y;
    data->point.y = display.height() - p.x;
    Serial.printf("x-%d,y-%d\n", data->point.x, data->point.y);
  } else {
    data->state = LV_INDEV_STATE_REL;
  } 
}

void configLVGLUI(void) {
  display.begin();        
  //display.setRotation(3);
  display.setRotation(1);
  display.setBrightness(255);
  if(!ts.begin(5, SDA_FT6236, SCL_FT6236)){
    Serial.println("Unable to start the capacitive touch Screen.");
  }
  lv_init();
  lv_disp_draw_buf_init(&draw_buffer, buffer, NULL, screen_width * screen_height / 2);
  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screen_width;
  disp_drv.ver_res = screen_height;
  disp_drv.flush_cb = drawDisplay;
  disp_drv.draw_buf = &draw_buffer;
  lv_disp_drv_register(&disp_drv);
  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = readTouchpad;
  lv_indev_drv_register(&indev_drv);
  ui_init();
  //lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_180);
}

void setDisplayBrightness(const uint8_t value) {
  display.setBrightness(value);
}

// WiFi Start Screen Menu Management
void drawCleanEEPROMMessageBox(void) {
  const char text[] = { "Por favor, confirme el procedimiento." }; 
  static const char* options[] = {"Aceptar", "Rechazar", ""}; 
  if (ui_reportwifimessagebox != NULL) {
    if (!eeprom_message_box_drawn) {
      ui_reportwifimessagebox = lv_msgbox_create(NULL, "'Borrar red actual'", text, options, false);
      lv_obj_center(ui_reportwifimessagebox);
      eeprom_message_box_drawn = true;
    }
    if (lv_msgbox_get_active_btn(ui_reportwifimessagebox) == 0) {
      writeSSIDFlash("");
      writePasswordFlash("");
      eeprom_message_box_requested = false;
      eeprom_message_box_drawn = false;
      lv_msgbox_close(ui_reportwifimessagebox);
    } else if (lv_msgbox_get_active_btn(ui_reportwifimessagebox) == 1) {
      eeprom_message_box_requested = false;
      eeprom_message_box_drawn = false;
      lv_msgbox_close(ui_reportwifimessagebox);
    }
  }  
}

void drawCredentialsMessageBox(void) { 
  if (ui_reportwifimessagebox != NULL) {
    if (!credentials_message_box_drawn) {    
      ui_wifikeyboard = lv_keyboard_create(ui_loadingscreen);
      vTaskDelay(50);
      ui_wifipasswordarea = lv_textarea_create(ui_loadingscreen);
      lv_obj_set_size(ui_wifipasswordarea, 480, 160);
      lv_textarea_set_placeholder_text(ui_wifipasswordarea, "Escriba la clave...");
      selected_password = lv_textarea_get_text(ui_wifipasswordarea);
      lv_keyboard_set_textarea(ui_wifikeyboard, ui_wifipasswordarea);
      credentials_message_box_drawn = true;
    }
    if (lv_keyboard_get_selected_btn(ui_wifikeyboard) == 39) {
      if (tryWiFi(selected_ssid, selected_password)) {
        Serial.print("\n CONEXION CORRECTA ");
        const char* message = "Las claves introducidas han sido correctas. Se han guardado en la memoria interna.";
        ui_reportwifimessagebox = lv_msgbox_create(NULL, "'Claves correctas'", message, NULL, true);
        lv_obj_center(ui_reportwifimessagebox);
        lv_label_set_text(ui_loadingscreenssid, selected_ssid);
      } else {
        Serial.print("\n CONEXION ERRONEA ");
        const char* message = "Las claves introducidas han sido incorrectas. Introduzcalas de nuevo.";
        ui_reportwifimessagebox = lv_msgbox_create(NULL, "'Claves incorrectas'", message, NULL, true);
        lv_obj_center(ui_reportwifimessagebox);
      }
      credentials_message_box_requested = false;
      credentials_message_box_drawn = false;
      lv_obj_del_async(ui_wifipasswordarea);
      lv_obj_del_async(ui_wifikeyboard);     
    }
  } 
}

// WiFi menu events
void uiEventCloseWiFiWindow( lv_event_t * e) {
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED) {
    _ui_screen_change( ui_screen1, LV_SCR_LOAD_ANIM_NONE, 0, 0);
  }
}

void uiEventCleanEEPROM( lv_event_t * e) {
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t * target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED) {
    eeprom_message_box_requested = true;
    lv_label_set_text(ui_loadingscreenssid, "");
  }
}

void uiEventWriteWiFiCredentials( lv_event_t * e) {
  lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
  if (event_code == LV_EVENT_CLICKED) {
    selected_ssid = lv_list_get_btn_text(ui_wifilistoptions, target);
    credentials_message_box_requested = true;
  }
}

// WiFi menu
void drawWiFiMenu(WifiScanData wifi_data) {
  current_found_networks = wifi_data.size();
  if (ui_wifilist != NULL) {
    if (current_found_networks != last_found_networks) {
      lv_obj_clean(ui_wifilist);
      lv_list_add_text(ui_wifilist, "Opciones");
      ui_wifilistoptions = lv_list_add_btn(ui_wifilist, LV_SYMBOL_TRASH, "Borrar red actual");
      lv_obj_add_event_cb(ui_wifilistoptions, uiEventCleanEEPROM, LV_EVENT_ALL, NULL);
      ui_wifilistoptions = lv_list_add_btn(ui_wifilist, LV_SYMBOL_CLOSE, "Continuar con los ajustes actuales");
      lv_obj_add_event_cb(ui_wifilistoptions, uiEventCloseWiFiWindow, LV_EVENT_ALL, NULL);
      lv_list_add_text(ui_wifilist, "Redes disponibles");
      for (int i = 0; i < wifi_data.size(); i++) {
        ui_wifilistoptions = lv_list_add_btn(ui_wifilist, LV_SYMBOL_WIFI, wifi_data[i][0].c_str());
        lv_obj_add_event_cb(ui_wifilistoptions, uiEventWriteWiFiCredentials, LV_EVENT_ALL, NULL);
      }
      last_found_networks = current_found_networks;
    }
    if (eeprom_message_box_requested) {
      drawCleanEEPROMMessageBox();
    } else if (credentials_message_box_requested) {
      drawCredentialsMessageBox();
    }
  }  
}