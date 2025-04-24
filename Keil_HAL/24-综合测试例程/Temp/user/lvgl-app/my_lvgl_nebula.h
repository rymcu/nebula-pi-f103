#ifndef __MY_LVGL_NEBULA_H__
#define __MY_LVGL_NEBULA_H__

#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_SILVER LV_COLOR_MAKE(0xC0, 0xC0, 0xC0)
#define LV_COLOR_GRAY LV_COLOR_MAKE(0x80, 0x80, 0x80)
#define LV_COLOR_BLACK LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_RED LV_COLOR_MAKE(0xFF, 0x00, 0x00)
#define LV_COLOR_MAROON LV_COLOR_MAKE(0x80, 0x00, 0x00)
#define LV_COLOR_YELLOW LV_COLOR_MAKE(0xFF, 0xFF, 0x00)
#define LV_COLOR_OLIVE LV_COLOR_MAKE(0x80, 0x80, 0x00)
#define LV_COLOR_LIME LV_COLOR_MAKE(0x00, 0xFF, 0x00)
#define LV_COLOR_GREEN LV_COLOR_MAKE(0x00, 0x80, 0x00)
#define LV_COLOR_CYAN LV_COLOR_MAKE(0x00, 0xFF, 0xFF)
#define LV_COLOR_AQUA LV_COLOR_CYAN
#define LV_COLOR_TEAL LV_COLOR_MAKE(0x00, 0x80, 0x80)
#define LV_COLOR_BLUE LV_COLOR_MAKE(0x00, 0x00, 0xFF)
#define LV_COLOR_NAVY LV_COLOR_MAKE(0x00, 0x00, 0x80)
#define LV_COLOR_MAGENTA LV_COLOR_MAKE(0xFF, 0x00, 0xFF)
#define LV_COLOR_PURPLE LV_COLOR_MAKE(0x80, 0x00, 0x80)
#define LV_COLOR_ORANGE LV_COLOR_MAKE(0xFF, 0xA5, 0x00)

LV_FONT_DECLARE(my_web_font_30)

#define MY_ICON_LED "\xEE\xA4\x8E"    // LED
#define MY_ICON_RGB "\xEE\x99\xB7"    // RGB
#define MY_ICON_SET "\xEE\x98\x8F"    // SET
#define MY_ICON_BTN "\xEE\x98\xAF"    // BTN
#define MY_ICON_CAN "\xEE\x9B\x9F"    // CAN
#define MY_ICON_UART "\xEE\x98\x8A"   // UART
#define MY_ICON_MUYU_S "\xEE\x98\x81" // MUYU

lv_obj_t *my_lvgl_nebula(void);

#endif
