#pragma once

#include "Sensor/SensorDef.h"

void BH1750_process(void*) ;
SensorStatus_t BH1750_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t addr;
} BH1750_Option_t;

static Sensor_t BH1750_Sensor = {
    .id = "bh1750",
    .type = AMBIENT_LIGHT,
    .option = NULL,
    .error = WORK_WELL,
    .process = BH1750_process,
    .getValue = BH1750_getValue,
};
