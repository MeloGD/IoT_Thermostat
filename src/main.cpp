#include "display.cpp"
#include "lvgl.h"
#include "ui.h"
#include "ft6236.h"


Display display;
#define SDA_FT6236 38
#define SCL_FT6236 39
FT6236 ts = FT6236();

/*Change to your screen resolution*/
static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];


/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ) {
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  display.startWrite();
  display.setAddrWindow( area->x1, area->y1, w, h );
  //tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  //probar con pushPixels
  display.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  display.endWrite();

  lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data ) {
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

void setup(void) {
  Serial.begin(115200);
  
  display.begin();        
  display.setRotation(1);
  display.setBrightness(255);
  //uint16_t calData[] = { 25, 53, 0, 458, 319, 19, 319, 441 };
  //display.setTouchCalibrate( calData );

  if(!ts.begin(10, SDA_FT6236, SCL_FT6236)){
     Serial.println("Unable to start the capacitive touch Screen.");
  }
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );
   
  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  
  ui_init();
  
  Serial.println( "Setup done" );
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}



