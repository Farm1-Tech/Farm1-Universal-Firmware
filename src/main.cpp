#include <Arduino.h>
#include "SPIFFS.h"
#include "WiFi.h"

#include "./Sensor/Sensor.h"
#include "./Cloud/Cloud.h"
#include "./Board/Board.h"
#include "./SmartConfig.h"
#include "./StorageConfigs.h"
#include "./WiFiManager.h"
#include "./Time/SystemTime.h"
#include "./Display/Display.h"
#include "./WebServer/WebServer.h"

#define MAX_PRESS_TIME (5 * 1000)

static const char *TAG = "main";

bool CheckButtonEnterToConfigsMode() {
#ifdef BOARD_FARM1
    bool state_of_output = digitalRead(CONFIGS_BUTTON_PIN);
#endif
    pinMode(CONFIGS_BUTTON_PIN, INPUT);
#ifdef BOARD_FARM1
    delay(10); // wait signal level up by R-pullup and stable
#endif
    bool configs_button_is_pressed = digitalRead(CONFIGS_BUTTON_PIN) == CONFIGS_BUTTON_ACTIVE;
#ifdef BOARD_FARM1
    pinMode(CONFIGS_BUTTON_PIN, OUTPUT_OPEN_DRAIN);
    digitalWrite(CONFIGS_BUTTON_PIN, state_of_output);
#endif
    static int state = 0;
    static uint64_t start_press_ms = 0;
    if (state == 0) {
        if (configs_button_is_pressed) {
            start_press_ms = millis();
            state = 1;
        }
    } else if (state == 1) {
        if (configs_button_is_pressed) {
            if ((millis() - start_press_ms) > MAX_PRESS_TIME) {
                state = 0;
                return true;
            }
        } else {
            state = 0;
        }
    }
    
    return false;
}

// Main Program
void setup() {
  Serial.begin(115200);

  // Init system module
  StorageConfigs_init();
  Sensor_init();
  Time_init();
  
  // Init SPIFFS
  if (!SPIFFS.begin(true)) { // Format if failed
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_AP_STA);
  WiFiManager_init();

  // Init HTTP Web Server
  WebServer_init();
  
  // Load configs
  StorageConfigs_load();
}

void loop() {
    static bool modeConfig = false;
    if (!modeConfig) {
        Cloud_process();
        if (CheckButtonEnterToConfigsMode()) {
            modeConfig = true;
            WiFiManager_stop();
            Serial.println("Enter to config mode");
        }
    } else {
        if (SmartConfig_process() == FINISH_CONFIG) {
            modeConfig = false;
            WiFiManager_run();
            Serial.println("Exit config mode");
        }
    }
    
    uint32_t start = millis();
    Display_process();
    uint32_t diff = millis() - start;
    if (diff > 100) {
        ESP_LOGI(TAG, "Display use %d ms", diff);
    }
    
    delay(10); // Stop for other task work
}
