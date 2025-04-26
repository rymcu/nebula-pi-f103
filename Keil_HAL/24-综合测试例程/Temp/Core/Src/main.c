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
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <string.h>
#include "bsp_lcd_driver.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "bsp_ft6336_lcd.h"
#include "bsp_oled.h"
#include "rtthread.h"  // 先包�? RT-Thread，确�? rt_malloc 等函数声明有�?
#include "lvgl.h"  // 包含 LVGL 头文件，此时宏已生效
#include "my_lvgl_app.h"
#include "bsp_rgb.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t rx_buff[100];  
uint8_t rx_done = 0; 
uint8_t rx_cnt = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//cubemxʱRT-Thread board.c SysTick_Handlerֶ lv_tick_inc(1);RT_HEAP_SIZE (54*1024)//ֶ
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void my_print(lv_log_level_t level, const char * buf);
void my_lvgl_app(void)
{
	//lv_ex_label_1();//lv_example_get_started_2();//lv_line_test_start();//my_lvgl_nebula();//my_lvgl_pages();//my_lvgl_rgb();//ry_test();//ry_test
	 lv_example_anim_1();
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
u8g2_t u8g2;
uint8_t i;
unsigned char str_temp[]="0";
unsigned char str_temp_b[]="0";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#define LVGL_THREAD_STACK_SIZE 3072
#define LVGL_THREAD_PRIORITY 20
// OLED 硬件参数（根据实际电路修改）
#define OLED_THREAD_STACK_SIZE  1024   // 线程栈
#define OLED_THREAD_PRIORITY    20      // 线程优先级
#define OLED_THREAD_TIMESLICE   5       // 时间片

// uart
#define UART_THREAD_STACK_SIZE  1024   // 线程栈
#define UART_THREAD_PRIORITY    20      // 线程优先级
#define UART_THREAD_TIMESLICE   5       // 时间片

static void lvgl_thread_entry(void *parameter) {
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	my_lvgl_app();//lv_example_anim_1();//lv_log_register_print_cb(my_print);
  
	rt_kprintf("lvgl_thread_entry!\n");
    while (1) {
        lv_task_handler();              // 处理 LVGL 任务
        rt_thread_delay(10);            // 延时 10ms
    }
}
static void oled_thread_entry(void *parameter) {
    // 初始? OLED
  OLED_Init();
	rt_kprintf("oled_thread_entry!\n");	
   while (1) 
		{
			i++;
			if(i>9)i=0;
			
			str_temp[0] = '0' + i; 
			u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
			u8g2_SetDrawColor(&u8g2, 0); // 设置为背景色，用于擦除旧字符
			u8g2_DrawStr(&u8g2, 100,50,(const char*)str_temp_b);
			u8g2_SetDrawColor(&u8g2, 1);  // 设置为前景色，用于绘制新字符
			u8g2_DrawStr(&u8g2, 100,50,(const char*)str_temp);
			u8g2_SendBuffer(&u8g2);
			str_temp_b[0] = str_temp[0];
			HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);//rt_kprintf("rymcu oled test!\n");	
			rt_thread_delay(1000);
		}
}
static void uart_thread_entry(void *parameter) {
	rt_kprintf("uart_thread_entry!\n");	
   while (1) 
		{
			if(1 == rx_done)
			{
				rx_done = 0;
				my_lvgl_set_receive_box();
				memset(rx_buff, 0, sizeof(rx_buff));
        rx_cnt =0;//清除接收长度
			}
			rt_thread_yield();//rt_thread_delay(1000);
		}
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
  MX_USART1_UART_Init();
  MX_FSMC_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	rt_thread_t oled_thread  = rt_thread_create("oled_thread",oled_thread_entry,RT_NULL, OLED_THREAD_STACK_SIZE,OLED_THREAD_PRIORITY,20);
  if (oled_thread != RT_NULL) {if(RT_EOK == rt_thread_startup(oled_thread)){rt_kprintf("oled_thread create ok!\n");}}	

	rt_thread_t uart_thread  = rt_thread_create("uart_thread",uart_thread_entry,RT_NULL, UART_THREAD_STACK_SIZE,UART_THREAD_PRIORITY,20);
  if (uart_thread != RT_NULL) {if(RT_EOK == rt_thread_startup(uart_thread)){rt_kprintf("uart_thread create ok!\n");}}	
	
	rt_thread_t lvgl_thread = rt_thread_create("lvgl",lvgl_thread_entry,RT_NULL, LVGL_THREAD_STACK_SIZE,LVGL_THREAD_PRIORITY,20);
  if (lvgl_thread != RT_NULL) {if(RT_EOK == rt_thread_startup(lvgl_thread)){rt_kprintf("lvgl_thread create ok!\n");}}else{rt_kprintf("Thread creation failed! Error code: %d\n", rt_get_errno());}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			//HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);//rt_kprintf("rymcu rt-thread test!\n");
      //rt_thread_delay(1000);
		rt_kprintf("end of main thread!\n");
		return 0;
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
}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);
    return ch;
}
void my_print(lv_log_level_t level, const char * buf)
{
	//printf("%s",buf);
	uint16_t length;
	length  = lv_strlen(buf);
	HAL_UART_Transmit(&huart1,(uint8_t *)buf,length,0xFFFF);//ָ򴮿USART1
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
