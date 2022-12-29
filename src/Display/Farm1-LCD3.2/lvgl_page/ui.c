// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.1.1
// LVGL VERSION: 8.3.3
// PROJECT: Farm1_LCD32

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_overview;
lv_obj_t * ui_overview_top_header;
lv_obj_t * ui_overview_top_header_body;
lv_obj_t * ui_overview_cloud_img;
lv_obj_t * ui_overview_wifi_img;
lv_obj_t * ui_datetime_label;
lv_obj_t * ui_overview_temp_body;
lv_obj_t * ui_overview_temp_icon;
lv_obj_t * ui_overview_temp_value_label;
lv_obj_t * ui_overview_temp_label;
lv_obj_t * ui_overview_temp_unit_label;
lv_obj_t * ui_overview_humi_body;
lv_obj_t * ui_overview_humi_icon;
lv_obj_t * ui_overview_humi_value_label;
lv_obj_t * ui_overview_humi_label;
lv_obj_t * ui_overview_humi_unit_label;
lv_obj_t * ui_overview_light_body;
lv_obj_t * ui_overview_light_icon;
lv_obj_t * ui_overview_light_value_label;
lv_obj_t * ui_overview_light_label;
lv_obj_t * ui_overview_light_unit_label;
lv_obj_t * ui_overview_soil_body;
lv_obj_t * ui_overview_soil_icon;
lv_obj_t * ui_overview_soil_value_label;
lv_obj_t * ui_overview_soil_label;
lv_obj_t * ui_overview_soil_unit_label;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_overview_screen_init(void)
{
    ui_overview = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_overview, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_overview_top_header = lv_obj_create(ui_overview);
    lv_obj_set_width(ui_overview_top_header, 320);
    lv_obj_set_height(ui_overview_top_header, 85);
    lv_obj_set_align(ui_overview_top_header, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_overview_top_header, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_overview_top_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_overview_top_header, lv_color_hex(0x10C172), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_overview_top_header, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_overview_top_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_overview_top_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_overview_top_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_overview_top_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_overview_top_header, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_top_header_body = lv_obj_create(ui_overview_top_header);
    lv_obj_set_height(ui_overview_top_header_body, 50);
    lv_obj_set_width(ui_overview_top_header_body, lv_pct(100));
    lv_obj_set_align(ui_overview_top_header_body, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_overview_top_header_body, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_overview_top_header_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_overview_top_header_body, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_overview_top_header_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_overview_top_header_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_cloud_img = lv_img_create(ui_overview_top_header_body);
    lv_img_set_src(ui_overview_cloud_img, &ui_img_330755264);
    lv_obj_set_width(ui_overview_cloud_img, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_cloud_img, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_overview_cloud_img, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_overview_cloud_img, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_overview_cloud_img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_overview_cloud_img, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_overview_cloud_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_wifi_img = lv_img_create(ui_overview_top_header_body);
    lv_img_set_src(ui_overview_wifi_img, &ui_img_wifi_png);
    lv_obj_set_width(ui_overview_wifi_img, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_wifi_img, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_wifi_img, -45);
    lv_obj_set_y(ui_overview_wifi_img, 0);
    lv_obj_set_align(ui_overview_wifi_img, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_overview_wifi_img, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_overview_wifi_img, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_overview_wifi_img, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_overview_wifi_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_datetime_label = lv_label_create(ui_overview_top_header_body);
    lv_obj_set_width(ui_datetime_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_datetime_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_datetime_label, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_datetime_label, "00:00:00");
    lv_obj_set_style_text_color(ui_datetime_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_datetime_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_datetime_label, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_temp_body = lv_obj_create(ui_overview);
    lv_obj_set_width(ui_overview_temp_body, 145);
    lv_obj_set_height(ui_overview_temp_body, 83);
    lv_obj_set_x(ui_overview_temp_body, 10);
    lv_obj_set_y(ui_overview_temp_body, 50);
    lv_obj_clear_flag(ui_overview_temp_body, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_overview_temp_body, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_overview_temp_body, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_overview_temp_body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_overview_temp_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_overview_temp_body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_overview_temp_body, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_overview_temp_body, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_overview_temp_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_overview_temp_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_overview_temp_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_temp_icon = lv_img_create(ui_overview_temp_body);
    lv_img_set_src(ui_overview_temp_icon, &ui_img_thermometer_png);
    lv_obj_set_width(ui_overview_temp_icon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_temp_icon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_temp_icon, -9);
    lv_obj_set_y(ui_overview_temp_icon, 0);
    lv_obj_set_align(ui_overview_temp_icon, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_overview_temp_icon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_overview_temp_icon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_overview_temp_icon, lv_color_hex(0x10C273), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_overview_temp_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_temp_value_label = lv_label_create(ui_overview_temp_body);
    lv_obj_set_width(ui_overview_temp_value_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_temp_value_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_temp_value_label, -12);
    lv_obj_set_y(ui_overview_temp_value_label, 4);
    lv_obj_set_align(ui_overview_temp_value_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_temp_value_label, "99.9");
    lv_obj_set_style_text_color(ui_overview_temp_value_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_temp_value_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_temp_value_label, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_temp_label = lv_label_create(ui_overview_temp_body);
    lv_obj_set_width(ui_overview_temp_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_temp_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_temp_label, 5);
    lv_obj_set_y(ui_overview_temp_label, -1);
    lv_obj_set_align(ui_overview_temp_label, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_overview_temp_label, "อุณหภูมิ");
    lv_obj_set_style_text_color(ui_overview_temp_label, lv_color_hex(0x808B96), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_temp_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_temp_label, &ui_font_Kanit18_th_en, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_temp_unit_label = lv_label_create(ui_overview_temp_body);
    lv_obj_set_width(ui_overview_temp_unit_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_temp_unit_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_temp_unit_label, 5);
    lv_obj_set_y(ui_overview_temp_unit_label, 15);
    lv_obj_set_align(ui_overview_temp_unit_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_temp_unit_label, "°C");
    lv_obj_set_style_text_color(ui_overview_temp_unit_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_temp_unit_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_temp_unit_label, &ui_font_Kanit16_en, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_humi_body = lv_obj_create(ui_overview);
    lv_obj_set_width(ui_overview_humi_body, 145);
    lv_obj_set_height(ui_overview_humi_body, 83);
    lv_obj_set_x(ui_overview_humi_body, 165);
    lv_obj_set_y(ui_overview_humi_body, 50);
    lv_obj_clear_flag(ui_overview_humi_body, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_overview_humi_body, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_overview_humi_body, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_overview_humi_body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_overview_humi_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_overview_humi_body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_overview_humi_body, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_overview_humi_body, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_overview_humi_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_overview_humi_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_overview_humi_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_humi_icon = lv_img_create(ui_overview_humi_body);
    lv_img_set_src(ui_overview_humi_icon, &ui_img_humidity_png);
    lv_obj_set_width(ui_overview_humi_icon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_humi_icon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_humi_icon, -8);
    lv_obj_set_y(ui_overview_humi_icon, 0);
    lv_obj_set_align(ui_overview_humi_icon, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_overview_humi_icon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_overview_humi_icon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_overview_humi_icon, lv_color_hex(0x10C273), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_overview_humi_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_humi_value_label = lv_label_create(ui_overview_humi_body);
    lv_obj_set_width(ui_overview_humi_value_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_humi_value_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_humi_value_label, -28);
    lv_obj_set_y(ui_overview_humi_value_label, 4);
    lv_obj_set_align(ui_overview_humi_value_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_humi_value_label, "99.9");
    lv_obj_set_style_text_color(ui_overview_humi_value_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_humi_value_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_humi_value_label, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_humi_label = lv_label_create(ui_overview_humi_body);
    lv_obj_set_width(ui_overview_humi_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_humi_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_humi_label, 4);
    lv_obj_set_y(ui_overview_humi_label, -1);
    lv_obj_set_align(ui_overview_humi_label, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_overview_humi_label, "ความชื้น");
    lv_obj_set_style_text_color(ui_overview_humi_label, lv_color_hex(0x808B96), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_humi_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_humi_label, &ui_font_Kanit18_th_en, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_humi_unit_label = lv_label_create(ui_overview_humi_body);
    lv_obj_set_width(ui_overview_humi_unit_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_humi_unit_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_humi_unit_label, 5);
    lv_obj_set_y(ui_overview_humi_unit_label, 15);
    lv_obj_set_align(ui_overview_humi_unit_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_humi_unit_label, "%RH");
    lv_obj_set_style_text_color(ui_overview_humi_unit_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_humi_unit_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_humi_unit_label, &ui_font_Kanit16_en, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_light_body = lv_obj_create(ui_overview);
    lv_obj_set_width(ui_overview_light_body, 145);
    lv_obj_set_height(ui_overview_light_body, 83);
    lv_obj_set_x(ui_overview_light_body, 10);
    lv_obj_set_y(ui_overview_light_body, 145);
    lv_obj_clear_flag(ui_overview_light_body, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_overview_light_body, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_overview_light_body, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_overview_light_body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_overview_light_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_overview_light_body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_overview_light_body, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_overview_light_body, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_overview_light_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_overview_light_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_overview_light_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_light_icon = lv_img_create(ui_overview_light_body);
    lv_img_set_src(ui_overview_light_icon, &ui_img_sun_png);
    lv_obj_set_width(ui_overview_light_icon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_light_icon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_light_icon, -4);
    lv_obj_set_y(ui_overview_light_icon, 0);
    lv_obj_set_align(ui_overview_light_icon, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_overview_light_icon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_overview_light_icon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_overview_light_icon, lv_color_hex(0x10C273), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_overview_light_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_light_value_label = lv_label_create(ui_overview_light_body);
    lv_obj_set_width(ui_overview_light_value_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_light_value_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_light_value_label, -30);
    lv_obj_set_y(ui_overview_light_value_label, 4);
    lv_obj_set_align(ui_overview_light_value_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_light_value_label, "99.9");
    lv_obj_set_style_text_color(ui_overview_light_value_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_light_value_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_light_value_label, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_light_label = lv_label_create(ui_overview_light_body);
    lv_obj_set_width(ui_overview_light_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_light_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_light_label, 5);
    lv_obj_set_y(ui_overview_light_label, -1);
    lv_obj_set_align(ui_overview_light_label, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_overview_light_label, "แสง");
    lv_obj_set_style_text_color(ui_overview_light_label, lv_color_hex(0x808B96), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_light_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_light_label, &ui_font_Kanit18_th_en, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_light_unit_label = lv_label_create(ui_overview_light_body);
    lv_obj_set_width(ui_overview_light_unit_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_light_unit_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_light_unit_label, 5);
    lv_obj_set_y(ui_overview_light_unit_label, 15);
    lv_obj_set_align(ui_overview_light_unit_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_light_unit_label, "kLux");
    lv_obj_set_style_text_color(ui_overview_light_unit_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_light_unit_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_light_unit_label, &ui_font_Kanit16_en, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_soil_body = lv_obj_create(ui_overview);
    lv_obj_set_width(ui_overview_soil_body, 145);
    lv_obj_set_height(ui_overview_soil_body, 83);
    lv_obj_set_x(ui_overview_soil_body, 165);
    lv_obj_set_y(ui_overview_soil_body, 145);
    lv_obj_clear_flag(ui_overview_soil_body, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_overview_soil_body, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_overview_soil_body, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_overview_soil_body, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_overview_soil_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_overview_soil_body, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_overview_soil_body, 60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_overview_soil_body, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_overview_soil_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_overview_soil_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_overview_soil_body, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_soil_icon = lv_img_create(ui_overview_soil_body);
    lv_img_set_src(ui_overview_soil_icon, &ui_img_797577270);
    lv_obj_set_width(ui_overview_soil_icon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_soil_icon, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_soil_icon, -7);
    lv_obj_set_y(ui_overview_soil_icon, 0);
    lv_obj_set_align(ui_overview_soil_icon, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_overview_soil_icon, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_overview_soil_icon, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_img_recolor(ui_overview_soil_icon, lv_color_hex(0x10C273), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_overview_soil_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_soil_value_label = lv_label_create(ui_overview_soil_body);
    lv_obj_set_width(ui_overview_soil_value_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_soil_value_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_soil_value_label, -10);
    lv_obj_set_y(ui_overview_soil_value_label, 4);
    lv_obj_set_align(ui_overview_soil_value_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_soil_value_label, "99.9");
    lv_obj_set_style_text_color(ui_overview_soil_value_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_soil_value_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_soil_value_label, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_soil_label = lv_label_create(ui_overview_soil_body);
    lv_obj_set_width(ui_overview_soil_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_soil_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_soil_label, 5);
    lv_obj_set_y(ui_overview_soil_label, -1);
    lv_obj_set_align(ui_overview_soil_label, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_overview_soil_label, "ความชื้นดิน");
    lv_obj_set_style_text_color(ui_overview_soil_label, lv_color_hex(0x808B96), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_soil_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_soil_label, &ui_font_Kanit18_th_en, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_overview_soil_unit_label = lv_label_create(ui_overview_soil_body);
    lv_obj_set_width(ui_overview_soil_unit_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_overview_soil_unit_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_overview_soil_unit_label, 5);
    lv_obj_set_y(ui_overview_soil_unit_label, 15);
    lv_obj_set_align(ui_overview_soil_unit_label, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_overview_soil_unit_label, "%");
    lv_obj_set_style_text_color(ui_overview_soil_unit_label, lv_color_hex(0x2C3E50), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_overview_soil_unit_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_overview_soil_unit_label, &ui_font_Kanit16_en, LV_PART_MAIN | LV_STATE_DEFAULT);

}

