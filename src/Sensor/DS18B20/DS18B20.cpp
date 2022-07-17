#include "./DS18B20.h"
#include "./OneWire.h"
#include "./DallasTemperature.h"

#define DS18B20_DEFAULT_PIN 26

OneWire oneWire;
DallasTemperature sensors(&oneWire);

static DS18B20_Option_t GlobalOption = {
    .pin = DS18B20_DEFAULT_PIN
};

void DS18B20_getOption(void* args, JsonObject jsonOption) {
    Sensor_t *self = (Sensor_t *) args;

    if (jsonOption.containsKey("pin")) {
        GlobalOption.pin = jsonOption["pin"].as<unsigned int>();
    }

    self->option = &GlobalOption;
}

void DS18B20_process(void* args) {
    // Sensor_t *self = (Sensor_t *) args;

}

static bool init_sensor = false;

SensorStatus_t DS18B20_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    if (!init_sensor) {
        if (self->option) {
            oneWire.begin(((DS18B20_Option_t*)(self->option))->pin);
        } else {
            oneWire.begin(DS18B20_DEFAULT_PIN); // Fix to D3 of Farm1
        }
        sensors.begin();
        init_sensor = true;
    }

    float *value_f = (float *)value;

    if (type == TEMPERATURE) {
        sensors.requestTemperatures(); 
        *value_f = sensors.getTempCByIndex(0);
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}
