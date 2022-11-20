#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "./StorageConfigs.h"

#define GLOBAL_FILE_CONFIGS "/configs.json"
static const char* TAG = "StorageConfigs";

// StaticJsonDocument <CONFIG_BUFFER_SIZE>GlobalConfigs;
DynamicJsonDocument *GlobalConfigsJsonDoc;

void StorageConfigs_init() {
    GlobalConfigsJsonDoc = new DynamicJsonDocument(CONFIG_BUFFER_SIZE);
}

static bool fileWrite(String fileName, String content) {
  File file = SPIFFS.open(fileName, FILE_WRITE);
  if (!file) {
    ESP_LOGE(TAG, "failed to open file '/%s' for writing\n", fileName.c_str());
    return false;
  }
  file.print(content);
  file.close();

  return true;
}

static String fileRead(String fileName) {
  File file = SPIFFS.open(fileName, FILE_READ);
  if (!file) {
    ESP_LOGE(TAG, "failed to open file '/%s' for reading\n", fileName.c_str());
    return String("");
  }
  String content = file.readString();
  file.close();

  return content;
}

bool StorageConfigs_load() {
    String configsContentText = fileRead(GLOBAL_FILE_CONFIGS);
    // Serial.println(configsContentText);
    GlobalConfigs.clear();
    if (deserializeJson(GlobalConfigs, configsContentText) != DeserializationError::Ok) {
        ESP_LOGE(TAG, "Load configs error");
        return false;
    }

    return true;
}

bool StorageConfigs_save() {
    String configsContentText;
    serializeJson(GlobalConfigs, configsContentText);
    return fileWrite(GLOBAL_FILE_CONFIGS, configsContentText);
}
