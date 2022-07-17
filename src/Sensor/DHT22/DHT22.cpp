#include "./DHT22.h"
#include "./DHT22_lib.h"

#define DHT22_DEFAULT_PIN 26

static DHT22_Option_t GlobalOption = {
    .pin = DHT22_DEFAULT_PIN
};

void DHT22_getOption(void* args, JsonObject jsonOption) {
    Sensor_t *self = (Sensor_t *) args;

    if (jsonOption.containsKey("pin")) {
        GlobalOption.pin = jsonOption["pin"].as<unsigned int>();
    }

    self->option = &GlobalOption;
}

void DHT22_process(void* args) {
    // Sensor_t *self = (Sensor_t *) args;

}

// static bool init_dht = false;

SensorStatus_t DHT22_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;
    
    if (self->option) {
        setDHTgpio(((DHT22_Option_t*)(self->option))->pin);
    } else {
        setDHTgpio(DHT22_DEFAULT_PIN); // Fix to D3 of Farm1
    }

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
