#pragma once
#include "ArduinoJson.h"

#define CONFIG_BUFFER_SIZE (16 * 1024)

extern StaticJsonDocument <CONFIG_BUFFER_SIZE>GlobalConfigs;

bool StorageConfigs_load() ;
bool StorageConfigs_save() ;
