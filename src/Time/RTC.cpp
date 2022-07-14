#include <Wire.h>
#include "Sensor/I2CDev.h"
#include "./RTC.h"

static bool CheckI2CDevice(int addr) {
    Wire.beginTransmission(addr);
    return Wire.endTransmission() == 0;
}

static uint8_t BCDtoDEC(uint8_t n) {
    return ((n >> 4)  * 10) + (n & 0x0F);
}

static uint8_t DECtoBCD(uint8_t n) {
    return (((n / 10) << 4) & 0xF0) | ((uint8_t)(n % 10) & 0x0F);
}

static RTC_Type type = DS1338;

bool RTC_init() {
    if (CheckI2CDevice(DS1338)) {
        type = DS1338;
        Serial.println("RTC chip found DS1338");
    } else if (CheckI2CDevice(MCP79411)) {
        type = MCP79411;
        Serial.println("RTC chip found MCP79411");
    } else if (CheckI2CDevice(PCF8563)) {
        type = PCF8563;
        Serial.println("RTC chip found PCF8563");
    } else {
        Serial.println("Error, Not found RTC device");
        return false;
    }

    if (type == DS1338) {
        if (I2CWrite(DS1338, (uint8_t*) "\x07\x00", 2) != 0) {
            return false;
        }
    }

    if (type == MCP79411) {
        uint8_t rtcsec = 0;
        if (I2CRead(MCP79411, (uint8_t*) "\x00", 1, &rtcsec, 1) != 0) {
            return false;
        }
        Serial.println((byte) rtcsec, HEX);
        
        if ((rtcsec & 0x80) == 0) { // ST flag is 0
            Serial.printf("Set start flag 0x%02x\n", rtcsec);
            uint8_t write_data[2] = {
                0x00,
                (uint8_t)(rtcsec | 0x80)
            };
            if (I2CWrite(MCP79411, write_data, 2) != 0) {
                return false;
            }
        }

        if (I2CWrite(MCP79411, (uint8_t*) "\x07\x00", 2) != 0) { // Write 0 to EXTOSC flag, Disable external 32.768 kHz input
            return false;
        }
    }

    if (type == PCF8563) {
        if (I2CWrite(PCF8563, (uint8_t*) "\x00\x00", 2) != 0) { // Write 0 to Oscillator Stop Flag for start
            return false;
        }
    }

    return true;
}

bool RTC_read(struct tm* timeinfo) {
    if (!timeinfo) {
        return false;
    }

    uint8_t buff[7];
    if (I2CRead(type, (uint8_t*)(type == PCF8563 ? "\x02" : "\x00"), 1, buff, 7) != 0) {
        return false;
    }

    timeinfo->tm_sec = BCDtoDEC(buff[0] & 0x7F);
    timeinfo->tm_min = BCDtoDEC(buff[1] & 0x7F);
    timeinfo->tm_hour = BCDtoDEC(buff[2] & 0x3F);
    if (type == PCF8563) {
        timeinfo->tm_mday = BCDtoDEC(buff[3] & 0x3F);
        timeinfo->tm_wday = BCDtoDEC(buff[4] & 0x07);
    } else {
        timeinfo->tm_wday = BCDtoDEC(buff[3] & 0x07);
        timeinfo->tm_mday = BCDtoDEC(buff[4] & 0x3F);
    }
    timeinfo->tm_mon = BCDtoDEC(buff[5] & 0x1F);
    timeinfo->tm_year = BCDtoDEC(buff[6]) + 2000 - 1900;

    return true;
}

bool RTC_write(struct tm* timeinfo) {
    uint8_t buff[7];
    buff[0] = DECtoBCD(timeinfo->tm_sec) & 0x7F;
    if (type == MCP79411) {
        buff[0] |= 0x80;
    }
    buff[1] = DECtoBCD(timeinfo->tm_min) & 0x7F;
    buff[2] = DECtoBCD(timeinfo->tm_hour) & 0x3F;
    if (type == PCF8563) {
        buff[3] = DECtoBCD(timeinfo->tm_mday) & 0x3F;
        buff[4] = DECtoBCD(timeinfo->tm_wday) & 0x07;
    } else {
        buff[3] = DECtoBCD(timeinfo->tm_wday) & 0x07;
        if (type == MCP79411) {
            buff[3] |= (1 << 3); // Set VBATEN flag
        }
        buff[4] = DECtoBCD(timeinfo->tm_mday) & 0x3F;
    }
    buff[5] = DECtoBCD(timeinfo->tm_mon) & 0x1F;
    buff[6] = DECtoBCD((timeinfo->tm_year + 1900) % 100);

    uint8_t write_data[1 + 7]; // Address + data
    write_data[0] = type == PCF8563 ? 0x02 : 0x00;
    memcpy(&write_data[1], &buff[0], 7);
    if (I2CWrite(type, write_data, 1 + 7) != 0) {
        return false;
    }
    
    return true;
}