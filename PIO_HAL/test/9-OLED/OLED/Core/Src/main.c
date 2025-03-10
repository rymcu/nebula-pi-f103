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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "bmp.h"
#include "math.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t fall_pot[128]; // 记录下落点的坐标
void OLED_Snow_Test(void);
void SnowLike(void);
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
// 0.96 OLED测试
void OLED_IIC_Test(void)
{
  uint16_t ms = 1000;
  OLED_Init();       // 初始化OLED
  OLED_Clear();      // 清除屏幕
  OLED_Display_On(); // �?启OLED

  /*****************************************
   *
   *0.96 OLED 字符显示测试
   *
   *******************************************/
  OLED_ShowChar(0, 0, 'A', 16, 0);
  OLED_ShowChar(8, 0, 'B', 16, 0);
  OLED_ShowChar(16, 0, 'C', 16, 0);
  OLED_ShowChar(24, 0, 'D', 16, 0);

  OLED_ShowChar(0, 2, 'A', 8, 0);
  OLED_ShowChar(8, 2, 'B', 8, 0);
  OLED_ShowChar(16, 2, 'C', 8, 0);
  OLED_ShowChar(24, 2, 'D', 8, 0);

  OLED_ShowString(25, 6, "Char Test!", 16, 1);

  HAL_Delay(ms);
  OLED_Clear(); // 清除屏幕

  /*****************************************
   *
   *0.96 OLED 数字显示测试
   *
   *******************************************/

  OLED_ShowNum(0, 1, 12, 2, 16, 0);
  OLED_ShowNum(48, 1, 34, 2, 16, 0);
  OLED_ShowNum(96, 1, 56, 2, 16, 0);

  OLED_ShowString(25, 6, "Num Test!", 16, 1);

  HAL_Delay(ms);
  OLED_Clear(); // 清除屏幕

  /*****************************************
   *
   *0.96 OLED 中文显示测试
   *
   *******************************************/
  OLED_ShowCHinese(22, 3, 1, 0);      // �?
  OLED_ShowCHinese(22 + 16, 3, 2, 0); // �?
  OLED_ShowCHinese(22 + 32, 3, 3, 0); // �?
  OLED_ShowCHinese(22 + 48, 3, 4, 0); // �?
  OLED_ShowCHinese(22 + 64, 3, 5, 0); // �?

  OLED_ShowString(25, 6, "CHN Test!", 16, 1);

  HAL_Delay(ms);
  OLED_Clear(); // 清除屏幕

  /*****************************************
   *
   *0.96 OLED 字符串显示测�?
   *
   *******************************************/

  OLED_ShowString(0, 2, "Nebula-Pi,RYMCU!", 16, 0);
  OLED_ShowString(25, 6, "Str Test!", 16, 1);
  HAL_Delay(ms);
  OLED_Clear(); // 清除屏幕
  /*****************************************
   *
   *0.96 OLED 图片显示测试
   *
   *******************************************/

  OLED_DrawBMP(0, 0, Logo, 0); // 显示图片
  OLED_ShowString(25, 6, "PIC Test!", 16, 1);
  HAL_Delay(2000);
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    OLED_IIC_Test();
    OLED_Snow_Test();
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
// 0.96 OLED测试
void OLED_Snow_Test(void)
{
  OLED_Init();       // 初始化OLED
  OLED_Clear();      // 清除屏幕
  OLED_Display_On(); // 开启OLED
  static u8 x, y;

  // 设置128列下落点的初始值，随机产生0-63之间的数值
  for (int i = 0; i < 128; i++)
    fall_pot[i] = rand() % 64;

  while (1)
  {
    HAL_Delay(50);
    SnowLike();
  }
}

/*柱状显示*/
#define u_char unsigned char
void SnowLike(void)
{
  OLED_ClearGram();               // 清除屏幕
  for (u_char i = 0; i < 32; i++) // 每4列选一点
  {
    // OLED_Fill(x,0,x+2,y+2,1);
    // 画下落的点，每次下落2个像素，每4列选一个，保证雪花不至于太密
    OLED_Fill(i * 4, fall_pot[i], i * 4 + 1, fall_pot[i] + 1, 1);
    if (fall_pot[i] >= 2)
      fall_pot[i] -= 2;
    else
      fall_pot[i] = 63;
  }
  OLED_Refresh_Gram(); // 重新填充屏幕
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
