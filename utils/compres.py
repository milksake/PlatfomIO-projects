from pydub import AudioSegment

audio = AudioSegment.from_wav("escucha.wav")

# Recortar a máximo 3 segundos
audio = audio[:3000]

# Convertir a mono, 8000Hz, 8 bits
audio = audio.set_channels(1)
audio = audio.set_frame_rate(8000)
audio = audio.set_sample_width(1)  # 1 byte = 8 bits

audio.export("escucha_corto.wav", format="wav")
print(f"Listo: {len(audio)/1000:.1f}s — {len(audio.raw_data)/1024:.1f} KB")