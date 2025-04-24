#ifndef __BSP_DRIVER_I2C_H__
#define __BSP_DRIVER_I2C_H__

#include <main.h>
#include <stdio.h>
#include "bsp_delay_us.h"

//IIC所有操作函数
void CTP_IIC_Init(void);                	//初始化IIC的IO口				 
void CTP_IIC_Start(void);				//发送IIC开始信号
void CTP_IIC_Stop(void);	  				//发送IIC停止信号
void CTP_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t CTP_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
uint8_t CTP_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CTP_IIC_Ack(void);					//IIC发送ACK信号
void CTP_IIC_NAck(void);					//IIC不发送ACK信号
//void delay_us(uint32_t udelay);

#endif
