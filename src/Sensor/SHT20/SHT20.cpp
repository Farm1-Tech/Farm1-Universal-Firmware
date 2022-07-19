#include "./SHT20.h"
#include "../I2CDev.h"

Artron_SHT20::Artron_SHT20(TwoWire* wire) : _wire(wire) {

}

bool Artron_SHT20::begin() {
    return write(0b11111110) == 0;
}

float Artron_SHT20::readTemperature() {
    uint8_t buff[3];
    if (this->read(0b11110011, buff, 3, 100) != 0) { // Trigger T measurement, no hold master, wait 100 mS for measurement
      // Serial.printf("SHT20 Read Temperature measurement fail\n");
      return -999;
    }

    return -46.85 + 175.72 * (int)((buff[0] << 8) | buff[1]) / (float)pow(2, 16);
}

float Artron_SHT20::readHumidity() {
    uint8_t buff[3];
    if (this->read(0b11110101, buff, 3, 100) != 0) { // Trigger RH measurement, no hold master, wait 100 mS for measurement
      // Serial.printf("SHT20 Read Humidity measurement fail\n");
      return -999;
    }

    return -6 + 125 * (int)((buff[0] << 8) | buff[1]) / (float)pow(2, 16);
}

int Artron_SHT20::write(uint8_t command, uint8_t *data, uint8_t len) {
    uint8_t buff[1 + len];
    buff[0] = command;
    if (data && (len > 0)) {
        memcpy(&buff[1], data, len);
    }

    return I2CWrite(SHT20_ADDR, buff, 1 + len);
}

int Artron_SHT20::read(uint8_t command, uint8_t *data, int len, int stop) {
    I2CWrite(SHT20_ADDR, &command, 1);
    delay(stop);
    return I2CRead(SHT20_ADDR, NULL, 0, data, len);
}

static Artron_SHT20 sht(&Wire);

static bool init_sht = false;
#define CHECK_SENSOR    ({ \
                            if (!init_sht) {\
                                init_sht = sht.begin() ? true : false;\
                            }\
                            \
                            if (!init_sht) {\
                                return -10;\
                            }\
                        })


// --------
void SHT20_process(void* args) {
    // Sensor_t *self = (Sensor_t *) args;

}

SensorStatus_t SHT20_getValue(void* args, SensorType_t type, void* value) {
    // Sensor_t *self = (Sensor_t *) args;
    
    if (!init_sht) {
        init_sht = sht.begin() ? true : false;
        if (init_sht) {
            delay(10);
        } else {
            return INIT_FAIL;
        }
    }

    float *value_f = (float *)value;
    if (type == TEMPERATURE) {
        *value_f = sht.readTemperature();
    }

    if (type == HUMIDITY) {
        *value_f = sht.readHumidity();
    }

    if (*value_f == -999.0f) {
        init_sht = false;
        return READ_FAIL;
    } else if ((type & (TEMPERATURE | HUMIDITY)) != 0) {
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}