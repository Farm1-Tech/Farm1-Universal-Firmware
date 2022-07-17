#include "./BH1750.h"
#include "Sensor/I2CDev.h"

#define BH1750_ADDR 0x23
#define BH1750_ADDR2 0x5C

static BH1750_Option_t GlobalOption = {
    .addr = BH1750_ADDR
};

void BH1750_getOption(void* args, JsonObject jsonOption) {
    Sensor_t *self = (Sensor_t *) args;

    if (jsonOption.containsKey("address")) {
        GlobalOption.addr = I2CAddressCStringToInt(jsonOption["address"].as<const char*>());
        if ((GlobalOption.addr != BH1750_ADDR) && (GlobalOption.addr != BH1750_ADDR2)) {
            GlobalOption.addr = BH1750_ADDR;
        }
    }

    self->option = &GlobalOption;
}

void BH1750_process(void* args) {
    // Sensor_t *self = (Sensor_t *) args;

}

static bool init_sensor = false;

SensorStatus_t BH1750_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    uint8_t addr = BH1750_ADDR;
    if (self->option) {
        addr = ((BH1750_Option_t*)(self->option))->addr;
    }

    if (!init_sensor) {
        init_sensor = I2CWrite(addr, (uint8_t*) "\x10", 1) == 0; // Continuously H-Resolution Mode
        if (!init_sensor) {
            return INIT_FAIL;
        }

        delay(200);
    }

    float *value_f = (float *)value;
    if (type == AMBIENT_LIGHT) {
        uint8_t buff[2];
        if (I2CRead(addr, NULL, 0, buff, 2) != 0) {
            return READ_FAIL;
        }

        *value_f = (uint16_t)((((uint16_t)buff[0]) << 8) | buff[1]) / 1.2f;
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}
