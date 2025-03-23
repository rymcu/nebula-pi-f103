#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* 硬件相关配置（根据实际引脚修改） */
#define FLASH_CS_PORT        GPIOA
#define FLASH_CS_PIN         GPIO_PIN_1

/* SPI句柄指针（在用户代码中初始化） */
extern SPI_HandleTypeDef *hspi_flash;

/* W25Q64指令集 */
#define W25X_WriteEnable        0x06
#define W25X_WriteDisable       0x04
#define W25X_ReadStatusReg1     0x05
#define W25X_PageProgram        0x02
#define W25X_ReadData           0x03
#define W25X_SectorErase        0x20
#define W25X_ManufactDeviceID   0x90
#define W25X_JedecID            0x9F

/* 超时配置 */
#define FLASH_TIMEOUT_MS        5000  // 5秒超时

/* 函数声明 */
void Flash_Init(void);
void Flash_WriteEnable(void);
void Flash_WriteDisable(void);
uint8_t Flash_ReadStatusReg(void);
HAL_StatusTypeDef Flash_WaitBusy(void);
HAL_StatusTypeDef Flash_ReadData(uint32_t addr, uint8_t *pData, uint16_t Size);
void FLASH_WriteData(uint32_t addr,uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef Flash_PageProgram(uint32_t addr, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef Flash_SectorErase(uint32_t addr);
void Flash_ReadID(uint16_t *ManufacturerID, uint16_t *DeviceID);
void Flash_ReadJedecID(uint8_t *id);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_SPI_FLASH_H */
