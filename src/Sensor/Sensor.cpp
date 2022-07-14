#include "./Sensor.h"

SensorStatus_t Sensor_getValueOne(SensorType_t type, void* value) {
    for (uint8_t i=0;i<Sensor_length;i++) {
        if ((Sensor[i].type & type) != 0) {
            return Sensor[i].getValue((void*)&Sensor[i], type, value);
        }
    }
    
    return TYPE_NOT_SUPPORT;
}

