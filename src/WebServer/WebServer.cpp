#include <Arduino.h>

#include "./WebServer.h"
#include "./REST-API.h"
#include "./Cloud/Cloud.h"

AsyncWebServer server(80);

void WebServer_init() {
    REST_API_init();
    Cloud_apiInit();

    server.begin();
}
