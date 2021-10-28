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
#define LIST_BUF_SIZE 32U
#define OVERTIMES 10U

typedef struct
{
	/*通id*/
	uint8_t id;
	/*数据接收完成标志*/
	bool data_flag;
	/*数据长度*/
	uint8_t data_len;
	/*数据缓冲�??*/
	uint16_t data_buf[LIST_BUF_SIZE];
	/*时�??*/
	uint32_t consum_times;
}Dwin_Dcb;

typedef struct DLink
{
	/*每个通道记录10次波形数�??*/
	Dwin_Dcb dcb_data[LISTNODE_SIZE];
	/*节点指针*/
	struct DLink *next;
  /*记录当前节点*/
  uint8_t current_node;
  /*当前捕获的边沿极�?*/
  uint8_t current_edge;
  /*数据块超时时�?*/
  uint16_t overtimes;
  /*计数器溢出次数*/
  uint8_t overflows_num;
}Dwin_List;

extern Dwin_List List_Map[LIST_SIZE];
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
