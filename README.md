# 🌡️ Monitor I2C de Temperatura y Humedad (Sensor 0x38)

Proyecto en ESP32 + PlatformIO para leer temperatura y humedad desde un sensor I2C (dirección 0x38) y mostrar los resultados por el Monitor Serie.

---

## 🚀 Descripción

Este programa detecta si el sensor está conectado, envía el comando de medición, lee los datos (7 bytes), convierte los valores RAW a humedad (%RH) y temperatura (°C), verifica la integridad mediante CRC8 y muestra toda la información por Serial.

---

## ⚙️ Hardware necesario

- ESP32 (ej: ESP32-S3 DevKit)  
- Sensor de temperatura y humedad I2C (0x38)  
- Conexiones:
  - SDA → GPIO (ej: 8)  
  - SCL → GPIO (ej: 9)  
  - VCC → 3.3V  
  - GND → GND  

---

## 🔌 Comunicación I2C

Dirección del sensor: 0x38  
Bytes leídos: 7  
Frecuencia de lectura: cada 2 segundos  

---

## 🧠 Funcionamiento interno

### 1. Comprobación del sensor
Se verifica si el dispositivo responde en el bus I2C mediante:

Wire.beginTransmission(0x38);
Wire.endTransmission();
2. Envío de comando de medición

Se envía el comando:

0xAC 0x33 0x00
3. Lectura de datos

Se leen 7 bytes con la siguiente estructura:

Byte	Contenido
0	Estado
1-3	Humedad RAW
3-5	Temperatura RAW
6	CRC
4. Cálculo de humedad
humidity = raw * 100 / 2^20
5. Cálculo de temperatura
temperature = raw * 200 / 2^20 - 50
6. Verificación CRC8

Se utiliza el polinomio 0x31. Si coincide → OK, si no → FAIL.

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

Sensor no detectado:

sensor 0x38 NO detectado

→ Revisar SDA, SCL, VCC y GND

Error de lectura:

error al leer datos del sensor

→ Revisar cableado o timing

CRC incorrecto:

CRC FAIL

→ Posible corrupción de datos o interferencias I2C

🧩 Estructura del código
isSensorPresent() → detección del sensor
requestMeasurement() → inicio de medición
readSensorData() → lectura de datos
calculateCRC8() → validación de datos
loop() → ejecución cada 2 segundos
🛠️ Configuración
constexpr uint8_t SENSOR_ADDRESS = 0x38;
constexpr size_t READ_BYTES = 7;
constexpr unsigned long LOOP_DELAY_MS = 2000;
📚 Librerías usadas
Arduino.h
Wire.h
🎯 Objetivo

Practicar comunicación I2C, lectura de sensores digitales, procesado de datos RAW, verificación de integridad (CRC) y debug por Serial.
