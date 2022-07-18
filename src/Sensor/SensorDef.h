#pragma once

#include "Arduino.h"
#include "ArduinoJson.h"
#include <functional>

#define SENSOR_TYPE_NUM 4
enum SensorType_t {
    TEMPERATURE   = 1 << 0,
    HUMIDITY      = 1 << 1,
    SOIL          = 1 << 2,
    AMBIENT_LIGHT = 1 << 3, 
};

enum SensorStatus_t {
    WORK_WELL = 0,
    SENSOR_NOT_FOUND = -1,
    INIT_FAIL = -2,
    READ_FAIL = -3,
    TYPE_NOT_SUPPORT = -4,
};

typedef std::function<void(void*, JsonObject)> TGenOptionHandlerFunction;
typedef std::function<void(void*)> TProcessHandlerFunction;
typedef std::function<SensorStatus_t(void*, SensorType_t, void*)> TGetValueHandlerFunction;

typedef struct {
    const char* id;
    uint32_t type;
    void *option;
    SensorStatus_t error;
    TGenOptionHandlerFunction genOption;
    TProcessHandlerFunction process;
    TGetValueHandlerFunction getValue;
} Sensor_t;
