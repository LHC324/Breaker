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
#define Capture_Channel_1_Pin GPIO_PIN_6
#define Capture_Channel_1_GPIO_Port GPIOA
#define Capture_Channel_2_Pin GPIO_PIN_7
#define Capture_Channel_2_GPIO_Port GPIOA
#define Capture_Channel_3_Pin GPIO_PIN_0
#define Capture_Channel_3_GPIO_Port GPIOB
#define Run_Led_Pin GPIO_PIN_1
#define Run_Led_GPIO_Port GPIOB
#define Capture_Channel_5_Pin GPIO_PIN_12
#define Capture_Channel_5_GPIO_Port GPIOD
#define Capture_Channel_6_Pin GPIO_PIN_13
#define Capture_Channel_6_GPIO_Port GPIOD
#define Capture_Channel_4_Pin GPIO_PIN_9
#define Capture_Channel_4_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
/*Debug options*/
#define USING_DEBUG   0U
#define LIST_SIZE     6U
#define LISTNODE_SIZE 10U
#define DLINKX(x) (DLink##x)
/*Buffer size*/
#define LIST_BUF_SIZE 512U
/*Count value*/
#define CVALUE 65535U
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

typedef enum class
{ 
  Falling_Edge = 0U,
  Rising_Edge = !Falling_Edge
}EDGE;
typedef struct
{
  /*First detection mark*/
  bool first_flag;
  /*First value*/
  uint16_t first_value;
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
  /*Edge polarity currently captured*/
  EDGE current_edge;
  /*Node pointer*/
	struct DLink *next;
}Dwin_List;

extern Dwin_List List_Map[LIST_SIZE];
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
