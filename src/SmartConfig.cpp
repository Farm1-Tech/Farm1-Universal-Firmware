#include "Arduino.h"
#include "WiFi.h"

#include "./SmartConfig.h"
#include "./StorageConfigs.h"
#include "./Board/Board.h"

static int state = 0;

SmartConfigStatus_t SmartConfig_process() {
    if (state == 0) {
        // Delete old configs
        if (GlobalConfigs.containsKey("wifi")) {
            GlobalConfigs["wifi"].clear();
        }

        WiFi.beginSmartConfig();

        state = 1;
    } else if (state == 1) {
        if (WiFi.smartConfigDone()) {
            Serial.println("SmartConfig received.");

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
            
            state = 0;
            return FINISH_CONFIG;
        }
    }

    return WAIT_CONFIG;
}

