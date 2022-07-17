#include "Arduino.h"
#include "Wire.h"

#include "../Board/Board.h"

SemaphoreHandle_t I2CSemaphore;

void I2CSetup() {
  I2CSemaphore = xSemaphoreCreateMutex();

  /* ------- Setup I2C0 ------- */
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); // SDA, SCL
  Wire.setClock(400E3); // Set clock to 400 kHz
  /* ------- END of Setup I2C0 ------- */

  xSemaphoreGive(I2CSemaphore);
}

int I2CWrite(uint8_t addr, uint8_t *data, uint8_t len, bool stop) {
  if (xSemaphoreTake(I2CSemaphore, 100 / portTICK_PERIOD_MS) == pdFALSE) {
    return -1;
  }

  Wire.beginTransmission(addr);
  Wire.write(data, len);
  int res = Wire.endTransmission(stop);

  xSemaphoreGive(I2CSemaphore);

  if (res != 0) {
    Serial.printf("I2C [0x%02X] write Error code %d\n", addr, res);
  }

  return res;
}

int I2CRead(uint8_t addr, uint8_t *writeData, uint8_t writeLen, uint8_t *readData, uint8_t readLen) {
  if (writeLen > 0) {
    if (I2CWrite(addr, writeData, writeLen, false) != 0) {
      return -1;
    }
  }
  
  if (xSemaphoreTake(I2CSemaphore, 100 / portTICK_PERIOD_MS) == pdFALSE) {
    return 0;
  }

  int n = Wire.requestFrom(addr, readLen);
  Wire.readBytes(readData, n);

  if (n != readLen) {
    Serial.printf("I2C [0x%02X] read error\n", addr);
  }

  xSemaphoreGive(I2CSemaphore);

  return n == readLen ? 0 : -1;
}

int I2CAddressCStringToInt(const char* str) {
  unsigned int addr = 0;
  if (str && strlen(str) > 2) {
    sscanf(str, "0x%X", &addr);
  }
  return addr;
}
