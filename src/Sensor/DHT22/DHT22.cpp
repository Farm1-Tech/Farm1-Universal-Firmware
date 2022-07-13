#include "./DHT22.h"
#include "./DHT22_lib.h"

void DHT22_process(void* args) {
    // Sensor_t *self = (Sensor_t *) args;

}

// static bool init_dht = false;

SensorStatus_t DHT22_getValue(void* args, SensorType_t type, void* value) {
    // Sensor_t *self = (Sensor_t *) args;
    
    setDHTgpio(4); // Fix to D3 of Farm1

    float *value_f = (float *)value;

    if (readDHT() != DHT_OK) {
        return READ_FAIL;
    }

    if (type == TEMPERATURE) {
        *value_f = getTemperature();
        return WORK_WELL;
    }

    if (type == HUMIDITY) {
        *value_f = getHumidity();
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}
