#include "./JXBS-3001-TR.h"
#include "Sensor/ModbusRTU.h"

#define JXBS_3001_TR_DEFAULT_ID 1

void JXBS_3001_TR_process(void*) {

}

SensorStatus_t JXBS_3001_TR_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    ModbusDeviceConfigs_t modbus_configs = {
        .id = JXBS_3001_TR_DEFAULT_ID,
        .serial = {
            .baudrate = 9600,
            .config = SERIAL_8N1
        }
    };
    if (self->option) {
        JXBS_3001_TR_Option_t *configs = (JXBS_3001_TR_Option_t *) self->option;
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

    READ_TYPE_FROM_MODBUS(SOIL, 0x02);
    READ_TYPE_FROM_MODBUS(TEMPERATURE, 0x03);

    return TYPE_NOT_SUPPORT;
}
