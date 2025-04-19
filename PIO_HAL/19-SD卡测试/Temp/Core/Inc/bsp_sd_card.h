#ifndef __SD_CARD_H
#define __SD_CARD_H

#include "stm32f1xx_hal.h"

// SD卡类型定义
#define SD_TYPE_ERR     0x00
#define SD_TYPE_MMC     0x01
#define SD_TYPE_V1      0x02
#define SD_TYPE_V2      0x04
#define SD_TYPE_V2HC    0x06

// SD卡指令表
#define CMD0    0       // 卡复位
#define CMD8    8       // 发送接口条件
#define CMD55   55      // APP命令前缀
#define CMD41   41      // 初始化命令
#define CMD58   58      // 读OCR寄存器
#define CMD17   17      //命令17，读sector
#define CMD24   24      //命令24，写sector
#define CMD9    9       //命令9 ，读CSD数据
#define CMD12   12      //停止数据传送
// 全局变量和函数声明
extern SPI_HandleTypeDef hspi1;
extern uint8_t SD_Type;

uint8_t SD_Init(void);
uint8_t SD_ReadBlock(uint32_t block_addr, uint8_t *buf);
uint8_t SD_WriteBlock(uint32_t block_addr, const uint8_t *buf);
uint32_t SD_GetCapacity(void);
uint8_t SPI_ReadWriteByte(uint8_t data);
#endif /* __SD_CARD_H */
