#pragma once

#include "./SensorDef.h"

// Sensor Header
#include "./SHT20/SHT20.h"
#include "./SHT30/SHT30.h"

// Sensor list defined
const Sensor_t Sensor[] = {
    SHT20_Sensor,
    SHT30_Sensor
};
