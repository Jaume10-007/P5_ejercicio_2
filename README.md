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
📤 2. Envío de comando de medición
0xAC 0x33 0x00
📥 3. Lectura de datos

Se leen 7 bytes:

Byte	Contenido
0	Estado
1-3	Humedad RAW
3-5	Temperatura RAW
6	CRC
🧮 4. Cálculo de humedad
humidity = raw * 100 / 2^20
🌡️ 5. Cálculo de temperatura
temperature = raw * 200 / 2^20 - 50
🧪 6. Verificación CRC8

Se usa polinomio:

0x31

Si el CRC coincide → ✅ OK
Si no → ❌ FAIL

🖥️ Ejemplo de salida
=== Monitor sensor 0x38: Humedad y Temperatura ===

--------------------------------------------------
Tiempo: 12345 ms

Estado: 0x18  busy=no  | calibrado=si

Bytes: 0x18 0x66 0x80 0x00 0x7A 0x10 0x5C

Humedad: 45.23 %RH
Temperatura: 23.67 C

CRC: OK
⚠️ Posibles errores
❌ Sensor no detectado
sensor 0x38 NO detectado

✔️ Revisar conexiones SDA, SCL, VCC, GND

❌ Error de lectura
error al leer datos del sensor

✔️ Revisar cableado o timing

❌ CRC FAIL
CRC FAIL

✔️ Datos corruptos → interferencias o fallo I2C

🧩 Estructura del código
🔹 isSensorPresent() → detecta sensor
🔹 requestMeasurement() → inicia medición
🔹 readSensorData() → lee datos
🔹 calculateCRC8() → valida datos
🔹 loop() → ejecución principal cada 2s
🛠️ Configuración
constexpr uint8_t SENSOR_ADDRESS = 0x38;
constexpr size_t READ_BYTES = 7;
constexpr unsigned long LOOP_DELAY_MS = 2000;
📚 Librerías usadas
Arduino.h
Wire.h
🎯 Objetivo

Practicar:

Comunicación I2C
Lectura de sensores digitales
Procesado de datos RAW
Verificación de integridad (CRC)
Debug por Serial
