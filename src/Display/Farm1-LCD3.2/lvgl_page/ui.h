// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.3.3
// PROJECT: Farm1_LCD32

#ifndef _FARM1_LCD32_UI_H
#define _FARM1_LCD32_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t * ui_overview;
extern lv_obj_t * ui_overview_top_header;
extern lv_obj_t * ui_overview_top_header_body;
extern lv_obj_t * ui_overview_cloud_img;
extern lv_obj_t * ui_overview_wifi_img;
extern lv_obj_t * ui_datetime_label;
extern lv_obj_t * ui_overview_temp_body;
extern lv_obj_t * ui_overview_temp_icon;
extern lv_obj_t * ui_overview_temp_value_label;
extern lv_obj_t * ui_overview_temp_label;
extern lv_obj_t * ui_overview_temp_unit_label;
extern lv_obj_t * ui_overview_humi_body;
extern lv_obj_t * ui_overview_humi_icon;
extern lv_obj_t * ui_overview_humi_value_label;
extern lv_obj_t * ui_overview_humi_label;
extern lv_obj_t * ui_overview_humi_unit_label;
extern lv_obj_t * ui_overview_light_body;
extern lv_obj_t * ui_overview_light_icon;
extern lv_obj_t * ui_overview_light_value_label;
extern lv_obj_t * ui_overview_light_label;
extern lv_obj_t * ui_overview_light_unit_label;
extern lv_obj_t * ui_overview_soil_body;
extern lv_obj_t * ui_overview_soil_icon;
extern lv_obj_t * ui_overview_soil_value_label;
extern lv_obj_t * ui_overview_soil_label;
extern lv_obj_t * ui_overview_soil_unit_label;


LV_IMG_DECLARE(ui_img_330755264);    // assets\cloud-computing.png
LV_IMG_DECLARE(ui_img_wifi_png);    // assets\wifi.png
LV_IMG_DECLARE(ui_img_thermometer_png);    // assets\thermometer.png
LV_IMG_DECLARE(ui_img_humidity_png);    // assets\humidity.png
LV_IMG_DECLARE(ui_img_sun_png);    // assets\sun.png
LV_IMG_DECLARE(ui_img_797577270);    // assets\soil-analysis.png


LV_FONT_DECLARE(ui_font_Kanit16_en);
LV_FONT_DECLARE(ui_font_Kanit18_th_en);


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
