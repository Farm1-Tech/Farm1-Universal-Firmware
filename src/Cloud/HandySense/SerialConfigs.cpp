#include "Arduino.h"
#include "ArduinoJson.h"
#include "StorageConfigs.h"

DynamicJsonDocument jsonDoc(1024);

static int state = 0;

byte STX = 02;
byte ETX = 03;
uint8_t START_PATTERN[] = {0, 0, 0, 111, 222};

static void send_configs_to_serial() {
    Serial.write(START_PATTERN, sizeof(START_PATTERN));
    Serial.flush();

    Serial.write(STX); // STX

    jsonDoc.clear();
    JsonObject connectionInfo = GlobalConfigs["handysense"]["connection"].as<JsonObject>();
    if (
        connectionInfo["server"].isNull() ||
        connectionInfo["port"].isNull() ||
        connectionInfo["client"].isNull() ||
        connectionInfo["user"].isNull() ||
        connectionInfo["pass"].isNull() ||
        GlobalConfigs["wifi"]["ssid"].isNull() ||
        GlobalConfigs["wifi"]["password"].isNull()
    ) {
        Serial.print("null");
    } else {
        jsonDoc["server"] = connectionInfo["server"].as<String>();
        jsonDoc["port"] = connectionInfo["port"].as<String>();
        jsonDoc["client"] = connectionInfo["client"].as<String>();
        jsonDoc["user"] = connectionInfo["user"].as<String>();
        jsonDoc["pass"] = connectionInfo["pass"].as<String>();

        // WiFi Connect
        jsonDoc["ssid"]     = GlobalConfigs["wifi"]["ssid"].as<String>();
        jsonDoc["password"] = GlobalConfigs["wifi"]["password"].as<String>();

        serializeJson(jsonDoc, Serial);
        jsonDoc.clear();
    }

    Serial.write(ETX); // ETX
}

void SerialConfigs_process() {
    if (state == 0) {
        // delay(2000);
        send_configs_to_serial();

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
                    GlobalConfigs["handysense"]["connection"]["server"] = jsonDoc["server"].as<String>();
                    GlobalConfigs["handysense"]["connection"]["port"] = jsonDoc["port"].as<int>();
                    GlobalConfigs["handysense"]["connection"]["client"] = jsonDoc["client"].as<String>();
                    GlobalConfigs["handysense"]["connection"]["user"] = jsonDoc["user"].as<String>();
                    GlobalConfigs["handysense"]["connection"]["pass"] = jsonDoc["pass"].as<String>();

                    // WiFi Connect
                    GlobalConfigs["wifi"]["ssid"] = jsonDoc["ssid"].as<String>();
                    GlobalConfigs["wifi"]["password"] = jsonDoc["password"].as<String>();
                    // serializeJsonPretty(GlobalConfigs, Serial);

                    StorageConfigs_save();

                    delay(1000);
                    ESP.restart();
                    while(1) delay(100);
                }
            } else {
                Serial.println("deserializeJson fail : " + data_str);
            }
        }
    }
}
