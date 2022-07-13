#pragma once

#include <stdint.h>

enum ModbusRTUStatus_t {
  MODBUS_OK = 0,
  ERROR_TIMEOUT = -1,
  ERROR_INVALID_PACKET = -2,
  ERROR_CRC_CHECK = -3,
};

typedef struct {
  uint32_t baudrate;
  uint32_t config;
} ModbusSerialConfigs_t;

typedef struct {
  uint8_t id;
  ModbusSerialConfigs_t serial;
} ModbusDeviceConfigs_t;

void ModbusRTUInit() ;
ModbusRTUStatus_t ModbudRTUReadInputRegister(ModbusDeviceConfigs_t configs, uint16_t start_address, uint8_t len, uint8_t *data) ;
