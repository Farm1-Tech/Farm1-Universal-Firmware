#pragma once

#include <stdint.h>
#include "Sensor/SensorDef.h"

void XY_MD02_getOption(void*, JsonObject) ;
void XY_MD02_process(void*) ;
SensorStatus_t XY_MD02_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t id;
    uint32_t baudrate;
} XY_MD02_Option_t;

static Sensor_t XY_MD02_Sensor = {
    .id = "xy-md02",
    .type = TEMPERATURE | HUMIDITY,
    .option = NULL,
    .error = WORK_WELL,
    .genOption = XY_MD02_getOption,
    .process = XY_MD02_process,
    .getValue = XY_MD02_getValue,
};
