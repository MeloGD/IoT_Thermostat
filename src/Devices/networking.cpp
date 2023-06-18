#include "Devices/networking.h"

/* Variables */
bool wifi_status = false;
const ulong wifi_timeout = 10 * 1000;
Preferences device_data;
WifiScanData wifi_data;
String payload;
IPAddress MQTT_SERVER(5 , 75 , 186,  218);
AsyncMqttClient mqtt_client;
// Timers
TimerHandle_t mqtt_reconnect_timer;
TimerHandle_t wifi_reconnect_timer;

/* Functions */
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
}

void writePasswordFlash(const char *password) {
  device_data.putString("PASSWORD", password);
}

const String readSSIDFlash(void) {
  const String ssid = device_data.getString("SSID", "");
  return ssid;
}

const String readPasswordFlash(void) {
  const String password = device_data.getString("PASSWORD", "");
  return password;
}

bool tryWiFi(const char *ssid, const char *password) {
  const ulong start_time = millis();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid , password);
  while (WiFi.status() != WL_CONNECTED && (millis() - start_time) < wifi_timeout) {
    vTaskDelay(250 / portTICK_RATE_MS);
    Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED) {
    Serial.print("\nCONECTADO");
    writeSSIDFlash(ssid);
    writePasswordFlash(password);
    return true;
  } else {
    Serial.print("\nNO CONECTADO");
    return false;
  }
}

void wifiEvent(WiFiEvent_t event) {
  switch (event) {
  case SYSTEM_EVENT_STA_GOT_IP:
    Serial.println("Evento: Se ha obtenido IP");
    lv_obj_add_state(ui_wifiswitch, LV_STATE_CHECKED);
    connectMQTT();
    xTimerStop(wifi_reconnect_timer, 0);
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    Serial.println("Se ha desconectado del WiFi.");
    xTimerStop(mqtt_reconnect_timer, 0);
    xTimerStart(wifi_reconnect_timer, 0);
    lv_obj_clear_state(ui_wifiswitch, LV_STATE_CHECKED);
    break;
  }
}

void connectWiFi(void) {
  Serial.println("Entrando al WiFi.");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.println("Número de redes disponibles: ");
  Serial.print(WiFi.scanNetworks());
  WiFi.begin(readSSIDFlash().c_str(), readPasswordFlash().c_str());
  while (WiFi.status() != WL_CONNECTED) { 
     delay(100);  
     Serial.print('.'); 
  }
  Serial.println("Conexión establecida.");
  Serial.println("IP:");
  Serial.println(WiFi.localIP());

}

void wifiReconnect(void) {
  WiFi.getAutoReconnect();
  while (WiFi.status() != WL_CONNECTED) { 
     delay(100);  
     Serial.print('.'); 
  }
}

void configMQTT(void) {
  mqtt_reconnect_timer = xTimerCreate("MQTTTimer",
                                      pdMS_TO_TICKS(2000),
                                      pdFALSE,
                                      (void*)0,
                                      reinterpret_cast<TimerCallbackFunction_t>(connectMQTT)
                                      );
  wifi_reconnect_timer = xTimerCreate("WiFiTimer",
                                      pdMS_TO_TICKS(5000),
                                      pdFALSE,
                                      (void*)0,
                                      reinterpret_cast<TimerCallbackFunction_t>(wifiReconnect)
                                      );
  mqtt_client.onConnect(onMQTTConnect);
  mqtt_client.onDisconnect(onMQTTDisconnect);
  mqtt_client.onSubscribe(onMQTTSubscribe);
  mqtt_client.onUnsubscribe(onMQTTUnsubscribe);
  mqtt_client.onMessage(onMQTTReceived);
  mqtt_client.onPublish(onMQTTPublish);
  mqtt_client.setServer( MQTT_SERVER , 30000);
}

void connectMQTT() {
  Serial.println("Conectando al servidor MQTT...");
  mqtt_client.connect();
}

void onMQTTConnect(bool status) {
  Serial.println("Conectado al servidor MQTT!");
  subscribeMQTT();
}

void onMQTTDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Desconectado del servidor MQTT.");
  if(WiFi.isConnected()) {
    xTimerStart(mqtt_reconnect_timer, 0);
  }
}

void onMQTTSubscribe(uint16_t packet_id, uint8_t qos) {
  Serial.println("Subscripción correcta!");
  Serial.print("  packetId: ");
  Serial.println(packet_id);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMQTTUnsubscribe(uint16_t packet_id) {
  Serial.println("Unsubscripción correcta.");
  Serial.print("  packetId: ");
  Serial.println(packet_id);
}

void onMQTTReceived(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Topico recibido: ");
  Serial.println(topic);
  Serial.print(" : ");

  String content = getPayloadContent(payload, len);
  
  StaticJsonDocument<300> received;
  DeserializationError error = deserializeJson(received, content);
  if (error) {
    Serial.println("ERROR!");
    return; 
  }
  
  float data = received["environment_temp"];
  Serial.print(data);
  Serial.println();
}

void onMQTTPublish(uint16_t packet_id) {
  Serial.println("Publicación reconocida: ");
  Serial.print("  packetId: ");
  Serial.println(packet_id);
}

void publishMQTT(unsigned long data) {
  payload = String(data);
  mqtt_client.publish("iot-esp32/tfg", 0, true, (char*)payload.c_str());
}

void subscribeMQTT(void) {
  uint16_t packetIdSub = mqtt_client.subscribe("iot-esp32/tfg", 0);
  Serial.print("Subscrito a QoS 0, id del paquete: ");
  Serial.println(packetIdSub);
}

String getPayloadContent(char* data, size_t len) {
  String content = "";
  for(size_t i = 0; i < len; i++) {
    content.concat(data[i]);
  }
  return content;
}
