// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.3.3
// PROJECT: Farm1_LCD32CT

#ifndef _FARM1_LCD32CT_UI_H
#define _FARM1_LCD32CT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t * ui_farm1_lcd35ct_overview;
extern lv_obj_t * ui_di35ct_overview_top_head;
extern lv_obj_t * ui_overview_top_header_body;
extern lv_obj_t * ui_overview_cloud_img;
extern lv_obj_t * ui_overview_wifi_img;
extern lv_obj_t * ui_datetime_label;
extern lv_obj_t * ui_di35ct_overview_temp_body;
extern lv_obj_t * ui_overview_temp_icon;
extern lv_obj_t * ui_overview_temp_value_label;
extern lv_obj_t * ui_overview_temp_label;
extern lv_obj_t * ui_overview_temp_unit_label;
extern lv_obj_t * ui_di35ct_overview_humi_body;
extern lv_obj_t * ui_overview_humi_icon;
extern lv_obj_t * ui_overview_humi_value_label;
extern lv_obj_t * ui_overview_humi_label;
extern lv_obj_t * ui_overview_humi_unit_label;
extern lv_obj_t * ui_di35ct_overview_lux_body;
extern lv_obj_t * ui_overview_light_icon;
extern lv_obj_t * ui_overview_light_value_label;
extern lv_obj_t * ui_overview_light_label;
extern lv_obj_t * ui_overview_light_unit_label;
extern lv_obj_t * ui_di35ct_overview_soil_body;
extern lv_obj_t * ui_overview_soil_icon;
extern lv_obj_t * ui_overview_soil_value_label;
extern lv_obj_t * ui_overview_soil_label;
extern lv_obj_t * ui_overview_soil_unit_label;
void ui_event_di35ct_overview_relay1_btn(lv_event_t * e);
extern lv_obj_t * ui_di35ct_overview_relay1_btn;
void ui_event_di35ct_overview_relay2_btn(lv_event_t * e);
extern lv_obj_t * ui_di35ct_overview_relay2_btn;
void ui_event_di35ct_overview_relay3_btn(lv_event_t * e);
extern lv_obj_t * ui_di35ct_overview_relay3_btn;
void ui_event_di35ct_overview_relay4_btn(lv_event_t * e);
extern lv_obj_t * ui_di35ct_overview_relay4_btn;
void ui_event_di35ct_overview_relay4_btn1(lv_event_t * e);
extern lv_obj_t * ui_di35ct_overview_relay4_btn1;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_settings;
extern lv_obj_t * ui_di35ct_wifi_settings_top_head;
extern lv_obj_t * ui_wifi_settings_top_header_body;
extern lv_obj_t * ui_wifi_settings_header_label;
void ui_event_farm1_lcd35ct_wifi_settings_back(lv_event_t * e);
extern lv_obj_t * ui_farm1_lcd35ct_wifi_settings_back;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_settings_Labe;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_settings_Pane;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_name_label;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_name_inp;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_settings_Butt;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_pass_inp;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_pass_label;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_save_btn;
extern lv_obj_t * ui_farm1_lcd35ct_wifi_save_label;


LV_IMG_DECLARE(ui_img_330755264);    // assets\cloud-computing.png
LV_IMG_DECLARE(ui_img_wifi_png);    // assets\wifi.png
LV_IMG_DECLARE(ui_img_thermometer_png);    // assets\thermometer.png
LV_IMG_DECLARE(ui_img_humidity_png);    // assets\humidity.png
LV_IMG_DECLARE(ui_img_sun_png);    // assets\sun.png
LV_IMG_DECLARE(ui_img_797577270);    // assets\soil-analysis.png
LV_IMG_DECLARE(ui_img_517598710);    // assets\on-off-button.png
LV_IMG_DECLARE(ui_img_gear_png);    // assets\gear.png


LV_FONT_DECLARE(ui_font_Kanit16_en);
LV_FONT_DECLARE(ui_font_Kanit18_th_en);


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
