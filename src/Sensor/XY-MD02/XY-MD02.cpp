#include "./XY-MD02.h"
#include "Sensor/ModbusRTU.h"

#define XY_MD02_DEFAULT_ID 1

static XY_MD02_Option_t GlobalOption = {
    .id = XY_MD02_DEFAULT_ID,
    .baudrate = 9600
};

void XY_MD02_getOption(void* args, JsonObject jsonOption) {
    Sensor_t *self = (Sensor_t *) args;

    if (jsonOption.containsKey("id")) {
        GlobalOption.id = jsonOption["id"].as<unsigned int>();
        if ((GlobalOption.id <= 0) || (GlobalOption.id >= 255)) {
            GlobalOption.id = XY_MD02_DEFAULT_ID;
        }
    }
    if (jsonOption.containsKey("baudrate")) {
        GlobalOption.baudrate = jsonOption["baudrate"].as<unsigned long>();
    }

    self->option = &GlobalOption;
}

void XY_MD02_process(void*) {

}

ModbusDeviceConfigs_t modbus_configs = {
    .id = XY_MD02_DEFAULT_ID,
    .serial = {
        .baudrate = 9600,
        .config = SERIAL_8N1
    }
};

SensorStatus_t XY_MD02_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    if (self->option) {
        XY_MD02_Option_t *configs = (XY_MD02_Option_t *) self->option;
        modbus_configs.id = configs->id;
        modbus_configs.serial.baudrate = configs->baudrate;
    }

    float *value_f = (float *)value;

#define READ_TYPE_FROM_MODBUS(TYPE_OF_READ, REGISTER) ({ \
    if (type == TYPE_OF_READ) { \
        uint8_t buff[2]; \
        if (ModbudRTUReadInputRegister(modbus_configs, REGISTER, 1, buff) != MODBUS_OK) { \
            *value_f = -999.0; \
            return READ_FAIL; \
        } \
        *value_f = ((uint16_t)(buff[0] << 8) | buff[1]) / 10.0f; \
        return WORK_WELL; \
    } \
})

    READ_TYPE_FROM_MODBUS(TEMPERATURE, 0x01);
    READ_TYPE_FROM_MODBUS(HUMIDITY, 0x02);

    return TYPE_NOT_SUPPORT;
}
