#include <Arduino.h>

#include "./WebServer.h"
#include "./REST-API.h"

AsyncWebServer server(80);

void WebServer_init() {
    REST_API_init();

    server.begin();
}
