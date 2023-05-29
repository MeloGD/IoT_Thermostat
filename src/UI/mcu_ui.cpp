#include "UI/mcu_ui.h"

/*Change to your screen resolution*/
// probar cambiar u32 a u16
static const uint32_t screen_width  = 480;
static const uint32_t screen_height = 320;
static lv_disp_draw_buf_t draw_buffer;
// Tamaño del buffer original, mucho más pequeño
// Si veo que la RAM se llena demasiado, utilizar este
// static lv_color_t buf[ screenWidth * 10 ];
// Dividimos entre 2, porque la librería está escrita con u32s
// y con esto dimensionamos la memoria del buffer de u32s a u16s
// Ojo que tengo que actualizar esto abajo también --> lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 2  );
static lv_color_t buffer[ screen_width * screen_height / 2 ];

Display display;
FT6236 ts = FT6236();

/* Display flushing */
void drawDisplay( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  display.startWrite();
  display.setAddrWindow( area->x1, area->y1, w, h );
  display.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  display.endWrite();

  lv_disp_flush_ready( disp );
}

void readTouchpad( lv_indev_drv_t * indev_driver, lv_indev_data_t * data ) {
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

void launchUi(void) {
  display.begin();        
  display.setRotation(1);
  display.setBrightness(255);
  
  if(!ts.begin(5, SDA_FT6236, SCL_FT6236)){
    Serial.println("Unable to start the capacitive touch Screen.");
  }
  
  lv_init();
  lv_disp_draw_buf_init( &draw_buffer, buffer, NULL, screen_width * screen_height / 2  );
   
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
}

void setDisplayBrightness(const uint8_t value) {
  display.setBrightness(value);
}

// WIFI Management
void ui_event_closepopupwifiwindow( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED) {
        //_ui_screen_change( ui_screen1, LV_SCR_LOAD_ANIM_NONE, 0, 0);
        lv_obj_add_flag( ui_wifilist,  LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state( ui_wifiswitch, LV_STATE_CHECKED);
        lv_obj_add_flag( ui_nextbuttonscreen1,  LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag( ui_wifiswitch,  LV_OBJ_FLAG_CLICKABLE);
    }
}

int current_found_networks = 0;
int last_found_networks = 0;

void drawWiFiMenu(WifiScanData wifi_data) {
  current_found_networks = wifi_data.size();
  if (current_found_networks != last_found_networks) {
    lv_obj_clean(ui_wifilist);
    lv_list_add_text(ui_wifilist, "Opciones");
    //añadir a eeprom, comprobar si hay claves guardadas, mostrando ventana de información
    //implementar las funciones en networking.h y networking.cpp, luego llamarlas
    //aquí
    ui_wifilistoptions = lv_list_add_btn(ui_wifilist, LV_SYMBOL_TRASH, "Borrar red actual");
    ui_wifilistoptions = lv_list_add_btn(ui_wifilist, LV_SYMBOL_CLOSE, "Salir");
    lv_obj_add_event_cb(ui_wifilistoptions, ui_event_closepopupwifiwindow, LV_EVENT_ALL, NULL);
    lv_list_add_text(ui_wifilist, "Redes disponibles");
    for (int i = 0; i < wifi_data.size(); i++) {
      ui_wifilistoptions = lv_list_add_btn(ui_wifilist, LV_SYMBOL_WIFI, wifi_data[i][0].c_str());
    }
    last_found_networks = current_found_networks;
    
  }
}