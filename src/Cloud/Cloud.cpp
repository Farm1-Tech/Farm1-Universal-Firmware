#include "./Cloud.h"

static Cloud_t *cloud_select = NULL;

void Cloud_process() {
    if (cloud_select == NULL) {
        cloud_select = (Cloud_t *) &Cloud[0];
    }
    cloud_select->process((void*) cloud_select);
}

CloudStatus_t Cloud_getStatus() {
    if (cloud_select == NULL) {
        return CLOUD_WAIT_CONNECT;
    }

    return cloud_select->status;
}
