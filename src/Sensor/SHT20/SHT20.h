#pragma once

#include "Sensor/SensorDef.h"

// ------------------
#include <Arduino.h>
#include <Wire.h>

#define SHT20_ADDR 0x40

class Artron_SHT20 {
    private:
        TwoWire *_wire;

        int write(uint8_t command, uint8_t *data = NULL, uint8_t len = 0) ;
        int read(uint8_t command, uint8_t *data = NULL, int len = 0, int stop = 100) ;

    public:
        Artron_SHT20(TwoWire*);

        bool begin() ;
        float readTemperature() ;
        float readHumidity() ;
    
};
// ------------------
void SHT20_process(void*) ;
SensorStatus_t SHT20_getValue(void*, SensorType_t, void*) ;

// Sensor definded
static Sensor_t SHT20_Sensor = {
    .id = "sht20",
    .type = TEMPERATURE | HUMIDITY,
    .option = NULL,
    .error = WORK_WELL,
    .genOption = NULL,
    .process = SHT20_process,
    .getValue = SHT20_getValue,
};
