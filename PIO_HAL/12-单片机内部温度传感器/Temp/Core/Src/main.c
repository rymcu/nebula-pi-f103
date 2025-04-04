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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"//printf函数
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t rx_buff[100];  //接收缓存
uint8_t rx_done = 0; //接收完成标志
uint8_t rx_cnt = 0;//接收数据长度
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
void FLASH_Inside_Wr(uint32_t addr, uint32_t Pdata);
uint32_t FLASH_Inside_Rd(uint32_t addr);
void FLASH_Inside_Test(void);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void Get_MCU_Info(void);
void Get_Temp(void);
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
 // HAL_Delay(1000);
 // Get_MCU_Info();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      //采集温度
      Get_Temp();
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  //�?启接收中断，空闲中断
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : K1_Pin K2_Pin */
  GPIO_InitStruct.Pin = K1_Pin|K2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#endif /* __GNUC__ */
//重定向printf函数
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);//输出指向串口USART1
    return ch;
}
/**
 * @brief �?内部FLASH写入数据
 * @param addr 写入地址
 * @param pdata 存储待写数据
 * @retval None
 */
void FLASH_Inside_Wr(uint32_t addr, uint32_t Pdata)
{
  // 定义�?部变�?
  uint32_t PageError = 0;
  HAL_StatusTypeDef HAL_Status;
  // 擦除配置信息结构体，包括擦除地址、方式�?�页数等
  FLASH_EraseInitTypeDef pEraseInit;
  pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES; // 按页擦除
  pEraseInit.PageAddress = addr;                // 擦除地址
  pEraseInit.NbPages = 1;                       // 擦除页数�?
  // step1 解锁内部FLASH，允许读写功�?
  HAL_FLASH_Unlock();
  // step2 �?始擦除addr对应�?
  HAL_Status = HAL_FLASHEx_Erase(&pEraseInit, &PageError); // 擦除
  if (HAL_Status != HAL_OK)
    printf("内部FlASH擦除失败!\r\n");
  // step3 写入数据
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, Pdata);
  // step4 锁定FLASH
  HAL_FLASH_Lock();
}
/**
 * @brief 读取内部FLASH数据
 * @param addr 读取地址
 * @retval 读出的数�?
 */
uint32_t FLASH_Inside_Rd(uint32_t addr)
{
  // 定义�?部变�?
  uint32_t RdData = 0;
  // step1 解锁内部FLASH，允许读写功�?
  HAL_FLASH_Unlock();
  RdData = *(__IO uint32_t *)addr;
  // step2 锁定FLASH
  HAL_FLASH_Lock();

  return RdData;
}
/**
 * @brief 内部FLASH读写测试
 */
void FLASH_Inside_Test(void)
{
  printf("\r\n\r\n------------------on chip FLASH write/read test------------------\r\n\r\n");
  uint32_t addr = 0x08010000;   // 确保该地�?内部FLASH是空余的�?
  uint32_t WrData = 0x01234567; // 待写入数�?
  uint32_t RdData = 0;          // 存储读取数据

  printf("addr:0x%x write data:0x%x\r\n", addr, WrData);
  FLASH_Inside_Wr(addr, WrData);  // 写入数据
  RdData = FLASH_Inside_Rd(addr); // 读取数据
  printf("addr:0x%x  read data:0x%x\r\n", addr, RdData);
}
/**
 * @brief 获取MCU设备信息
 */
void Get_MCU_Info(void)
{
    printf("\r\n\r\n------------------get mcu information------------------\r\n\r\n");
    uint32_t  FLASH_Size_Addr = 0x1FFFF7E0;//FLASH大小存储地址
    //获取存储器大�?
    printf("FLASH  Size: %dKB\r\n",*(uint16_t *)FLASH_Size_Addr);
    //获取HAL版本
    uint32_t HALVer = HAL_GetHalVersion();
    printf("HAL Version: V%d.%d.%d\r\n",HALVer>>24,(HALVer>>16)&0xFF,(HALVer>>8)&0xFF);
   //获取保留ID�?
    printf("Revision ID: 0x%X\r\n",HAL_GetREVID());
    //获取全球唯一UID
    printf("Unique ID(UID): 0x%08X %08X %08X\r\n",HAL_GetUIDw0(),HAL_GetUIDw1(),HAL_GetUIDw2());

    //获取设备ID
    printf("Device ID or Chip ID or Product ID(PID): 0x%x\r\n",HAL_GetDEVID());

}
/**
  * @brief 读取内部温度传感器
  */
 void Get_Temp(void)
 {
     uint32_t Temp;//温度采样分层值
     float Vsense = 0.0;//温度采样电压值
     float Temperature = 0.0;//温度值
     //数据手册温度转换公式：T = ((V25-Vsense)/Avg_Slope) + 25
     float V25 = 1.43;//查阅手册获得
     float Avg_Slope = 0.0043;//4.3mV/摄氏度
     printf("\r\n\r\n------------------MCU inside Temperature sensor------------------\r\n\r\n");
     //step1 启动ADC
     HAL_ADC_Start(&hadc1);
     //step2 温度采集转换
     HAL_ADC_PollForConversion(&hadc1,5);
     //step3 转换计算
     Temp = HAL_ADC_GetValue(&hadc1);//获取采样值分层值
     Vsense = Temp *(3.3/4096);//采样精度12bit,最大分层值4096
     Temperature = ((V25-Vsense)/Avg_Slope) + 25;//按公式计算温度值
     //step4 串口打印
     printf("Temp:%d\r\nVsense:%0.3f\r\nTemperature:%0.3f\r\n", (int)Temp, Vsense, Temperature);
     HAL_Delay(1000);
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
