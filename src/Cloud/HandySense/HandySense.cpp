#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "Time/SystemTime.h"
#include "Sensor/Sensor.h"
#include "Output/Output.h"
#include "StorageConfigs.h"

#define DATA_SEND_INTERVAL_MS (2 * 60 * 60 * 1000) // 2 min = 2 * 60 * 60 * 1000 ms
#define SHADOW_UPDATE_INTERVAL_MS (1 * 60 * 1000) // 1 min
static int state = 0;
static bool force_shadow_update = false;

static Client *tcpClient = NULL;
static PubSubClient *client = NULL;

static void HandySense_receive_callback(String topic, uint8_t* payload, unsigned int length) {
    String payload_str = "";
    for (int i=0;i<length;i++) {
        payload_str += (char) payload[i];
    }

    int ch = 0;
    if (sscanf(topic.c_str(), "@private/%*s/%d", &ch) != 1) {
        Serial.println("CH error");
    }
    if (topic.startsWith("@private/led")) { // Relay direct control
        Output_setValueOne(ch, payload_str == "on");
        force_shadow_update = true;
    } else if (topic.startsWith("@private/timer")) { // Control output by Timer configs
        /*
          Msg:   1,0,1,1,1,1,1,1,17:20:00,17:22:00 => <EN, repeat_mo, ...., start, end>
          topic: @private/timer00 => @private/timer<output_n, timer_n>
        */
        int timer = 0;
        if (sscanf(topic.c_str(), "@private/timer%1d%1d", &ch, &timer) != 2) {
            Serial.println("Topic error");
        }
        
        int enable, repeat[7], start_hour, start_min, stop_hour, stop_min;
        if (sscanf(payload_str.c_str(), 
            "%d,%d,%d,%d,%d,%d,%d,%d,%d:%d:%*d,%d:%d:%*d", 
            &enable, &repeat[0], &repeat[1], &repeat[2], &repeat[3], &repeat[4], &repeat[5], &repeat[6], &start_hour, &start_min, &stop_hour, &stop_min
        ) != 12) {
            Serial.println("Payload error");
        }

        JsonObject timerConfigs = GlobalConfigs["handysense"]["control_by_timer"][ch][timer];
        timerConfigs["enable"] = enable == 1;
        timerConfigs["start"] = String(start_hour) + ":" + String(start_min);
        timerConfigs["stop"] = String(stop_hour) + ":" + String(stop_min);
        for (int i=0;i<7;i++) {
            timerConfigs["repeat"][i] = repeat[i] == 1;
        }
    } else if (topic.startsWith("@private/min_temp")) { // Control output by Temp min configs
        GlobalConfigs["handysense"]["control_by_temp"][ch]["min"] = payload_str.toInt();
    } else if (topic.startsWith("@private/max_temp")) { // Control output by Temp max configs
        GlobalConfigs["handysense"]["control_by_temp"][ch]["max"] = payload_str.toInt();
    } else if (topic.startsWith("@private/min_soil")) { // Control output by Soil min configs
        GlobalConfigs["handysense"]["control_by_soil"][ch]["min"] = payload_str.toInt();
    } else if (topic.startsWith("@private/max_soil")) { // Control output by Soil max configs
        GlobalConfigs["handysense"]["control_by_soil"][ch]["max"] = payload_str.toInt();
    }
    if (topic.startsWith("@private/min_") || topic.startsWith("@private/max_") || topic.startsWith("@private/timer")) {
        StorageConfigs_save();
    }
    force_shadow_update = true;
}

void HandySense_process(void* args) {
    if (!tcpClient || !client) {
        state = 0;
    }

    if (state == 0) { // Init
        if (tcpClient) {
            delete tcpClient;
            tcpClient = NULL;
        }
        if (client) {
            delete client;
            client = NULL;
        }

        tcpClient = new WiFiClient();
        client = new PubSubClient(*tcpClient);
        client->setCallback(HandySense_receive_callback);

        state = 1;
    }
    if (state == 1) { // Wait WiFi connect
        if (WiFi.isConnected()) {
            state = 2;
        }
    }
    if (state == 2) { // Connect
        JsonObject connectionInfo = GlobalConfigs["handysense"]["connection"].as<JsonObject>();
        if (
            !connectionInfo.isNull() ||
            !connectionInfo.containsKey("server") ||
            !connectionInfo.containsKey("port") ||
            !connectionInfo.containsKey("client") ||
            !connectionInfo.containsKey("user") ||
            !connectionInfo.containsKey("pass")
        ) {
            return;
        }

        const char *server = connectionInfo["server"].as<const char*>();
        const int port = connectionInfo["port"].as<int>();
        const char *client_id = connectionInfo["client"].as<const char*>();
        const char *username = connectionInfo["user"].as<const char*>();
        const char *password = connectionInfo["pass"].as<const char*>();

        client->setServer(server, port);
        if (client->connect(client_id, username, password)) { // Connected
            client->subscribe("@private/#");

            state = 3;
        } else {
            Serial.println("NETPIE connect fail");
            state = 1;
        }
    }
    if (state == 3) {
        client->loop(); // keep mqtt work

        if (!client->connected()) {
            Serial.println("NETPIE connection lost");
            state = 1;
            return;
        }

        static uint64_t last_send_sensor = 0;
        if ((last_send_sensor == 0) || ((millis() - last_send_sensor) > DATA_SEND_INTERVAL_MS)) {
            float tempC = 20.3;
            float humiRH = 10.3;
            float lightKLUX = 58.1;
            float soilPERCENT = 80.3;

            // Read real data from sensor
            Sensor_getValueOne(TEMPERATURE, (void*) &tempC);
            Sensor_getValueOne(HUMIDITY, (void*) &humiRH);
            Sensor_getValueOne(AMBIENT_LIGHT, (void*) &lightKLUX);
            Sensor_getValueOne(SOIL, (void*) &soilPERCENT);
            // ---------

            char buff[256];
            memset(buff, 0, sizeof(buff));
            sprintf(
                buff,
                "{\"data\": {\"temperature\": %.2f,\"humidity\": %.2f,\"lux\": %.2f,\"soil\": %.2f}}",
                tempC, humiRH, lightKLUX, soilPERCENT
            );
            if (client->publish("@shadow/data/update", buff)) {
                last_send_sensor = millis();
            } else {
                Serial.println("Send data to NETPIE fail");
                delay(1000); // stop 1 sec for try in next cycle
            }
        }

        static uint64_t last_send_shadow_update = 0;
        if ((last_send_shadow_update == 0) || ((millis() - last_send_shadow_update) > (SHADOW_UPDATE_INTERVAL_MS)) || force_shadow_update) {
            int relay_status[4] = { 0, 0, 0, 0 };
            for (int i=0;i<4;i++) {
                bool value = 0;
                Output_getValueOne(i, &value);
                relay_status[i] = value ? 1 : 0;
            }

            struct {
                uint8_t min;
                uint8_t max;
            } soil_set[4], temp_set[4];
            
            for (int i=0;i<4;i++) {
                soil_set[i].min = GlobalConfigs["handysense"]["control_by_soil"][i]["min"].as<int>();
                soil_set[i].max = GlobalConfigs["handysense"]["control_by_soil"][i]["max"].as<int>();
            }

            for (int i=0;i<4;i++) {
                temp_set[i].min = GlobalConfigs["handysense"]["control_by_temp"][i]["min"].as<int>();
                temp_set[i].max = GlobalConfigs["handysense"]["control_by_temp"][i]["max"].as<int>();
            }

            char buff[256];
            memset(buff, 0, sizeof(buff));
            sprintf(
                buff,
                "{\"data\": {" \
                    "\"led0\": %d,\"led1\": %d,\"led2\": %d,\"led3\": %d" \
                    "\"min_soil0\": %d,\"max_soil0\": %d,"  \
                    "\"min_soil1\": %d,\"max_soil1\": %d," \
                    "\"min_soil2\": %d,\"max_soil2\": %d," \
                    "\"min_soil3\": %d,\"max_soil3\": %d," \
                    "\"min_temp0\": %d,\"max_temp0\": %d," \
                    "\"min_temp1\": %d,\"max_temp1\": %d," \
                    "\"min_temp2\": %d,\"max_temp2\": %d," \
                    "\"min_temp3\": %d,\"max_temp3\": %d" \
                "}}",
                relay_status[0], relay_status[1], relay_status[2], relay_status[3],
                soil_set[0].min, soil_set[0].max,
                soil_set[1].min, soil_set[1].max,
                soil_set[2].min, soil_set[2].max,
                soil_set[3].min, soil_set[3].max,
                temp_set[0].min, temp_set[0].max,
                temp_set[1].min, temp_set[1].max,
                temp_set[2].min, temp_set[2].max,
                temp_set[3].min, temp_set[3].max
            );
            if (client->publish("@shadow/data/update", buff)) {
                last_send_shadow_update = millis();
                if (force_shadow_update) {
                    force_shadow_update = false;
                }
            } else {
                Serial.println("Send data to NETPIE fail");
                state = 1; // Go back to check WiFi and reconnect
            }
        }
    }

#define ON_OUTPUT() ({ \
            bool active = false; \
            Output_getValueOne(ch, &active); \
            if (!active) { \
                Output_setValueOne(ch, true); \
                force_shadow_update = true; \
            } \
})

#define OFF_OUTPUT() ({ \
            bool active = false; \
            Output_getValueOne(ch, &active); \
            if (active) { \
                Output_setValueOne(ch, false); \
                force_shadow_update = true; \
            } \
})

    // Keep output timer run normal
    struct tm timeinfo;
    if (Time_now(&timeinfo) == TIME_OK) {
        int8_t week_day = timeinfo.tm_wday == 0 ? 6 : timeinfo.tm_wday - 1; // Cloud start Monday but time.h start Sunday
        for (int ch=0;ch<4;ch++) {
            for (int timer=0;timer<3;timer++) {
                JsonObject timerConfigs = GlobalConfigs["handysense"]["control_by_timer"][ch][timer];
                if (timerConfigs["enable"].isNull() || timerConfigs["start"].isNull() || timerConfigs["stop"].isNull()) { // Skip if no timer info
                    continue;
                }
                if (!timerConfigs["enable"].as<bool>()) { // Skip if not enable
                    continue;
                }
                if (!timerConfigs["repeat"][week_day].as<bool>()) { // Skip if today not in repeat day
                    continue;
                }
                int start_hour, start_min;
                sscanf(timerConfigs["start"].as<const char*>(), "%d:%d", &start_hour, &start_min);

                int stop_hour, stop_min;
                sscanf(timerConfigs["stop"].as<const char*>(), "%d:%d", &stop_hour, &stop_min);

                if ((start_hour == timeinfo.tm_hour) && (start_min == timeinfo.tm_min)) {
                    ON_OUTPUT();
                } else if ((stop_hour == timeinfo.tm_hour) && (stop_min == timeinfo.tm_min)) {
                    OFF_OUTPUT();
                }
            }
        }
    }

    // Keep output by soil & temp run normal
    float soil_value = 0, temp_value = 0;
    Sensor_getValueOne(SOIL, (void*)&soil_value);
    Sensor_getValueOne(TEMPERATURE, (void*)&temp_value);
    for (int ch=0;ch<4;ch++) {
        int soil_min = GlobalConfigs["handysense"]["control_by_soil"][ch]["min"].as<int>();
        int soil_max = GlobalConfigs["handysense"]["control_by_soil"][ch]["max"].as<int>();

        if ((soil_min != 0) && (soil_max != 0)) {
            // Soil Low more then soil_min then ON (eg. pump), Soil High over soil_max then OFF
            if (soil_value < soil_min) {
                ON_OUTPUT();
            } else if (soil_value > soil_max) {
                OFF_OUTPUT();
            }
        }

        int temp_min = GlobalConfigs["handysense"]["control_by_temp"][ch]["min"].as<int>();
        int temp_max = GlobalConfigs["handysense"]["control_by_temp"][ch]["max"].as<int>();

        if ((temp_min != 0) && (temp_max != 0)) {
            // Temp High over temp_max then ON (eg. fan), Temp Low more then temp_min then OFF
            if (temp_value > temp_max) {
                ON_OUTPUT();
            } else if (temp_value < temp_min) {
                OFF_OUTPUT();
            }
        }
    }
}