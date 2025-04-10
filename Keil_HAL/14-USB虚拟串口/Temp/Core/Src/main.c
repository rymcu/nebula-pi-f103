/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"//printf������
#include "string.h"
#include "bsp_spi_flash.h"//SPI flash����
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t rx_buff[100];  //���ջ���
uint8_t rx_done = 0; //������ɱ�־
uint8_t rx_cnt = 0;//�������ݳ���
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
unsigned char USB_CDC_buff[] = {"STM32 HAL USB virtual port com test\r\n"};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
SPI_HandleTypeDef *hspi_flash = &hspi1; // ���붨��
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void spi_flash_test(void);//SPI flash���Ժ���
void FATFS_FLASH_Test(void);
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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
	//HAL_Delay(1000);	
  //spi_flash_test();//SPI flash���Ժ���
	FATFS_FLASH_Test();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    CDC_Transmit_FS(USB_CDC_buff, sizeof(USB_CDC_buff));
    HAL_Delay(2000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  //���������жϣ������ж�
  __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE|UART_IT_RXNE);
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : USB_EN_Pin */
  GPIO_InitStruct.Pin = USB_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USB_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WS2812_Pin */
  GPIO_InitStruct.Pin = WS2812_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(WS2812_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_CS_Pin */
  GPIO_InitStruct.Pin = FLASH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : K1_Pin */
  GPIO_InitStruct.Pin = K1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(K1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : K2_EXIT_Pin */
  GPIO_InitStruct.Pin = K2_EXIT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(K2_EXIT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#endif /* __GNUC__ */
//�ض���printf����
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);//���ָ�򴮿�USART1
    return ch;
}
//��дGPIO�жϴ�����
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    //��תLED1
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    //��ӡ�жϴ�����Ϣ
    printf("K2 ExTi interrput!\r\n");
}
void spi_flash_test(void)
{
  /* ����Flash���� */
  uint16_t manu_id, dev_id;
  uint8_t jedec_id[3];
  uint8_t write_buf[256] = {0xAA, 0xBB, 0xCC, 0xDD};
  uint8_t read_buf[256];
  
  Flash_ReadID(&manu_id, &dev_id);
  printf("Flash Manufacturer ID: 0x%04X\r\n", manu_id);
  printf("Flash Device ID: 0x%04X\r\n", dev_id);
  Flash_ReadJedecID(jedec_id);
  printf("Flash JEDEC ID: 0x%02X, 0x%02X, 0x%02X\r\n", jedec_id[0], jedec_id[1], jedec_id[2]);
  
  if (Flash_SectorErase(0x0000) == HAL_OK) {
    Flash_PageProgram(0x0000, write_buf, 4);
    Flash_ReadData(0x0000, read_buf, 4);
  }
  printf("Flash Write Data: 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n", write_buf[0], write_buf[1], write_buf[2], write_buf[3]);
  printf("Flash Read  Data: 0x%02X, 0x%02X, 0x%02X, 0x%02X\r\n", read_buf[0], read_buf[1], read_buf[2], read_buf[3]);
  // ��֤����
  if (memcmp(write_buf, read_buf, 4) == 0) {
    printf("\r\nSPI Flash Test Passed!\r\n");
  }
}

// FATFS����
void FATFS_FLASH_Test(void)
{
  static FATFS fs;                                                              // �ļ�ϵͳ����
  static FIL fnew;                                                       // �ļ�����
  BYTE FATFS_Wr_Buff[128] = "hello, www.rymcu.com make it easy!have fun.\r\n"; // д������
  BYTE FATFS_Rd_Buff[128] = {0};                                         // ��������
  UINT fnum;                                                             // �ɹ���д����
  FRESULT res;                                                           // ����

  printf("\r\n\r\n------------------FLASH FATFS TEST------------------\r\n\r\n");
  res = f_mount(&fs, "0:", 1);
  if (res == FR_NO_FILESYSTEM)
  {
    printf("no file system,begin mkfs\r\n");
    res = f_mkfs("0:", 0, 0); //��ʽ��
    if (res == FR_OK)
    {
      printf("file system mkfs ok\r\n");
      // ��ʽ���ɹ�����ȡ���������¹��أ�
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
  else
  {
    printf("file system alreadly existed.\r\n");
  }
  printf("\r\n\r\n-------------------FATFS write test-------------------\r\n");
  // ���ļ����ļ��������򴴽�����
  res = f_open(&fnew, "RY.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res == FR_OK)
    printf("open or create RY.txt ok.\r\n");
  else
    printf("open or create file failed\r\n");
  // д����
  res = f_write(&fnew, FATFS_Wr_Buff, sizeof(FATFS_Wr_Buff), &fnum);
  if (res == FR_OK)
    printf("write to RY.txt:\r\n%s", FATFS_Wr_Buff);
  else
    printf("failed to write RY.txt,code: %d!\r\n", res);
  // ���д�����󣬹ر��ļ�
  f_close(&fnew);
  printf("\r\n-------------------FATFS read test-------------------\r\n\r\n");
  // ���ļ�������ʽ���Ѵ������ļ�
  res = f_open(&fnew, "RY.txt", FA_OPEN_EXISTING | FA_READ);
  if (res != FR_OK)
  {
    printf("open RY.txt failed\r\n");
    return;
  }
  // ��ȡ�ļ�����
  res = f_read(&fnew, FATFS_Rd_Buff, sizeof(FATFS_Rd_Buff), &fnum);
  if (res != FR_OK)
  {
    printf("read file failed\r\n");
    return;
  }
  printf("read file data:\r\n%s\r\n", FATFS_Rd_Buff);
  f_close(&fnew); // ��ȡ��ϣ��ر��ļ�
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

#ifdef  USE_FULL_ASSERT
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
