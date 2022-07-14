#pragma once

#include "time.h"

enum SystemTimeStatus_t {
    TIME_OK,
    SYNC_TIME_ERROR,
    RTC_ERROR,
};

SystemTimeStatus_t Time_init() ;
SystemTimeStatus_t Time_now(struct tm*) ;
