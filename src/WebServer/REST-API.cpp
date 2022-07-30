#include "Arduino.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "AsyncTCP.h"
#include "AsyncJson.h"
#include "Update.h"

#include "Board/Board.h"
#include "./WebServer.h"
#include "StorageConfigs.h"
#include "Sensor/Sensor.h"
#include "Output/Output.h"

static StaticJsonDocument <4 * 1024>jsonDoc;

void REST_API_init() {
    // Device Info
    server.on("/api/info", HTTP_GET, [] (AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->setCode(200);
        jsonDoc.clear();
        jsonDoc["sku"] = BOARD_NAME;
        jsonDoc["mac_address"] = String(WiFi.BSSIDstr());
        jsonDoc["uptime"] = millis();
        jsonDoc["wifi"]["ssid"] = String(WiFi.SSID());
        jsonDoc["wifi"]["connected"] = WiFi.isConnected();
        serializeJsonPretty(jsonDoc, *response);
        request->send(response);
    });

    // Configs
    server.on("/api/configs", HTTP_GET, [] (AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->setCode(200);
        jsonDoc.clear();
        jsonDoc["device"] = GlobalConfigs["device"].as<JsonObject>();
        jsonDoc["cloud"] = GlobalConfigs["cloud"].as<JsonObject>();
        serializeJsonPretty(jsonDoc, *response);
        serializeJsonPretty(GlobalConfigs, Serial);
        request->send(response);
    });

    server.addHandler(new AsyncCallbackJsonWebHandler("/api/configs", [](AsyncWebServerRequest *request, JsonVariant &json) {
        if (request->method() == HTTP_POST) {
            JsonObject jsonPost = json.as<JsonObject>();
            GlobalConfigs["device"] = jsonPost["device"].as<JsonObject>();
            GlobalConfigs["cloud"] = jsonPost["cloud"].as<JsonObject>();
            serializeJsonPretty(GlobalConfigs, Serial);
            StorageConfigs_save();
            request->send(200, "text/plain", "OK");
            
            // Fixed bug
            delay(50);
            StorageConfigs_load();
        }
    }, 8 * 1024));

    // Direct I/O control
    server.on("/api/io", HTTP_GET, [] (AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->setCode(200);

        jsonDoc.clear();

        // Sensor
        jsonDoc["sensor"]["temperature"] = (char*)NULL;
        jsonDoc["sensor"]["humidity"] = (char*)NULL;
        jsonDoc["sensor"]["soil"] = (char*)NULL;
        jsonDoc["sensor"]["light"] = (char*)NULL;

        float temperature;
        if (Sensor_getValueOneNonBlock(TEMPERATURE, (void*) &temperature) == WORK_WELL) {
            jsonDoc["sensor"]["temperature"] = temperature;
        }

        float humidity;
        if (Sensor_getValueOneNonBlock(HUMIDITY, (void*) &humidity) == WORK_WELL) {
            jsonDoc["sensor"]["humidity"] = humidity;
        }

        float soil;
        if (Sensor_getValueOneNonBlock(SOIL, (void*) &soil) == WORK_WELL) {
            jsonDoc["sensor"]["soil"] = soil;
        }

        float light;
        if (Sensor_getValueOneNonBlock(AMBIENT_LIGHT, (void*) &light) == WORK_WELL) {
            jsonDoc["sensor"]["light"] = light;
        }

        // Output
        JsonObject output = jsonDoc.createNestedObject("output");
        for (uint8_t i=0;i<4;i++) {
            bool output_status;
            if (Output_getValueOne(i, &output_status) == OUTPUT_WORK_WELL) {
                output["o" + String(i + 1)].set(output_status ? 1 : 0);
            } else {
                output["o" + String(i + 1)].set((char*)NULL);
            }
        }

        // Input
#ifdef BOARD_FARM1
        JsonArray input = jsonDoc.createNestedArray("input");
        uint8_t input_pin[ONBOARD_INPUT_NUM] = ONBOARD_INPUT_PIN;
        for (uint8_t i=0;i<ONBOARD_INPUT_NUM;i++) {
            pinMode(input_pin[i], INPUT_PULLUP);
            input.add(digitalRead(input_pin[i]) == ONBOARD_INPUT_ACTIVE ? 1 : 0);
        }
#endif

        serializeJsonPretty(jsonDoc, *response);
        request->send(response);
    });

    server.addHandler(new AsyncCallbackJsonWebHandler("/api/io", [](AsyncWebServerRequest *request, JsonVariant &json) {
        if (request->method() == HTTP_POST) {
            // Set
            if (json["output"].is<JsonObject>()) {
                JsonObject output_set = json["output"].as<JsonObject>();
                for (uint8_t i=0;i<4;i++) {
                    String key = "o" + String(i + 1);
                    if (!output_set[key].isNull()) {
                        int value = output_set[key].as<int>();
                        if (Output_setValueOne(i, value == 1) != OUTPUT_WORK_WELL) {
                            Serial.printf("Output %d write error", i);
                        }
                    }
                }
            } else {
                request->send(400, "text/plain", "object 'output' not object");
                return;
            }
            
            // Get
            AsyncResponseStream *response = request->beginResponseStream("application/json");
            response->setCode(200);

            jsonDoc.clear();
            JsonObject output = jsonDoc.createNestedObject("output");
            for (uint8_t i=0;i<4;i++) {
                bool output_status;
                if (Output_getValueOne(i, &output_status) == OUTPUT_WORK_WELL) {
                    output["o" + String(i + 1)].set(output_status ? 1 : 0);
                } else {
                    output["o" + String(i + 1)].set((char*)NULL);
                }
            }

            serializeJsonPretty(jsonDoc, *response);
            request->send(response);
        }
    }, 4 * 1024));

    // OTA
    server.on("/api/update", HTTP_POST, [](AsyncWebServerRequest *request){
        bool shouldReboot = !Update.hasError();
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot ? "OK" : "FAIL");
        response->addHeader("Connection", "close");
        request->send(response);

        // Reset after OTA
        if (shouldReboot) {
            xTaskCreate([](void*) {
                delay(300);
                ESP.restart();

                vTaskDelete(NULL);
            }, "ResetAfterOTA", 512, NULL, 20, NULL);
        }
    },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
        if(!index){
            Serial.printf("Update Start: %s\n", filename.c_str());
            if(!Update.begin()){
                Update.printError(Serial);
            }
        }

        if(!Update.hasError()){
            if(Update.write(data, len) != len){
                Update.printError(Serial);
            }
        }

        if(final){
            if(Update.end(true)){
                Serial.printf("Update Success: %uB\n", index+len);
            } else {
                Update.printError(Serial);
            }
        }
  });
}
