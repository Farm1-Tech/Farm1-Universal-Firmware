#pragma once

#include "./CloudDef.h"

void Cloud_process() ;
CloudStatus_t Cloud_getStatus() ;
void Cloud_apiInit() ;

// Cloud defined
#include "./HandySense/HandySense.h"

const Cloud_t Cloud[] = {
    HandySense_Cloud
};

