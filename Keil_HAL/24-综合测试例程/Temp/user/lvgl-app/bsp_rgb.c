#include "bsp_rgb.h"

// 发送单个比特
#define HIGH 5
#define LOW  2
uint16_t sendBit(uint8_t bitVal)
{
    if (bitVal)
    {
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);
        // 根据WS2812时序要求，高电平时间
        for (int i = 0; i < HIGH; i++)
        {
					__ASM volatile ("nop");//14ns
        }
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_RESET);
        // 低电平时间
        for (int j = 0; j < LOW; j++)
        {
					__ASM volatile ("nop");
        }
    }
    else
    {
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);
        for (int i = 0; i < LOW; i++)
        {
					__ASM volatile ("nop");
        }
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_RESET);
        for (int j = 0; j < HIGH; j++)
        {
					__ASM volatile ("nop");
        }
    }
}

// 发送一个字节
void sendByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        sendBit(byte & 0x80);
        byte <<= 1;
    }
}

void ry_set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    sendByte(g);
    sendByte(r);
    sendByte(b);
}