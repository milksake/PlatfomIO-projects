#include <Arduino.h>
#include <ESP32Servo.h>

// Variables para nuestro relojito interno (el cronómetro)
int contador = 0;
unsigned long tiempoAnterior = 0;

// Pines donde conectamos los botones
const int BOTON_1 = 18;
const int BOTON_2 = 19;
const int BOTON_3 = 21;
const int BOTON_4 = 22;

// Pin donde va el cable de señal (el amarillo/naranja) del Servo
const int PIN_SERVO = 13;
Servo miServo;

void setup() {
  Serial.begin(115200);

  // Activamos las resistencias internas (PULLUP) para los botones. 
  // Así nos ahorramos poner resistencias físicas en la protoboard.
  pinMode(BOTON_1, INPUT_PULLUP);
  pinMode(BOTON_2, INPUT_PULLUP);
  pinMode(BOTON_3, INPUT_PULLUP);
  pinMode(BOTON_4, INPUT_PULLUP);

  // Enganchamos el servo a su pin y lo mandamos a la posición inicial (casi 0 grados)
  miServo.attach(PIN_SERVO);
  miServo.write(3);
}

void loop() {
  // Checamos cuánto tiempo (en milisegundos) ha pasado desde que arrancó el ESP32
  unsigned long tiempoActual = millis();
  
  // Si ya pasó 1 segundo (1000 ms), imprimimos el contador y le sumamos uno.
  // ¡Lo genial de esto es que hace una pausa sin frenar el resto del programa!
  if (tiempoActual - tiempoAnterior >= 1000) {
    tiempoAnterior = tiempoActual;
    Serial.print("Contador: ");
    Serial.println(contador);
    contador++;
  }

  // Checamos los botones (con PULLUP, si lee LOW significa que lo están presionando).
  // Tienen un delay chiquito para que no detecte un montón de clics seguidos por accidente.
  if (digitalRead(BOTON_1) == LOW) { 
    Serial.println("Boton 1 pulsado - Moviendo a 0 grados"); 
    miServo.write(3); 
    delay(200); 
  }
  if (digitalRead(BOTON_2) == LOW) { 
    Serial.println("Boton 2 pulsado - Moviendo a 60 grados"); 
    miServo.write(60); 
    delay(200); 
  }
  if (digitalRead(BOTON_3) == LOW) { 
    Serial.println("Boton 3 pulsado - Moviendo a 120 grados"); 
    miServo.write(120); 
    delay(200); 
  }
  if (digitalRead(BOTON_4) == LOW) { 
    Serial.println("Boton 4 pulsado - Moviendo a 180 grados"); 
    miServo.write(180); 
    delay(200); 
  }
}