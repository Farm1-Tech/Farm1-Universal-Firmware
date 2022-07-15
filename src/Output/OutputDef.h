#pragma once

#include "Arduino.h"
#include <functional>

enum OutputStatus_t {
    OUTPUT_WORK_WELL = 0,
    OUTPUT_WRITE_FAIL = -1,
    OUTPUT_READ_FAIL = -2,
    OUTPUT_CH_INVALID = -3,
};

typedef std::function<OutputStatus_t(void*, int, bool)> TOutputSetValueHandlerFunction; // self, ch, value(out)
typedef std::function<OutputStatus_t(void*, int, bool*)> TOutputGetValueHandlerFunction; // self, ch, value(out)

typedef struct {
    const char* id;
    void *option;
    TOutputSetValueHandlerFunction setValue;
    TOutputGetValueHandlerFunction getValue;
} Output_t;

