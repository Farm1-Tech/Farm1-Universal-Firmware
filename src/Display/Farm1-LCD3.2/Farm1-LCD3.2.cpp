#include "Arduino.h"
#include <WiFi.h>

#include <TFT_eSPI.h>
#include <lvgl.h>

#include "./Farm1-LCD3.2.h"
#include "../DisplayDef.h"
#include "Board/Board.h"
#include "Sensor/Sensor.h"
#include "Cloud/Cloud.h"
#include "Time/SystemTime.h"

#ifdef BOARD_FARM1
#define LCD_BL_PIN 32
#else
#error "Board not support"
#endif

TFT_eSPI tft = TFT_eSPI();

static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

// -----------
extern void load_page() ;

extern lv_obj_t* txtTime;
extern lv_obj_t* imgWiFi;
extern lv_obj_t* imgNexpie;

extern lv_obj_t* txtTemp;
extern lv_obj_t* txtHumi;
extern lv_obj_t* txtHumi2;
extern lv_obj_t* txtLight;
// -----------

/* Display flushing */
static void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

static void updateSensorValueTask(lv_task_t*) {
    float temp;
    lv_label_set_text(txtTemp, Sensor_getValueOne(TEMPERATURE, (void*) &temp) == WORK_WELL ? String(temp, 1).c_str() : "");
    lv_obj_align(txtTemp, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -30, -37);

    float humidity;
    lv_label_set_text(txtHumi, Sensor_getValueOne(HUMIDITY, (void*) &humidity) == WORK_WELL ? String(humidity, 1).c_str() : "");
    lv_obj_align(txtHumi, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -23, -38);

    float soil;
    lv_label_set_text(txtHumi2, Sensor_getValueOne(SOIL, (void*) &soil) == WORK_WELL ? String(soil, 1).c_str() : "");
    lv_obj_align(txtHumi2, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -24, -38);

    float light;
    lv_label_set_text(txtLight, Sensor_getValueOne(AMBIENT_LIGHT, (void*) &light) == WORK_WELL ? String(light / 1000.0f, 1).c_str() : "");
    lv_obj_align(txtLight, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -47, -38); 
}

static void updateTimeTask(lv_task_t*) {
    struct tm timeinfo;
    Time_now(&timeinfo);

    static char time_str_buff[20];
    sprintf(time_str_buff, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    lv_label_set_text(txtTime, time_str_buff);
    lv_obj_align(txtTime, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);  
}

static void updateConnectionTask(lv_task_t*) {
    if (WiFi.isConnected()) {
      lv_obj_set_hidden(imgWiFi, false);
      if (Cloud_getStatus() == CLOUD_CONNECTED) {
        lv_obj_set_hidden(imgNexpie, false);
      } else {
        lv_obj_set_hidden(imgNexpie, !lv_obj_get_hidden(imgNexpie));
      }
    } else {
      lv_obj_set_hidden(imgWiFi, !lv_obj_get_hidden(imgWiFi));
      lv_obj_set_hidden(imgNexpie, true);
    }
}

static int state = 0;

DisplayStatus_t Farm1_LCD_3_2_process(void *args) {
    if (state == 0) { // display init
        tft.begin(); /* TFT init */
        tft.setRotation(3); /* Landscape orientation */

        lv_init();

        lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

        /*Initialize the display*/
        lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.hor_res = 320;
        disp_drv.ver_res = 240;
        disp_drv.flush_cb = my_disp_flush;
        disp_drv.buffer = &disp_buf;
        lv_disp_drv_register(&disp_drv);

        load_page();

        // Set all label
        lv_label_set_text(txtTime, "LOADING");
        lv_obj_align(txtTime, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);

        lv_obj_set_hidden(imgWiFi, false);
        lv_obj_set_hidden(imgNexpie, false);

        lv_label_set_text(txtTemp, "");
        lv_obj_align(txtTemp, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -30, -37);

        lv_label_set_text(txtHumi, "");
        lv_obj_align(txtHumi, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -23, -38);

        lv_label_set_text(txtHumi2, "");
        lv_obj_align(txtHumi2, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -24, -38);

        lv_label_set_text(txtLight, "");
        lv_obj_align(txtLight, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -47, -38);

        pinMode(LCD_BL_PIN, OUTPUT);
        digitalWrite(LCD_BL_PIN, LOW);

        lv_task_create(updateSensorValueTask, 1000, LV_TASK_PRIO_MID, NULL);
        lv_task_create(updateTimeTask, 300, LV_TASK_PRIO_MID, NULL);
        lv_task_create(updateConnectionTask, 300, LV_TASK_PRIO_MID, NULL);

        state = 1;
    }
    if (state == 1) {
        lv_task_handler(); /* let the GUI do its work */
    }

    return DISPLAY_WORK_WELL;
}

