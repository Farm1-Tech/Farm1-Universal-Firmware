#include "WiFi.h"

#include "./StorageConfigs.h"
#include "./Board/Board.h"

static int state = 0;

void WiFiManager_process() {
    if (state == 0) {
        if (!WiFi.isConnected()) {
            if (GlobalConfigs.containsKey("wifi") && GlobalConfigs["wifi"].containsKey("ssid") && GlobalConfigs["wifi"].containsKey("password")) {
                WiFi.begin(
                    GlobalConfigs["wifi"]["ssid"].as<const char*>(),
                    GlobalConfigs["wifi"]["password"].as<const char*>()
                );
                state = 1;
            }
        }
    } else if (state == 1) {
        if (WiFi.isConnected()) {
            state = 0;
        } else {
            // LED Blink status
            static uint64_t last_led_blink = 0;
            if ((millis() - last_led_blink) >= 300) {
                digitalWrite(NET_LED_PIN, !digitalRead(NET_LED_PIN));

                last_led_blink = millis();
            }
        }
    }
}
