#ifndef __BSP_LCD_DRIVER_H
#define __BSP_LCD_DRIVER_H

#include "stm32f1xx_hal.h"

//-------------------------------
// 用户配置
//-------------------------------
/*----------------------------------
FSMC信号	STM32引脚	LCD引脚
D0-D15	  PD0-PD15	DB0-DB15
A16	      PD11	    RS
NE1	      PD7	      CS
NOE	      PD4	      RD
NWE	      PD5	      WR
-------------------------------------
          PE1       LCD_RST
					PD12      LCD_BL
-----------------------------------*/
#define LCD_WIDTH   240   // 屏幕宽度
#define LCD_HEIGHT  320   // 屏幕高度

#define ILI9341_LESS_PIXEL  240
#define ILI9341_MORE_PIXEL  320
//-------------------------------
// LCD型号枚举
//-------------------------------
typedef enum {
    LCD_UNKNOWN = 0,
    LCD_ILI9341,
    LCD_ST7789V
} LCD_TypeDef;

//-------------------------------
// FSMC地址定义（A16=RS）
//-------------------------------
#define LCD_CMD_ADDR   ((uint32_t)0x60000000)  // RS=0（命令）
#define LCD_DATA_ADDR  ((uint32_t)0x60020000)  // RS=1（数据，A16=1）

//-------------------------------
// 颜色格式（RGB565）
//-------------------------------
typedef uint16_t LCD_ColorTypeDef;

//-------------------------------
// 函数声明
//-------------------------------
LCD_TypeDef LCD_DetectModel(void);  // 检测LCD型号
void LCD_Init(void);                // 初始化LCD
void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LCD_FillColor(LCD_ColorTypeDef color);
void LCD_DrawPixel(uint16_t x, uint16_t y, LCD_ColorTypeDef color);
void LCD_SetGramScan(uint8_t scan_mode);
void ry_lcd_fill(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,uint8_t* color);
#endif /* __BSP_LCD_DRIVER_H */
