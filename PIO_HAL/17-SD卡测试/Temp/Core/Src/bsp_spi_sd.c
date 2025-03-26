/* bsp_spi_sd.c */
#include "bsp_spi_sd.h"

/* 外部SPI句柄（在用户代码中初始化） */
extern SPI_HandleTypeDef *hspi_flash_sd;
static SD_Type sd_type;
uint8_t buff[4];
static void SD_CS_Enable(void) { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); }
static void SD_CS_Disable(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
    SPI_ReadWriteByte(0xFF);
}

static uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t tx_buf[6] = {0x40 | cmd,
                         (uint8_t)(arg >> 24),
                         (uint8_t)(arg >> 16),
                         (uint8_t)(arg >> 8),
                         (uint8_t)arg,
                         crc};
    uint8_t response;
    SD_CS_Disable(); // 先取消片选
    SD_CS_Enable();  // 再使能片选
    for (uint8_t i = 0; i < 10; i++)
    {
        response = SPI_ReadWriteByte(0xFF);
        if (response == 0xFF) break;
    }
    for (uint8_t i = 0; i < 6; i++)
    {
        SPI_ReadWriteByte(tx_buf[i]);
    }
    response = SPI_ReadWriteByte(0xFF);
    return response;
}

#define SD_INIT_TIMEOUT 1000 // 全局超时阈值

uint8_t SD_Init(void)
{
    uint8_t response, ocr[4];
    uint32_t timeout;

    // 初始化阶段保持低速时钟
    // MX_SPI1_Init(SPI_BAUDRATEPRESCALER_256);

    // 发送至少74个时钟脉冲（发送80个空字节）
    SD_CS_Disable();

    for (uint8_t i = 0; i < 9; i++)
        SPI_ReadWriteByte(0xFF);
    SD_CS_Enable();
    // 发送CMD0（带超时）
    timeout = SD_INIT_TIMEOUT;
    do
    {
        response = SD_SendCommand(SD_CMD0, 0, 0x95);
        if (--timeout == 0)
            return 0xF0; // 错误码0xFF表示CMD0无响应
    } while (response != 0x01);

    // 发送CMD8检测卡类型
    response = SD_SendCommand(SD_CMD8, 0x1AA, 0x87);
    // return response;
    if (response == 0x01)
    { // SDC V2+
        HAL_SPI_Receive(&hspi1, ocr, 4, 100);
        if (ocr[3] != 0xAA)
            return 0xFE; // 错误码0xFE表示电压不匹配
        // return ocr[3];
        //  发送ACMD41初始化（带超时）
        timeout = SD_INIT_TIMEOUT;
        do
        {
            SD_SendCommand(SD_CMD55, 0, 0x65);
            response = SD_SendCommand(SD_ACMD41, 0x40000000, 0x77);
            if (--timeout == 0)
                return 0xFD; // 错误码0xFD表示初始化超时
        } while (response != 0x00);

        // 检测高容量卡
        SD_SendCommand(SD_CMD58, 0, 0xFD);
        HAL_SPI_Receive(&hspi1, ocr, 4, 100);
        sd_type = (ocr[0] & 0x40) ? SD_TYPE_V2HC : SD_TYPE_V2;
    }
    else if (response == 0x05)
    { // SDC V1
        timeout = SD_INIT_TIMEOUT;
        do
        {
            response = SD_SendCommand(SD_CMD1, 0, 0xF9);
            if (--timeout == 0)
                return 0xFC; // 错误码0xFC表示V1卡初始化失败
        } while (response != 0x00);
        sd_type = SD_TYPE_V1;
    }
    else
    {
        return 0xFB; // 错误码0xFB表示未知卡类型
    }

    // 设置块长度（512字节）
    if (SD_SendCommand(SD_CMD16, 512, 0x15) != 0x00)
        return 0xFA; // 错误码0xFA表示块长度设置失败

    // 初始化成功后切换高速模式
    // MX_SPI1_Init(SPI_BAUDRATEPRESCALER_8);
    return 0;
}

uint8_t SD_ReadBlock(uint32_t block_addr, uint8_t *buffer)
{
    uint8_t token, tx_dummy = 0xFF;

    if (sd_type != SD_TYPE_V2HC)
        block_addr <<= 9;

    if (SD_SendCommand(SD_CMD17, block_addr, 0xFF) != 0x00)
        return 1;

    do
    {
        HAL_SPI_TransmitReceive(&hspi1, &tx_dummy, &token, 1, 100);
    } while (token == 0xFF);

    if (token != 0xFE)
        return 2;

    for (uint16_t i = 0; i < 512; i++)
        HAL_SPI_TransmitReceive(&hspi1, &tx_dummy, &buffer[i], 1, 100);

    HAL_SPI_Receive(&hspi1, &tx_dummy, 2, 100);
    return 0;
}

uint8_t SD_WriteBlock(uint32_t block_addr, uint8_t *buffer)
{
    uint8_t response, token, tx_dummy = 0xFF;
    uint8_t start_token = 0xFE;

    if (sd_type != SD_TYPE_V2HC)
        block_addr <<= 9;

    if (SD_SendCommand(SD_CMD24, block_addr, 0xFF) != 0x00)
        return 1;

    SD_CS_Enable();
    HAL_SPI_Transmit(&hspi1, &start_token, 1, 100);
    HAL_SPI_Transmit(&hspi1, buffer, 512, 500);
    HAL_SPI_Transmit(&hspi1, (uint8_t[]){0xFF, 0xFF}, 2, 100);

    HAL_SPI_Receive(&hspi1, &token, 1, 100);
    if ((token & 0x1F) != 0x05)
    {
        SD_CS_Disable();
        return 2;
    }

    uint32_t timeout = 100000;
    do
    {
        HAL_SPI_TransmitReceive(&hspi1, &tx_dummy, &token, 1, 10);
    } while (token == 0x00 && --timeout);

    SD_CS_Disable();
    return timeout ? 0 : 3;
}

uint32_t SD_GetCapacity(void)
{
    uint8_t csd[16], token, tx_dummy = 0xFF;

    if (SD_SendCommand(SD_CMD9, 0, 0xFF) != 0x00)
        return 0;

    uint32_t timeout = 100000;
    do
    {
        HAL_SPI_TransmitReceive(&hspi1, &tx_dummy, &token, 1, 10);
    } while (token == 0xFF && --timeout);
    if (token != 0xFE)
        return 0;

    HAL_SPI_Receive(&hspi1, csd, 16, 1000);
    HAL_SPI_Receive(&hspi1, &tx_dummy, 2, 100);

    if ((csd[0] >> 6) == 0x01)
    {
        uint32_t c_size = ((csd[7] & 0x3F) << 16) | (csd[8] << 8) | csd[9];
        return (c_size + 1) * 512 * 1024;
    }
    else
    {
        uint16_t c_size = ((csd[6] & 0x03) << 10) | (csd[7] << 2) | (csd[8] >> 6);
        uint8_t c_mult = ((csd[9] & 0x03) << 1) | (csd[10] >> 7);
        uint8_t bl_len = csd[5] & 0x0F;
        return (uint32_t)(c_size + 1) << (c_mult + bl_len + 2);
    }
}
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(hspi_flash_sd, &TxData, &Rxdata, 1, 1000);
    return Rxdata; // 返回收到的数据
}
