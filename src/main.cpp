#include <Arduino.h>

// Pines donde conecté los botones
const int BOTON_1 = 12;
const int BOTON_2 = 14;
const int BOTON_3 = 33;
const int BOTON_4 = 32;

// Dónde va conectado el motorcito Servo
const int PIN_SERVO = 13;

void setup() {
  Serial.begin(115200);

  // Usamos el PULLUP interno para no tener que poner resistencias físicas
  // Esto significa que cuando el botón no se presiona, lee HIGH. Y presionado lee LOW.
  pinMode(BOTON_1, INPUT_PULLUP);
  pinMode(BOTON_2, INPUT_PULLUP);
  pinMode(BOTON_3, INPUT_PULLUP);
  pinMode(BOTON_4, INPUT_PULLUP);
}

void loop() {
  // Vamos a ir checando botón por botón.
  // El pequeño delay de 200ms es un "anti-rebote" casero para no saturar el monitor serie.
  if (digitalRead(BOTON_1) == LOW) { 
    Serial.println("Boton 1 pulsado - Moviendo a 0 grados"); 
   
    delay(200); 
  }
  if (digitalRead(BOTON_2) == LOW) { 
    Serial.println("Boton 2 pulsado - Moviendo a 60 grados"); 

    delay(200); 
  }
  if (digitalRead(BOTON_3) == LOW) { 
    Serial.println("Boton 3 pulsado - Moviendo a 120 grados"); 
  
    delay(200); 
  }
  if (digitalRead(BOTON_4) == LOW) { 
    Serial.println("Boton 4 pulsado - Moviendo a 180 grados"); 
  
    delay(200); 
  }
}
