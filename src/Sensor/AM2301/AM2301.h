#pragma once

#include "Sensor/SensorDef.h"
#include "../DHT22/DHT22.h"

static Sensor_t AM2301_Sensor = {
    .id = "am2301",
    .type = TEMPERATURE | HUMIDITY,
    .option = NULL,
    .error = WORK_WELL,
    .genOption = DHT22_getOption,
    .process = DHT22_process,
    .getValue = DHT22_getValue,
};
