/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Dwin.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#if (USING_DEBUG)
uint32_t g_Value = 0;
#endif
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
static void MPU_Config(void);
void MX_FREERTOS_Init(void);
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

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
  /*Enable update interrupt*/
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
  /*open timer6*/
  HAL_TIM_Base_Start_IT(&htim6);
  /*Initialize linked list*/
  for (uint8_t i = 0; i < LIST_SIZE; i++)
  {
    Init_List(&List_Map[i], i);
  }
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // Wave_Handle();
    // HAL_Delay(500);
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

  /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief  Get the time consumed by each node
 * @param  list Pointer to the first node of the current linked list
 * @retval None
 */
// static void Get_NodeTimes(Dwin_List *list)
// {
//   /*Counter overflow, time greater than 6.5536ms*/
//   if (list->dcb_data[list->current_node].overflows_num > OVERFLOW_COUNTS(TIMES, FREQ))
//   {
//     list->dcb_data[list->current_node].consum_times = (float)((CVALUE - list->dcb_data[list->current_node].first_value) +
//                                                               (list->dcb_data[list->current_node].overflows_num - (OVERFLOW_COUNTS(TIMES, FREQ) + 1U)) * CVALUE +
//                                                               list->dcb_data[list->current_node].buf[list->dcb_data[list->current_node].data_len]) /
//                                                       ACCU();
//   }
//   else /*The counter does not overflow and the time is within 6.5536ms*/
//   {
//     /*Calculate the settling time,last value - first value*/
//     list->dcb_data[list->current_node].consum_times =
//         (float)(list->dcb_data[list->current_node].buf[list->dcb_data[list->current_node].data_len] -
//                 list->dcb_data[list->current_node].first_value) /
//         ACCU();
//   }
// }

#define GET_NODETIMES(list)                                                                                                                                         \
  {                                                                                                                                                                 \
    if (list->dcb_data[list->current_node].overflows_num > OVERFLOW_COUNTS(TIMES, FREQ))                                                                            \
    {                                                                                                                                                               \
      list->dcb_data[list->current_node].consum_times = (float)((CVALUE - list->dcb_data[list->current_node].first_value) +                                         \
                                                                (list->dcb_data[list->current_node].overflows_num - (OVERFLOW_COUNTS(TIMES, FREQ) + 1U)) * CVALUE + \
                                                                list->dcb_data[list->current_node].buf[list->dcb_data[list->current_node].data_len]) /              \
                                                        ACCU();                                                                                                     \
    }                                                                                                                                                               \
    else                                                                                                                                                            \
    {                                                                                                                                                               \
      list->dcb_data[list->current_node].consum_times =                                                                                                             \
          (float)(list->dcb_data[list->current_node].buf[list->dcb_data[list->current_node].data_len] -                                                             \
                  list->dcb_data[list->current_node].first_value) /                                                                                                 \
          ACCU();                                                                                                                                                   \
    }                                                                                                                                                               \
  }

/**
 * @brief  Overflow processing
 * @param  list Pointer to the first node of the current linked list
 * @retval None
 */
static __inline void Overflow_Handle(Dwin_List *list)
{
  if (list->dcb_data[list->current_node].timer_synflag)
  {
    /*counter Number of overflows*/
    list->dcb_data[list->current_node].overflows_num++;
  }
}

#define OVERFLOW_HANDLE(list)                           \
  {                                                     \
    if (list.dcb_data[list.current_node].timer_synflag) \
    {                                                   \
      list.dcb_data[list.current_node].overflows_num++; \
    }                                                   \
  }

/**
 * @brief  timeout handler
 * @param  list Pointer to the first node of the current linked list
 * @retval None
 */
static __inline void Timeout_Handle(Dwin_List *list)
{
  /*Avoid the error caused by the first overtime = 0*/
  if ((list->dcb_data[list->current_node].overtimes) && (list->dcb_data[list->current_node].timer_synflag))
  {
    if (!(--list->dcb_data[list->current_node].overtimes))
    {
      /*Data collection completed*/
      list->dcb_data[list->current_node].data_flag = true;
      /*Reset first detection flag*/
      list->dcb_data[list->current_node].first_flag = false;
      /*Reset timer synchronization flag*/
      list->dcb_data[list->current_node].timer_synflag = false;
      /*Calculate the stabilization time on / off*/
      // Get_NodeTimes(list);
      GET_NODETIMES(list);
#if (USING_DEBUG)
      g_Value = list->dcb_data[list->current_node].overflows_num;
#endif
      /*Clear counter overflow times*/
      list->dcb_data[list->current_node].overflows_num = 0;
      /*Clear first recorded value*/
      list->dcb_data[list->current_node].first_value = 0;
      /*Record currently captured nodes*/
      list->complete_node = list->current_node;
      /*Point to the next node*/
      list->current_node++;
      list->current_node %= LISTNODE_SIZE;
    }
  }
}
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x20000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  /*Timer update interrupt (overflow) interrupt processing callback function,
  in HAL_ TIM_ IRQHandler will be called*/
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  /*Overflow interrupt*/
  else if (htim->Instance == TIM3)
  {
    for (uint8_t i = 0; i < LIST_SIZE - 2U; i++)
    {
      Overflow_Handle(&List_Map[i]);
      // OVERFLOW_HANDLE(List_Map[i]);
    }
  }
  else if (htim->Instance == TIM4)
  {
    for (uint8_t i = 4; i < LIST_SIZE; i++)
    {
      Overflow_Handle(&List_Map[i]);
      // OVERFLOW_HANDLE(List_Map[i]);
    }
  }
  else if (htim->Instance == TIM6)
  {
    for (uint8_t i = 0; i < LIST_SIZE; i++)
    {
      Timeout_Handle(&List_Map[i]);
    }
  }
  /* USER CODE END Callback 1 */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
