#pragma once

#include "Sensor/SensorDef.h"

void DHT22_getOption(void*, JsonObject) ;
void DHT22_process(void*) ;
SensorStatus_t DHT22_getValue(void*, SensorType_t, void*) ;

// Sensor definded
typedef struct {
    uint8_t pin;
} DHT22_Option_t;

static Sensor_t DHT22_Sensor = {
    .id = "dht22",
    .type = TEMPERATURE | HUMIDITY,
    .option = NULL,
    .error = WORK_WELL,
    .genOption = DHT22_getOption,
    .process = DHT22_process,
    .getValue = DHT22_getValue,
};
