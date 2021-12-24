/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"
#include "usart.h"
#include "Dwin.h"

ALIGN_32BYTES(uint16_t adc_buf[ADC_CHANNEL_SIZE]) __attribute__((at(0x38000000))) = {0};
// ALIGN_32BYTES(uint16_t adc_buf[ADC_CHANNEL_SIZE]) __attribute__((section(".ARM.__at_0x24000000")))  = {0};

/*Each 12V battery consists of 6 units*/ 
static const float Voltage_Interval[][2] = 
{	
	{0, 							 						MIN_BATTERY_VOLTAGE}, /*Low voltage, shutdown*/
	{MIN_BATTERY_VOLTAGE , MIN_BATTERY_VOLTAGE + 1U * VOLTAGE_REGION_RATIO()},
	{MIN_BATTERY_VOLTAGE + 1U * VOLTAGE_REGION_RATIO(), MIN_BATTERY_VOLTAGE + 2U * VOLTAGE_REGION_RATIO()},
	{MIN_BATTERY_VOLTAGE + 2U * VOLTAGE_REGION_RATIO(), MIN_BATTERY_VOLTAGE + 3U * VOLTAGE_REGION_RATIO()},
	{MIN_BATTERY_VOLTAGE + 3U * VOLTAGE_REGION_RATIO(), MIN_BATTERY_VOLTAGE + 4U * VOLTAGE_REGION_RATIO()},
	{MIN_BATTERY_VOLTAGE + 4U * VOLTAGE_REGION_RATIO(), MIN_BATTERY_VOLTAGE + 5U * VOLTAGE_REGION_RATIO()},
  {MIN_BATTERY_VOLTAGE + 5U * VOLTAGE_REGION_RATIO(), MAX_BATTERY_VOLTAGE},
};	
/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc3;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = ENABLE;
  hadc1.Init.Oversampling.Ratio = 31;
  hadc1.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_5;
  hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc1.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}
/* ADC3 init function */
void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */
  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_16B;
  hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.NbrOfConversion = 2;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc3.Init.OversamplingMode = ENABLE;
  hadc3.Init.Oversampling.Ratio = 31;
  hadc3.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_5;
  hadc3.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc3.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC12_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC4     ------> ADC1_INP4
    PC5     ------> ADC1_INP8
    */
    GPIO_InitStruct.Pin = Input_Pvoltage_Pin|Input_Nvoltage_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Stream2;
    hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
  else if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */
    /* ADC3 clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**ADC3 GPIO Configuration
    PF7     ------> ADC3_INP3
    */
    GPIO_InitStruct.Pin = BatVoltage_Check_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BatVoltage_Check_GPIO_Port, &GPIO_InitStruct);

    /* ADC3 DMA Init */
    /* ADC3 Init */
    hdma_adc3.Instance = BDMA_Channel0;
    hdma_adc3.Init.Request = BDMA_REQUEST_ADC3;
    hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc3.Init.Mode = DMA_CIRCULAR;
    hdma_adc3.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc3);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC12_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC4     ------> ADC1_INP4
    PC5     ------> ADC1_INP8
    */
    HAL_GPIO_DeInit(GPIOC, Input_Pvoltage_Pin|Input_Nvoltage_Pin);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
  else if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspDeInit 0 */

  /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC3_CLK_DISABLE();

    /**ADC3 GPIO Configuration
    PF7     ------> ADC3_INP3
    */
    HAL_GPIO_DeInit(BatVoltage_Check_GPIO_Port, BatVoltage_Check_Pin);

    /* ADC3 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC3_MspDeInit 1 */

  /* USER CODE END ADC3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*Battery detection*/
static void Battery_Detection(uint16_t site)
{
  uint16_t last_icon = 0x0000;
  uint16_t temp_icon = 0x0000;
  static uint16_t counters = 0;

  /*The battery voltage is too low, give a prompt and shut down*/
  if (!site)
  { 
    Dwin_PageChange(WARNING_PAGE);
    osDelay(1000);
    Dwin_PageChange(0x02);
    /*Continuous detection for 30s*/
    if (++counters == 15U)
    {
      counters = 0U;
      HAL_GPIO_WritePin(Power_Off_GPIO_Port, Power_Off_Pin, GPIO_PIN_RESET);
    }
  }
  else
  {
    counters = 0U;
  }
  /*Charger currently plugged in*/
  if (HAL_GPIO_ReadPin(Charge_State_GPIO_Port, Charge_State_Pin) == GPIO_PIN_SET)
  {
    site <= 2U ? (temp_icon = LOW_CHARGERDIS_ICON) : 
    (site <= 5U ? (temp_icon = NORMAL_CHARGERDIS_ICON) : (temp_icon = FULL_CHARGERDIS_ICON));
  }
  else
  { /*An icon is missing*/
    temp_icon = site;
    // if (site <= 1U)
    // {
    //   temp_icon = 1U;
    // }
    // else 
    if (site >= 5U)
    {
      temp_icon = 5U;
    }
  }
  if (site != last_icon)
  {
    /*Record the last status of the battery icon*/
    last_icon = site;
    /*The data is reported to the address specified on the Devon screen*/
	  Endian_Swap((uint8_t *)&temp_icon, 0U, sizeof(temp_icon));
    Dwin_Write(BATTERY_ICON_ADDR, (uint8_t *)&temp_icon, sizeof(temp_icon));
  }
}


/**
 * @brief  Obtain the range of the current battery voltage
 * @param  voltage Input voltage
 * @retval current location
 */
static uint16_t Get_VoltageInterval(float voltage)
{
	uint16_t length = (sizeof(Voltage_Interval) / sizeof(float)) / 2U;

	for(uint16_t i = 0; i < length; i++)
	{
		if((voltage > Voltage_Interval[i][0]) && (voltage < Voltage_Interval[i][1]))
		{
			return i;
		}
	}
	return 0;
}

/*Process the collected ADC value*/
void Adc_Handle(void)
{
  float temp_value[4U] = {0};

  /* CThe read variable is in the cache, invalidate the cache*/
  SCB_InvalidateDCache_by_Addr((uint32_t *)adc_buf, ADC_CHANNEL_SIZE);
  temp_value[0U] = GET_CHECK_VOLTAGE(adc_buf[0U], P110V_P2) + 1.3F;
  temp_value[1U] = GET_CHECK_VOLTAGE(adc_buf[1U], N110V_P2) - 0.35F;
  temp_value[2U] = GET_BAT_VOLTAGE(adc_buf[2U]);
  temp_value[3U] = GET_CPU_TEMPERATURE(adc_buf[3U]);
#if (!USING_DEBUG)
#if (!ADC_DEBUG)
  /*Get the battery icon corresponding to the current power*/
  Battery_Detection(Get_VoltageInterval(temp_value[2U]));
  /*Shield unwanted interference*/
  for(uint8_t i = 0; i < sizeof(temp_value) / sizeof(float); i++)
  { /*Remove jitter error*/
    if (fabs(temp_value[i]) < 0.5F)
    {
      temp_value[i] = 0U;
    } 
    /*The data is reported to the address specified on the Devon screen*/
	  Endian_Swap((uint8_t *)&temp_value[i], 0U, sizeof(float));
    Dwin_Write(POSITIVE_110V_ADDR + i * sizeof(float), (uint8_t *)&temp_value[i], sizeof(float));
  }
#endif
#else
#if (ADC_DEBUG)
  /* Clean Data Cache to update the content of the SRAM to be used by the DMA */
  // SCB_CleanDCache_by_Addr((uint32_t *)adc_buf, ADC_CHANNEL_SIZE);
  // DEBUG("+110v is %fV, -110v is %fV, VBAT is %fV, temperature is %fC.\r\n", temp_value[0], temp_value[1], temp_value[2], temp_value[3]);
  DmaPrintf("+110v is %fV, -110v is %fV, VBAT is %fV, temperature is %fC.\r\n", temp_value[0], temp_value[1], temp_value[2], temp_value[3]);
  // DmaPrintf("+110v is %d, -110v is %d, VBAT is %d, temperature is %d.\r\n", adc_buf[0], adc_buf[1], adc_buf[2], adc_buf[3]);
#endif
#endif
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
