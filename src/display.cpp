#ifndef DISPLAY_GUARD
#define DISPLAY_GUARD

#include <LovyanGFX.hpp>

class Display : public lgfx::LGFX_Device {
// Tipo de panel LCD en uso ILI9488
  lgfx::Panel_ILI9488     _panel_instance;
// Tipo de bus con el que se conecta la LCD
  lgfx::Bus_SPI           _bus_instance;  
// Control de brillo de la LCD
  lgfx::Light_PWM         _light_instance;

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
#endif

