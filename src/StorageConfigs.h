#pragma once
#include "ArduinoJson.h"

#define CONFIG_BUFFER_SIZE (32 * 1024)

// extern StaticJsonDocument <CONFIG_BUFFER_SIZE>GlobalConfigs;
extern DynamicJsonDocument *GlobalConfigsJsonDoc;
#define GlobalConfigs (*GlobalConfigsJsonDoc)

void StorageConfigs_init() ;
bool StorageConfigs_load() ;
bool StorageConfigs_save() ;
