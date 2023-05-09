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
