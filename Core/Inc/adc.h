/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

/* USER CODE BEGIN Private defines */
#define ADC_CHANNEL_SIZE 4U
/*ADC reference voltage*/
#define ADC_BITS 16U
#define ADC_MAX_NUM (pow(2U, ADC_BITS) - 1U)
/*ADC reference voltage*/
#define REF_VOLTAGE 3.30F
#define BAT_IN_R1 200.0F
#define BAT_IN_R2 51.0F
#define BAT_IN_OFFSET 0.44F
#define BAT_IN_VOLTAGE() (REF_VOLTAGE * ((BAT_IN_R1 + BAT_IN_R2) / BAT_IN_R2))
#define GET_BAT_VOLTAGE(_value) ((_value / ADC_MAX_NUM) * BAT_IN_VOLTAGE() + BAT_IN_OFFSET)
#define COMM_110V_P1 0.00618F
#define P110V_P2  190.5520F
#define N110V_P2  187.6006F
#define GET_CHECK_VOLTAGE(_value, _P2) (_value * COMM_110V_P1 - _P2)
/*Upper and lower limits of CPU temperature*/
#define CPU_TEMPERATURE_MAX 110.0F
#define CPU_TEMPERATURE_MIN 30.0F
/*ADC temperature channel register value*/
#define TSCAL1() (*(__IO uint16_t *)(0x1FF1E820))
#define TSCAL2() (*(__IO uint16_t *)(0x1FF1E840))
#define GET_CPU_TEMPERATURE(_value) ((CPU_TEMPERATURE_MAX - CPU_TEMPERATURE_MIN) * (_value - TSCAL1()) / (TSCAL2() - TSCAL1()) + 30.0F)
/*Corresponding interval number of battery power segment*/
#define REGION_NUM 5.0F
/*Battery cut-off discharge voltage*/
#define MIN_BATTERY_VOLTAGE 10.80F
/*Charger output voltage*/
#define CHARGER_OUTPUT_VOLTAGE 12.56F
/*Battery charging cut-off voltage*/
#define MAX_BATTERY_VOLTAGE 14.40F
/*Battery voltage interval coefficient*/
#define VOLTAGE_REGION_RATIO() ((CHARGER_OUTPUT_VOLTAGE - MIN_BATTERY_VOLTAGE) / REGION_NUM)
/*Get the corresponding interval of battery power*/
#define GET_REGION(_voltage) (_voltage )
/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC3_Init(void);

/* USER CODE BEGIN Prototypes */
extern uint16_t adc_buf[ADC_CHANNEL_SIZE];
extern void Adc_Handle(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
