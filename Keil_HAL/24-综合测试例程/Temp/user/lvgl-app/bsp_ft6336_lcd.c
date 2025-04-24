#include "bsp_ft6336_lcd.h"

/**
 * @brief FT6336初始化
 *
 * @note cs引脚拉低至少1ms后拉高，并等待足够的时间进行后续操作
 *
 * @param None
 *
 * @retval None
 */
void FT6336_Init(void)
{
    HAL_GPIO_WritePin(CTP_CS_GPIO_Port, CTP_CS_Pin, GPIO_PIN_RESET); // 低电平复位至少持续1ms
    rt_thread_delay(50);
    HAL_GPIO_WritePin(CTP_CS_GPIO_Port, CTP_CS_Pin, GPIO_PIN_SET); // 文档要求SDA，SCL上拉
    rt_thread_delay(500);
		uint8_t buf = MODE_POLLING;
		FT6336_Write_N_Data(FT6336_REG_G_MODE,&buf,1);//设置为轮询模式，即按下时中断引脚一直输出低电平
}

/**
 * @brief 设置FT6336数据地址
 *
 * @note 1.发送启动信号
 * @note 2.发送设备地址，写模式
 * @note 3.等待ack
 * @note 4.发送1 byte数据地址
 * @note 5.等待ack
 * @note 6.发送停止信号
 *
 * @param None
 *
 * @retval None
 */
void FT6336_WriteAddr(uint8_t addr)
{
    CTP_IIC_Start();
    CTP_IIC_Send_Byte(0x70); // 设备地址，写模式
    CTP_IIC_Wait_Ack();
    CTP_IIC_Send_Byte(addr); // 数据地址
    CTP_IIC_Wait_Ack();
}
/**
 * @brief 读取FT6336 N byte数据
 *
 * @note 1.发送启动信号
 * @note 2.发送设备地址，读模式
 * @note 3.等待ack
 * @note 4.读取N byte数据并每字节后发送ack
 * @note 5.发送停止信号
 *
 * @param buf 读取数据缓存区
 * @param N 读取数据长度
 *
 * @retval None
 */
void FT6336_Read_N_Data(uint8_t *buf, uint8_t N)
{
    CTP_IIC_Start();
    CTP_IIC_Send_Byte(0x71); // 设备地址，读模式
    CTP_IIC_Wait_Ack();
    for (int8_t i = 0; i < N; i++)
        buf[i] = CTP_IIC_Read_Byte(1); // 读取数据，每字节后发送ACK
    CTP_IIC_Stop();
}
/**
 * @brief 写入FT6336 N byte数据
 *
 * @note 1.发送启动信号
 * @note 2.发送设备地址，写模式
 * @note 3.等待ack
 * @note 4.写寄存器地址，并等待ACK
 * @note 5.发送N字节数据，每字节等待ACK
 * @note 6.发送停止信号
 *
 * @param addr 寄存器地址
 * @param buf 写入数地址
 * @param N 写入数据长度
 *
 * @retval None
 */
void FT6336_Write_N_Data(uint8_t addr,uint8_t *buf,uint8_t N)
{
    CTP_IIC_Start();
    CTP_IIC_Send_Byte(0x70); //设备地址，写模式
    CTP_IIC_Wait_Ack();
    CTP_IIC_Send_Byte(addr); 
    CTP_IIC_Wait_Ack();
		for (int8_t i = 0; i < N; i++) 
		{
			CTP_IIC_Send_Byte(buf[i]); 
			CTP_IIC_Wait_Ack();
		}
		CTP_IIC_Stop();
}
/**
 * @brief 读取FT6336 ID数据
 *
 * @note 1.写ID寄存器地址数据
 * @note 2.读取ID数据
 * @note 3.返回ID数据
 *
 * @param addr ID寄存器地址
 * @param N 读取数据长度
 *
 * @retval ID数据
 */
uint8_t FT6336_ReadID(uint8_t addr)
{
    uint8_t buf[1];
    FT6336_WriteAddr(addr);
    FT6336_Read_N_Data(buf, 1);
    return buf[0];
}

/**
 * @brief 读取FT6336寄存器数据
 *
 * @param addr ID寄存器地址
 * @param N 读取数据长度
 * @param buf 读取数据缓存区
 *
 * @retval none
 */
void FT6336_ReadReg(uint8_t addr, uint8_t *buf, uint8_t N)
{
    FT6336_WriteAddr(addr);
    FT6336_Read_N_Data(buf, N);
}
/**
 * @brief 读取FT6336触摸坐标数据
 *
 * @param x X坐标像素值
 * @param y Y坐标像素值
 *
 * @retval none
 */
void FT6336_ReadXY(int32_t *x,int32_t *y,uint8_t scr_scan_mode)
{
    uint8_t buf[4];
    FT6336_ReadReg(FT6336_REG_P1_XH, buf, 4); // 读取XH，XL，YH，YL
		uint16_t temp;
		//if(0x80 == (buf[0] & 0xF0))
		//{
			temp = buf[0] & 0x0F;
			*x = buf[1] + (temp << 8);

			temp = buf[2] & 0x0F;
			*y = buf[3] + (temp << 8);	

		if(scr_scan_mode == 6)
		{
			*x = 239 - *x;
			*y = 319 - *y;
		}			
		//}

		//else
		//{
			//*x = -1;
			//*y = -1;
		//}
}
/**
 * @brief 触摸检测
 *
 * @param none

 * @retval 1 有触摸，0 无触摸
 */
uint8_t CTP_TouchDetect(void)
{
	uint8_t buf;
	if(CTP_PENIRQ_Read() == CTP_PENIRQ)
	{
		delay_us(10);//20us防抖
		if(CTP_PENIRQ_Read() == CTP_PENIRQ)
		{
			FT6336_ReadReg(FT6336_REG_P1_XH, &buf, 1);
			if(0x80 == (buf & 0xF0)) return 1;
			else return 0;
		}
	return 0;	
	}
	else
		return 0;
}
