#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "Time/SystemTime.h"
#include "Sensor/Sensor.h"
#include "StorageConfigs.h"

#define DATA_SEND_INTERVAL_MS (2 * 60 * 60 * 1000) // 2 min = 2 * 60 * 60 * 1000 ms
#define SHADOW_UPDATE_INTERVAL_MS (500) // 500 mS
static int state = 0;
static bool force_shadow_update = false;

static Client *tcpClient = NULL;
static PubSubClient *client = NULL;

static void HandySense_receive_callback(String topic, uint8_t* payload, unsigned int length) {
    if (topic.startsWith("@private/led")) { // Relay direct control
        uint8_t ch = 0;
        if (!sscanf(topic.c_str(), "@private/led/%d", &ch) != 1) {
            Serial.println("CH of relay error");
        }

        // digitalWrite(....)
    } else if (topic.startsWith("@private/max_temp")) {

    } else if (topic.startsWith("@private/min_temp")) {

    } else if (topic.startsWith("@private/max_soil")) {

    } else if (topic.startsWith("@private/min_soil")) {

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
        if (
            !GlobalConfigs.containsKey("handysense") ||
            !GlobalConfigs["handysense"].containsKey("server") ||
            !GlobalConfigs["handysense"].containsKey("port") ||
            !GlobalConfigs["handysense"].containsKey("client") ||
            !GlobalConfigs["handysense"].containsKey("user") ||
            !GlobalConfigs["handysense"].containsKey("pass")
        ) {
            return;
        }

        const char *server = GlobalConfigs["handysense"]["server"].as<const char*>();
        const int port = GlobalConfigs["handysense"]["port"].as<int>();
        const char *client_id = GlobalConfigs["handysense"]["client"].as<const char*>();
        const char *username = GlobalConfigs["handysense"]["user"].as<const char*>();
        const char *password = GlobalConfigs["handysense"]["pass"].as<const char*>();

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
            struct {
                uint8_t min;
                uint8_t max;
            } soil_set[4], temp_set[4];
            
            for (int i=0;i<4;i++) {
                soil_set[i].min = 0;
                soil_set[i].max = 0;
            }

            for (int i=0;i<4;i++) {
                temp_set[i].min = 0;
                temp_set[i].max = 0;
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

    // Keep output timer run normal
    struct tm timeinfo;
    if (Time_now(&timeinfo) == TIME_OK) {
        uint16_t curentTimer = (timeinfo.tm_hour * 60) + timeinfo.tm_min;
        uint8_t dayofweek = timeinfo.tm_wday;

    }

    // Keep output by soil run normal
    float soil_value = 0;
    Sensor_getValueOne(SOIL, (void*)&soil_value);
    for (int i=0;i<4;i++) {

    }

    // Keep output by temp run normal
    float temp_value = 0;
    Sensor_getValueOne(TEMPERATURE, (void*)&temp_value);
    for (int i=0;i<4;i++) {

    }
}