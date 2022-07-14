#include "Configs.h"

#ifdef BOARD_FARM1
#include "./Farm1/Farm1.h"
#else
#error "Board not support or not select board"
#endif