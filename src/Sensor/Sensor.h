#pragma once

#include "./SensorDef.h"

// Sensor Header
#include "./SHT20/SHT20.h"
#include "./SHT30/SHT30.h"
#include "./XY-MD02/XY-MD02.h"
#include "./DHT22/DHT22.h"
#include "./DS18B20/DS18B20.h"

// Sensor list defined
const Sensor_t Sensor[] = {
    SHT20_Sensor,
    SHT30_Sensor,
    XY_MD02_Sensor,
    DHT22_Sensor,
    DS18B20_Sensor
};
