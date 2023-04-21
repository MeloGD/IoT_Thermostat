#include <LovyanGFX.hpp>

// cambiar LGFX y LGFX en el contructor por Screen

class Display : public lgfx::LGFX_Device {
// Tipo de panel LCD en uso ILI9488
  lgfx::Panel_ILI9488     _panel_instance;
// Tipo de bus con el que se conecta la LCD
  lgfx::Bus_SPI           _bus_instance;  
// Control de brillo de la LCD
  lgfx::Light_PWM         _light_instance;
// Control del panel táctil, en este caso, FT6236
  lgfx::Touch_FT5x06      _touch_instance; 

public:
  Display(void) {
    // Configuración del bus SPI, para controlar la LCD 
    { 
      auto cfg        = _bus_instance.config();   
      cfg.spi_host    = SPI3_HOST;     
      cfg.spi_mode    = 0;             
      cfg.freq_write  = 40000000;    
      cfg.freq_read   = 16000000;    
      cfg.spi_3wire   = true;        
      cfg.use_lock    = true;        
      cfg.dma_channel = 1; 
      cfg.pin_sclk    = 14;            
      cfg.pin_mosi    = 13;            
      cfg.pin_miso    = 12;            
      cfg.pin_dc      = 21;            
      // Carga la configuración para preparar el bus SPI
      _bus_instance.config(cfg);    
      _panel_instance.setBus(&_bus_instance);      
    }
    // Connfiguración del panel LCD
    { 
      auto cfg             = _panel_instance.config();    
      cfg.pin_cs           = 15;  
      cfg.pin_rst          = -1;  
      cfg.pin_busy         = -1;  
      cfg.panel_width      = 320;  
      cfg.panel_height     = 480;  
      cfg.dummy_read_pixel = 8;  
      cfg.dummy_read_bits  = 1;  
      cfg.readable         = true;  
      cfg.invert           = false;  
      cfg.rgb_order        = false;  
      cfg.dlen_16bit       = false;  // Probar si funciona el modo color 16 bits
      cfg.bus_shared       = true;   // True, el bus está compartido con la SD -> PREGUNTAR A JONAY, POR SI ACASO
      // Carga la configuración para preparar el panel LCD
      _panel_instance.config(cfg);
    }
    // Configuración de panel táctil
    { 
      auto cfg            = _touch_instance.config();
      cfg.x_min           = 0;    
      cfg.x_max           = 319;  
      cfg.y_min           = 0;    
      cfg.y_max           = 479;  
      cfg.pin_int         = 40;   // GPIO al cual está conectado el panel
      cfg.bus_shared      = false; 
      cfg.offset_rotation = 0;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定
      // Configuración del panel conectado a I2C
      cfg.i2c_port        = 1;      
      cfg.i2c_addr        = 0x38;   
      cfg.pin_sda         = 38;     
      cfg.pin_scl         = 39;     
      cfg.freq            = 400000;     
      // Carga la configuración para preparar el panel táctil
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); 
    }
    // Configuración del control de brillo de la LCD
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = 48;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 7;
      // Carga la configuración para preparar el control de brillo
      _light_instance.config(cfg);
      _panel_instance.light(&_light_instance);
    }
    // Una vez realizados todos la configuración previa, terminamos de crear el objeto con setPanel()
    setPanel(&_panel_instance); 
  }
};

Display display;

void setup(void) {
  Serial.begin(115200);
  display.init();
  display.setTextSize((std::max(display.width(), display.height()) + 255) >> 8);
  if (display.touch()) {
    if (display.width() < display.height()) display.setRotation(display.getRotation() ^ 1);
    display.setTextDatum(textdatum_t::middle_center);
    display.drawString("touch the arrow marker.", display.width()>>1, display.height() >> 1);
    display.setTextDatum(textdatum_t::top_left);
    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (display.isEPD()) std::swap(fg, bg);
    uint16_t calibrationData[8];
    display.calibrateTouch(calibrationData, fg, bg, std::max(display.width(), display.height()) >> 3);
    // Información de la calibración, por terminal serial
    for(int i=0; i<8; i++) {
      Serial.print(calibrationData[i]);
      Serial.print(", ");
    }
  }
  display.fillScreen(TFT_BLACK);
}

uint32_t count = ~0;
void loop(void) {
  display.setTextDatum(textdatum_t::middle_center);
  display.drawString("Segunda pantalla.", display.width()>>1, display.height() >> 1);
  display.setTextDatum(textdatum_t::top_left);
  for (int i = 0; i < 255; i++) {
    display.setBrightness(i);
    delay(10);
  }
  for (int i = 0; i < 255; i++) {
    display.setBrightness(255 - i);
    delay(10);
  }
}