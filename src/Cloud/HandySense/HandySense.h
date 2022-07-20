#pragma once

#include "../CloudDef.h"

void HandySense_process(void*) ;

static Cloud_t HandySense_Cloud = {
    .id = "handysense",
    .option = NULL,
    .status = CLOUD_WAIT_CONNECT,
    .process = HandySense_process,
    .cleanup = NULL,
};
