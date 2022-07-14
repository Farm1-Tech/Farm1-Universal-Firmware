#pragma once

#include "../CloudDef.h"

void HandySense_process(void*) ;

static Cloud_t HandySense_Cloud = {
    .id = "handysense",
    .option = NULL,
    .process = HandySense_process,
};
