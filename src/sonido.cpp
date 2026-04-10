#include <Arduino.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorRTTTL.h"
#include "AudioOutputI2S.h"

/* ========================================================================= *
 *                            OBJETOS DE AUDIO                               *
 * ========================================================================= */
AudioGeneratorRTTTL *rtttl;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;

/* Melodía en formato RTTTL (Ejemplo: Tema de Mario Bros) almacenada en memoria Flash (PROGMEM) */
const char PROGMEM melodia[] = "Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e";

// Declaración de la función encargada de iniciar la reproducción
void reproducirSonido();

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando sistema de sonido...");

  // Configura la salida de audio hacia el DAC interno del ESP32 (Pines 25 y 26 por defecto)
  out = new AudioOutputI2S(0, AudioOutputI2S::INTERNAL_DAC);
  rtttl = new AudioGeneratorRTTTL();
  file = nullptr;

  // Iniciamos la reproducción de la música inmediatamente
  reproducirSonido();
}

void loop() {
  // IMPORTANTE: El método loop() del generador debe llamarse constantemente.
  // Evitar el uso de delay() o tareas bloqueantes en esta función para que el audio no tartamudee.
  if (rtttl && rtttl->isRunning()) {
    if (!rtttl->loop()) {
      // Si la reproducción ha terminado, detenemos el generador y liberamos la memoria del archivo
      rtttl->stop();
      if (file) { delete file; file = nullptr; }
      
      // Volvemos a reproducir para crear un bucle infinito
      reproducirSonido();
    }
  }
}

/**
 * @brief Inicializa o reinicia la reproducción de la melodía actual.
 * Si la melodía ya está sonando, no hace nada. De lo contrario, carga la melodía
 * desde la memoria flash y arranca el generador de audio.
 */
void reproducirSonido() {
  if (!rtttl->isRunning()) {
    if (file) { delete file; file = nullptr; }
    file = new AudioFileSourcePROGMEM(melodia, strlen(melodia));
    rtttl->begin(file, out);
  }
}