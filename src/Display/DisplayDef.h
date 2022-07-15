#pragma once

#include <functional>

enum DisplayStatus_t {
    DISPLAY_WORK_WELL = 0,
    DISPLAY_FAIL = -1,
};

typedef std::function<DisplayStatus_t(void*)> TDisplayProcessHandlerFunction; // self

typedef struct {
    const char* id;
    void *option;
    TDisplayProcessHandlerFunction process;
} Display_t;

