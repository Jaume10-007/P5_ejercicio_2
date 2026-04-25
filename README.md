# 🌡️ Monitor I2C de Temperatura y Humedad (Sensor 0x38)

Proyecto en **ESP32 + PlatformIO** para leer **temperatura y humedad** desde un sensor I2C (dirección `0x38`) y mostrar los resultados por el **Monitor Serie**.

---

## 🚀 Descripción

Este programa:

- 🔍 Detecta si el sensor está conectado (I2C 0x38)
- 📡 Envía comando de medición
- 📥 Lee los datos del sensor (7 bytes)
- 🧮 Convierte los datos RAW a:
  - Humedad (%RH)
  - Temperatura (°C)
- 🧪 Verifica la integridad con **CRC8**
- 🖥️ Muestra toda la información por Serial

---

## ⚙️ Hardware necesario

- 🧠 ESP32 (ej: ESP32-S3 DevKit)
- 🌡️ Sensor de temperatura y humedad (I2C, dirección `0x38`)
- 🔌 Conexiones:
  - SDA → GPIO (ej: 8)
  - SCL → GPIO (ej: 9)
  - VCC → 3.3V
  - GND → GND

---

## 🔌 Comunicación I2C

- Dirección del sensor: `0x38`
- Bytes leídos: `7`
- Frecuencia de lectura: cada `2 segundos`

---

## 🧠 Funcionamiento interno

### 📡 1. Comprobación del sensor
Verifica si el dispositivo responde en I2C:

```cpp
Wire.beginTransmission(0x38);
Wire.endTransmission();
