#pragma once
#include "Sensor/SensorDef.h"

void SHT30_getOption(void*, JsonObject) ;
void SHT30_process(void*) ;
SensorStatus_t SHT30_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t addr;
} SHT30_Option_t;

static Sensor_t SHT30_Sensor = {
    .id = "sht30",
    .type = TEMPERATURE | HUMIDITY,
    .option = NULL,
    .error = WORK_WELL,
    .genOption = SHT30_getOption,
    .process = SHT30_process,
    .getValue = SHT30_getValue,
};
