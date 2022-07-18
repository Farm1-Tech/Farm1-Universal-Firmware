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

        Serial.setTimeout(100);
        state = 1;
    } else if (state == 1) {
        if (Serial.available() > 0) {
            String data_str = Serial.readString();
            // Serial.println("Data : " + data_str);
            if (deserializeJson(jsonDoc, data_str) == DeserializationError::Ok) {
                if (jsonDoc["command"].as<String>() == "restart") {
                    delay(100);
                    ESP.restart();
                    while(1) delay(100);
                }
                
                if (jsonDoc.containsKey("client")) {
                    JsonObject connectionInfo = GlobalConfigs["handysense"]["connection"];
                    if (!jsonDoc["client"].as<String>().equals(connectionInfo["client"].as<String>())) {
                        GlobalConfigs["handysense"]["connection"]["server"] = jsonDoc["server"].as<String>();
                        GlobalConfigs["handysense"]["connection"]["port"] = jsonDoc["port"].as<int>();
                        GlobalConfigs["handysense"]["connection"]["client"] = jsonDoc["client"].as<String>();
                        GlobalConfigs["handysense"]["connection"]["user"] = jsonDoc["user"].as<String>();
                        GlobalConfigs["handysense"]["connection"]["pass"] = jsonDoc["pass"].as<String>();
                    }
                    // WiFi Connect
                    GlobalConfigs["wifi"]["ssid"] = jsonDoc["ssid"].as<String>();
                    GlobalConfigs["wifi"]["password"] = jsonDoc["password"].as<String>();
                    // serializeJsonPretty(GlobalConfigs, Serial);

                    StorageConfigs_save();

                    delay(100);
                    ESP.restart();
                }
            } else {
                Serial.println("deserializeJson fail : " + data_str);
            }
        }
    }
}
