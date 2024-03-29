#include "WiFi.h"

#include "./StorageConfigs.h"
#include "./Board/Board.h"

bool wifi_force_disconnect = false;

static void WiFiEvent(WiFiEvent_t event){
    switch(event) {
        case SYSTEM_EVENT_STA_STOP:
        case SYSTEM_EVENT_STA_DISCONNECTED:
        case SYSTEM_EVENT_STA_LOST_IP:
            // Serial.println("WiFi Disconnect");
            wifi_force_disconnect = true;
            WiFi.removeEvent(WiFiEvent);
            break;
        default:
            break;
    }
}

static TaskHandle_t WiFiManagerTaskHandle = NULL;
static EventGroupHandle_t WiFiEventGroup = NULL;

#define WIFI_UPDATE_REQ_FLAG BIT0
#define WIFI_SCAN_REQ_FLAG   BIT1

static int16_t wifi_scan_found = 0;

static void WiFiManagerTask(void*) {
    int state = 0;
    
    while(1) {
        EventBits_t wifi_flag = xEventGroupGetBits(WiFiEventGroup);
        if (wifi_flag & WIFI_SCAN_REQ_FLAG) {
            WiFi.disconnect();
            delay(50);
            wifi_scan_found = WiFi.scanNetworks();
            xEventGroupClearBits(WiFiEventGroup, WIFI_SCAN_REQ_FLAG);
            state = 0;
        }

        bool bypass_check_connect = false;
        if (wifi_flag & WIFI_UPDATE_REQ_FLAG) {
            bypass_check_connect = true;
            state = 0;
        }

        if (state == 0) {
            if ((!WiFi.isConnected()) || bypass_check_connect) {
                if (!GlobalConfigs["wifi"]["ssid"].isNull()) {
                    // Serial.println("Reconnect WiFi");
                    WiFi.onEvent(WiFiEvent);
                    WiFi.begin(
                        GlobalConfigs["wifi"]["ssid"].as<const char*>(),
                        GlobalConfigs["wifi"]["password"].as<const char*>()
                    );
                    state = 1;
                }
            }
        } else if (state == 1) {
            if (WiFi.isConnected()) {
                WiFi.removeEvent(WiFiEvent);
                
                /*
                Serial.print("WiFi connected, IP: ");
                Serial.print(WiFi.localIP());
                Serial.print("\tSSID: ");
                Serial.print(WiFi.SSID());
                Serial.println();
                */

                /*
                if (!GlobalConfigs["wifi"]["ssid"].as<String>().equals(WiFi.SSID())) {
                    GlobalConfigs["wifi"]["ssid"] = WiFi.SSID();
                    GlobalConfigs["wifi"]["password"] = WiFi.psk();
                    // serializeJson(GlobalConfigs, Serial);

                    // Save Configs
                    if (!StorageConfigs_save()) {
                        Serial.println("Save configs fail !");
                    }
                }
                */
                
                digitalWrite(NET_LED_PIN, LOW);
                state = 0;
            } else if (wifi_force_disconnect) {
                WiFi.disconnect();

                wifi_force_disconnect = false;
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

        delay(50);
    }

    vTaskDelete(NULL);
}

void WiFiManager_init() {
    WiFiEventGroup = xEventGroupCreate();
    xTaskCreatePinnedToCore(WiFiManagerTask, "WiFiManagerTask", 4 * 1024, NULL, 20, &WiFiManagerTaskHandle, PRO_CPU_NUM);
}

void WiFiManager_stop() {
    vTaskSuspend(WiFiManagerTaskHandle);
}

void WiFiManager_run() {
    vTaskResume(WiFiManagerTaskHandle);
}

void WiFiManager_scanReq() {
    wifi_scan_found = 0;
    xEventGroupSetBits(WiFiEventGroup, WIFI_SCAN_REQ_FLAG);
}

bool WiFiManager_isScanDone() {
    EventBits_t wifi_flag = xEventGroupGetBits(WiFiEventGroup);
    return (wifi_flag & WIFI_SCAN_REQ_FLAG) == 0;
}

int16_t WiFiManager_scanFound() {
    return wifi_scan_found;
}
