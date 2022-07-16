#pragma once

#include "./DisplayDef.h"

DisplayStatus_t Display_process() ;

// defined display list
#include "./Farm1-LCD3.2/Farm1-LCD3.2.h"

const Display_t Display[] = {
    Farm1_LCD_3_2_Display,
};
