/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define USING_RTOS 1
#define USING_DMA 1
#define USING_PRINTF 1
#define USING_UART4 0

#if (USING_DMA == 1)
#define TX_BUF_SIZE 512U
#define RX_BUF_SIZE 128U
#if (USING_PRINTF)
#if (USING_DMA)
#define DEBUG(fmt, ...)                                                                   \
  do                                                                                      \
  {                                                                                       \
    DmaPrintf("<<File:%s  Line:%d  Function:%s>>\r\n", __FILE__, __LINE__, __FUNCTION__); \
    DmaPrintf(fmt, ##__VA_ARGS__);                                                        \
  } while (0);
#else
  do                                                                                      \
  {                                                                                       \
    printf("<<File:%s  Line:%d  Function:%s>>\r\n", __FILE__, __LINE__, __FUNCTION__);    \
    printf(fmt, ##__VA_ARGS__);                                                           \
  } while (0);
#endif
#else
#define DEBUG(fmt, ...)
#endif
  typedef struct
  {
    /*RX complete flag*/
    __IO bool recv_end_flag;
    /*TX Frame length*/
    __IO uint32_t tx_len;
    /*TX receive buffer*/
    uint8_t tx_buffer[TX_BUF_SIZE];
    /*RX Frame length*/
    __IO uint32_t rx_len;
    /*RX receive buffer*/
    uint8_t rx_buffer[RX_BUF_SIZE];
  } UART_DMA_BLOCK;

  extern UART_DMA_BLOCK Uart_Dma;
  extern UART_DMA_BLOCK Uart_Dma4;
#endif
/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
  extern void DmaPrintf(const char *format, ...);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
