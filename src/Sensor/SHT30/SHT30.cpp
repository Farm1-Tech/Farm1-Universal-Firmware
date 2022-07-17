#include "./SHT30.h"
#include "Sensor/I2CDev.h"

#define SHT30_ADDR 0x44
#define SHT30_ADDR2 0x45

// --------
static SHT30_Option_t GlobalOption = {
    .addr = SHT30_ADDR
};

void SHT30_getOption(void* args, JsonObject jsonOption) {
    Sensor_t *self = (Sensor_t *) args;

    if (jsonOption.containsKey("address")) {\
        GlobalOption.addr = I2CAddressCStringToInt(jsonOption["address"].as<const char*>());
        if ((GlobalOption.addr != SHT30_ADDR) && (GlobalOption.addr != SHT30_ADDR2)) {
            GlobalOption.addr = SHT30_ADDR;
        }
    }

    self->option = &GlobalOption;
}

void SHT30_process(void* args) {
    // Sensor_t *self = (Sensor_t *) args;

}

static bool init_sht = false;

SensorStatus_t SHT30_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;
    
    uint8_t addr = SHT30_ADDR;
    if (self->option) {
        addr = ((SHT30_Option_t*)(self->option))->addr;
    }
    
    if (!init_sht) {
        init_sht = I2CWrite(addr, (uint8_t*)"\x27\x37", 2) == 0; // Soft reset
        if (!init_sht) {
            Serial.printf("SHT30 Soft reset fail\n");
            return INIT_FAIL;
        }
        delay(20);
    }

    float *value_f = (float *)value;
    if ((type == TEMPERATURE) || (type == HUMIDITY)) {
        if (I2CWrite(addr, (uint8_t*)"\xE0\x00", 2) != 0) {
            Serial.printf("SHT30 Triger measurement fail\n");
            return READ_FAIL;
        }
        
        delay(10);
            
        uint8_t buff[6];
        if (I2CRead(addr, NULL, 0, buff, 6) != 0) {
            Serial.printf("SHT30 Read measurement fail\n");
            return READ_FAIL;
        }
        float tempC = -45.0 + 175.0 * ((float)(((uint16_t)buff[0] << 8) | buff[1]) / 0xFFFF);
        float humiRH = 100.0 * ((float)(((uint16_t)buff[3] << 8) | buff[4]) / 0xFFFF);

        if (type == TEMPERATURE) {
            *value_f = tempC;
        }
        if (type == HUMIDITY) {
            *value_f = humiRH;
        }
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}
