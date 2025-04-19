#include "bsp_spi_flash.h"
#include <string.h>

/* 外部SPI句柄（在用户代码中初始化） */
extern SPI_HandleTypeDef *hspi_flash;

/* 私有宏 */
#define FLASH_CS_LOW()    HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_RESET)
#define FLASH_CS_HIGH()   HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET)

/* 私有函数声明 */
static HAL_StatusTypeDef SPI_Transmit(uint8_t *pData, uint16_t Size);
static HAL_StatusTypeDef SPI_Receive(uint8_t *pData, uint16_t Size);

/*********************** 驱动实现 ************************/

void Flash_Init(void) {
  FLASH_CS_HIGH();
}

void Flash_WriteEnable(void) {
  uint8_t cmd = W25X_WriteEnable;  // 修复：使用局部变量
  FLASH_CS_LOW();
  SPI_Transmit(&cmd, 1);
  FLASH_CS_HIGH();
}

void Flash_WriteDisable(void) {
  uint8_t cmd = W25X_WriteDisable;
  FLASH_CS_LOW();
  SPI_Transmit(&cmd, 1);
  FLASH_CS_HIGH();
}

uint8_t Flash_ReadStatusReg(void) {
  uint8_t cmd = W25X_ReadStatusReg1;  // 修复：使用局部变量
  uint8_t status;
  
  FLASH_CS_LOW();
  SPI_Transmit(&cmd, 1);
  SPI_Receive(&status, 1);
  FLASH_CS_HIGH();
  return status;
}

HAL_StatusTypeDef Flash_WaitBusy(void) {
  uint32_t tickstart = HAL_GetTick();
  while ((Flash_ReadStatusReg() & 0x01) == 0x01) {
    if (HAL_GetTick() - tickstart > FLASH_TIMEOUT_MS) {
      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

HAL_StatusTypeDef Flash_ReadData(uint32_t addr, uint8_t *pData, uint16_t Size) {
  uint8_t cmd[4] = {
    W25X_ReadData,
    (uint8_t)(addr >> 16),
    (uint8_t)(addr >> 8),
    (uint8_t)addr
  };
  
  FLASH_CS_LOW();
  if (SPI_Transmit(cmd, 4) != HAL_OK) {
    FLASH_CS_HIGH();
    return HAL_ERROR;
  }
  HAL_StatusTypeDef status = SPI_Receive(pData, Size);
  FLASH_CS_HIGH();
  return status;
}

HAL_StatusTypeDef Flash_PageProgram(uint32_t addr, uint8_t *pData, uint16_t Size) {
  /* 检查页边界 */
  if ((Size == 0) || (Size > 256) || ((addr % 256) + Size > 256)) {
    return HAL_ERROR;
  }

  uint8_t cmd[4] = {
    W25X_PageProgram,
    (uint8_t)(addr >> 16),
    (uint8_t)(addr >> 8),
    (uint8_t)addr
  };
  
  Flash_WriteEnable();
  
  FLASH_CS_LOW();
  if (SPI_Transmit(cmd, 4) != HAL_OK) {
    FLASH_CS_HIGH();
    return HAL_ERROR;
  }
  if (SPI_Transmit(pData, Size) != HAL_OK) {
    FLASH_CS_HIGH();
    return HAL_ERROR;
  }
  FLASH_CS_HIGH();
  
  return Flash_WaitBusy();
}

HAL_StatusTypeDef Flash_SectorErase(uint32_t addr) {
  uint8_t cmd[4] = {
    W25X_SectorErase,
    (uint8_t)(addr >> 16),
    (uint8_t)(addr >> 8),
    (uint8_t)addr
  };
  
  Flash_WriteEnable();
  
  FLASH_CS_LOW();
  if (SPI_Transmit(cmd, 4) != HAL_OK) {
    FLASH_CS_HIGH();
    return HAL_ERROR;
  }
  FLASH_CS_HIGH();
  
  return Flash_WaitBusy();
}

void Flash_ReadID(uint16_t *ManufacturerID, uint16_t *DeviceID) {
  uint8_t cmd[4] = {W25X_ManufactDeviceID, 0x00, 0x00, 0x00};
  uint8_t id[2];
  
  FLASH_CS_LOW();
  SPI_Transmit(cmd, 4);
  SPI_Receive(id, 2);
  FLASH_CS_HIGH();
  
  *ManufacturerID = id[0];
  *DeviceID = (id[1] << 8) | 0x00; // W25Q64设备ID为0x4017
}

void Flash_ReadJedecID(uint8_t *id) {
  uint8_t cmd = W25X_JedecID;  // 修复：使用局部变量
  FLASH_CS_LOW();
  SPI_Transmit(&cmd, 1);
  SPI_Receive(id, 3);
  FLASH_CS_HIGH();
}
/**
 * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
 * @param	pData，要写入数据的指针
 * @param  addr，写入地址
 * @param  Size，写入数据长度
 * @retval 无
 */
#define SPI_FLASH_PageSize  256
void FLASH_WriteData(uint32_t addr,uint8_t *pData, uint16_t Size)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
    Addr = addr % SPI_FLASH_PageSize;

    /*差count个数据值，刚好可以对齐到页地址*/
    count = SPI_FLASH_PageSize - Addr;
    /*计算出要写多少整数页*/
    NumOfPage =  Size / SPI_FLASH_PageSize;
    /*mod运算求余，计算出剩余不满一页的字节数*/
    NumOfSingle = Size % SPI_FLASH_PageSize;

    /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
    if (Addr == 0)
    {
        /* Size < SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
          Flash_PageProgram( addr,pData,Size);
        }
        else /* Size > SPI_FLASH_PageSize */
        {
            /*先把整数页都写了*/
            while (NumOfPage--)
            {
              Flash_PageProgram(addr,pData,  SPI_FLASH_PageSize);
              addr +=  SPI_FLASH_PageSize;
              pData += SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            Flash_PageProgram(addr, pData, NumOfSingle);
        }
    }
        /* 若地址与 SPI_FLASH_PageSize 不对齐  */
    else
    {
        /* Size < SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            /*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
            if (NumOfSingle > count)
            {
                temp = NumOfSingle - count;
                /*先写满当前页*/
                Flash_PageProgram(addr,pData,  count);

                addr +=  count;
                pData += count;
                /*再写剩余的数据*/
                Flash_PageProgram(addr,pData,  temp);
            }
            else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
            {
              Flash_PageProgram(addr, pData, Size);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            /*地址不对齐多出的count分开处理，不加入这个运算*/
            Size -= count;
            NumOfPage =  Size / SPI_FLASH_PageSize;
            NumOfSingle = Size % SPI_FLASH_PageSize;

            /* 先写完count个数据，为的是让下一次要写的地址对齐 */
            Flash_PageProgram(addr,pData, count);

            /* 接下来就重复地址对齐的情况 */
            addr +=  count;
            pData += count;
            /*把整数页都写了*/
            while (NumOfPage--)
            {
              Flash_PageProgram(addr,pData,  SPI_FLASH_PageSize);
              addr +=  SPI_FLASH_PageSize;
              pData += SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            if (NumOfSingle != 0)
            {
              Flash_PageProgram(addr,pData,NumOfSingle);
            }
        }
    }
}
/*********************** SPI底层传输 ************************/
static HAL_StatusTypeDef SPI_Transmit(uint8_t *pData, uint16_t Size) {
  return HAL_SPI_Transmit(hspi_flash, pData, Size, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef SPI_Receive(uint8_t *pData, uint16_t Size) {
  return HAL_SPI_Receive(hspi_flash, pData, Size, HAL_MAX_DELAY);
}
