#include "./Farm1-LCD3.2.h"

#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>

#include "../DisplayDef.h"
#include "Arduino.h"
#include "Board/Board.h"
#include "Cloud/Cloud.h"
#include "Sensor/Sensor.h"
#include "Time/SystemTime.h"

#ifdef BOARD_FARM1
#define LCD_BL_PIN 32
#else
#error "Board not support"
#endif

TFT_eSPI tft = TFT_eSPI();

#define DISPLAY_BUFFER_SIZE (10 * 1024)

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;

// -----------
#include "./lvgl_page/ui.h"
extern "C" {
#include "./lvgl_page/ui_helpers.h"
}
// -----------

/* Display flushing */
static void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors((uint16_t *) &color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

static void update_sensor_value_timer(lv_timer_t* timer) {
    lv_obj_t *value_label_map[4] = {
        ui_overview_temp_value_label,
        ui_overview_humi_value_label,
        ui_overview_light_value_label,
        ui_overview_soil_value_label,
    };
    SensorType_t type_map[4] = {
        TEMPERATURE,
        HUMIDITY,
        AMBIENT_LIGHT,
        SOIL
    };

    for (uint8_t i=0;i<4;i++) {
        float value = 0;
        if (Sensor_getValueOne(type_map[i], (void*)&value) == WORK_WELL) {
            lv_label_set_text_fmt(value_label_map[i], "%.1f", value);
        } else {
            lv_label_set_text(value_label_map[i], "");
        }
    }
}

static void update_time_timer(lv_timer_t* timer) {
    struct tm timeinfo;
    Time_now(&timeinfo);

    char time_str_buff[20];
    lv_snprintf(time_str_buff, sizeof(time_str_buff), "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    lv_label_set_text(ui_datetime_label, time_str_buff);
}

static void update_connection_timer(lv_timer_t* timer) {
    if (WiFi.isConnected()) {
        lv_obj_clear_flag(ui_overview_wifi_img, LV_OBJ_FLAG_HIDDEN);
        if (Cloud_getStatus() == CLOUD_CONNECTED) {
            lv_obj_clear_flag(ui_overview_cloud_img, LV_OBJ_FLAG_HIDDEN);
        } else {
            _ui_flag_modify(ui_overview_cloud_img, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
        }
    } else {
        _ui_flag_modify(ui_overview_wifi_img, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
        lv_obj_add_flag(ui_overview_cloud_img, LV_OBJ_FLAG_HIDDEN);
    }
}

static int state = 0;

DisplayStatus_t Farm1_LCD_3_2_process(void* args) {
    if (state == 0) {       // display init
        tft.begin();        /* TFT init */
        tft.setRotation(3); /* Landscape orientation */

        lv_init();

        buf = (lv_color_t*) malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t));

        lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISPLAY_BUFFER_SIZE);

        /*Initialize the display*/
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init( &disp_drv );
        /*Change the following line to your display resolution*/
        disp_drv.hor_res = 320;
        disp_drv.ver_res = 240;
        disp_drv.flush_cb = my_disp_flush;
        disp_drv.draw_buf = &draw_buf;
        lv_disp_drv_register( &disp_drv );

        ui_init();

        lv_obj_t *value_label_map[4] = {
            ui_overview_temp_value_label,
            ui_overview_humi_value_label,
            ui_overview_light_value_label,
            ui_overview_soil_value_label,
        };
        SensorType_t type_map[4] = {
            TEMPERATURE,
            HUMIDITY,
            AMBIENT_LIGHT,
            SOIL
        };

        for (uint8_t i=0;i<4;i++) {
            lv_label_set_text(value_label_map[i], "");
        }
        
        lv_timer_create(update_sensor_value_timer, 1000, NULL);
        lv_timer_create(update_time_timer, 300, NULL);
        lv_timer_create(update_connection_timer, 300, NULL);

        pinMode(LCD_BL_PIN, OUTPUT);
        digitalWrite(LCD_BL_PIN, LOW);

        state = 1;
    }
    if (state == 1) {
        lv_timer_handler();
    }

    return DISPLAY_WORK_WELL;
}
