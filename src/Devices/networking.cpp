#include "Devices/networking.h"

WifiScanData getWiFiSSIDs(void) {
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

void initFlashMemmory(void) {
  device_data.begin("credentials", false);
}

void writeSSIDFlash(const char *ssid) {
  device_data.putString("SSID", ssid);
  //device_data.end();
}

void writePasswordFlash(const char *password) {
  device_data.putString("PASSWORD", password);
  //device_data.end();
}

const String readSSIDFlash(void) {
  const String ssid = device_data.getString("SSID", "");
  //device_data.end();
  return ssid;
}

const String readPasswordFlash(void) {
  const String password = device_data.getString("PASSWORD", "");
  //device_data.end();
  return password;
}
