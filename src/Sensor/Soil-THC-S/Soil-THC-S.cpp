#include "./Soil-THC-S.h"
#include "Sensor/ModbusRTU.h"

#define SOIL_THC_S_TR_DEFAULT_ID 1

void SOIL_THC_S_process(void*) {
    // Sensor_t *self = (Sensor_t *) args;

}

SensorStatus_t SOIL_THC_S_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    ModbusDeviceConfigs_t modbus_configs = {
        .id = SOIL_THC_S_TR_DEFAULT_ID,
        .serial = {
            .baudrate = 4800,
            .config = SERIAL_8N1
        }
    };
    if (self->option) {
        SOIL_THC_S_Option_t *configs = (SOIL_THC_S_Option_t *) self->option;
        modbus_configs.id = configs->id;
        modbus_configs.serial.baudrate = configs->baudrate;
    }

    float *value_f = (float *)value;

#define READ_TYPE_FROM_MODBUS(TYPE_OF_READ, REGISTER) ({ \
    if (type == TYPE_OF_READ) { \
        uint8_t buff[2]; \
        ModbudRTUReadInputRegister(modbus_configs, REGISTER, 1, buff); \
        *value_f = ((uint16_t)(buff[0] << 8) | buff[1]) / 10.0f; \
        return WORK_WELL; \
    } \
})

    READ_TYPE_FROM_MODBUS(SOIL, 0x00);

    return TYPE_NOT_SUPPORT;
}
