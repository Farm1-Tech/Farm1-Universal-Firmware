#pragma once

#include "../DisplayDef.h"

DisplayStatus_t Farm1_LCD_3_5_CT_process(void*) ;

const Display_t Farm1_LCD_3_5_CT_Display = {
    .id = "farm1-lcd-3-5-ct",
    .option = NULL,
    .process = Farm1_LCD_3_5_CT_process
};

