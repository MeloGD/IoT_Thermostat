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

bool tryWiFi(const char *ssid, const char *password) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  const ulong start_time = millis();
  WiFi.begin(ssid , password);
  while (WiFi.status() != WL_CONNECTED && (millis() - start_time) < wifi_timeout) {
    vTaskDelay(250 / portTICK_RATE_MS);
    Serial.print(".");
  }
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.print("\nCONECTADO");
    writeSSIDFlash(ssid);
    writePasswordFlash(password);
    WiFi.disconnect();
    return true;
  } else {
    Serial.print("\nNO CONECTADO");
    WiFi.disconnect();
    return false;
  }
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}


bool connectWiFi(void) {

  if (WiFi.status() == WL_DISCONNECTED) {
    Serial.print("\n Hora antes1: \n");
    printLocalTime();
    WiFi.mode(WIFI_STA);
    WiFi.begin(readSSIDFlash().c_str(), readPasswordFlash().c_str());
    const ulong start_time = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start_time) < wifi_timeout) {
      vTaskDelay(250 / portTICK_RATE_MS);
    }
    Serial.print("\n IP Asignada\n");
    Serial.print(WiFi.localIP());
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    } else {
      return false;
    }
  } else {
    Serial.print("\n Hora antes2: \n");
    printLocalTime();
    configTime(0,3600, "pool.ntp.org"); 
    vTaskDelay(2000 / portTICK_RATE_MS);
    Serial.print("\n Hora despues: \n");
    printLocalTime();
    return false;
  }

}