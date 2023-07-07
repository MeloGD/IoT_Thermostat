/**
 * @brief Fichero que contiene la clase que configura el driver del panel táctil. Todos los
 * valores de este fichero son estáticos para funcionar con el modelo FT6236 de Focaltech.
 * Lo único que se ha añadido han sido los pines del bus I2C, que se comentan más abajo.
 * Documentación de la librería: 
 * https://github.com/DustinWatts/FT6236/tree/main
 * @file ft6236.h
 * @author Jesús Carmelo González Domínguez
 * @date 14-07-2023
*/

#ifndef FT6236_GUARD
#define FT6236_GUARD

#include "Arduino.h"
#include <Wire.h>

#define FT6236_ADDR 0x38           // I2C address
#define FT6236_G_FT5201ID 0xA8     // FocalTech's panel ID
#define FT6236_REG_NUMTOUCHES 0x02 // Number of touch points

#define FT6236_NUM_X 0x33 // Touch X position
#define FT6236_NUM_Y 0x34 // Touch Y position

#define FT6236_REG_MODE 0x00        // Device mode, either WORKING or FACTORY
#define FT6236_REG_CALIBRATE 0x02   // Calibrate mode
#define FT6236_REG_WORKMODE 0x00    // Work mode
#define FT6236_REG_FACTORYMODE 0x40 // Factory mode
#define FT6236_REG_THRESHHOLD 0x80  // Threshold for touch detection
#define FT6236_REG_POINTRATE 0x88   // Point rate
#define FT6236_REG_FIRMVERS 0xA6    // Firmware version
#define FT6236_REG_CHIPID 0xA3      // Chip selecting
#define FT6236_REG_VENDID 0xA8      // FocalTech's panel ID

#define FT6236_VENDID 0x11  // FocalTech's panel ID
#define FT6206_CHIPID 0x06  // FT6206 ID
#define FT6236_CHIPID 0x36  // FT6236 ID
#define FT6236U_CHIPID 0x64 // FT6236U ID

#define FT6236_DEFAULT_THRESHOLD 128 // Default threshold for touch detection

/**
 * @brief Pin SDA asignado al panel táctil. Se obtuvo mirando los schematics de la placa
 * de desarrollo: 
 * https://github.com/Makerfabs/Makerfabs-ESP32-S3-SPI-TFT-with-Touch/blob/main/hardware/ESP32-S3%20TFT%20Touch%20v1.1(3.5''%20ili9488).PDF
 * 
*/ 
#define SDA_FT6236 38
/**
 * @brief Pin SCL asignado al panel táctil. Se obtuvo mirando los schematics de la placa
 * de desarrollo: 
 * https://github.com/Makerfabs/Makerfabs-ESP32-S3-SPI-TFT-with-Touch/blob/main/hardware/ESP32-S3%20TFT%20Touch%20v1.1(3.5''%20ili9488).PDF
 * 
*/ 
#define SCL_FT6236 39


class TS_Point
{
public:
  TS_Point(void);
  TS_Point(int16_t x, int16_t y, int16_t z);

  bool operator==(TS_Point);
  bool operator!=(TS_Point);

  int16_t x;
  int16_t y;
  int16_t z;
};

class FT6236
{
public:
  FT6236(void);
  void debug(void);
  boolean begin(uint8_t thresh = FT6236_DEFAULT_THRESHOLD, int8_t sda = -1, int8_t scl = -1);
  uint8_t touched(void);
  TS_Point getPoint(uint8_t n = 0);

private:
  void writeRegister8(uint8_t reg, uint8_t val);
  uint8_t readRegister8(uint8_t reg);

  void readData(void);
  uint8_t touches;
  uint16_t touchX[2], touchY[2], touchID[2];
};

#endif