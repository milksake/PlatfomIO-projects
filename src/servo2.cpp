/* ==================== EJEMPLO 1: TECLADO MATRICIAL ==================== */
/*
#include <Keypad.h>

// Nuestro teclado tiene 4 filas y 4 columnas
const int filas = 4;
const int columnas = 4;

// Así están distribuidos los botones físicamente en el pad
char teclas[filas][columnas] = {
    {'1', '4', '7', '*'},
    {'2', '5', '8', '0'},
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'},
};

// A qué pines del ESP32 conectamos las filas y las columnas
byte pinesFilas[filas] = {14, 12, 21, 18};
byte pinesColumnas[columnas] = {5, 4, 2, 15};

// Creamos el objeto del teclado con toda esa info
Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

void setup() {
  Serial.begin(115200);
  Serial.println("¡Teclado listo! Presiona cualquier tecla...");
}

void loop() {
  // Le preguntamos al teclado si alguien presionó algo
  char teclaPresionada = teclado.getKey();

  // Si sí presionaron algo (NO_KEY significa "nada"), lo mostramos en pantalla
  if (teclaPresionada != NO_KEY) {
    Serial.print(teclaPresionada);
  }
}
*/

/* ==================== EJEMPLO 2: MOTOR A PASOS ==================== */
/*
#include <Arduino.h>
#include <Stepper.h>

// Los pasos que necesita este motorcito (28BYJ-48) para dar una vuelta completa.
const int pasosPorVuelta = 2048;

// Pines donde conectamos la placa controladora del motor (el driver ULN2003)
const int IN1 = 27;
const int IN2 = 26;
const int IN3 = 25;
const int IN4 = 33;

// ¡Cuidado! Para que este motor gire bien, el orden de los pines al crearlo 
// debe ser saltado: IN1, IN3, IN2, IN4.
Stepper miMotor(pasosPorVuelta, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);
  Serial.println("Arrancando prueba del motor a pasos...");

  // Velocidad del motor (Revoluciones por Minuto). Este modelo es medio lento, 
  // así que 12 RPM es una buena velocidad para empezar.
  miMotor.setSpeed(12);
}

void loop() {
  Serial.println("Girando para un lado...");
  miMotor.step(pasosPorVuelta);  // Da una vuelta entera
  delay(1000); // Descansa un segundito

  Serial.println("Girando para el lado contrario...");
  miMotor.step(-pasosPorVuelta); // Da una vuelta entera al revés (por el signo menos)
  delay(1000); 
}
*/