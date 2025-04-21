#include "u8g2.h"
#include "main.h"  // 确保包含HAL的GPIO头文件

// 定义U8g2全局对象
static u8g2_t u8g2;

// 微秒级延时（根据系统时钟调整）
static void Delay_us(uint32_t us) {
    us *= (SystemCoreClock / 1000000) / 5;
    while (us--) __NOP();
}

// 模拟I2C起始信号
static void u8x8_gpio_i2c_start(u8x8_t *u8x8) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // SDA高
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // SCL高
    Delay_us(5);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // SDA低
    Delay_us(5);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // SCL低
}

// 模拟I2C停止信号
static void u8x8_gpio_i2c_stop(u8x8_t *u8x8) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // SDA低
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);   // SCL高
    Delay_us(5);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);   // SDA高
    Delay_us(5);
}

// 模拟I2C字节传输
static uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    if (msg == U8X8_MSG_BYTE_START) {
        u8x8_gpio_i2c_start(u8x8);
    } else if (msg == U8X8_MSG_BYTE_SEND) {
        uint8_t *data = (uint8_t *)arg_ptr;
        for (uint8_t i = 0; i < arg_int; i++) {
            uint8_t byte = data[i];
            for (uint8_t j = 0; j < 8; j++) {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (byte & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
                byte <<= 1;
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // SCL高
                Delay_us(5);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // SCL低
                Delay_us(5);
            }
            // 应答位（忽略）
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // SDA高
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // SCL高
            Delay_us(5);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // SCL低
        }
    } else if (msg == U8X8_MSG_BYTE_END) {
        u8x8_gpio_i2c_stop(u8x8);
    }
    return 1;
}

// 初始化U8g2
void U8g2_Init(void) {
    // 禁用JTAG，释放PB3/PB5
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    // 初始化U8g2对象
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SendBuffer(&u8g2);
}

// 获取U8g2对象指针
u8g2_t* GetU8g2Ptr(void) {
    return &u8g2;
}