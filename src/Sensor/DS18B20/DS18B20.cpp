#include "./DS18B20.h"
#include "./OneWire.h"
#include "./DallasTemperature.h"

OneWire oneWire;
DallasTemperature sensors(&oneWire);

void DS18B20_process(void* args) {
    // Sensor_t *self = (Sensor_t *) args;

}

static bool init_sensor = false;

SensorStatus_t DS18B20_getValue(void* args, SensorType_t type, void* value) {
    // Sensor_t *self = (Sensor_t *) args;

    if (!init_sensor) {
        oneWire.begin(4); // Fix to D3 of Farm1
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
