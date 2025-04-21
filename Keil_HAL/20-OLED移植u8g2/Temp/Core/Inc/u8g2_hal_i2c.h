#ifndef __U8G2_HAL_I2C_H
#define __U8G2_HAL_I2C_H

#include "stm32f1xx_hal.h"
#include "u8g2.h"

// 初始化函数
void U8g2_Init(void);
// 获取U8g2对象指针（用于外部调用绘图函数）
u8g2_t* GetU8g2Ptr(void);

#endif /* __U8G2_HAL_I2C_H */
