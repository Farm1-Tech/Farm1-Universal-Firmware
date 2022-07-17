#include "Arduino.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "AsyncTCP.h"
#include "AsyncJson.h"

#include "./WebServer.h"
#include "StorageConfigs.h"

void REST_API_init() {
    // Device Info
    server.on("/api/info", HTTP_GET, [] (AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->setCode(200);
        DynamicJsonDocument jsonDoc(1 * 1024);
        jsonDoc["device"]["mac_address"] = String(WiFi.BSSIDstr());
        jsonDoc["device"]["uptime"] = millis();
        jsonDoc["wifi"]["ssid"] = String(WiFi.SSID());
        jsonDoc["wifi"]["connected"] = WiFi.isConnected();
        serializeJsonPretty(jsonDoc, *response);
        request->send(response);
    });

    // Configs
    server.on("/api/configs", HTTP_GET, [] (AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        response->setCode(200);
        DynamicJsonDocument jsonDoc(4 * 1024);
        jsonDoc["device"] = GlobalConfigs["device"].as<JsonObject>();
        jsonDoc["cloud"] = GlobalConfigs["cloud"].as<JsonObject>();
        serializeJsonPretty(jsonDoc, *response);
        request->send(response);
    });

    server.addHandler(new AsyncCallbackJsonWebHandler("/api/configs", [](AsyncWebServerRequest *request, JsonVariant &json) {
        if (request->method() == HTTP_POST) {
            JsonObject jsonPost = json.as<JsonObject>();
            GlobalConfigs["device"] = jsonPost["device"].as<JsonObject>();
            GlobalConfigs["cloud"] = jsonPost["cloud"].as<JsonObject>();
            StorageConfigs_save();
            request->send(200, "text/plain", "OK");
        }
    }));
}
