#pragma once

#include "../DisplayDef.h"

DisplayStatus_t Farm1_LCD_3_2_process(void*) ;

const Display_t Farm1_LCD_3_2_Display = {
    .id = "farm1-lcd-3-2",
    .option = NULL,
    .process = Farm1_LCD_3_2_process
};

