#pragma once

#include "Sensor/SensorDef.h"

void AnalogSoil_getOption(void*, JsonObject) ;
void AnalogSoil_process(void*) ;
SensorStatus_t AnalogSoil_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t pin;
    float min;
    float max;
} AnalogSoil_Option_t;

static Sensor_t AnalogSoil_Sensor = {
    .id = "analog-soil",
    .type = SOIL,
    .option = NULL,
    .error = WORK_WELL,
    .genOption = AnalogSoil_getOption,
    .process = AnalogSoil_process,
    .getValue = AnalogSoil_getValue,
};
