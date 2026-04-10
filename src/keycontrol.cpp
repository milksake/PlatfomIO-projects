#include <Keypad.h>
#include <Arduino.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorRTTTL.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "timbre.h"
#include "escucha.h"

// ─── Keypad ───────────────────────────────────────────────
const int baris = 4;
const int kolom = 4;

char keys[baris][kolom] = {
    {'1', '4', '7', '*'},
    {'2', '5', '8', '0'},
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'},
};
byte rowspin[baris] = {14, 12, 21, 18};
byte colspin[kolom]  = {5, 4, 2, 15};
Keypad keypad = Keypad(makeKeymap(keys), rowspin, colspin, baris, kolom);

// ─── LEDs ─────────────────────────────────────────────────
#define LED_VERDE 13
#define LED_ROJO  26

// ─── Audio ────────────────────────────────────────────────
AudioGeneratorRTTTL    *rtttl;
AudioGeneratorWAV      *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2S         *out;

// Beeps RTTTL por tecla
const char PROGMEM beep_1[]    = "t:d=4,o=6,b=200:16c";
const char PROGMEM beep_2[]    = "t:d=4,o=6,b=200:16d";
const char PROGMEM beep_3[]    = "t:d=4,o=6,b=200:16e";
const char PROGMEM beep_4[]    = "t:d=4,o=6,b=200:16f";
const char PROGMEM beep_5[]    = "t:d=4,o=6,b=200:16g";
const char PROGMEM beep_6[]    = "t:d=4,o=6,b=200:16a";
const char PROGMEM beep_7[]    = "t:d=4,o=6,b=200:16b";
const char PROGMEM beep_8[]    = "t:d=4,o=7,b=200:16c";
const char PROGMEM beep_9[]    = "t:d=4,o=7,b=200:16d";
const char PROGMEM beep_0[]    = "t:d=4,o=5,b=200:16b";
const char PROGMEM beep_A[]    = "t:d=4,o=6,b=200:16c#";
const char PROGMEM beep_B[]    = "t:d=4,o=6,b=200:16d#";
const char PROGMEM beep_C[]    = "t:d=4,o=6,b=200:16f#";
const char PROGMEM beep_D[]    = "t:d=4,o=6,b=200:16g#";
const char PROGMEM beep_star[] = "t:d=4,o=5,b=200:16g";

// ─── Estado de llamada ────────────────────────────────────
#define NUMERO_ESPECIAL "351103340"

String numeroMarcado = "";

// 0 = sin audio
// 1 = beep RTTTL
// 2 = timbre WAV
// 3 = escucha WAV
int estadoAudio = 0;

// ─── Funciones audio ──────────────────────────────────────
void reproducirWAV(const uint8_t* data, uint32_t len) {
  if (rtttl->isRunning()) rtttl->stop();
  if (wav->isRunning())   wav->stop();
  if (file) { delete file; file = nullptr; }
  file = new AudioFileSourcePROGMEM(data, len);
  wav->begin(file, out);
}

void reproducirRTTTL(const char* melodia) {
  if (wav->isRunning())   wav->stop();
  if (rtttl->isRunning()) rtttl->stop();
  if (file) { delete file; file = nullptr; }
  file = new AudioFileSourcePROGMEM(melodia, strlen_P(melodia));
  rtttl->begin(file, out);
}

const char* obtenerBeep(char key) {
  switch (key) {
    case '1': return beep_1;
    case '2': return beep_2;
    case '3': return beep_3;
    case '4': return beep_4;
    case '5': return beep_5;
    case '6': return beep_6;
    case '7': return beep_7;
    case '8': return beep_8;
    case '9': return beep_9;
    case '0': return beep_0;
    case 'A': return beep_A;
    case 'B': return beep_B;
    case 'C': return beep_C;
    case 'D': return beep_D;
    default:  return beep_1;
  }
}

void parpadearRojo() {
  digitalWrite(LED_ROJO, HIGH);
  delay(80);
  digitalWrite(LED_ROJO, LOW);
}

// ─── Setup ────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Teclado listo:");

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO,  OUTPUT);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_ROJO,  LOW);

  out   = new AudioOutputI2S(0, AudioOutputI2S::INTERNAL_DAC);
  rtttl = new AudioGeneratorRTTTL();
  wav   = new AudioGeneratorWAV();
  file  = nullptr;
}

// ─── Loop ─────────────────────────────────────────────────
void loop() {

  // ── Gestionar audio ──
  if (estadoAudio == 1) {
    // RTTTL beep
    if (rtttl && rtttl->isRunning()) {
      if (!rtttl->loop()) {
        rtttl->stop();
        if (file) { delete file; file = nullptr; }
        estadoAudio = 0;
      }
    }
  } else if (estadoAudio == 2) {
    // Timbre WAV → cuando termina pasa a escucha
    if (wav && wav->isRunning()) {
      if (!wav->loop()) {
        wav->stop();
        if (file) { delete file; file = nullptr; }
        // Timbre terminó → reproducir escucha
        Serial.println(">>> CONTESTARON — reproduciendo escucha...");
        reproducirWAV(escucha, escucha_len);
        estadoAudio = 3;
      }
    }
  } else if (estadoAudio == 3) {
    // Escucha WAV
    if (wav && wav->isRunning()) {
      if (!wav->loop()) {
        wav->stop();
        if (file) { delete file; file = nullptr; }
        Serial.println(">>> Llamada terminada.");
        estadoAudio = 0;
      }
    }
  }

  // ── Leer teclado ──
  char key = keypad.getKey();
  if (key == NO_KEY) return;

  parpadearRojo();

  // * = borrar
  if (key == '*') {
    if (numeroMarcado.length() > 0)
      numeroMarcado.remove(numeroMarcado.length() - 1);
    reproducirRTTTL(beep_star);
    estadoAudio = 1;
    Serial.print("Borrado. Actual: ");
    Serial.println(numeroMarcado);
    return;
  }

  // # = llamar
  if (key == '#') {
    Serial.print(">>> LLAMANDO A: ");
    Serial.println(numeroMarcado);

    if (numeroMarcado == NUMERO_ESPECIAL) {
      // Número especial → timbre + luego escucha automático
      Serial.println(">>> Número especial — sonando timbre...");
      reproducirWAV(timbre, timbre_len);
      estadoAudio = 2;  // cuando termine el timbre → pasa a escucha
    } else {
      // Cualquier otro número → solo timbre
      reproducirWAV(timbre, timbre_len);
      estadoAudio = 2;
    }

    numeroMarcado = "";
    return;
  }

  // Tecla normal → beep
  numeroMarcado += key;
  reproducirRTTTL(obtenerBeep(key));
  estadoAudio = 1;
  Serial.print("Tecla: ");
  Serial.print(key);
  Serial.print("  |  Marcado: ");
  Serial.println(numeroMarcado);
}