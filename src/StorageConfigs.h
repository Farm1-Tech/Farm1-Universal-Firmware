#pragma once
#include "ArduinoJson.h"

extern DynamicJsonDocument GlobalConfigs;

bool StorageConfigs_load() ;
bool StorageConfigs_save() ;
