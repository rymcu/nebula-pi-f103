/* bsp_spi_sd.c */
#include "bsp_spi_sd.h"

/* 外部SPI句柄（在用户代码中初始化） */
extern SPI_HandleTypeDef *hspi_flash_sd;
static SD_Type sd_type;
uint8_t buff[4];
static void SD_CS_Enable(void) { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); }
static void SD_CS_Disable(void) { HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); }

static uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t tx_buf[6] = {0x40 | cmd,
                         (uint8_t)(arg >> 24),
                         (uint8_t)(arg >> 16),
                         (uint8_t)(arg >> 8),
                         (uint8_t)arg,
                         crc};
    SD_CS_Enable(); // 再使能片选
    for (uint8_t i = 0; i < 6; i++)
        SPI_ReadWriteByte(tx_buf[i]);
    return 0;
}

#define SD_INIT_TIMEOUT 200 // 全局超时阈值
uint8_t get_response(void)
{
    uint8_t response, timeout = SD_INIT_TIMEOUT;
    do
    {
        response = SPI_ReadWriteByte(0xFF);
        if (--timeout == 0)
            return 0xFF; // 超时
    } while (response == 0xFF);
    return response;
}
uint8_t SD_Init(void)
{
    uint8_t response, ocr[4];
    uint32_t timeout;

    // 发送至少74个时钟脉冲（发送80个空字节）
    SD_CS_Disable();
    for (uint8_t i = 0; i < 10; i++)
        SPI_ReadWriteByte(0xFF);
    // 发送CMD0
    SD_SendCommand(SD_CMD0, 0, 0x95);
    response = get_response();
    if (response == 0x01)
    {
        printf("SD_SendCommand SD_CMD0 OK!\r\n");
    }
    else
    {
        printf("SD_SendCommand SD_CMD0 timeout!\r\n");
    }
    // 发送CMD8检测卡类型
    SD_SendCommand(SD_CMD8, 0x01AA, 0x01);
    response = get_response();
    if(response == 0xFF)
    {
        printf("SD_SendCommand SD_CMD8 timeout!\r\n");
        return 0xFF;
    }
    if (response != 0x01)//V1
    {
        printf("SD type: V1 0x%X!\r\n",response);
        sd_type = SD_TYPE_V1;
        for (uint8_t i = 0; i < 4; i++) ocr[i] = SPI_ReadWriteByte(0XFF);
        printf("ocr:%X,%X,%X,%X",ocr[0],ocr[1],ocr[2],ocr[3]);

        SD_SendCommand(SD_CMD1, 0, 0x01);
        response = get_response();
        printf("SD_SendCommand SD_CMD1:0x%X\r\n",response);

    }
    else
    {
        printf("SD type: V2!\r\n");
        sd_type = SD_TYPE_V2;
        
    }
    if (response == 0x01)
    { // SDC V2+
        for (uint8_t i = 0; i < 4; i++)
            ocr[i] = SPI_ReadWriteByte(0XFF);
        if (ocr[2] == 0x01 && ocr[3] == 0xAA)
        {
            ;
        }
        else
        {
            SD_SendCommand(SD_CMD55, 0, 0x01);
            response = SD_SendCommand(SD_ACMD41, 0, 0x01);
            if (response <= 0x01)
            {
                return 0xEE;
            }
            else
            {
                // return 0xED;
                timeout = SD_INIT_TIMEOUT;
                do
                {
                    response = SD_SendCommand(SD_CMD1, 0, 0xF9);
                    if (--timeout == 0)
                        return 0xFC; // 错误码0xFC表示V1卡初始化失败
                } while (response != 0x00);
                // 设置块长度（512字节）
                if (SD_SendCommand(SD_CMD16, 512, 0x15) != 0x00)
                    return 0xFA; // 错误码0xFA表示块长度设置失败
            }
        }
    }
    SD_CS_Disable();
    SPI_ReadWriteByte(0xFF);
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
    uint32_t Capacity;
    uint8_t n;
    uint16_t csize;
    if (SD_SendCommand(SD_CMD9, 0, 0x01) != 0x00)
        return 1;

    uint32_t timeout = 100000;
    do
    {
        token = SPI_ReadWriteByte(0XFF);
    } while (token == 0xFF && --timeout);
    if (token == 0xFE)
        return 2;
    for (uint8_t i = 0; i < 16; i++)
    {
        csd[i] = SPI_ReadWriteByte(0XFF);
    }
    // 下面是2个伪CRC（dummy CRC）
    SPI_ReadWriteByte(0xFF);
    SPI_ReadWriteByte(0xFF);

    if ((csd[0] & 0xC0) == 0x40) // V2.00的卡
    {
        csize = csd[9] + ((uint16_t)csd[8] << 8) + 1;
        Capacity = (uint32_t)csize << 10; // 得到扇区数
    }
    else // V1.XX的卡
    {
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
        Capacity = (uint32_t)csize << (n - 9); // 得到扇区数
    }
    return Capacity;
}
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(hspi_flash_sd, &TxData, &Rxdata, 1, 1000);
    return Rxdata; // 返回收到的数据
}
