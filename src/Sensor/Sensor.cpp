#include "./Sensor.h"
#include "./I2CDev.h"
#include "StorageConfigs.h"

static String SensorTypeToKey(SensorType_t type) {
    if (type == TEMPERATURE) {
        return "temperature";
    }
    if (type == HUMIDITY) {
        return "humidity";
    }
    if (type == SOIL) {
        return "soil";
    }
    if (type == AMBIENT_LIGHT) {
        return "light";
    }

    return String("");
}

static int SensorTypeToIndex(SensorType_t type) {
    if (type == TEMPERATURE) {
        return 0;
    }
    if (type == HUMIDITY) {
        return 1;
    }
    if (type == SOIL) {
        return 2;
    }
    if (type == AMBIENT_LIGHT) {
        return 3;
    }

    return -1;
}

static uint64_t last_value_update_time[SENSOR_TYPE_NUM];
static float last_value[SENSOR_TYPE_NUM];
static SensorStatus_t last_status[SENSOR_TYPE_NUM];

bool Sensor_init() {
    I2CSetup();
    for (uint8_t i=0;i<SENSOR_TYPE_NUM;i++) {
        last_value_update_time[i] = 0;
    }

    return true;
}

SensorStatus_t Sensor_getValueOne(SensorType_t type, void* value) {
    float *value_f = (float*) value;

    // Check and return last value
    int type_index = SensorTypeToIndex(type);
    if ((type_index != -1) && (type_index < SENSOR_TYPE_NUM)) {
        if ((last_value_update_time != 0) && ((millis() - last_value_update_time[type_index]) < 1000)) {
            *value_f = last_value[type_index];
            return last_status[type_index];
        }
    }

    // Real
    if ((uint8_t)(type) & (TEMPERATURE | HUMIDITY | SOIL | AMBIENT_LIGHT)) {
        *value_f = 0.0f;
    }

    SensorStatus_t res = TYPE_NOT_SUPPORT;
    int sensor_index = -1;
    {
        JsonObject sensor_select = GlobalConfigs["device"][SensorTypeToKey(type)];
        if (sensor_select.containsKey("type")) {
            for (uint8_t i=0;i<Sensor_length;i++) {
                if (sensor_select["type"].as<String>().equals(Sensor[i].id)) {
                    if (sensor_select.containsKey("option") && Sensor[i].genOption) {
                        Sensor[i].genOption((void*)&Sensor[i], sensor_select["option"].as<JsonObject>());
                    }
                    sensor_index = i;
                    break;
                }
            }
        } else {
            for (uint8_t i=0;i<Sensor_length;i++) {
                if ((Sensor[i].type & type) != 0) {
                    sensor_index = i;
                    break;
                }
            }
        }
    }

    if (sensor_index >= 0) {
        for (uint8_t try_n=0;try_n<5;try_n++) { // Try 5 times if read fail
            res = Sensor[sensor_index].getValue((void*)&Sensor[sensor_index], type, value);
            if (res == WORK_WELL) {
                break;
            } else {
                delay(50);
            }
        }
    }

    last_value[type_index] = *value_f;
    last_status[type_index] = res;
    last_value_update_time[type_index] = millis();
    
    Serial.println("Sensor type not support");
    return res;
} 

