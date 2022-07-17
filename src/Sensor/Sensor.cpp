#include "./Sensor.h"
#include "./I2CDev.h"
#include "StorageConfigs.h"

bool Sensor_init() {
    I2CSetup();

    return true;
}

String SensorTypeToKey(SensorType_t type) {
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

SensorStatus_t Sensor_getValueOne(SensorType_t type, void* value) {
    if ((uint8_t)(type) & (TEMPERATURE | HUMIDITY | SOIL | AMBIENT_LIGHT)) {
        *((float*)value) = 0.0f;
    }
    JsonObject sensor_select = GlobalConfigs["device"][SensorTypeToKey(type)];
    if (sensor_select.containsKey("type")) {
        for (uint8_t i=0;i<Sensor_length;i++) {
            if (sensor_select["type"].as<String>().equals(Sensor[i].id)) {
                if (sensor_select.containsKey("option") && Sensor[i].genOption) {
                    Sensor[i].genOption((void*)&Sensor[i], sensor_select["option"].as<JsonObject>());
                }
                return Sensor[i].getValue((void*)&Sensor[i], type, value);
            }
        }
    } else {
        for (uint8_t i=0;i<Sensor_length;i++) {
            if ((Sensor[i].type & type) != 0) {
                return Sensor[i].getValue((void*)&Sensor[i], type, value);
            }
        }
    }
    
    Serial.println("Sensor type not support");
    return TYPE_NOT_SUPPORT;
}

