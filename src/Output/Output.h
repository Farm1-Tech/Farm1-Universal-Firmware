#pragma once

#include "./OutputDef.h"
#include "./OnBoard/OnBoard.h"

OutputStatus_t Output_setValueOne(int, bool) ;
OutputStatus_t Output_getValueOne(int, bool*) ;

const Output_t Output[] = {
    OnBoard_Output
};
