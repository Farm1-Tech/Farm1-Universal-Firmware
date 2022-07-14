#pragma once
#include "ArduinoJson.h"

DynamicJsonDocument GlobalConfigs;

bool StorageConfigs_load() ;
bool StorageConfigs_save() ;
