#include "WiFi.h"

#include "./StorageConfigs.h"
#include "./Board/Board.h"

static bool wifi_force_disconnect = false;

static void WiFiEvent(WiFiEvent_t event){
    switch(event) {
        case SYSTEM_EVENT_STA_STOP:
        case SYSTEM_EVENT_STA_DISCONNECTED:
        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("WiFi Disconnect");
            wifi_force_disconnect = true;
            break;
        default:
            break;
    }
}

static int state = 0;

void WiFiManager_process() {
    if (state == 0) {
        WiFi.onEvent(WiFiEvent);
    } else if (state == 1) {
        if (!WiFi.isConnected()) {
            if (GlobalConfigs.containsKey("wifi") && GlobalConfigs["wifi"].containsKey("ssid") && GlobalConfigs["wifi"].containsKey("password")) {
                WiFi.begin(
                    GlobalConfigs["wifi"]["ssid"].as<const char*>(),
                    GlobalConfigs["wifi"]["password"].as<const char*>()
                );
                state = 2;
            }
        }
    } else if (state == 2) {
        if (WiFi.isConnected() || wifi_force_disconnect) {
            if (wifi_force_disconnect) {
                WiFi.disconnect();

                wifi_force_disconnect = false;
            }
            state = 1;
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
