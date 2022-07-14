#include "./AnalogSoil.h"

#define ANALOG_PIN 0

void AnalogSoil_process(void* args) {

}

SensorStatus_t AnalogSoil_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    uint8_t pin = ANALOG_PIN;
    uint16_t min = 0;
    uint16_t max = 4095;
    if (self->option) {
        AnalogSoil_Option_t *option = (AnalogSoil_Option_t*) self->option;
        pin = option->pin;
        min = option->min;
        max = option->max;
    }

    float *value_f = (float *)value;
    if (type == SOIL) {
        *value_f = map(analogRead(pin), 0, 4095, min, max);
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}
