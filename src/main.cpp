#include <Arduino.h>
#include "Sensor/Sensor.h"

void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(Sensor[0].id);
}

void loop() {
  // put your main code here, to run repeatedly:
}
