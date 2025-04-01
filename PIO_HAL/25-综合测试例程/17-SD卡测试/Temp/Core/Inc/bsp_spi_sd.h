/* bsp_spi_sd.h */
#ifndef __BSP_SPI_SD_H
#define __BSP_SPI_SD_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SD_CMD0    0
#define SD_CMD1    1  // SEND_OP_COND (V1卡初始化)
#define SD_CMD8    8
#define SD_CMD9    9
#define SD_CMD16   16
#define SD_CMD17   17
#define SD_CMD24   24
#define SD_CMD55   55
#define SD_CMD58   58
#define SD_ACMD41  41

typedef enum {
    SD_TYPE_V1,
    SD_TYPE_V2,
    SD_TYPE_V2HC
} SD_Type;

extern SPI_HandleTypeDef hspi1;

uint8_t SD_Init(void);
uint8_t SD_ReadBlock(uint32_t block_addr, uint8_t *buffer);
uint8_t SD_WriteBlock(uint32_t block_addr, uint8_t *buffer);
uint32_t SD_GetCapacity(void);
uint8_t SPI_ReadWriteByte(uint8_t TxData);
#endif
