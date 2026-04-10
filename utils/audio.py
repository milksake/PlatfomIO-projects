# convertir.py
import sys

input_file = "escucha.wav"   # cambia por tu archivo
output_file = "escucha.h"
array_name = "escucha"

with open(input_file, "rb") as f:
    data = f.read()

with open(output_file, "w") as f:
    f.write(f"#pragma once\n")
    f.write(f"#include <pgmspace.h>\n\n")
    f.write(f"const uint8_t {array_name}[] PROGMEM = {{\n")
    for i, byte in enumerate(data):
        if i % 16 == 0:
            f.write("  ")
        f.write(f"0x{byte:02X}, ")
        if (i + 1) % 16 == 0:
            f.write("\n")
    f.write("\n};\n")
    f.write(f"const uint32_t {array_name}_len = {len(data)};\n")

print(f"Listo: {output_file} ({len(data)} bytes)")