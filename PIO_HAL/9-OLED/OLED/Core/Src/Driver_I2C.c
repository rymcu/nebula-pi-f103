#include <main.h>
#include <stdio.h>

// #define GPIOB_BASE 0x48000400
// #define GPIOB_ODR_OFFSET 0x14
#define GPIOB_ODR (*((volatile unsigned long *)(GPIOB_BASE + 0x0C)))

#define SDA_IN()                    \
    {                               \
        GPIOB->CRL &= ~(0xF << 20); \
        GPIOB->CRL |= (0x4 << 20);  \
    }
#define SDA_OUT()                   \
    {                               \
        GPIOB->CRL &= ~(0xF << 20); \
        GPIOB->CRL |= (0x2 << 20);  \
    }

// #define SDA_0 HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET)
// #define SDA_1 HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET)

// #define SCL_0 HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET)
// #define SCL_1 HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET)

#define SDA_0 GPIOB_ODR &= ~(1 << 5)

#define SDA_1 GPIOB_ODR |= (1 << 5)

#define SCL_0 GPIOB_ODR &= ~(1 << 3)

#define SCL_1 GPIOB_ODR |= (1 << 3)

#define SDA_I2C HAL_GPIO_ReadPin(OLED_SDA_GPIO_Port, OLED_SDA_Pin)
void delay_us(uint32_t udelay)
{
    uint32_t startval, tickn, delays, wait;
    startval = SysTick->VAL;
    tickn = HAL_GetTick();
    // sysc=72000;//Systemcoreclock /(1000U /uwTickFreg)
    delays = udelay * 72; // sysc/1000 *udelay;
    if (delays > startval)
    {
        while (HAL_GetTick() == tickn)
        {
            ;
        }
        wait = 72000 + startval - delays;
        while (wait < SysTick->VAL)
        {
            ;
        }
    }
    else
    {
        wait = startval - delays;
        while (wait < SysTick->VAL && HAL_GetTick() == tickn)
        {
            ;
        }
    }
}

void Delay_I2C(void)
{ // 延时函数，设置传输速率
    /*_nop_();
    _nop_();
    _nop_();
    _nop_();
    */
    // delay_us(1);
    __NOP();
    __NOP();
}
// 总线起始信号
void Start_I2C(void)
{
    // SCL高电平期间，拉低SDA
    SCL_0;
    SDA_1; // 在SCL低电平期间先将SDA拉高，为起始信号做准备
    Delay_I2C();
    SCL_1;
    Delay_I2C();
    SDA_0; // 拉低SDA，发送起始信号
    Delay_I2C();
    SCL_0;
}
// 总线停止信号
void Stop_I2C(void)
{
    // SCL高电平期间，拉高SDA
    SCL_0;
    SDA_0; // 在SCL低电平期间先将SDA拉低，为停止信号做准备
    Delay_I2C();
    SCL_1;
    Delay_I2C();
    SDA_1; // 拉高SDA，发送停止信号
    Delay_I2C();
    SCL_0;
}
// I2C写入字节dat,返回应答信号
unsigned char Wr_I2C(unsigned char dat)
{
    unsigned char ack;  // 存储应答位
    unsigned char mask; // 探测字节内某一位值的掩码变量

    for (mask = 0x80; mask != 0; mask >>= 1) // 从高位依次到低位
    {
        if ((mask & dat) == 0)
            SDA_0;
        else
            SDA_1;

        Delay_I2C();
        SCL_1;
        Delay_I2C();
        SCL_0; // 完成一位的传送
    }

    SDA_1; // 主机释放总线
    Delay_I2C();
    SDA_IN();
    SCL_1;
    ack = SDA_I2C; // 获取应答位
    Delay_I2C();
    SDA_OUT();
    SCL_0;

    return ack; // 返回0写入成功，返回1写入失败
}

// I2C读操作,并发送非应答信号
unsigned char RdNAK_I2C(void)
{
    unsigned char mask; // 探测字节内某一位值的掩码变量
    unsigned char dat = 0;

    SDA_1;                                   // 确保主机释放SDA
    for (mask = 0x80; mask != 0; mask >>= 1) // 从高位依次到低位
    {
        Delay_I2C();
        SDA_IN();
        SCL_1;
        if (SDA_I2C == 0)
            dat &= ~mask; // 为0时，dat对应位清零
        else
            dat |= mask; // 否则置1
        Delay_I2C();
        SDA_OUT();
        SCL_0;
    }
    SDA_1; // 8位数据传送完后，拉高SDA发送非应答信号
    Delay_I2C();
    SCL_1;
    Delay_I2C();
    SCL_0;

    return dat;
}

// I2C读操作,并发送应答信号
unsigned char RdACK_I2C(void)
{
    unsigned char mask; // 探测字节内某一位值的掩码变量
    unsigned char dat = 0;

    SDA_1;                                   // 确保主机释放SDA
    for (mask = 0x80; mask != 0; mask >>= 1) // 从高位依次到低位
    {
        Delay_I2C();
        SDA_IN();
        SCL_1;
        if (SDA_I2C == 0)
            dat &= ~mask; // 为0时，dat对应位清零
        else
            dat |= mask; // 否则置1
        Delay_I2C();
        SDA_OUT();
        SCL_0;
    }
    SDA_0; // 8位数据传送完后，拉低SDA发送应答信号
    Delay_I2C();
    SCL_1;
    Delay_I2C();
    SCL_0;

    return dat;
}