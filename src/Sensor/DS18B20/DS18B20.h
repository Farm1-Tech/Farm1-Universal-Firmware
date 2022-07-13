#pragma once

#include "Sensor/SensorDef.h"

void DS18B20_process(void*) ;
SensorStatus_t DS18B20_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t pin;
} DS18B20_Option_t;

static Sensor_t DS18B20_Sensor = {
    .id = "ds18b20",
    .type = TEMPERATURE,
    .option = NULL,
    .error = WORK_WELL,
    .process = DS18B20_process,
    .getValue = DS18B20_getValue,
};
