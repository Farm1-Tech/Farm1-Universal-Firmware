#include "Arduino.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"

#include "./Cloud.h"
#include "../WebServer/WebServer.h"
#include "../StorageConfigs.h"

static Cloud_t *cloud_select = NULL;

static TaskHandle_t CloudTaskHandle = NULL;
static void CloudTask(void*) {
    while(1) {
        if (cloud_select == NULL) {
            cloud_select = (Cloud_t *) &Cloud[0];
        }
        cloud_select->process((void*) cloud_select);

        delay(10);
    }
}

static int state = 0;

void Cloud_process() {
    if (state == 0) {
        xTaskCreatePinnedToCore(CloudTask, "CloudTask", 8 * 1024, NULL, 20, &CloudTaskHandle, APP_CPU_NUM);
        state = 1;
    } else if (state == 1) {

    }
}

CloudStatus_t Cloud_getStatus() {
    if (cloud_select == NULL) {
        return CLOUD_WAIT_CONNECT;
    }

    return cloud_select->status;
}

void Cloud_ioUpdate() {
    if (cloud_select == NULL) {
        return;
    }

    return cloud_select->io_update((void*) cloud_select);
}

void Cloud_apiInit() {

    // Auto Control
    server.on("/api/handysense/auto", HTTP_GET, [] (AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->setCode(200);

        {
            DynamicJsonDocument jsonDoc(4 * 1024);

            jsonDoc["timer"] = GlobalConfigs["handysense"]["control_by_timer"].as<JsonArray>();
            jsonDoc["soil"] = GlobalConfigs["handysense"]["control_by_soil"].as<JsonArray>();
            jsonDoc["temp"] = GlobalConfigs["handysense"]["control_by_temp"].as<JsonArray>();

            serializeJsonPretty(GlobalConfigs, Serial);
            serializeJsonPretty(jsonDoc, Serial);
            
            serializeJsonPretty(jsonDoc, *response);
        }

        request->send(response);
    });

    server.addHandler(new AsyncCallbackJsonWebHandler("/api/handysense/auto", [](AsyncWebServerRequest *request, JsonVariant &json) {
        if (request->method() == HTTP_POST) {
            JsonObject jsonPost = json.as<JsonObject>();
            GlobalConfigs["handysense"]["control_by_timer"] = jsonPost["timer"].as<JsonArray>();
            GlobalConfigs["handysense"]["control_by_soil"] = jsonPost["soil"].as<JsonArray>();
            GlobalConfigs["handysense"]["control_by_temp"] = jsonPost["temp"].as<JsonArray>();
            serializeJsonPretty(GlobalConfigs, Serial);
            StorageConfigs_save();
            request->send(200, "text/plain", "OK");
            
            // Fixed bug
            delay(50);
            StorageConfigs_load();
        }
    }, 4 * 1024));

    server.addHandler(new AsyncCallbackJsonWebHandler("/api/handysense/auto-one", [](AsyncWebServerRequest *request, JsonVariant &json) {
        if (!request->hasParam("ch")) {
            request->send(401, "text/plain", "not found 'ch' in url");
            return;
        }

        int ch = request->getParam("ch")->value().toInt();
        Serial.printf("Req ! ch: %d\n", ch);

        if (request->method() == HTTP_POST) {
            JsonObject jsonPost = json.as<JsonObject>();
            GlobalConfigs["handysense"]["control_by_timer"][ch] = jsonPost["timer"].as<JsonArray>();
            GlobalConfigs["handysense"]["control_by_soil"][ch] = jsonPost["soil"].as<JsonObject>();
            GlobalConfigs["handysense"]["control_by_temp"][ch] = jsonPost["temp"].as<JsonObject>();
            serializeJsonPretty(GlobalConfigs, Serial);
            StorageConfigs_save();
            request->send(200, "text/plain", "OK");
            
            // Fixed bug
            delay(50);
            StorageConfigs_load();
        }
    }, 4 * 1024));
}
