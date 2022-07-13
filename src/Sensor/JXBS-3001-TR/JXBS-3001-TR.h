#pragma once

#include <stdint.h>
#include "Sensor/SensorDef.h"

void JXBS_3001_TR_process(void*) ;
SensorStatus_t JXBS_3001_TR_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t id;
    uint32_t baudrate;
} JXBS_3001_TR_Option_t;

static Sensor_t JXBS_3001_TR_Sensor = {
    .id = "jxbs-3001-tr",
    .type = SOIL | TEMPERATURE,
    .option = NULL,
    .error = WORK_WELL,
    .process = JXBS_3001_TR_process,
    .getValue = JXBS_3001_TR_getValue,
};
