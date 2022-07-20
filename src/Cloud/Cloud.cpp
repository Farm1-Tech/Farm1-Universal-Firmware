#include "Arduino.h"

#include "./Cloud.h"


static Cloud_t *cloud_select = NULL;

TaskHandle_t CloudTaskHandle = NULL;
void CloudTask(void*) {
    while(1) {
        if (cloud_select == NULL) {
            cloud_select = (Cloud_t *) &Cloud[0];
        }
        cloud_select->process((void*) cloud_select);

        delay(10);
    }
}

static int state = 0;

void Cloud_process() {
    if (state == 0) {
        xTaskCreatePinnedToCore(CloudTask, "CloudTask", 4 * 1024, NULL, 20, &CloudTaskHandle, APP_CPU_NUM);
        state = 1;
    } else if (state == 1) {

    }
}

CloudStatus_t Cloud_getStatus() {
    if (cloud_select == NULL) {
        return CLOUD_WAIT_CONNECT;
    }

    return cloud_select->status;
}
