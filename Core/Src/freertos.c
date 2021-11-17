/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "Dwin.h"
#include "iwdg.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_TASK_BIT (0x001 << 0U)
#define RUN_TASK_BIT (0x001 << 1U)
#define WAVE_TASK_BIT (0x001 << 2U)
#define DWIN_TASK_BIT (0x001 << 3U)
#define EVENT_ALL_BIT (UART_TASK_BIT | RUN_TASK_BIT | WAVE_TASK_BIT | DWIN_TASK_BIT)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
GPIO_PinState led_state = GPIO_PIN_RESET;
EventGroupHandle_t Event_Handle = NULL;
/* USER CODE END Variables */
osThreadId Uart_TaskHandle;
osThreadId Run_TaskHandle;
osThreadId Data_HandleTaskHandle;
osThreadId Dwin_TaskHandle;
osThreadId IWDG_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Report_Task(void const * argument);
void Led_Task(void const * argument);
void Wave_HandleTask(void const * argument);
void Dwin_HandleTask(void const * argument);
void Idog_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* Create event flag group */
  Event_Handle = xEventGroupCreate();
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Uart_Task */
  osThreadDef(Uart_Task, Report_Task, osPriorityNormal, 0, 256);
  Uart_TaskHandle = osThreadCreate(osThread(Uart_Task), NULL);

  /* definition and creation of Run_Task */
  osThreadDef(Run_Task, Led_Task, osPriorityLow, 0, 128);
  Run_TaskHandle = osThreadCreate(osThread(Run_Task), NULL);

  /* definition and creation of Data_HandleTask */
  osThreadDef(Data_HandleTask, Wave_HandleTask, osPriorityAboveNormal, 0, 1024);
  Data_HandleTaskHandle = osThreadCreate(osThread(Data_HandleTask), NULL);

  /* definition and creation of Dwin_Task */
  osThreadDef(Dwin_Task, Dwin_HandleTask, osPriorityHigh, 0, 512);
  Dwin_TaskHandle = osThreadCreate(osThread(Dwin_Task), NULL);

  /* definition and creation of IWDG_Task */
  osThreadDef(IWDG_Task, Idog_Task, osPriorityRealtime, 0, 128);
  IWDG_TaskHandle = osThreadCreate(osThread(IWDG_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Report_Task */
/**
 * @brief  Function implementing the Uart_Task thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_Report_Task */
void Report_Task(void const * argument)
{
  /* USER CODE BEGIN Report_Task */
  /* Infinite loop */
  for (;;)
  {
    /*Set event flag bit*/
    xEventGroupSetBits(Event_Handle, UART_TASK_BIT);
    /*Wait for the notification of Wave_HandleTask to enter blocking*/
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#if (!USING_DEBUG)
    Report_TimeConsum();
#endif
  }
  /* USER CODE END Report_Task */
}

/* USER CODE BEGIN Header_Led_Task */
/**
 * @brief Function implementing the Run_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Led_Task */
void Led_Task(void const * argument)
{
  /* USER CODE BEGIN Led_Task */
  /* Infinite loop */
  for (;;)
  {
    /*Set event flag bit*/
    xEventGroupSetBits(Event_Handle, RUN_TASK_BIT);
    led_state ^= 1;
    HAL_GPIO_WritePin(Run_Led_GPIO_Port, Run_Led_Pin, led_state);
    osDelay(500);
  }
  /* USER CODE END Led_Task */
}

/* USER CODE BEGIN Header_Wave_HandleTask */
/**
 * @brief Function implementing the Data_HandleTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Wave_HandleTask */
void Wave_HandleTask(void const * argument)
{
  /* USER CODE BEGIN Wave_HandleTask */
  /* Infinite loop */
  for (;;)
  {
    /*Set event flag bit*/
    xEventGroupSetBits(Event_Handle, WAVE_TASK_BIT);
    if (Wave_Handle())
    {
      /*If you send a notification without a notification value
      and do not retain the original notification value of the
      received task, the notification value of the received task
      will be increased by one.*/
      xTaskNotifyGive(Uart_TaskHandle);
    }
    osDelay(1);
  }
  /* USER CODE END Wave_HandleTask */
}

/* USER CODE BEGIN Header_Dwin_HandleTask */
/**
 * @brief Function implementing the Dwin_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Dwin_HandleTask */
void Dwin_HandleTask(void const * argument)
{
  /* USER CODE BEGIN Dwin_HandleTask */
  /* Infinite loop */
  for (;;)
  {
    /*Set event flag bit*/
    xEventGroupSetBits(Event_Handle, DWIN_TASK_BIT);
    if (Uart_Dma.recv_end_flag)
    { /*Clear receive completion flag*/
      Uart_Dma.recv_end_flag = false;
      /*Devon screen response event polling*/
      Dwin_Poll();
      /*Clear data buffer and data length*/
      memset(Uart_Dma.rx_buffer, 0, Uart_Dma.rx_len);
      Uart_Dma.rx_len = 0;
      /*Reopen DMA reception*/
      HAL_UART_Receive_DMA(&huart1, Uart_Dma.rx_buffer, RX_BUF_SIZE);
    }
    osDelay(1);
  }
  /* USER CODE END Dwin_HandleTask */
}

/* USER CODE BEGIN Header_Idog_Task */
/**
 * @brief Function implementing the IWDG_Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Idog_Task */
void Idog_Task(void const * argument)
{
  /* USER CODE BEGIN Idog_Task */
  EventBits_t uxBits;
  /* Infinite loop */
  for (;;)
  {
    /*Wait for event flags from all tasks*/
    uxBits = xEventGroupWaitBits(Event_Handle,   /* Event flag group handle*/
                                 EVENT_ALL_BIT,  /* Wait for EVENT_ALL_BIT is set */
                                 pdTRUE,         /* Task before exiting EVENT_ALL_BIT is cleared. This is EVENT_ALL_BITis set to indicate "exit"*/
                                 pdTRUE,         /* Set to pdtrue to wait for EVENT_ALL_BIT is set*/
                                 osWaitForever); /* Timeout has been waiting */
    if((uxBits & EVENT_ALL_BIT) == EVENT_ALL_BIT)
    {
      /*feed a dog*/
      HAL_IWDG_Refresh(&hiwdg1);
    }
    // osDelay(200);
  }
  /* USER CODE END Idog_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
