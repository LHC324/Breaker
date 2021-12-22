/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Run_Led_Pin GPIO_PIN_4
#define Run_Led_GPIO_Port GPIOE
#define Power_Output_Pin GPIO_PIN_9
#define Power_Output_GPIO_Port GPIOI
#define Power_Input_Pin GPIO_PIN_10
#define Power_Input_GPIO_Port GPIOI
#define Charge_State_Pin GPIO_PIN_3
#define Charge_State_GPIO_Port GPIOF
#define BatVoltage_Check_Pin GPIO_PIN_7
#define BatVoltage_Check_GPIO_Port GPIOF
#define Capture_Channel_1_Pin GPIO_PIN_6
#define Capture_Channel_1_GPIO_Port GPIOA
#define Capture_Channel_2_Pin GPIO_PIN_7
#define Capture_Channel_2_GPIO_Port GPIOA
#define Input_Pvoltage_Pin GPIO_PIN_4
#define Input_Pvoltage_GPIO_Port GPIOC
#define Input_Nvoltage_Pin GPIO_PIN_5
#define Input_Nvoltage_GPIO_Port GPIOC
#define Capture_Channel_3_Pin GPIO_PIN_0
#define Capture_Channel_3_GPIO_Port GPIOB
#define Capture_Channel_5_Pin GPIO_PIN_12
#define Capture_Channel_5_GPIO_Port GPIOD
#define Capture_Channel_6_Pin GPIO_PIN_13
#define Capture_Channel_6_GPIO_Port GPIOD
#define Capture_Channel_4_Pin GPIO_PIN_9
#define Capture_Channel_4_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
/*Debug options*/
#define USING_DEBUG   0U
#define CAPTURE_DEBUG 0U
#define ADC_DEBUG  0U
#define SWITCH_METHOD 0U
#define LIST_SIZE     6U
#define LISTNODE_SIZE 10U
#define DLINKX(x) (DLink##x)
/*Buffer size*/
#define LIST_BUF_SIZE 512U
/*Data accuracy*/
#define DATA_PRECISION 0.00001f
/*Count value*/
#define CVALUE 50000U
/*Interruption time,Unit: S*/
#define TIMES 1.0F
/*Timeout*/
#define OVERTIMES ((uint16_t)(TIMES * 100.0F))
/*Counter bits*/
#define CBITS 16U
/*Counter frequency,Unit: MHz*/
#define FREQ 10.0F
/*accuracy,*/
#define ACCU() (FREQ * ((float)pow(10U, 3U)))
/*Overflow times,Time unit: US*/
#define OVERFLOW_COUNTS(times, fosc) ((times * (1000000.0F)) / ((uint32_t)(CVALUE / (fosc))))
/*Get the state of the pin*/
#define GET_CHANNEL_PIN_STATE(list) \
  (                                 \
      list->id == 0U ? HAL_GPIO_ReadPin(Capture_Channel_1_GPIO_Port, Capture_Channel_1_Pin) : (list->id == 1U ? HAL_GPIO_ReadPin(Capture_Channel_2_GPIO_Port, Capture_Channel_2_Pin) : (list->id == 2U ? HAL_GPIO_ReadPin(Capture_Channel_3_GPIO_Port, Capture_Channel_3_Pin) : (list->id == 3U ? HAL_GPIO_ReadPin(Capture_Channel_4_GPIO_Port, Capture_Channel_4_Pin) : (list->id == 4U ? HAL_GPIO_ReadPin(Capture_Channel_5_GPIO_Port, Capture_Channel_5_Pin) : (list->id == 5U ? HAL_GPIO_ReadPin(Capture_Channel_6_GPIO_Port, Capture_Channel_6_Pin) : (GPIO_PIN_RESET)))))))
/*Define the current node*/
#define CURRENT_NODE (list->dcb_data[list->current_node])
/*Define the complete node*/
#define COMPLETE_NODE (list->dcb_data[list->complete_node])
#define LCOMPLETE_NODE (List_Map[i].dcb_data[List_Map[i].complete_node])
/*First node*/
#define FIRST_NODE()  (COMPLETE_NODE.buf[0])
/*end node*/
#define END_NODE()  (COMPLETE_NODE.buf[COMPLETE_NODE.data_len - 1U])
typedef enum class
{ 
  Falling_Edge = 0U,
  Rising_Edge = !Falling_Edge
}EDGE;

/*Current parity type*/
typedef enum
{
  Odd = 0U,
  Even = !Odd
}CTYPEDEF;
typedef struct
{
  /*First detection mark*/
  bool first_flag;
	/*Data receiving completion flag*/
	bool data_flag;
  /*Timer synchronization flag*/
  bool timer_synflag;
	/*Data length*/
	uint16_t data_len;
  /*Data buffer*/
  uint16_t buf[LIST_BUF_SIZE];
  /*Block timeout*/
  uint16_t overtimes;
  /*Counter overflow times*/
  uint32_t overflows_num;
  /*Effective overflow*/
  uint32_t effect_overflows;
  /*Error value*/
  float error_value;
	/*Time consumption*/
	float consum_times;
}Dwin_Dcb;

typedef struct DLink
{
	/*Channel ID*/
	uint8_t id;
  /*Record 10 times of waveform data for each channel*/
	Dwin_Dcb dcb_data[LISTNODE_SIZE];
  /*Record current node*/
  uint8_t current_node;
  /*Start level state*/
  GPIO_PinState start_sate;
  /*End level state*/
  GPIO_PinState end_sate;
  /*Completion node*/
  uint8_t complete_node;
  /*Edge polarity currently captured*/
  // EDGE current_edge;
  /*Node pointer*/
	// struct DLink *next;
}Dwin_List;

extern Dwin_List List_Map[LIST_SIZE];

typedef struct 
{
  /*Historical records*/
  // float records_buf[LIST_SIZE][5U];
  /*Time consuming data processing buffer*/
  float handle_buf[LIST_SIZE];
  /*Is the current number of snaps odd or even*/
  CTYPEDEF current_type;
  /*Current times*/
	uint8_t current_counts;
}Report_S;

#if (USING_DEBUG)
/*During the debugging phase, the error information is recorded*/
typedef struct 
{
  char file[32U];
  uint32_t current_line;
  uint32_t code;
}Error_Info;
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
