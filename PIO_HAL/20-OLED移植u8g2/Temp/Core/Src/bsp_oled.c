#include "bsp_oled.h"

// 定义模拟I2C引脚
#define SCL_PIN    GPIO_PIN_3
#define SCL_PORT   GPIOB
#define SDA_PIN    GPIO_PIN_5
#define SDA_PORT   GPIOB

// 全局U8g2对象定义
u8g2_t u8g2;

// 私有函数声明
uint8_t u8x8_byte_sw_i2c_1(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8x8_gpio_delay(uint8_t us);

//---------------------------------------------------------------------
// 模拟I2C驱动函数
//---------------------------------------------------------------------

// 延时函数（需根据实际时钟优化）
void u8x8_gpio_delay(uint8_t us) {
  // 粗略延时（HAL_Delay单位为ms，不适用于高速场景）
  HAL_Delay(1);  // 实际项目建议改用循环延时或定时器

}

// 模拟I2C字节传输函数
uint8_t u8x8_byte_sw_i2c_1(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
  switch(msg) {
    case U8X8_MSG_BYTE_SEND:
      for (uint8_t i = 0; i < arg_int; i++) {
        uint8_t data = ((uint8_t *)arg_ptr)[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
          HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
          data <<= 1;
          HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
          u8x8_gpio_delay(1);
          HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
        }
      }
      break;

    case U8X8_MSG_BYTE_INIT:
      HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
      HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
      break;

    case U8X8_MSG_BYTE_START_TRANSFER:
      HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
      HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
      u8x8_gpio_delay(1);
      HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
      u8x8_gpio_delay(1);
      HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
      break;

    case U8X8_MSG_BYTE_END_TRANSFER:
      HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
      u8x8_gpio_delay(1);
      HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
      u8x8_gpio_delay(1);
      break;

    default:
      return 0;
  }
  return 1;
}
// 示例 gpio_and_delay_cb 函数
unsigned char my_gpio_and_delay_cb(u8x8_t *u8x8, unsigned char msg, unsigned char arg_int, void *arg_ptr) {
  // 处理 GPIO 操作和延时
  switch (msg) {
      // 处理不同的消息
      default:
          return 0;
  }
}
//---------------------------------------------------------------------
// OLED初始化函数
//---------------------------------------------------------------------
void OLED_Init(void) {
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c_1,my_gpio_and_delay_cb);
  u8g2_SetI2CAddress(&u8g2, 0x3D << 1);  // 0x3C << 1 或 0x3D << 1
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearBuffer(&u8g2);
}

//---------------------------------------------------------------------
// 显示字符串函数（示例）
//---------------------------------------------------------------------
void OLED_DisplayString(uint8_t x, uint8_t y, const char *str) {
  u8g2_DrawStr(&u8g2, x, y, str);
  u8g2_SendBuffer(&u8g2);
}