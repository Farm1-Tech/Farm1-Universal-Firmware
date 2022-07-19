#include "./AnalogSoil.h"
#include "Board/Board.h"

#define ANALOG_SOIL_DEFAULT_PIN ONBOARD_ANALOG0_PIN

static float map_f(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static AnalogSoil_Option_t GlobalOption = {
    .pin = ANALOG_SOIL_DEFAULT_PIN,
    .min = 0,
    .max = 4095
};

void  AnalogSoil_getOption(void* args, JsonObject jsonOption) {
    Sensor_t *self = (Sensor_t *) args;

    if (jsonOption.containsKey("pin")) {
        GlobalOption.pin = jsonOption["pin"].as<unsigned int>();
    }

    if (jsonOption.containsKey("min")) {
        GlobalOption.min = jsonOption["min"].as<unsigned int>();
    }

    if (jsonOption.containsKey("max")) {
        GlobalOption.max = jsonOption["max"].as<unsigned int>();
    }

    self->option = &GlobalOption;
}

void AnalogSoil_process(void* args) {

}

SensorStatus_t AnalogSoil_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    uint8_t pin = ANALOG_SOIL_DEFAULT_PIN;
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
        *value_f = map_f(analogRead(pin), 0, 4095, min, max);
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}

