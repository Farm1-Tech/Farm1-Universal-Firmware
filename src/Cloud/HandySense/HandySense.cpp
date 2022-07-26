#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "./HandySense.h"
#include "Time/SystemTime.h"
#include "Sensor/Sensor.h"
#include "Output/Output.h"
#include "StorageConfigs.h"
#include "./SerialConfigs.h"

#define DATA_SEND_INTERVAL_MS (2 * 60 * 60 * 1000) // 2 min = 2 * 60 * 60 * 1000 ms
#define SHADOW_UPDATE_INTERVAL_MS (1 * 60 * 1000) // 1 min
static int state = 0;
static uint8_t force_shadow_update = 0;

static Client *tcpClient = NULL;
static PubSubClient *client = NULL;

#define UPDATE_ALL   0xFF
#define UPDATE_RELAY (1 << 0)
#define UPDATE_SOIL  (1 << 1)
#define UPDATE_TEMP  (1 << 2)
#define UPDATE_TIMER (1 << 3)

static bool update_shadow(uint8_t type) {
    DynamicJsonDocument shadowContent(1024);
    JsonObject shadowData = shadowContent.createNestedObject("data");

    if ((type & UPDATE_RELAY) != 0) {
        int relay_status[4] = { 0, 0, 0, 0 };
        for (int i=0;i<4;i++) {
            bool value = 0;
            Output_getValueOne(i, &value);
            relay_status[i] = value ? 1 : 0;
        }

        shadowData["led0"] = relay_status[0];
        shadowData["led1"] = relay_status[1];
        shadowData["led2"] = relay_status[2];
        shadowData["led3"] = relay_status[3];
    }

    if ((type & UPDATE_SOIL) != 0) {
        JsonArray control_by_soil = GlobalConfigs["handysense"]["control_by_soil"];
        shadowData["min_soil0"] = control_by_soil[0]["min"].as<int>();
        shadowData["max_soil0"] = control_by_soil[0]["max"].as<int>();
        shadowData["min_soil1"] = control_by_soil[1]["min"].as<int>();
        shadowData["max_soil1"] = control_by_soil[1]["max"].as<int>();
        shadowData["min_soil2"] = control_by_soil[2]["min"].as<int>();
        shadowData["max_soil2"] = control_by_soil[2]["max"].as<int>();
        shadowData["min_soil3"] = control_by_soil[3]["min"].as<int>();
        shadowData["max_soil3"] = control_by_soil[3]["max"].as<int>();
    }

    if ((type & UPDATE_TEMP) != 0) {
        JsonArray control_by_temp = GlobalConfigs["handysense"]["control_by_temp"];
        shadowData["min_temp0"] = control_by_temp[0]["min"].as<int>();
        shadowData["max_temp0"] = control_by_temp[0]["max"].as<int>();
        shadowData["min_temp1"] = control_by_temp[1]["min"].as<int>();
        shadowData["max_temp1"] = control_by_temp[1]["max"].as<int>();
        shadowData["min_temp2"] = control_by_temp[2]["min"].as<int>();
        shadowData["max_temp2"] = control_by_temp[2]["max"].as<int>();
        shadowData["min_temp3"] = control_by_temp[3]["min"].as<int>();
        shadowData["max_temp3"] = control_by_temp[3]["max"].as<int>();
    }

    if ((type & UPDATE_TIMER) != 0) {
        JsonArray control_by_timer = GlobalConfigs["handysense"]["control_by_timer"];

        #define BOOL2STR(a) String(a ? 1 : 0)
        for (int ch=0;ch<4;ch++) {
            for (int timer=0;timer<3;timer++) {
                JsonObject timerConfigs = control_by_timer[ch][timer];
                String text_timer = "";
                text_timer += BOOL2STR(timerConfigs["enable"].as<bool>()) + ",";
                for (int repeat_wday=0;repeat_wday<7;repeat_wday++) {
                    text_timer += BOOL2STR(timerConfigs["repeat"][repeat_wday].as<bool>()) + ",";
                }
                if ((!timerConfigs["start"].isNull()) && (!timerConfigs["stop"].isNull())) {
                    text_timer += timerConfigs["start"].as<String>() + ":00,";
                    text_timer += timerConfigs["stop"].as<String>() + ":00";
                } else {
                    text_timer += "00:00:00,00:00:00";
                }

                String name = String("value_timer") + String(ch) + String(timer);
                shadowData[name] = text_timer;
            }
        }
    }

    String shadow;
    serializeJson(shadowContent, shadow);
    shadowContent.clear();
    // Serial.printf("Send %s : %d\n", shadow.c_str(), shadow.length());
    return client->publish("@shadow/data/update", shadow.c_str());
}

static void HandySense_receive_callback(String topic, uint8_t* payload, unsigned int length) {
    String payload_str = "";
    for (int i=0;i<length;i++) {
        payload_str += (char) payload[i];
    }

    // Serial.printf("[%s]: %s\n", topic.c_str(), payload_str.c_str());
    if (topic.startsWith("@private/led")) { // Relay direct control
        int ch = 0;
        if (sscanf(topic.c_str(), "@private/led%d", &ch) != 1) {
            Serial.println("CH LED error");
        }
        // Serial.printf("Direct Write: %d => %s\n", ch, payload_str.c_str());
        Output_setValueOne(ch, payload_str == "on");
        update_shadow(UPDATE_RELAY);
    } else if (topic.startsWith("@private/timer")) { // Control output by Timer configs
        /*
          Msg:   1,0,1,1,1,1,1,1,17:20:00,17:22:00 => <EN, repeat_mo, ...., start, end>
          topic: @private/timer00 => @private/timer<output_n, timer_n>
        */
        int ch = 0, timer = 0;
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

        JsonArray control_by_timer = GlobalConfigs["handysense"].getOrAddMember("control_by_timer");
        control_by_timer[ch][timer]["enable"] = enable == 1;
        #define FORMAT_TIME(t) String(String(t < 10 ? "0" : "") + t)
        control_by_timer[ch][timer]["start"] = FORMAT_TIME(start_hour) + ":" + FORMAT_TIME(start_min);
        control_by_timer[ch][timer]["stop"] = FORMAT_TIME(stop_hour) + ":" + FORMAT_TIME(stop_min);
        for (int i=0;i<7;i++) {
            control_by_timer[ch][timer]["repeat"][i] = repeat[i] == 1;
        }

        // serializeJsonPretty(GlobalConfigs, Serial);

        update_shadow(UPDATE_TIMER);
    } else if (topic.startsWith("@private/min_temp")) { // Control output by Temp min configs
        int ch = 0;
        if (sscanf(topic.c_str(), "@private/min_temp%d", &ch) != 1) {
            Serial.println("CH LED error");
        }
        GlobalConfigs["handysense"]["control_by_temp"][ch]["min"] = payload_str.toInt();

        update_shadow(UPDATE_TEMP);
    } else if (topic.startsWith("@private/max_temp")) { // Control output by Temp max configs
        int ch = 0;
        if (sscanf(topic.c_str(), "@private/max_temp%d", &ch) != 1) {
            Serial.println("CH LED error");
        }
        GlobalConfigs["handysense"]["control_by_temp"][ch]["max"] = payload_str.toInt();

        update_shadow(UPDATE_TEMP);
    } else if (topic.startsWith("@private/min_soil")) { // Control output by Soil min configs
        int ch = 0;
        if (sscanf(topic.c_str(), "@private/min_soil%d", &ch) != 1) {
            Serial.println("CH LED error");
        }
        GlobalConfigs["handysense"]["control_by_soil"][ch]["min"] = payload_str.toInt();

        update_shadow(UPDATE_SOIL);
    } else if (topic.startsWith("@private/max_soil")) { // Control output by Soil max configs
        int ch = 0;
        if (sscanf(topic.c_str(), "@private/max_soil%d", &ch) != 1) {
            Serial.println("CH LED error");
        }
        GlobalConfigs["handysense"]["control_by_soil"][ch]["max"] = payload_str.toInt();
        
        update_shadow(UPDATE_SOIL);
    }
    if (topic.startsWith("@private/min_") || topic.startsWith("@private/max_") || topic.startsWith("@private/timer")) {
        StorageConfigs_save();
    }
}

void HandySense_process(void* args) {
    Cloud_t *self = (Cloud_t *) args;

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
        JsonObject connectionInfo = GlobalConfigs["handysense"]["connection"].as<JsonObject>();
        if (
            WiFi.isConnected() &&
            !connectionInfo.isNull() &&
            !connectionInfo["server"].isNull() &&
            !connectionInfo["port"].isNull() &&
            !connectionInfo["client"].isNull() &&
            !connectionInfo["user"].isNull() &&
            !connectionInfo["pass"].isNull()
        ) {
            state = 2;
        } else {
            self->status = CLOUD_WAIT_CONNECT;
        }
    }
    if (state == 2) { // Connect
        JsonObject connectionInfo = GlobalConfigs["handysense"]["connection"].as<JsonObject>();
        const char *server = connectionInfo["server"].as<const char*>();
        const int port = connectionInfo["port"].as<int>();
        const char *client_id = connectionInfo["client"].as<const char*>();
        const char *username = connectionInfo["user"].as<const char*>();
        const char *password = connectionInfo["pass"].as<const char*>();

        client->setBufferSize(1024);
        client->setServer(server, port);
        client->setSocketTimeout(2);
        if (client->connect(client_id, username, password)) { // Connected
            client->subscribe("@private/#");
            self->status = CLOUD_CONNECTED;
            // Serial.println("NETPIE connected");
            state = 3;
        } else {
            Serial.println("NETPIE connect fail");
            state = 1;
        }
    }
    if (state == 3) {
        client->loop(); // keep mqtt work

        if (!client->connected()) {
            // Serial.println("NETPIE connection lost");
            state = 1;
        } else {
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
                    tempC, humiRH, lightKLUX / 1000.0f, soilPERCENT
                );
                if (client->publish("@shadow/data/update", buff)) {
                    last_send_sensor = millis();
                } else {
                    Serial.println("Send data to NETPIE fail");
                    state = 99;
                }
            }
        }

        static uint64_t last_send_shadow_update = 0;
        if ((last_send_shadow_update == 0) || ((millis() - last_send_shadow_update) > (SHADOW_UPDATE_INTERVAL_MS)) || force_shadow_update) {
            if (update_shadow(force_shadow_update ? force_shadow_update : UPDATE_ALL)) {
                last_send_shadow_update = millis();
                if (force_shadow_update) {
                    force_shadow_update = 0;
                }
            } else {
                Serial.println("Send data to NETPIE fail");
                state = 99; // Go back to check WiFi and reconnect
            }
        }
    }
    if (state == 99) {
        static int sub_state = 0;
        static uint64_t start_stop = 0;
        if (sub_state == 0) {
            start_stop = millis();
            sub_state = 1;
        } else if (sub_state == 1) {
            if ((millis() - start_stop) > 2000) { // Stop 2 sec
                sub_state = 0;
                state = 3;
            }
        }
    }

#define ON_OUTPUT() ({ \
            bool active = false; \
            Output_getValueOne(ch, &active); \
            if (!active) { \
                Output_setValueOne(ch, true); \
                force_shadow_update |= UPDATE_RELAY; \
            } \
})

#define OFF_OUTPUT() ({ \
            bool active = false; \
            Output_getValueOne(ch, &active); \
            if (active) { \
                Output_setValueOne(ch, false); \
                force_shadow_update |= UPDATE_RELAY; \
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
    {
        float soil_value = 0;
        if (Sensor_getValueOne(SOIL, (void*)&soil_value) == WORK_WELL) {
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
            }
        }
    }

    {
        float temp_value = 0;
        if (Sensor_getValueOne(TEMPERATURE, (void*)&temp_value) == WORK_WELL) {
            for (int ch=0;ch<4;ch++) {
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
    }

    // Keep web configs via Serial work
    SerialConfigs_process();
}