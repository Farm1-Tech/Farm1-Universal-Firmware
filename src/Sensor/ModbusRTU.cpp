#include "Arduino.h"
#include "./ModbusRTU.h"
#include "Board/Board.h"

#define RS485_CH_NUM 2

HardwareSerial RS485(RS485_CH_NUM);

#define MODE_SEND HIGH
#define MODE_RECV LOW

uint16_t CRC16(uint8_t *buf, int len) {
	uint16_t crc = 0xFFFF;
	for (uint16_t pos = 0; pos < len; pos++) {
		crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
		for (int i = 8; i != 0; i--) { // Loop over each bit
			if ((crc & 0x0001) != 0)
			{			   // If the LSB is set
				crc >>= 1; // Shift right and XOR 0xA001
				crc ^= 0xA001;
			}
			else
			{			   // Else LSB is not set
				crc >>= 1; // Just shift right
			}
		}
	}

	return crc;
}

ModbusRTUStatus_t ModbudRTUReadInputRegister(ModbusDeviceConfigs_t configs, uint16_t start_address, uint8_t len, uint8_t *data) {
    pinMode(RS485_DIR_PIN, OUTPUT);
	digitalWrite(RS485_DIR_PIN, MODE_RECV);

	RS485.begin(configs.serial.baudrate, configs.serial.config, RS485_RX_PIN, RS485_TX_PIN); // Rx, Tx
	RS485.setTimeout(100);

	uint8_t buff[] = {
		configs.id, // Devices Address
		0x04, // Function code
		(uint8_t)(start_address >> 8), // Start Address HIGH
		(uint8_t)(start_address & 0xFF), // 0x01, // Start Address LOW
		0x00, // Quantity HIGH
		len, // 0x02, // Quantity LOW
		0,	  // CRC LOW
		0	  // CRC HIGH
	};

	uint16_t crc = CRC16(&buff[0], 6);
	buff[6] = crc & 0xFF;
	buff[7] = (crc >> 8) & 0xFF;

	digitalWrite(RS485_DIR_PIN, MODE_SEND);
	RS485.write(buff, sizeof(buff));
	RS485.flush(); // wait MODE_SEND completed
	digitalWrite(RS485_DIR_PIN, MODE_RECV);

    uint16_t read_byte_count = 3 + (len * 2) + 2; // ID, Function Code, Byte count, [...data], checksum
    uint8_t buff_read[read_byte_count];
    uint8_t read_len = RS485.readBytes(buff_read, read_byte_count);
    if (read_len == 0) {
        Serial.println("ERROR Timeout");
        return ERROR_TIMEOUT;
    }

	if (read_len != read_byte_count) {
        Serial.println("ERROR read size");
        return ERROR_INVALID_PACKET;
    }

    if (buff_read[0] != configs.id) {
        Serial.println("ERROR device id");
        return ERROR_INVALID_PACKET;
    }

	if (buff_read[1] != 0x04) {
        Serial.println("ERROR function code");
        return ERROR_INVALID_PACKET;
    }

	if (buff_read[2] != (len * 2)) {
        Serial.println("ERROR byte count");
        return ERROR_INVALID_PACKET;
    }

	memcpy(data, &buff_read[3], len * 2);

	return MODBUS_OK;
}
