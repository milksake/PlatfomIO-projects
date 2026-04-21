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

volatile int posicionActual = 0;
volatile int pending = 0;

void moverA2(int pasosDestino)
{
  int pasosActual = posicionActual;

  int pasosMover = pasosDestino - pasosActual;
  
  Serial.print("\tSteps: ");
  Serial.println(pasosMover);

  if (pasosMover >= 0)
    myStepper.step(pasosMover);
  else
  {
    myStepper.step(pasosMover - 300);
    myStepper.step(300);
  }

  posicionActual = pasosDestino;
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  if (pending)
    return;

  memcpy(&mensaje, incomingData, sizeof(mensaje));

  Serial.print("Boton recibido: ");
  Serial.println(mensaje.boton);

  pending = mensaje.boton;
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
  if (pending)
  {
    // 2796 - 1398 - 233

    if (pending == 1) moverA2(0);        // 01:15
    if (pending == 2) moverA2(233 * 3);  // 01:00
    if (pending == 3) moverA2(233 * 5);  // 12:50
    if (pending == 4) moverA2(233 * 6);  // 12:45
    
    // if (pending == 4) myStepper.step(10);

    pending = 0;
  }
}