#include "bsp_oled.h"
#include "main.h"
// 定义模拟I2C引脚
#define SCL_PIN    GPIO_PIN_3
#define SCL_PORT   GPIOB
#define SDA_PIN    GPIO_PIN_5
#define SDA_PORT   GPIOB

// 私有函数声明
uint8_t u8g2_gpio_and_delay_stm32_soft_iic(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
void u8g2_logo(u8g2_t *u8g2)
{
		uint8_t x0=20;
		u8g2_ClearBuffer(u8g2); 
    u8g2_SetFontMode(u8g2, 1); 
    u8g2_SetFontDirection(u8g2, 0); 
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, x0, 20, "U");
    
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21+x0,8,"8");
        
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51+x0,30,"g");
    u8g2_DrawStr(u8g2, 67+x0,30,"\xb2");
    
    u8g2_DrawHLine(u8g2, 2+x0, 35, 47);
    u8g2_DrawHLine(u8g2, 3+x0, 36, 47);
    u8g2_DrawVLine(u8g2, 45+x0, 32, 12);
    u8g2_DrawVLine(u8g2, 46+x0, 33, 12);
  
    u8g2_SetFont(u8g2, u8g2_font_pxplusibmvga8_m_all);
    u8g2_DrawStr(u8g2, 30,60,"rymcu.com");
	
		u8g2_SendBuffer(u8g2);
		rt_thread_delay(1000);
}
//---------------------------------------------------------------------
// OLED初始化函数
//---------------------------------------------------------------------
void OLED_Init(void) {
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c,u8g2_gpio_and_delay_stm32_soft_iic);
	u8g2_InitDisplay(&u8g2); 
	u8g2_SetPowerSave(&u8g2,0);
	u8g2_ClearBuffer(&u8g2);
	rt_thread_delay(1000);
	u8g2_logo(&u8g2);
}

//---------------------------------------------------------------------
// 显示字符串函数（示例）
//---------------------------------------------------------------------
void OLED_DisplayString(uint8_t x, uint8_t y, const char *str) {
  u8g2_DrawStr(&u8g2, x, y, str);
  u8g2_SendBuffer(&u8g2);
}
//---------------------------------------------------------------------
// 软件I2C
//---------------------------------------------------------------------
uint8_t u8g2_gpio_and_delay_stm32_soft_iic(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg){
			case U8X8_MSG_DELAY_MILLI:
				rt_thread_delay(arg_int);
			break;
			case U8X8_MSG_DELAY_10MICRO:
				delay_us(10);
			break;	
			case U8X8_MSG_DELAY_100NANO:
				__NOP();
			break;
			case U8X8_MSG_GPIO_I2C_CLOCK:
				if (arg_int) HAL_GPIO_WritePin(SCL_PORT,SCL_PIN,GPIO_PIN_SET);
				else HAL_GPIO_WritePin(SCL_PORT,SCL_PIN,GPIO_PIN_RESET);
			break;
			case U8X8_MSG_GPIO_I2C_DATA:
				if (arg_int) HAL_GPIO_WritePin(SDA_PORT,SDA_PIN,GPIO_PIN_SET);
				else HAL_GPIO_WritePin(SDA_PORT,SDA_PIN,GPIO_PIN_RESET);;
			break;
			default:
				return 0; 
	}
	return 1; 
}
