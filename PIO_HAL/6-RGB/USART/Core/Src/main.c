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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "stm32f1xx_hal.h"

// 定义WS2812控制引脚
#define WS2812_PIN GPIO_PIN_13
#define WS2812_GPIO_PORT GPIOC

void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = WS2812_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(WS2812_GPIO_PORT, &GPIO_InitStruct);
}
// 发送单个比特
void sendBit(uint8_t bitVal)
{
    if (bitVal)
    {
        HAL_GPIO_WritePin(WS2812_GPIO_PORT, WS2812_PIN, GPIO_PIN_SET);
        // 根据WS2812时序要求，高电平时间
        for (int i = 0; i < 10; i++)
        {
            __NOP();
        }
        HAL_GPIO_WritePin(WS2812_GPIO_PORT, WS2812_PIN, GPIO_PIN_RESET);
        // 低电平时间
        for (int j = 0; j < 5; j++)
        {
            __NOP();
        }
    }
    else
    {
        HAL_GPIO_WritePin(WS2812_GPIO_PORT, WS2812_PIN, GPIO_PIN_SET);
        for (int i = 0; i < 5; i++)
        {
            __NOP();
        }
        HAL_GPIO_WritePin(WS2812_GPIO_PORT, WS2812_PIN, GPIO_PIN_RESET);
        for (int j = 0; j < 10; j++)
        {
            __NOP();
        }
    }
}

// 发送一个字节
void sendByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        sendBit(byte & 0x80);
        byte <<= 1;
    }
}
void sendRGB(uint8_t r, uint8_t g, uint8_t b)
{
    sendByte(g);
    sendByte(r);
    sendByte(b);
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t rx_buff[100]; // 接收缓存
uint8_t rx_done = 0;  // 接收完成标志
uint8_t rx_cnt = 0;   // 接收数据长度
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
    HAL_Init();
    GPIO_Init();
    // 发送红色数据（示例）
    sendRGB(255,0,0);
  /* USER CODE END 2 */
  HAL_Delay(500);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  #define LED_NUMBER 1
  while (1)
  {
           for (int i = 0; i < LED_NUMBER; i++)
        {
            sendRGB(0, 25, 0); // 设置绿色
        }
        HAL_Delay(1000);

        for (int i = 0; i < LED_NUMBER; i++)
        {
            sendRGB(0, 0, 25); // 设置蓝色
        }
         HAL_Delay(1000);

        for (int i = 0; i < LED_NUMBER; i++)
        {
            sendRGB(25, 0, 0); // 设置蓝色
        }
         HAL_Delay(1000);
    



    // printf("rymcu.com!\r\n");
    // HAL_Delay(1);
    if (rx_done == 1) // 判读是否接收完成
    {
      rx_done = 0; // 清除接收标志
      // 数据处理，打印接收长度以及接收的数据
      printf("length of rx data: %d!\r\n", rx_cnt);
      for (int i = 0; i < rx_cnt; i++)
        printf("%c", rx_buff[i]);
      printf("\r\n");

      rx_cnt = 0; // 清除接收长度
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); // 切换亮、灭状态，添加此语句防止优化
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
