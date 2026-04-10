# Custom Clock ESP32 

- **Sonido**: (formato RTTTL) usando el DAC que ya trae el ESP32 adentro, gracias a la librería `ESP8266Audio`.
- **WiFi**: Funciona como cliente y servidor para controlarlo por internet.
- **Movimiento y control**: Mueve motores para las manecillas o funciones físicas y lee lo que tocas en un teclado matricial.

- **Placa**: uPESY ESP32 Wroom
- **Librerías principales**: `ESP8266Audio` (para el sonido), `Keypad` (para el teclado) y `Stepper` (para los motores). Todo programado sobre el framework de Arduino en PlatformIO.

## Cómo compilar y subir el código

Como hay un montón de cosas mezcladas, dividí el proyecto en varios entornos (`environments`) en PlatformIO. Así puedo probar, por ejemplo, solo el sonido sin tener que compilar la parte del servidor web. 

Aquí te dejo los comandos rápidos para la consola:
- `pio run -e mac -t upload -t monitor` → Para probar la configuración de la MAC.
- `pio run -e server -t upload -t monitor` → Para compilar y subir el servidor web.
- `pio run -e client -t upload -t monitor` → Para subir el código del cliente WiFi.
- `pio run -e key -t upload -t monitor` → Para probar si el teclado matricial responde bien.
- `pio run -e run --file serverControl.cpp` → Si solo quieres compilar el servidor para ver si hay errores, sin subirlo.
- `pio device monitor` → Para abrir el monitor serie y ver qué está pasando (a 115200 baudios).



Si vas a meterle tus propios audios o canciones y necesitas convertirlos o procesarlos antes de pasarlos al ESP32, te súper recomiendo usar `pydub` en Python. Te ahorra muchos dolores de cabeza. Lo instalas con:

```bash
pip install pydub
```

## Estructura del Código Fuente (`src/`)
- `main.cpp`: Ejecución principal e integradora.
- `sonido.cpp`: Procesador de I2S y melodías (Requiere ejecución en bucle no bloqueante).
- `clientControl.cpp` / `serverControl.cpp`: Lógica de red.
- `keycontrol.cpp`: Entradas del usuario.
- `servo.cpp` / `servo2.cpp`: Control de actuadores.

---
*Desarrollado con PlatformIO y C++.*
