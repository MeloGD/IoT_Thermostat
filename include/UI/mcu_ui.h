/**
 * @brief Fichero que contiene todas las definiciones de las funciones creadas por 
 * el autor para interactuar con la libería LVGL. 
 * @file mcu_ui.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

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
/**
 * @brief Variable que se utiliza para saber si se ha mostrado en 
 * pantalla la ventana que otorga la opción de borrar la configuración
 * WiFi en la memoria flash.
*/
extern bool flash_message_box_drawn;
/**
 * @brief Variable que se utiliza para saber si se ha solicitado la ventana 
 * de borrado de la configuración WiFi en la memoria flash.
*/
extern bool flash_message_box_requested;
/**
 * @brief Variable que se utiliza para saber si se ha solicitado la ventana 
 * de introducción de credenciales WiFi.
*/
extern bool credentials_message_box_drawn;
/**
 * @brief Variable que se utiliza para saber si se mostrado la ventana 
 * de introducción de credenciales WiFi.
*/
extern bool credentials_message_box_requested;
/**
 * @brief Variable que contiene el número de redes WiFi encontradas.
*/
extern int current_found_networks;
/**
 * @brief Variable que contiene el último número de redes WiFi encontradas, 
 * con el fin de evitar dibujar elementos ya existentes, lo cual hacía que el
 * ESP32 se reiniciara.
*/
extern int last_found_networks;
/**
 * @brief Variable que contiene el ssid de la red wifi seleccionada en el menu.
*/
extern const char *selected_ssid;
/**
 * @brief Variable que contiene la contraseña WiFi escrita en el menú.
*/
extern const char *selected_password;
// Screen variables
/**
 * @brief Variable que contiene el ancho de la pantalla en píxeles.
*/
extern const uint32_t screen_width;
/**
 * @brief Variable que contiene el alto de la pantalla en píxeles.
*/
extern const uint32_t screen_height;
/**
 * @brief Búffer que se utiliza para escribir la información en la pantalla.
*/
extern lv_disp_draw_buf_t draw_display_buffer;
/**
 * @brief Búffer que almacena la información de la pantalla .
*/
extern lv_color_t display_buffer;
/**
 * @brief Instancia de la pantalla LCD.
*/
extern Display display;
/**
 * @brief Instancia del panel táctil.
*/
extern FT6236 ts;

/* Functions */
/**
 * @brief Se encarga de dibujar la pantalla de bienvenida y permite configurar
 * la red WiFi. Si el usuario no introduce ningúna opción en 20 segundos, el
 * sistema proseguirá con la configuración guardada previamente.
*/
void drawStartWiFiMenuScreen(void);
/**
 * @brief Se encarga de dibujar la información en la pantalla con la información
 * de los buffers.
 * @param disp Búffer de la pantalla
 * @param area Área de la pantalla
 * @param color_p Información de color de la pantalla. 5 para rojo, 6 para verde y
 * 5 para azul.
*/
void drawDisplay(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
/**
 * @brief Se encarga de leer los puntos de presión en el panel táctil.
 * @param indev_driver Instancia del driver del panel táctil
 * @param data Información relacionada a las coordenadas x-y del panel.
*/
void readTouchpad(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
/**
 * @brief Se encarga iniciar y configurar el driver de la LCD y el panel táctil,
 * conectando estos a su vez con la librería LVGL.
*/
void configLVGLUI(void);
/**
 * @brief Fija el brillo que utilizará la pantalla para el resto de la sesión.
 * @param value Valor de brillo que va de 0 a 255.
*/
void setDisplayBrightness(const uint8_t value);
/**
 * @brief Muestra la ventana que permite al usuario borrar la configuración
 * WiFi de la memoria Flash.
*/
void drawCleanFlashMessageBox(void);
/**
 * @brief Lee un evento y cierra la ventana de gestión WiFi si se cumple.
 * @param e Tipo de evento registrado, es decir, si el botón ha sido pulsado o no.
*/
void uiEventCloseWiFiWindow( lv_event_t *e);
/**
 * @brief Lee un evento y borra la configuración WiFi si se cumple.
 * @param e Tipo de evento registrado, es decir, si el botón sido pulsado o no.
*/
void uiEventCleanFlash( lv_event_t *e);
/**
 * @brief Lee un evento y escribe la configuración WiFi en la flash si se cumple.
 * @param e Tipo de evento registrado, es decir, si el botón ha sido pulsado o no.
*/
void uiEventWriteWiFiCredentials( lv_event_t * e);
/**
 * @brief Muestra la configuración WiFi en la pantalla de bienvenida.
 * @param data de tipo WifiScanData, es una matriz de strings, la cual
 * contiene toda la información obtenida durante el escaneo de las redes WiFi.
*/
void drawWiFiMenu(WifiScanData data);

#endif