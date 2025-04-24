#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"
#include  "u8g2.h"
#include "u8x8.h"
#include "bsp_delay_us.h"

// 全局U8g2对象声明
extern u8g2_t u8g2;

// OLED初始化函数
void OLED_Init(void);
void OLED_DisplayString(uint8_t x, uint8_t y, const char *str);
#endif /* __OLED_H */
