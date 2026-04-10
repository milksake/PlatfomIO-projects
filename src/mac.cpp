#include <WiFi.h>

void setup(){
  Serial.begin(115200);
  delay(2000);   // esperar a que abra el monitor

  WiFi.mode(WIFI_STA);

  Serial.println("MAC ESP32:");
  Serial.println(WiFi.macAddress());
}

void loop(){}