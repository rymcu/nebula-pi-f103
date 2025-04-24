#include "bsp_driver_i2c.h"

// #define GPIOB_BASE 0x48000400
// #define GPIOB_ODR_OFFSET 0x14
// #define GPIOB_ODR (*((volatile unsigned long *)(GPIOB_BASE + 0x0C)))

#define SDA_IN()                   \
    {                              \
        GPIOE->CRL &= ~(0xF << 8); \
        GPIOE->CRL |= (0x4 << 8);  \
    }
#define SDA_OUT()                  \
    {                              \
        GPIOE->CRL &= ~(0xF << 8); \
        GPIOE->CRL |= (0x2 << 8);  \
    }

#define SDA_0 HAL_GPIO_WritePin(CTP_SDA_GPIO_Port, CTP_SDA_Pin, GPIO_PIN_RESET)
#define SDA_1 HAL_GPIO_WritePin(CTP_SDA_GPIO_Port, CTP_SDA_Pin, GPIO_PIN_SET)

#define SCL_0 HAL_GPIO_WritePin(CTP_SCL_GPIO_Port, CTP_SCL_Pin, GPIO_PIN_RESET)
#define SCL_1 HAL_GPIO_WritePin(CTP_SCL_GPIO_Port, CTP_SCL_Pin, GPIO_PIN_SET)

// #define SDA_0 GPIOB_ODR &= ~(1 << 5)

// #define SDA_1 GPIOB_ODR |= (1 << 5)

// #define SCL_0 GPIOB_ODR &= ~(1 << 3)

// #define SCL_1 GPIOB_ODR |= (1 << 3)

#define SDA_I2C HAL_GPIO_ReadPin(CTP_SDA_GPIO_Port, CTP_SDA_Pin)

void Delay_I2C(void)
{ // 延时函数，设置传输速率
    /*_nop_();
    _nop_();
    _nop_();
    _nop_();
    */
    // elay_us(1);
    __NOP();
    __NOP();
}
// 产生IIC起始信号
void CTP_IIC_Start(void)
{
    SDA_OUT(); // sda线输出
    SDA_1;
    SCL_1;
    delay_us(5);
    SDA_0; // START:when CLK is high,DATA change form high to low
    delay_us(5);
    SCL_0; // 钳住I2C总线，准备发送或接收数据
}
// 产生IIC停止信号
void CTP_IIC_Stop(void)
{
    SDA_OUT(); // sda线输出
    SCL_1;
    delay_us(5);
    SDA_0; // STOP:when CLK is high DATA change form low to high
    delay_us(5);
    SDA_1; // 发送I2C总线结束信号
}
// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t CTP_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN(); // SDA设置为输入
    SDA_1;
    SCL_1;
    delay_us(5);
    while (SDA_I2C)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            CTP_IIC_Stop();
            return 1;
        }
        delay_us(5);
    }
    SCL_0; // 时钟输出0
    return 0;
}
// 产生ACK应答
void CTP_IIC_Ack(void)
{
    SCL_0;
    SDA_OUT();
    delay_us(5);
    SDA_0;
    delay_us(5);
    SCL_1;
    delay_us(5);
    SCL_0;
}
// 不产生ACK应答
void CTP_IIC_NAck(void)
{
    SCL_0;
    SDA_OUT();
    delay_us(5);
    SDA_1;
    delay_us(5);
    SCL_1;
    delay_us(5);
    SCL_0;
}
// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void CTP_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    SCL_0; // 拉低时钟开始数据传输
    delay_us(5);
    for (t = 0; t < 8; t++)
    {
        if ((txd & 0x80) >> 7)
        {
            SDA_1;
        }
        else
        {
            SDA_0;
        }
        txd <<= 1;
        SCL_1;
        delay_us(5);
        SCL_0;
        delay_us(5);
    }
}
// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t CTP_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;
    SDA_IN(); // SDA设置为输入
    delay_us(30);
    for (i = 0; i < 8; i++)
    {
        SCL_0;
        delay_us(5);
        SCL_1;
        receive <<= 1;
        if (SDA_I2C)
            receive++;
    }
    if (!ack)
        CTP_IIC_NAck(); // 发送nACK
    else
        CTP_IIC_Ack(); // 发送ACK
    return receive;
}
