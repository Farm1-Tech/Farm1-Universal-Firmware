#include "Arduino.h"
#include "WiFi.h"

#include "./SmartConfig.h"
#include "./StorageConfigs.h"
#include "./Board/Board.h"

#define WAIT_WIFI_CONNECT_TIMEOUT (30 * 1000)

static int state = 0;
static uint64_t start_wait_wifi_connected = 0;

SmartConfigStatus_t SmartConfig_process() {
    if (state == 0) {
        // Delete old configs
        /*if (GlobalConfigs.containsKey("wifi")) {
            GlobalConfigs["wifi"].clear();
        }*/

        Serial.println("SmartConfig begin");
        WiFi.beginSmartConfig();

        state = 1;
    } else if (state == 1) {
        if (WiFi.smartConfigDone()) {
            Serial.println("SmartConfig received.");
            start_wait_wifi_connected = millis();
            state = 2;
        }
    } else if (state == 2) {
        if (WiFi.isConnected()) {
            Serial.print("SSID: ");
            Serial.println(WiFi.SSID());
            Serial.print("Password: ");
            Serial.println(WiFi.psk());

            // Save configs
            GlobalConfigs["wifi"]["ssid"] = WiFi.SSID();
            GlobalConfigs["wifi"]["password"] = WiFi.psk();
            serializeJson(GlobalConfigs, Serial);

            // Save Configs
            if (!StorageConfigs_save()) {
                Serial.println("Save configs fail !");
            }

            //GlobalConfigs["wifi"]["ssid"] = "";
            
            state = 0;
            return FINISH_CONFIG;
        } else if ((millis() - start_wait_wifi_connected) > WAIT_WIFI_CONNECT_TIMEOUT) {
            Serial.println("Wait WiFi connnect timeout");
            state = 0;
        }
    }

    return WAIT_CONFIG;
}

