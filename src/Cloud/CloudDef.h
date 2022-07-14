#pragma once

#include <functional>

typedef std::function<void(void*)> TProcessHandlerFunction;

typedef struct {
    const char* id;
    void *option;
    TProcessHandlerFunction process;
    TProcessHandlerFunction cleanup;
} Cloud_t;

