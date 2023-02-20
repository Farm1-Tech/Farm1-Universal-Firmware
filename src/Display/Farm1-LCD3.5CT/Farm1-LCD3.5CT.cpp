#include "./Farm1-LCD3.5CT.h"

#include <WiFi.h>
#include <lvgl.h>
#include <Wire.h>

#include "../DisplayDef.h"
#include "Arduino.h"
#include "Board/Board.h"
#include "Cloud/Cloud.h"
#include "Sensor/Sensor.h"
#include "Time/SystemTime.h"
#include "Output/Output.h"
#include "StorageConfigs.h"
#include "WiFiManager.h"
#include "./ILI9488.h"

#ifdef BOARD_FARM1
#define LCD_BL_PIN 32
#else
#error "Board not support"
#endif

#define TOUCH_ADDR (0x38)

static ILI9488 tft;

#define DISPLAY_BUFFER_SIZE (10 * 1024)

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;

// -----------
#include "./lvgl_page/ui.h"
extern "C" {
#include "./lvgl_page/ui_helpers.h"

extern void ui_farm1_lcd35ct_overview_screen_init();
extern void ui_farm1_lcd35ct_wifi_settings_screen_init();
extern lv_obj_t * ui_farm1_lcd35ct_overview;
}
// -----------

bool reg_read(uint8_t addr, uint8_t *data, size_t len) {
    Wire1.beginTransmission(TOUCH_ADDR);
    Wire1.write(addr);
    Wire1.endTransmission(false);

    int n = Wire1.requestFrom(TOUCH_ADDR, len);
    if (n != len) {
        Serial.printf("read fail code %d\n", n);
        return false;
    }
    Wire1.readBytes(data, len);

    return true;
}

bool reg_write(uint8_t addr, uint8_t *data, size_t len) {
    Wire1.beginTransmission(TOUCH_ADDR);
    Wire1.write(addr);
    Wire1.write(data, len);
    return Wire1.endTransmission() == 0;
}

/* Display flushing */
static void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ) {
    tft.drawBitmap(area->x1, area->y1, area->x2, area->y2, &color_p->full);

    lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
static void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint8_t touch_point = 0;
    if (reg_read(0x02, &touch_point, 1)) {
        if (touch_point > 0) {
            struct {
                uint8_t XH;
                uint8_t XL;
                uint8_t YH;
                uint8_t YL;
            } P1;
            if (reg_read(0x03, (uint8_t *)&P1, sizeof(P1))) {
                uint16_t x = ((uint16_t)(P1.XH & 0x0F) << 8) | P1.XL;
                uint16_t y = ((uint16_t)(P1.YH & 0x0F) << 8) | P1.YL;
                // Serial.printf("Touch %d, %d\n", x, y);

                y = map(y, 30, 480, 0, 480);
                data->point.x = 480 - y;
                data->point.y = x;
                data->state = LV_INDEV_STATE_PR;
            } else {
                Serial.println("read point 1 error");
                data->state = LV_INDEV_STATE_REL;
            }
        }
    } else {
        Serial.println("read point count error");
        data->state = LV_INDEV_STATE_REL;
    }

    return;
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
            lv_label_set_text_fmt(value_label_map[i], "%.1f",type_map[i] == AMBIENT_LIGHT ? (value / 1000.0f) : value);
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

static void relay_control_cb(lv_event_t *e) {
    lv_obj_t * obj = lv_event_get_target(e);
    bool value = lv_obj_has_state(obj, LV_STATE_CHECKED);
    int8_t i = -1;
    if (obj == ui_di35ct_overview_relay1_btn) {
        i = 0;
    } else if (obj == ui_di35ct_overview_relay2_btn) {
        i = 1;
    } else if (obj == ui_di35ct_overview_relay3_btn) {
        i = 2;
    } else if (obj == ui_di35ct_overview_relay4_btn) {
        i = 3;
    }
    if (i >= 0) {
        if (Output_setValueOne(i, value) != OUTPUT_WORK_WELL) {
            Serial.printf("Output %d write error", i);
        }
    }
}

static void update_output_status_timer(lv_timer_t* timer) {
    lv_obj_t * obj[4] = {
        ui_di35ct_overview_relay1_btn,
        ui_di35ct_overview_relay2_btn,
        ui_di35ct_overview_relay3_btn,
        ui_di35ct_overview_relay4_btn
    };
    for (uint8_t i=0;i<4;i++) {
        bool value = false;
        if (Output_getValueOne(i, &value) != OUTPUT_WORK_WELL) {
            Serial.printf("Output %d read error", i);
            continue;
        }
        value ? lv_obj_add_state(obj[i], LV_STATE_CHECKED) : lv_obj_clear_state(obj[i], LV_STATE_CHECKED);
    }
    Cloud_ioUpdate();
}

static void ta_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = (lv_obj_t *) lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, 0);
            lv_obj_update_layout(ui_farm1_lcd35ct_wifi_settings_box);   /*Be sure the sizes are recalculated*/
            lv_obj_set_height(ui_farm1_lcd35ct_wifi_settings_box, LV_VER_RES - lv_obj_get_height(kb));
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
        }
    } else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_set_height(ui_farm1_lcd35ct_wifi_settings_box, LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);

    } else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_set_height(ui_farm1_lcd35ct_wifi_settings_box, LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
}


static int state = 0;
static lv_obj_t * backdrop;
static lv_obj_t * spinner;

DisplayStatus_t Farm1_LCD_3_5_CT_process(void* args) {
    if (state == 0) {       // display init
        Wire1.begin(19, 16, 100E3);
        tft.init();

        // Config Touch chip
        {
            // Adjust threshold
            uint8_t threshold = 128; // Default threshold for touch detection
            reg_write(0x80, &threshold, 1);
        }

        lv_init();

        buf = (lv_color_t*) malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t));

        lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISPLAY_BUFFER_SIZE);

        /*Initialize the display*/
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init( &disp_drv );
        /*Change the following line to your display resolution*/
        disp_drv.hor_res = 480;
        disp_drv.ver_res = 320;
        disp_drv.flush_cb = my_disp_flush;
        disp_drv.draw_buf = &draw_buf;
        lv_disp_drv_register(&disp_drv);

        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = my_touchpad_read;
        lv_indev_drv_register(&indev_drv);
        
        ui_farm1_lcd35ct_overview_screen_init();
        ui_farm1_lcd35ct_wifi_settings_screen_init();
        lv_disp_load_scr(ui_farm1_lcd35ct_overview);

        lv_obj_t *value_label_map[4] = {
            ui_overview_temp_value_label,
            ui_overview_humi_value_label,
            ui_overview_light_value_label,
            ui_overview_soil_value_label,
        };

        for (uint8_t i=0;i<4;i++) {
            lv_label_set_text(value_label_map[i], "");
        }
          
        lv_timer_create(update_sensor_value_timer, 1000, NULL);
        lv_timer_create(update_time_timer, 300, NULL);
        lv_timer_create(update_connection_timer, 300, NULL);

        lv_obj_add_event_cb(ui_di35ct_overview_relay1_btn, relay_control_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_di35ct_overview_relay2_btn, relay_control_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_di35ct_overview_relay3_btn, relay_control_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_add_event_cb(ui_di35ct_overview_relay4_btn, relay_control_cb, LV_EVENT_CLICKED, NULL);
        lv_timer_create(update_output_status_timer, 300, NULL);

        /*Create a keyboard*/
        lv_obj_t * kb = lv_keyboard_create(ui_farm1_lcd35ct_wifi_settings);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_event_cb(ui_farm1_lcd35ct_wifi_name_inp, ta_event_cb, LV_EVENT_ALL, kb);
        lv_obj_add_event_cb(ui_farm1_lcd35ct_wifi_pass_inp, ta_event_cb, LV_EVENT_ALL, kb);

        lv_obj_add_event_cb(ui_di35ct_go_to_settings_btn, [](lv_event_t *e) {
            lv_textarea_set_text(ui_farm1_lcd35ct_wifi_name_inp, GlobalConfigs["wifi"]["ssid"].as<String>().c_str());
            lv_textarea_set_text(ui_farm1_lcd35ct_wifi_pass_inp, GlobalConfigs["wifi"]["password"].as<String>().c_str());
        }, LV_EVENT_CLICKED, NULL);

        lv_obj_add_event_cb(ui_farm1_lcd35ct_wifi_save_btn, [](lv_event_t *e) {
            WiFiManager_stop();
            const char *wifi_name = lv_textarea_get_text(ui_farm1_lcd35ct_wifi_name_inp);
            const char *wifi_password = lv_textarea_get_text(ui_farm1_lcd35ct_wifi_pass_inp);
            GlobalConfigs["wifi"]["ssid"] = wifi_name;
            GlobalConfigs["wifi"]["password"] = wifi_password;
            StorageConfigs_save();
            WiFi.disconnect();
            extern bool wifi_force_disconnect;
            wifi_force_disconnect = true;
            WiFiManager_run();

            lv_event_send(ui_farm1_lcd35ct_wifi_settings_back, LV_EVENT_CLICKED, NULL);
        }, LV_EVENT_CLICKED, NULL);

        lv_obj_add_event_cb(ui_farm1_lcd35ct_wifi_scan_btn, [](lv_event_t *e) {
            backdrop = lv_obj_create(lv_scr_act());
            lv_obj_set_size(backdrop, lv_pct(100), lv_pct(100));
            lv_obj_set_style_bg_color(backdrop, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(backdrop, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(backdrop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(backdrop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

            spinner = lv_spinner_create(backdrop, 1000, 60);
            lv_obj_set_size(spinner, 80, 80);
            lv_obj_center(spinner);
            lv_obj_set_style_arc_color(spinner, lv_color_hex(0x10C172), LV_PART_INDICATOR);

            WiFiManager_scanReq();
            lv_timer_create([](lv_timer_t * timer) {
                if (WiFiManager_isScanDone()) {
                    lv_obj_t * wifi_list = lv_list_create(backdrop);
                    lv_obj_set_style_border_width(wifi_list, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_size(wifi_list, 300, lv_pct(100));
                    lv_obj_set_align(wifi_list, LV_ALIGN_TOP_MID);

                    lv_event_cb_t event_handler = [](lv_event_t * e) {
                        const char * ssid = lv_list_get_btn_text(NULL, lv_event_get_target(e));
                        lv_textarea_set_text(ui_farm1_lcd35ct_wifi_name_inp, ssid);
                        lv_obj_del(backdrop);
                    };

                    int16_t found = WiFiManager_scanFound();
                    for (uint16_t i=0;i<found;i++) {
                        lv_obj_t * btn = lv_list_add_btn(wifi_list, LV_SYMBOL_WIFI, WiFi.SSID(i).c_str());
                        lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void *) WiFi.SSID(i).c_str());
                    }

                    lv_obj_del(spinner);
                    lv_timer_del(timer);
                }
            }, 100, NULL);
        }, LV_EVENT_CLICKED, NULL);

        pinMode(LCD_BL_PIN, OUTPUT);
        digitalWrite(LCD_BL_PIN, LOW);

        state = 1;
    }
    if (state == 1) {
        lv_timer_handler();
    }

    return DISPLAY_WORK_WELL;
}
