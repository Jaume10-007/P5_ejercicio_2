#include <Arduino.h>
#include <Wire.h>

constexpr uint8_t SENSOR_ADDRESS = 0x38;
constexpr uint8_t MEASURE_COMMAND = 0xAC;
constexpr uint8_t MEASURE_ARG1 = 0x33;
constexpr uint8_t MEASURE_ARG2 = 0x00;
constexpr size_t READ_BYTES = 7;
constexpr unsigned long LOOP_DELAY_MS = 2000;

uint8_t calculateCRC8(const uint8_t *data, size_t length) {
  uint8_t crc = 0xFF;
  for (size_t i = 0; i < length; ++i) {
    crc ^= data[i];
    for (uint8_t bit = 0; bit < 8; ++bit) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

bool isSensorPresent() {
  Wire.beginTransmission(SENSOR_ADDRESS);
  return Wire.endTransmission() == 0;
}

bool requestMeasurement() {
  Wire.beginTransmission(SENSOR_ADDRESS);
  Wire.write(MEASURE_COMMAND);
  Wire.write(MEASURE_ARG1);
  Wire.write(MEASURE_ARG2);
  return Wire.endTransmission() == 0;
}

bool readSensorData(uint8_t *buffer, size_t length) {
  if (Wire.requestFrom(SENSOR_ADDRESS, static_cast<uint8_t>(length)) != length) {
    return false;
  }
  for (size_t i = 0; i < length; ++i) {
    buffer[i] = Wire.read();
  }
  return true;
}

void printHex(uint8_t value) {
  if (value < 0x10) Serial.print('0');
  Serial.print(value, HEX);
}

void printBytes(const uint8_t *data, size_t length) {
  for (size_t i = 0; i < length; ++i) {
    Serial.print("0x");
    printHex(data[i]);
    if (i + 1 < length) {
      Serial.print(' ');
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Wire.begin();
  delay(100);
  Serial.println();
  Serial.println("=== Monitor sensor 0x38: Humedad y Temperatura ===");
}

void loop() {
  Serial.println("--------------------------------------------------");
  Serial.print("Tiempo: ");
  Serial.print(millis());
  Serial.println(" ms");

  if (!isSensorPresent()) {
    Serial.println("Estado: sensor 0x38 NO detectado. Revisa SDA, SCL, GND y VCC.");
    Serial.println();
    delay(LOOP_DELAY_MS);
    return;
  }

  if (!requestMeasurement()) {
    Serial.println("Estado: error al enviar comando de medicion.");
    Serial.println();
    delay(LOOP_DELAY_MS);
    return;
  }

  delay(80);

  uint8_t buffer[READ_BYTES] = {0};
  if (!readSensorData(buffer, READ_BYTES)) {
    Serial.println("Estado: error al leer datos del sensor.");
    Serial.println();
    delay(LOOP_DELAY_MS);
    return;
  }

  uint8_t status = buffer[0];
  bool busy = (status & 0x80) != 0;
  bool calibrated = (status & 0x08) != 0;

  Serial.print("Estado: 0x");
  printHex(status);
  Serial.print("  busy=");
  Serial.print(busy ? "si" : "no");
  Serial.print("  | calibrado=");
  Serial.println(calibrated ? "si" : "no");

  Serial.print("Bytes: ");
  printBytes(buffer, READ_BYTES);

  uint32_t humidityRaw = (static_cast<uint32_t>(buffer[1]) << 12) |
                         (static_cast<uint32_t>(buffer[2]) << 4) |
                         (static_cast<uint32_t>(buffer[3]) >> 4);

  uint32_t temperatureRaw = ((static_cast<uint32_t>(buffer[3]) & 0x0F) << 16) |
                            (static_cast<uint32_t>(buffer[4]) << 8) |
                            static_cast<uint32_t>(buffer[5]);

  float humidity = humidityRaw * 100.0f / 1048576.0f;
  float temperature = temperatureRaw * 200.0f / 1048576.0f - 50.0f;

  Serial.print("Humedad: ");
  Serial.print(humidity, 2);
  Serial.println(" %RH");

  Serial.print("Temperatura: ");
  Serial.print(temperature, 2);
  Serial.println(" C");

  uint8_t crc = calculateCRC8(buffer, READ_BYTES - 1);
  Serial.print("CRC: ");
  if (crc == buffer[READ_BYTES - 1]) {
    Serial.println("OK");
  } else {
    Serial.print("FAIL (esperado 0x");
    printHex(crc);
    Serial.print(" != 0x");
    printHex(buffer[READ_BYTES - 1]);
    Serial.println(")");
  }

  Serial.println();
  delay(LOOP_DELAY_MS);
}
