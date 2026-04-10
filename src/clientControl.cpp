#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// BOTONES
const int BOTON_1 = 12;
const int BOTON_2 = 14;
const int BOTON_3 = 33;
const int BOTON_4 = 32;

// MAC del ESP32 receptor
uint8_t broadcastAddress[] = {0xF0,0x24,0xF9,0x45,0xD6,0x8C};

// estructura del mensaje
typedef struct {
  int boton;
} Mensaje;

Mensaje mensaje;

esp_now_peer_info_t peerInfo;

void setup() {

  Serial.begin(115200);

  pinMode(BOTON_1, INPUT_PULLUP);
  pinMode(BOTON_2, INPUT_PULLUP);
  pinMode(BOTON_3, INPUT_PULLUP);
  pinMode(BOTON_4, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void enviar(int boton)
{
  mensaje.boton = boton;

  esp_now_send(broadcastAddress, (uint8_t *) &mensaje, sizeof(mensaje));

  Serial.print("Enviado boton: ");
  Serial.println(boton);
}

void loop() {

  if (digitalRead(BOTON_1) == LOW) {
    enviar(1);
    delay(300);
  }

  if (digitalRead(BOTON_2) == LOW) {
    enviar(2);
    delay(300);
  }

  if (digitalRead(BOTON_3) == LOW) {
    enviar(3);
    delay(300);
  }

  if (digitalRead(BOTON_4) == LOW) {
    enviar(4);
    delay(300);
  }
}