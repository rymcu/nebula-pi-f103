/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "bsp_spi_flash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void FATFS_FLASH_Test(void);
#define FLASHBUFFSIZE 64
void SPI_FLASH_Test(void)
{
  uint16_t FLASH_ID;
  uint8_t Flash_Wr_Buff[FLASHBUFFSIZE], Flash_Rd_Buff[FLASHBUFFSIZE];
  printf("SPI总线通信实验，读�??8M外部FLASH W25Q64!\r\n", FLASH_ID);
  FLASH_ID = W25QXX_ReadID(); // 读取W25Q64器件ID
  HAL_Delay(100);
  printf("the FLASH Device ID is 0x%X!\r\n", FLASH_ID);
  // W25Q64 = 8MByte,地址范围�??0x000000-0x7FFFFF
  // Sector：每4KB为一个Sector,8M/4K=2048�??
  // Block：每64KB为一个Block,8M/64K=128个，每个Block包括16个Sector
  SPI_FLASH_SectorErase(0x00); // 擦除第一个Sector
  for (int i = 0; i < FLASHBUFFSIZE; ++i)
    Flash_Wr_Buff[i] = i;
  SPI_FLASH_BufferWrite(Flash_Wr_Buff, 0x00, FLASHBUFFSIZE);
  SPI_FLASH_BufferRead(Flash_Rd_Buff, 0x00, FLASHBUFFSIZE);
  printf("Read data form W25Q64: \r\n");
  for (uint16_t i = 0; i < FLASHBUFFSIZE; i++)
  {
    printf("0x%02x ", Flash_Rd_Buff[i]);
  }
  printf("\r\n");
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  SPI_FLASH_Test();
  FATFS_FLASH_Test();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// FATFS测试
void FATFS_FLASH_Test(void)
{
  FATFS fs;                                                              // 文件系统对象
  static FIL fnew;                                                       // 文件对象
  BYTE FATFS_Wr_Buff[128] = "hello, www.rymcu.com make it easy!have fun.\r\n"; // 写缓冲区
  BYTE FATFS_Rd_Buff[128] = {0};                                         // 读缓冲区
  UINT fnum;                                                             // 成功读写数量
  FRESULT res;                                                           // 返回�?

  printf("\r\n\r\n------------------FLASH FATFS TEST------------------\r\n\r\n");
  res = f_mount(&fs, "0:", 1);
  if (res == FR_NO_FILESYSTEM)
  {
    printf("no files ystem,begin mkfs\r\n");
    res = f_mkfs("0:", 0, 0); // 格式�?
    if (res == FR_OK)
    {
      printf("file system mkfs ok\r\n");
      // 格式化成功后先取消，再重新挂载！
      res = f_mount(NULL, "0:", 1);
      printf("cancel mount ok:%d\r\n", res);
      res = f_mount(&fs, "0:", 1);
      printf("re-mount ok:%d\r\n", res);
    }
    else
    {
      printf("failed mount\r\n");
    }
  }
  printf("\r\n\r\n-------------------FATFS write test-------------------\r\n");
  // 打开文件，文件不存在则创建并打开
  res = f_open(&fnew, "RY.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res == FR_OK)
    printf("open or create RY.txt ok.\r\n");
  else
    printf("open or create file failed\r\n");
  // 写测�?
  res = f_write(&fnew, FATFS_Wr_Buff, sizeof(FATFS_Wr_Buff), &fnum);
  if (res == FR_OK)
    printf("write to RY.txt:\r\n%s", FATFS_Wr_Buff);
  else
    printf("failed to write RY.txt,code: %d!\r\n", res);
  // 完成写操作后，关闭文�?
  f_close(&fnew);
  printf("\r\n-------------------FATFS read test-------------------\r\n\r\n");
  // 打开文件，读方式打开已创建的文件
  res = f_open(&fnew, "RY.txt", FA_OPEN_EXISTING | FA_READ);
  if (res != FR_OK)
  {
    printf("open RY.txt failed\r\n");
    return;
  }
  // 读测�?
  res = f_read(&fnew, FATFS_Rd_Buff, sizeof(FATFS_Rd_Buff), &fnum);
  if (res != FR_OK)
  {
    printf("read file failed\r\n");
    return;
  }
  printf("read file data:\r\n%s\r\n", FATFS_Rd_Buff);
  f_close(&fnew); // 读取完毕，关闭文件�?
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch.FILE *f)
#endif /* __GNUC__ */

// 重定向printf函数
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF); // 输出指向串口USART1
  return ch;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
