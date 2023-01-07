#pragma once

#include <functional>

typedef enum {
    CLOUD_WAIT_CONNECT,
    CLOUD_CONNECTED
} CloudStatus_t;

typedef std::function<void(void*)> TProcessHandlerFunction;

typedef struct {
    const char* id;
    void *option;
    CloudStatus_t status;
    TProcessHandlerFunction process;
    TProcessHandlerFunction io_update;
    TProcessHandlerFunction cleanup;
} Cloud_t;

