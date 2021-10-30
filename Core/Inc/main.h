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
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LIST_SIZE     6U
#define LISTNODE_SIZE 10U
#define DLINKX(x) (DLink##x)
/*Buffer size*/
#define LIST_BUF_SIZE 64U
/*Timeout*/
#define OVERTIMES 10U
/*Overflow times,Time unit: US*/
#define OVERFLOW_COUNTS(times, bits, fosc) ((times * (1000000UL)) / ((uint32_t)(pow(2U, bits) / (fosc))))

typedef enum class
{ 
  Falling_Edge = 0U,
  Rising_Edge = !Falling_Edge
}EDGE;
typedef struct
{
	/*Channel ID*/
	uint8_t id;
	/*Data receiving completion flag*/
	bool data_flag;
	/*Data length*/
	uint8_t data_len;
  /*Data buffer*/
  uint16_t buf[LIST_BUF_SIZE];
  /*Block timeout*/
  uint16_t overtimes;
  /*Counter overflow times*/
  uint32_t overflows_num;
	/*Time consumption*/
	float consum_times;
}Dwin_Dcb;

typedef struct DLink
{
	/*Record 10 times of waveform data for each channel*/
	Dwin_Dcb dcb_data[LISTNODE_SIZE];
  /*First detection mark*/
  uint8_t first_flag;
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
