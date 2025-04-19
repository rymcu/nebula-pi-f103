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
#include "stdio.h"//printf������
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t rx_buff[100];  //���ջ���
uint8_t rx_done = 0; //������ɱ�־
uint8_t rx_cnt = 0;//�������ݳ���
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
#include "string.h"
#define WS2812_PIN    GPIO_PIN_13
#define WS2812_PORT   GPIOC

// WS2812时序参数（72MHz系统时钟，已校准）
#define T0H  24    // 0码高电平时间：24 NOP ≈ 0.333us (目标0.35us)
#define T1H  48    // 1码高电平时间：48 NOP ≈ 0.666us (目标0.7us)
#define T0L  50    // 0码低电平时间：50 NOP ≈ 0.694us (补偿GPIO操作延迟)
#define T1L  26    // 1码低电平时间：26 NOP ≈ 0.361us (补偿GPIO操作延迟)
#define RESET_DELAY 3600 // 复位时间：3600 NOP ≈ 50us

// 精确的NOP延时（禁用编译器优化）
__attribute__((optimize("O0"))) 
void WS2812_Delay(uint32_t nops) {
    while (nops--) {
        __ASM volatile ("nop"); // 强制插入NOP指令
    }
}

// 发送一个字节到WS2812（优化GPIO操作）
void WS2812_SendByte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        if (byte & (1 << (7 - i))) { // 发送1码
            // 高电平阶段
            HAL_GPIO_WritePin(WS2812_PORT, WS2812_PIN, GPIO_PIN_SET);
            WS2812_Delay(T1H);
            // 低电平阶段
            HAL_GPIO_WritePin(WS2812_PORT, WS2812_PIN, GPIO_PIN_RESET);
            WS2812_Delay(T1L);
        } else { // 发送0码
            // 高电平阶段
            HAL_GPIO_WritePin(WS2812_PORT, WS2812_PIN, GPIO_PIN_SET);
            WS2812_Delay(T0H);
            // 低电平阶段
            HAL_GPIO_WritePin(WS2812_PORT, WS2812_PIN, GPIO_PIN_RESET);
            WS2812_Delay(T0L);
        }
    }
}

// 发送RGB颜色（GRB格式）
void WS2812_SendColor(uint8_t r, uint8_t g, uint8_t b) {
    WS2812_SendByte(g); // 先发绿色分量
    WS2812_SendByte(r);
    WS2812_SendByte(b);
    WS2812_Delay(RESET_DELAY); // 复位信号
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
//����RGB���ݣ���ʾ��Ӧ��ɫ
void sendRGB(uint8_t r, uint8_t g, uint8_t b);
// ����һ���ֽ�
void sendByte(uint8_t byte);
// ���͵�������
void sendBit(uint8_t bitVal);
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
  /* USER CODE BEGIN 2 */
  //���ڷ������ݲ���
  unsigned char Sendbuf[] = "RYMCU nebula-pi usart1 test!\r\n";
  HAL_UART_Transmit(&huart1,Sendbuf, sizeof(Sendbuf),HAL_MAX_DELAY); // ���ڷ�������
  HAL_Delay(1000);
  //printf��ӡ���ݲ���
  printf("hello,enjoy!\r\n");
  HAL_Delay(1000);

    // 强制关闭编译器优化（确保时序稳定）
    __ASM volatile ("nop");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        // 红色 → 延时1秒
        WS2812_SendColor(255, 0, 0);
        HAL_Delay(1000);

        // 绿色 → 延时1秒
        WS2812_SendColor(0, 255, 0);
        HAL_Delay(1000);

        // 蓝色 → 延时1秒
        WS2812_SendColor(0, 0, 255);
        HAL_Delay(1000);
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
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : WS2812_Pin */
  GPIO_InitStruct.Pin = WS2812_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(WS2812_GPIO_Port, &GPIO_InitStruct);

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
//����RGB���ݣ���ʾ��Ӧ��ɫ
void sendRGB(uint8_t r, uint8_t g, uint8_t b)
{
    sendByte(g);
    sendByte(r);
    sendByte(b);
}
// ����һ���ֽ�
void sendByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        sendBit(byte & 0x80);
        byte <<= 1;
    }
}
// ���͵�������
void sendBit(uint8_t bitVal)
{
    if (bitVal)
    {
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);
        // ����WS2812ʱ��Ҫ�󣬸ߵ�ƽʱ��
        for (int i = 0; i < 10; i++)
        {
            __NOP();
        }
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_RESET);
        // �͵�ƽʱ��
        for (int j = 0; j < 5; j++)
        {
            __NOP();
        }
    }
    else
    {
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_SET);
        for (int i = 0; i < 5; i++)
        {
            __NOP();
        }
        HAL_GPIO_WritePin(WS2812_GPIO_Port, WS2812_Pin, GPIO_PIN_RESET);
        for (int j = 0; j < 10; j++)
        {
            __NOP();
        }
    }
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
