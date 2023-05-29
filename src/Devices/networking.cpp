#include "Devices/networking.h"

WifiScanData getWiFiSSIDs(void) {
  //wifi_data.clear();
  WifiScanData wifi_data;
  int networks_number = WiFi.scanNetworks();
  wifi_data.resize(networks_number);
  for (int i = 0; i < wifi_data.size(); i++) {
    wifi_data[i].resize(2);
    wifi_data[i][0] = WiFi.SSID(i);      
    if (WiFi.RSSI(i) <= (-67)) {
        wifi_data[i][1] = "Señal mala";
    } else if (WiFi.RSSI(i) > (-66) && WiFi.RSSI(i) <= (-50)) {
        wifi_data[i][1] = "Señal media";
    } else {
        wifi_data[i][1] = "Señal buena";
    }
  }
  return wifi_data; 
}