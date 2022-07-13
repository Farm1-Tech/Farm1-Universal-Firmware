#pragma once

#include <stdint.h>
#include "Sensor/SensorDef.h"

void SOIL_THC_S_process(void*) ;
SensorStatus_t SOIL_THC_S_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t id;
    uint32_t baudrate;
} SOIL_THC_S_Option_t;

static Sensor_t SOIL_THC_S_Sensor = {
    .id = "soil-thc-s",
    .type = SOIL,
    .option = NULL,
    .error = WORK_WELL,
    .process = SOIL_THC_S_process,
    .getValue = SOIL_THC_S_getValue,
};
