#pragma once

#include "../OutputDef.h"

OutputStatus_t OnBoard_setValue(void*, int, bool) ;
OutputStatus_t OnBoard_getValue(void*, int, bool*) ;

const Output_t OnBoard_Output = {
    .id = "on-board",
    .option = NULL,
    .setValue = OnBoard_setValue,
    .getValue = OnBoard_getValue,
};
