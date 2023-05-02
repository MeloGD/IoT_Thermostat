#include <Drivers/display.cpp>
#include "lvgl.h"
#include "UI/ui.h"
#include "Drivers/ft6236.h"

Display display;

#define SDA_FT6236 38
#define SCL_FT6236 39
FT6236 ts = FT6236();

/*Change to your screen resolution*/
// probar cambiar u32 a u16
static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
// Tamaño del buffer original, mucho más pequeño
// Si veo que la RAM se llena demasiado, utilizar este
// static lv_color_t buf[ screenWidth * 10 ];
// Dividimos entre 2, porque la librería está escrita con u32s
// y con esto dimensionamos la memoria del buffer de u32s a u16s
// Ojo que tengo que actualizar esto abajo también --> lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 2  );
static lv_color_t buf[ screenWidth * screenHeight / 2 ];


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
// FT6236
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

// Task timer
TaskHandle_t updateBrightnessTaskHandler = NULL;

// functions
int last_state = HIGH;
int current_state;

static void updateScreenBrightnessTask(void *pv_parameters) {
  //TickType_t xlast_wake_time;
  //const TickType_t xfrecuency = 140;
  //xlast_wake_time = xTaskGetTickCount();
  while (1) {
    //vTaskDelayUntil(&xlast_wake_time, xfrecuency);
    current_state = digitalRead(16);
    //Serial.print("task ready");
    if (last_state == HIGH && current_state == LOW ) {
      display.setBrightness(0);
      last_state = LOW;
      Serial.print("last_state = low");
    } else if (last_state == LOW && current_state == LOW) {
      display.setBrightness(255);
      last_state = HIGH;
      Serial.print("last_state = high");
    }
    vTaskDelay(140/portTICK_RATE_MS);
  }
}

// setup y main
void setup(void) {
  Serial.begin(115200);
  
  display.begin();        
  display.setRotation(1);
  display.setBrightness(255);

  
  if(!ts.begin(5, SDA_FT6236, SCL_FT6236)){
    Serial.println("Unable to start the capacitive touch Screen.");
  }
  
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 2  );
   
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
  
  // Brightness button
  pinMode(16, INPUT_PULLUP);
  xTaskCreate(updateScreenBrightnessTask,"UpdateScreenBrightnessTask",2048,NULL,1,&updateBrightnessTaskHandler);
  
  Serial.println( "Setup done" );
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}