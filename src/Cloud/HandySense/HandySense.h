#pragma once

#include "../CloudDef.h"

void HandySense_process(void*) ;
void HandySense_ioUpdate(void*) ;

static Cloud_t HandySense_Cloud = {
    .id = "handysense",
    .option = NULL,
    .status = CLOUD_WAIT_CONNECT,
    .process = HandySense_process,
    .io_update = HandySense_ioUpdate,
    .cleanup = NULL,
};
