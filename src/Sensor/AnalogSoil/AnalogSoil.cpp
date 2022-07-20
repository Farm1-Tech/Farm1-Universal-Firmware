#include "./AnalogSoil.h"
#include "Board/Board.h"
#include "../ESP32AnalogRead.h"

#define ANALOG_SOIL_DEFAULT_PIN ONBOARD_ANALOG0_PIN

static float map_f(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static AnalogSoil_Option_t GlobalOption = {
    .pin = ANALOG_SOIL_DEFAULT_PIN,
    .min = 0.0f,
    .max = 3.3f
};

void  AnalogSoil_getOption(void* args, JsonObject jsonOption) {
    Sensor_t *self = (Sensor_t *) args;

    if (jsonOption.containsKey("pin")) {
        GlobalOption.pin = jsonOption["pin"].as<unsigned int>();
    }

    if (jsonOption.containsKey("min")) {
        GlobalOption.min = jsonOption["min"].as<float>();
    }

    if (jsonOption.containsKey("max")) {
        GlobalOption.max = jsonOption["max"].as<float>();
    }

    self->option = &GlobalOption;
}

void AnalogSoil_process(void* args) {

}

SensorStatus_t AnalogSoil_getValue(void* args, SensorType_t type, void* value) {
    Sensor_t *self = (Sensor_t *) args;

    uint8_t pin = ANALOG_SOIL_DEFAULT_PIN;
    uint16_t min_v = 1.5f;
    uint16_t max_v = 2.8f;
    if (self->option) {
        AnalogSoil_Option_t *option = (AnalogSoil_Option_t*) self->option;
        pin = option->pin;
        min_v = option->min;
        max_v = option->max;
    }

    float *value_f = (float *)value;
    if (type == SOIL) {
        ESP32AnalogRead adc;
        adc.attach(pin);
        float realVolt = adc.readVoltage() / 3.3f * 9.9f;
        // Serial.printf("Volt = %.02f\n", realVolt);

        *value_f = ((realVolt - min_v) / (max_v - min_v)) * 100.0f;
        *value_f = 100.0f - (*value_f);
        *value_f = min(max(*value_f, 0.0f), 100.0f);
        return WORK_WELL;
    }

    return TYPE_NOT_SUPPORT;
}

