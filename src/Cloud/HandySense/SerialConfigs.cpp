#include "Arduino.h"
#include "ArduinoJson.h"
#include "StorageConfigs.h"

DynamicJsonDocument jsonDoc(1024);

static int state = 0;

void SerialConfigs_process() {
    if (state == 0) {
        Serial.write((uint8_t*) "\x00\x6F\xDE", 3);
        Serial.write(0x02); // STX

        JsonObject connectionInfo = GlobalConfigs["handysense"]["connection"].as<JsonObject>();
        jsonDoc["server"] = connectionInfo["server"].as<String>();
        jsonDoc["port"] = connectionInfo["port"].as<int>();
        jsonDoc["client"] = connectionInfo["client"].as<String>();
        jsonDoc["user"] = connectionInfo["user"].as<String>();
        jsonDoc["pass"] = connectionInfo["pass"].as<String>();

        // WiFi Connect
        jsonDoc["ssid"]     = GlobalConfigs["wifi"]["ssid"].as<String>();
        jsonDoc["password"] = GlobalConfigs["wifi"]["password"].as<String>();
        serializeJsonPretty(jsonDoc, Serial);

        Serial.write(0x03); // ETX

        Serial.setTimeout(1000);
        state = 1;
    } else if (state == 1) {
        if (Serial.available() > 0) {
            if (deserializeJson(jsonDoc, Serial) == DeserializationError::Ok) {
                if (jsonDoc["command"].as<String>() == "restart") {
                    delay(100);
                    ESP.restart();
                    while(1) delay(100);
                }
                
                if (jsonDoc.containsKey("client")) {
                    JsonObject connectionInfo = GlobalConfigs["handysense"]["connection"].as<JsonObject>();
                    if (!jsonDoc["client"].as<String>().equals(connectionInfo["client"].as<String>())) {
                        connectionInfo["server"] = jsonDoc["server"].as<String>();
                        connectionInfo["port"] = jsonDoc["port"].as<int>();
                        connectionInfo["client"] = jsonDoc["client"].as<String>();
                        connectionInfo["user"] = jsonDoc["user"].as<String>();
                        connectionInfo["pass"] = jsonDoc["pass"].as<String>();
                    }
                    // WiFi Connect
                    GlobalConfigs["wifi"]["ssid"] = jsonDoc["ssid"].as<String>();
                    GlobalConfigs["wifi"]["password"] = jsonDoc["password"].as<String>();

                    StorageConfigs_save();
                }
            }
        }
    }
}
