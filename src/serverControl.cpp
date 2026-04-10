#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;

const int IN1 = 27;
const int IN2 = 26;
const int IN3 = 25;
const int IN4 = 33;

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

typedef struct {
  int boton;
} Mensaje;

Mensaje mensaje;

int posicionActual = 0;

void moverA(int angulo)
{
  int pasosDestino = (angulo * stepsPerRevolution) / 360;
  int pasosActual = (posicionActual * stepsPerRevolution) / 360;

  int pasosMover = pasosDestino - pasosActual;

  myStepper.step(pasosMover);

  posicionActual = angulo;
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&mensaje, incomingData, sizeof(mensaje));

  Serial.print("Boton recibido: ");
  Serial.println(mensaje.boton);

  if (mensaje.boton == 1) moverA(0);
  if (mensaje.boton == 2) moverA(90);
  if (mensaje.boton == 3) moverA(180);
  if (mensaje.boton == 4) moverA(270);
}

void setup() {

  Serial.begin(115200);

  myStepper.setSpeed(12);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}