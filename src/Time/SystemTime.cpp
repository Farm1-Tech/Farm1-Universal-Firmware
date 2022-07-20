#include "SystemTime.h"
#include <WiFi.h>
#include "./RTC.h"

#define TIMEZONE (7 * 60 * 60)
#define NTP_SERVER  "pool.ntp.org"
#define NTP_SERVER1 "time1.nimt.or.th"
#define NTP_SERVER2 "time.navy.mi.th"

#define RTC_SYNC_INTERVAL_MS (10 * 60 * 1000) // 10 min

SystemTimeStatus_t Time_init() {
    if (!RTC_init()) {
        return RTC_ERROR;
    }

    return TIME_OK;
}

static time_t last_time_sync = 0;
static uint64_t last_time_of_sync = 0;

SystemTimeStatus_t Time_now(struct tm* now) {
    if ((last_time_of_sync != 0) && ((millis() - last_time_of_sync) < (60 * 1000))) {
        time_t time_now = last_time_sync + ((millis() - last_time_of_sync) / 1000);
        struct tm* now_got = localtime(&time_now);
        memcpy(now, now_got, sizeof(struct tm));
        return TIME_OK;
    }

    bool getTimeFromInternet = false;
    if (WiFi.isConnected()) {
        configTime(TIMEZONE, 0, NTP_SERVER, NTP_SERVER1, NTP_SERVER2);
        getTimeFromInternet = getLocalTime(now, 1000);
    }

    if (!getTimeFromInternet) {
        if (!RTC_read(now)) {
            return RTC_ERROR;
        }
    } else {
        static uint64_t last_sync_rtc = 0;
        if ((last_sync_rtc == 0) || ((millis() - last_sync_rtc) >= RTC_SYNC_INTERVAL_MS)) {
            if (RTC_write(now)) {
                last_sync_rtc = millis();
            } else {
                Serial.println("RTC write sync time FAIL");
            }
        }
    }

    last_time_sync = mktime(now);
    last_time_of_sync = millis();

    return TIME_OK;
}