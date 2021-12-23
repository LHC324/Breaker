/*
 * Dwin.c
 *
 *  Created on: 2021年10月23日
 *      Author: LHC
 */

#include "main.h"
#include "Dwin.h"
#include "usart.h"
#include "string.h"
#include "cmsis_os.h"

Dwin_T g_Dwin = {0};
DwinMap g_map[32U] = {0};

#if (USING_DEBUG)
extern uint16_t test_buf[256];
extern uint32_t count;
#endif

#if (USING_CRC_TABLE)
/*Devin screen CRC checklist*/
uint8_t CRCTABH[256] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40};

uint8_t CRCTABL[256] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40};
#endif

/*定义6条链表*/
// Dwin_List DLink0, DLink1, DLink2, DLink3, DLink4, DLink5;

/*6行10列的二维数组*/
Dwin_List List_Map[LIST_SIZE] = {0};
/*定义时耗上报数据结构*/
Report_S Report = {0};
/*记录当前波形采集的节点*/
// uint8_t g_CurNode[LIST_SIZE][LISTNODE_SIZE] = { 0 };

//#define LIST_SIZE (sizeof(List_Map) / sizeof(Dwin_List))
static void Shut_Down_option(uint8_t *dat, uint8_t len);
static void Reset_Option(uint8_t *dat, uint8_t len);
static void Sure_Option(uint8_t *dat, uint8_t len);
/*Devon screen task event group*/
DwinMap RecvHandle[] =
	{
		{SHUT_ADDR, Shut_Down_option},
		{RESET_ADDR, Reset_Option},
		{SURE_ADDR, Sure_Option}};

#define EVENT_HANDLE_SIZE sizeof(RecvHandle) / sizeof(DwinMap)

/**
 * @brief  Dwin screen shut down
 * @param  dat Data pointer
 * @param  len Dtat length
 * @retval None
 */
void Shut_Down_option(uint8_t *dat, uint8_t len)
{
	/*Check whether the variable data is legal*/
	uint16_t data = ((uint16_t)dat[0]) << 8 | dat[1];
	if (data != SHUT_DOWN_CODE)
	{
		return;
	}
	osDelay(500);
	HAL_GPIO_WritePin(Power_Off_GPIO_Port, Power_Off_Pin, GPIO_PIN_RESET);
}

/**
 * @brief  Dwin screen reset
 * @param  dat Data pointer
 * @param  len Dtat length
 * @retval None
 */
void Reset_Option(uint8_t *dat, uint8_t len)
{
	/*Check whether the variable data is legal*/
	uint16_t data = ((uint16_t)dat[0]) << 8 | dat[1];
	if (data != RESET_CODE)
	{
		return;
	}
	/*Reset current report type*/
	Report.current_type = Even;
	/*Clear the current reported times record*/
	Report.current_counts = 0;
	/*Clear the current report buffer*/
	memset((uint8_t *)Report.handle_buf, 0, sizeof(Report.handle_buf));
	/*Clear all channel waveforms*/
	for (uint8_t i = 0; i < LIST_SIZE; i++)
	{
		/*Clear the waveform of full screen even number of times*/
		Dwin_Curve_Clear(List_Map[i].id);
		/*Reset the node currently being captured*/
		List_Map[i].current_node = 0U;
		/*The serial port screen is too slow and requires a certain delay*/
		// osDelay(1);
	}
	/*Clear foreground data*/
	for (uint8_t i = 0; i < LIST_SIZE * 2U; i++)
	{
		Dwin_Write(TIMECOUNSUM_ADDR + i * 4U, "\x00\x00\x00\x00", sizeof(float));
		/*The serial port screen is too slow and requires a certain delay*/
		// osDelay(1);
	}
	/*Clear background data*/
	for (uint8_t i = 0; i < LIST_SIZE * 2U * 5U; i++)
	{
		Dwin_Write(RECORDS_ADDR, "\x00\x00\x00\x00", sizeof(float));
		/*The serial port screen is too slow and requires a certain delay*/
		// osDelay(1);
	}
}

/**
 * @brief  Dwin screen confirmation
 * @param  dat Data pointer
 * @param  len Dtat length
 * @retval None
 */
void Sure_Option(uint8_t *dat, uint8_t len)
{
	/*Check whether the variable data is legal*/
	uint16_t data = ((uint16_t)dat[0]) << 8 | dat[1];
	if (data != SURE_CODE)
	{
		return;
	}
	/*Clear background data*/
	for (uint8_t i = 0; i < LIST_SIZE * 2U * 5U; i++)
	{
		Dwin_Write(RECORDS_ADDR + i * 4U, "\x00\x00\x00\x00", sizeof(float));
		/*The serial port screen is too slow and requires a certain delay*/
		// osDelay(1);
	}
}

/**
 * @brief  Get the time consumed by each node
 * @param  list Pointer to the first node of the current linked list
 * @note  There are calculation errors (the number of overflows, the critical conditions of the calculation formula are not accurate)
 * @retval None
 */
#if (!SWITCH_METHOD)
static void Get_NodeTimes(Dwin_List *list)
{
	uint16_t first_value = 0U;
	uint16_t end_value = 0U;
	uint16_t times_value = 0U;
	/*Entry parameter check*/
	if (list == NULL)
	{
		return;
	}
	/*During the capture, the counter overflowed*/
	// if (COMPLETE_NODE.overflows_num > OVERFLOW_COUNTS(TIMES, FREQ))
	if (COMPLETE_NODE.effect_overflows > 0U)
	{
		/*The first node capture value is not 0*/
		if (FIRST_NODE() > 0U)
		{ /*Separately handle irregular values that overflow for the first time*/
			first_value = CVALUE - FIRST_NODE();
			COMPLETE_NODE.effect_overflows -= 1U;
		}
		/*The last node happened to be not an overflow node*/
		if (END_NODE() < CVALUE)
		{
			end_value = END_NODE();
		}
		/*Calculate the duration of the capture*/
		// COMPLETE_NODE.consum_times = (float)(first_value +
		// 									 (COMPLETE_NODE.overflows_num - (OVERFLOW_COUNTS(TIMES, FREQ) + 1.0F)) * CVALUE +
		// 									 end_value) /
		// 							 ACCU();
		COMPLETE_NODE.consum_times = (float)(first_value + (COMPLETE_NODE.effect_overflows) * CVALUE + end_value) /
									 ACCU();
	}
	else /*The pulse counter did not overflow*/
	{	 /*Very good mechanical properties*/
		if (COMPLETE_NODE.data_len == 1U)
		{
			COMPLETE_NODE.consum_times = 0.099F;
		}
		else /*There are multiple pulses*/
		{

			// DmaPrintf("first volue is %d, end value is %d, end - first = %d.\r\n",
			// 		  FIRST_NODE(), COMPLETE_NODE.buf[COMPLETE_NODE.data_len - 1U], value);
			// DmaPrintf("result = %f\t\n", COMPLETE_NODE.consum_times);

			/*Avoid overflowing data, resulting in negative results*/
			for (uint16_t i = COMPLETE_NODE.data_len - 1U; i != 0U; i--)
			{
				if (COMPLETE_NODE.buf[i] < COMPLETE_NODE.buf[i - 1U])
				{
					times_value += COMPLETE_NODE.buf[i] + (CVALUE - COMPLETE_NODE.buf[i - 1U]);
				}
				else
				{
					times_value += (COMPLETE_NODE.buf[i] - COMPLETE_NODE.buf[i - 1U]);
				}
			}
			COMPLETE_NODE.consum_times = (float)times_value / ACCU();
		}
	}
}
#else
static void Get_NodeTimes(Dwin_List *list)
{
	uint32_t temp_times = 0;
	for (uint16_t i = COMPLETE_NODE.data_len; i != 0U; i--)
	{
		{ /*Counter overflow*/
			if (COMPLETE_NODE.buf[i] < COMPLETE_NODE.buf[i - 1U])
			{
				temp_times += COMPLETE_NODE.buf[i] +
							  (CVALUE - COMPLETE_NODE.buf[i - 1U]);
			}
			else
			{
				temp_times += (COMPLETE_NODE.buf[i] -
							   COMPLETE_NODE.buf[i - 1U]);
			}
		}
	}
	COMPLETE_NODE.consum_times = (float)temp_times / ACCU();
}
#endif

/**
 * @brief  Capture error processing
 * @param  list Pointer to the first node of the current linked list
 * @retval error value
 */
// float Get_Capture_Error(Dwin_List *list)
// {
// 	float v_error = 0.0F;
// 	float v_temp = 0.0F;
// 	float v_temp1 = 0.0F;
// 	float v_temp2 = 0.0F;

// 	for (uint16_t i = 0; i < COMPLETE_NODE.data_len; i++)
// 	{	/*16bit counter overflow*/
// 		if (COMPLETE_NODE.buf[i + 1] < COMPLETE_NODE.buf[i])
// 		{
// 			v_temp1 = COMPLETE_NODE.buf[i];
// 			v_temp2 = (CVALUE - COMPLETE_NODE.buf[i] + COMPLETE_NODE.buf[i + 1]);
// 		}
// 		else
// 		{
// 			v_temp1 = COMPLETE_NODE.buf[i];
// 			v_temp2 = COMPLETE_NODE.buf[i + 1];
// 		}
// 		v_temp = v_temp2 - v_temp1;

// 		v_error += v_temp;
// 	}
// 	return (v_error / ACCU());
// }

/**
 * @brief  多通道波形轮询显示
 * @param  None
 * @retval None
 */
bool Wave_Handle(void)
{
	bool Success_flag = false;
	// #if (USING_DEBUG)
	// 	float error = 0;
	// #endif
	for (uint8_t i = 0; i < LIST_SIZE; i++)
	{
		/*One of the channels captured data*/
		if (LCOMPLETE_NODE.data_flag)
		{ /*Capture success flag*/
			Success_flag = true;
			/*Calculate the duration of the capture*/
			Get_NodeTimes(&List_Map[i]);
			/*Get the parity of the current capture times*/
			if (!(List_Map[i].complete_node % 2U))
			{ /*Even number of captures*/
				Report.current_type = Even;
#if (!USING_DEBUG)
				/*Clear the waveform of full screen even number of times*/
				Dwin_Curve_Clear(List_Map[i].id);
#endif
			}
			else
			{ /*odd number of captures*/
				Report.current_type = Odd;
			}
#if (USING_DEBUG)
#if (CAPTURE_DEBUG)
			/*error compensation*/
			// error = Get_Capture_Error(&List_Map[i]);
			// LCOMPLETE_NODE.consum_times += error;
			DmaPrintf("i= %d,channel %d, node %d, overcounts %d, times is %02fms, first_value %d, effct overflows %d, count = %d.\r\n",
					  i, List_Map[i].id, List_Map[i].complete_node, LCOMPLETE_NODE.overflows_num, LCOMPLETE_NODE.consum_times,
					  LCOMPLETE_NODE.first_value, LCOMPLETE_NODE.effect_overflows, LCOMPLETE_NODE.data_len);

			for (uint16_t k = 0; k < LCOMPLETE_NODE.data_len; k++)
				DmaPrintf("buf[%d],value %d.\r\n", k, LCOMPLETE_NODE.buf[k]);
				// for (uint16_t k = 0; k < count; k++)
				// 	DmaPrintf("test_buf[%d],value %d, count = %d.\r\n", k, test_buf[k], count);
				// count = 0U;
#endif
#else
			/*The existence of electromagnetic interference leads to the most ideal
			situation. Only one pulse has been captured, and the equipment has been
			close to stability*/
			if (LCOMPLETE_NODE.consum_times >= DATA_PRECISION)
			{
				/*Store time data in temporary buffer*/
				Report.handle_buf[i] = LCOMPLETE_NODE.consum_times;
			}
			/*Draw waveform*/
			Dwin_Curve_SchMd(&List_Map[i]);
#endif
			/*Reset data processing completion flag*/
			LCOMPLETE_NODE.data_flag = false;
			/*Reset first detection flag*/
			LCOMPLETE_NODE.first_flag = false;
			/*Clear counter overflow times*/
			LCOMPLETE_NODE.overflows_num = 0U;
			/*Clear valid overflow times*/
			LCOMPLETE_NODE.effect_overflows = 0U;
			/*Clear data length*/
			LCOMPLETE_NODE.data_len = 0U;
			/*Clearing time consumption*/
			LCOMPLETE_NODE.consum_times = 0U;
			/*After processing a node, clear the current buffer*/
			memset((uint8_t *)LCOMPLETE_NODE.buf, 0U, LIST_BUF_SIZE);
		}
		/*If osdelay is used, it is possible that if and else conditions occur at the same time*/
	}
	return Success_flag;
}

/**
 * @brief  链表初始化
 * @param  list 首节点指针
 * @retval None
 */
void Init_List(Dwin_List *list, uint8_t channel_id)
{
	list->id = channel_id;
	list->current_node = 0;
	// list->current_edge = Falling_Edge;
	for (uint16_t i = 0; i < LISTNODE_SIZE; i++)
	{
		list->dcb_data[i].overtimes = 10U;
		list->dcb_data[i].first_flag = false;
		list->dcb_data[i].data_flag = false;
		list->dcb_data[i].timer_synflag = false;
		list->dcb_data[i].data_len = 0;
		list->dcb_data[i].overflows_num = 0;
		list->dcb_data[i].effect_overflows = 0;
		list->dcb_data[i].consum_times = 0.0F;
		list->dcb_data[i].error_value = 0.0F;
	}
}

/**
 * @brief  轮询上报各通道测量得到的时间
 * @param  r 上报对象指针
 * @retval None
 */
void Report_TimeConsum(void)
{
	uint16_t addr = 0;

	/*The current collection times are even, which is on the left side of the report screen*/
	if (Report.current_type == Even)
	{
		addr = TIMECOUNSUM_ADDR;
	}
	else
	{
		addr = TIMECOUNSUM_ADDR + sizeof(float);
	}
	/*End storage mode conversion for float type variables*/
	for (uint8_t i = 0; i < sizeof(Report.handle_buf) / sizeof(float); i++)
	{
		/*Channel data no data generation*/
		if (Report.handle_buf[i] == 0U)
		{
			/*Clear the waveform of full screen even number of times*/
			Dwin_Curve_Clear(List_Map[i].id);
			continue;
		}
		/*Report one after the exchange is completed*/
		Endian_Swap((uint8_t *)&Report.handle_buf[i], 0U, sizeof(float));
		Dwin_Write(addr + i * 8U, (uint8_t *)&Report.handle_buf[i], sizeof(float));
		/*The serial port screen is too slow and requires a certain delay*/
		// osDelay(1);
		/*Fill the history area with data*/
		Dwin_Write(RECORDS_ADDR + i * sizeof(float) * 10U + Report.current_counts * 4U, (uint8_t *)&Report.handle_buf[i], sizeof(float));
	}
	/*Historical data record plus one*/
	if ((++Report.current_counts) >= LISTNODE_SIZE)
	{
		/*The serial port screen is too slow and requires a certain delay*/
		// osDelay(1);
		Report.current_counts = 0;
		/*Switch to prompt page*/
		Dwin_PageChange(0x0B);
	}
	/*When there is no data, empty the buffer*/
	memset((uint8_t *)Report.handle_buf, 0, sizeof(Report.handle_buf));
}

/**
 * @brief  带CRC的发送数据帧
 * @param  _pBuf 数据缓冲区指针
 * @param  _ucLen 数据长度
 * @retval None
 */
void Dwin_SendWithCRC(uint8_t *_pBuf, uint16_t _ucLen)
{
	uint16_t crc = 0;
	uint8_t buf[256U] = {0};

	memcpy(buf, _pBuf, _ucLen);
	/*The first three bytes do not participate in verification*/
	crc = Get_Crc16(&_pBuf[3U], _ucLen - 3U, 0xffff);
	buf[_ucLen++] = crc;
	buf[_ucLen++] = crc >> 8U;

	/* Clean Data Cache to update the content of the SRAM to be used by the DMA */
	SCB_CleanDCache_by_Addr((uint32_t *)buf, _ucLen);
	// HAL_UART_Transmit(&huart1, _pBuf, _ucLen, 0x1000);
	HAL_UART_Transmit_DMA(&huart1, buf, _ucLen);
	/*等待串口接收完成期间，进行系统调度*/
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
	{
		osDelay(30);
	}
}

/**
 * @brief  发送数据帧(不带CRC)
 * @param  _pBuf 数据缓冲区指针
 * @param  _ucLen 数据长度
 * @retval None
 */
void Dwin_Send(uint8_t *_pBuf, uint16_t _ucLen)
{
	/*Clear data in DCache*/
	// SCB_CleanDCache();
	/* Clean Data Cache to update the content of the SRAM to be used by the DMA */
	SCB_CleanDCache_by_Addr((uint32_t *)_pBuf, _ucLen);
	// HAL_UART_Transmit(&huart1, _pBuf, _ucLen, 0x1000);
	HAL_UART_Transmit_DMA(&huart1, _pBuf, _ucLen);
	/*等待串口接收完成期间，进行系统调度*/
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
	{
		osDelay(30);
	}
}


/**
 * @brief  读数据处理
 * @param  None
 * @retval None
 */
void Dwin_83H(void)
{
	uint16_t Addr = ((uint16_t)g_Dwin.RxBuf[4] << 8) | g_Dwin.RxBuf[5];
	uint8_t i = 0;
	/*有效数据缓冲区*/
	uint8_t valid_buf[16U] = {0};
	/*有效数据长度*/
	uint8_t valid_length = g_Dwin.RxBuf[6] * 2U;

	memcpy(valid_buf, &g_Dwin.RxBuf[7], valid_length);

	for (i = 0; i < EVENT_HANDLE_SIZE; i++)
	{
		if (Addr == g_map[i].addr)
		{
			g_map[i].event(valid_buf, valid_length);
			/*Only one event is processed at a time*/
			break;
		}
	}
}

/**
 * @brief  接收新数据
 * @param  rxBuf 数据接收缓冲区
 * @param  len 数据长度
 * @retval None
 */
__inline void Dwin_ReciveNew(uint16_t len)
{
	/* Clean Data Cache to update the content of the SRAM to be used by the DMA */
    // SCB_CleanDCache_by_Addr((uint32_t *)g_Dwin.RxBuf, len);
	// memcpy(g_Dwin.RxBuf, rxBuf, len);
	// SCB_CleanDCache();
	g_Dwin.RxCount = len;
}

/**
 * @brief  初始化
 * @param  None
 * @retval None
 */
void Dwin_Init(void)
{
	/*Initialize receive and send buffers*/
	g_Dwin.TxBuf = Uart_Dma.tx_buffer;
	g_Dwin.TxCount = 0;
	g_Dwin.RxBuf = Uart_Dma.rx_buffer;
	g_Dwin.RxCount = 0;
	/*Callback mapping relationship of address variables corresponding to imported Diwen*/
	for (uint8_t i = 0; i < EVENT_HANDLE_SIZE; i++)
	{
		g_map[i].addr = RecvHandle[i].addr;
		g_map[i].event = RecvHandle[i].event;
	}
}

/**
 * @brief  导入事件映射
 * @param  addr 变量地址
 * @param  event 事件回调函数指针
 * @retval None
 */
// void Dwin_InportMap(uint32_t addr, pfunc event)
// {
// 	g_map[mapindex].addr = addr;
// 	g_map[mapindex].event = event;
// 	mapindex++;
// }

/**
 * @brief  判断数据帧是否正确
 * @param  None
 * @retval None
 */
void Dwin_Poll(void)
{
	// 	static GPIO_PinState state ;
	// 	state ^= 1;
	// HAL_GPIO_WritePin(Run_Led_GPIO_Port, Run_Led_Pin, state);
	// DmaPrintf("rx->%s\r\t", g_Dwin.RxBuf);
	// SCB_InvalidateDCache();

	if ((g_Dwin.RxBuf[0] != 0x5A) || (g_Dwin.RxBuf[1] != 0xA5))
	{
		g_Dwin.RxCount = 0;
		return;
	}	
	switch (g_Dwin.RxBuf[3])
	{
		case READ_CMD:
		{
			Dwin_83H();
		}break;
	default:
		break;
	}
}

/**
 * @brief  写数据变量到指定地址并显示
 * @param  start_addr 开始地址
 * @param  dat 指向数据的指针
 * @param  length 数据长度
 * @retval None
 */
void Dwin_Write(uint16_t start_addr, uint8_t *dat, uint16_t length)
{
	uint8_t i = 0;
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = length + 3U + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = length + 3U;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = WRITE_CMD;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = start_addr >> 8U;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = start_addr;

	for (; i < length; i++)
	{
		g_Dwin.TxBuf[g_Dwin.TxCount++] = dat[i];
	}
#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  读出指定地址指定长度数据
 * @param  start_addr 开始地址
 * @param  dat 指向数据的指针
 * @param  length 数据长度
 * @retval None
 */
void Dwin_Read(uint16_t start_addr, uint16_t words)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x04 + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x04;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = READ_CMD;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = start_addr >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = start_addr;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = words;

#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  进行页面切换处理
 * @param  Page 页
 * @example 5A A5 07 82 00 84 5A 01 00 01   切到第一页
 * @retval None
 */
void Dwin_PageChange(uint16_t Page)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x07 + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x07;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x84;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Page >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Page;

#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  进行一次触摸动作
 * @param  type 触摸类型
 * @param  Pos_x  x坐标
 * @param  Pos_y  y坐标
 * @example A A5 0B 82 00 D4 5A A5 00 04 00 EE 00 8F
 * @retval None
 */
void Dwin_TouchAction(TouchType type, uint16_t Pos_x, uint16_t Pos_y)
{
	uint16_t typeval;
	switch (type)
	{
	case press:
		typeval = 0x0001;
		break;
	case uplift:
		typeval = 0x0002;
		break;
	case longpress:
		typeval = 0x0003;
		break;
	case press_uplift:
		typeval = 0x0004;
		break;
	}
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x0B + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x0B;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = TOUCH_CMD;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = typeval >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = typeval;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Pos_x >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Pos_x;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Pos_y >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Pos_y;

#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  发送曲线点
 * @param  Channel 通道号
 * @param  *dat    数据源头指针
 * @param  length  数据长度
 * @retval None
 */
void Dwin_Curve(uint16_t Channel, uint16_t *dat, uint16_t length)
{
	uint8_t i = 0;

	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = length * 2U + 9U + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = length * 2U + 9U;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x03;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x10;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Channel;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = length;

	for (; i < length; i++)
	{
#if (USING_LITTLE == 1)
		/*先对数据进行转换*/
		Endian_Swap((uint8_t *)&dat[i], 0, sizeof(uint16_t));
#endif
		g_Dwin.TxBuf[g_Dwin.TxCount++] = dat[i] >> 8;
		g_Dwin.TxBuf[g_Dwin.TxCount++] = dat[i];
	}
#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  曲线显示多通道，但是只能是多通道一个数据
 * @param  Channelnum 通道数量
 * @param  *dat    数据源头指针
 * @example 5A A5 13 82 03 10 5A A5 03 00 02 01 F9 07 01 01 F7 07 00 01 00 00
 * @retval None
 */
void Dwin_Curve_MuitiChannel(uint16_t Channelnum, DwinCurve *dat)
{
	uint8_t i = 0;

	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Channelnum * 4 + 7U + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Channelnum * 4 + 7U;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x03;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x10;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Channelnum;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;

	for (; i < Channelnum; i++)
	{
		g_Dwin.TxBuf[g_Dwin.TxCount++] = dat[i].channel;
		g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01;
		g_Dwin.TxBuf[g_Dwin.TxCount++] = dat[i].val >> 8;
		g_Dwin.TxBuf[g_Dwin.TxCount++] = dat[i].val;
	}
#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  显示单个通道上一组数据
 * @param  list 当前链表首节点指针
 * @retval None
 */
void Dwin_Curve_SchMd(Dwin_List *list)
{
	uint32_t real_size = COMPLETE_NODE.data_len;
	uint32_t v_temp = 0;
	uint32_t iter = 0;

	/*采集到的数据点超过了半个通道长度*/
	if ((real_size > SINGLE_SIZE) || (real_size < SINGLE_SIZE))
	{
		real_size = SINGLE_SIZE;
	}
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 9U + real_size * 2U + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 9U + real_size * 2U;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x03; /*通道起始地址*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x10;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01; /*通道块数*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00; /*低字节无效*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = list->id;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = real_size;
	/*Get the effective data length after removing the frame header and end in the data frame*/
	real_size = real_size * 2U - FIXED_SIZE;
	if (list->start_sate == GPIO_PIN_SET)
	{
		GARTHER_FRAME(0x0A);
	}
	else
	{
		GARTHER_FRAME(0x5A);
	}

	while (1)
	{
		/*16bit counter overflow*/
		if (COMPLETE_NODE.buf[iter + 1] < COMPLETE_NODE.buf[iter])
		{
			v_temp = (CVALUE - COMPLETE_NODE.buf[iter] + COMPLETE_NODE.buf[iter + 1]); //-
																					   // COMPLETE_NODE.buf[iter];
		}
		else
		{
			v_temp = COMPLETE_NODE.buf[iter + 1] - COMPLETE_NODE.buf[iter];
		}
		/*Quantify proportionally*/
		v_temp /= RATIO();

		if (real_size <= FIXED_SIZE)
		{
			if (list->start_sate == GPIO_PIN_SET)
			{
				GARTHER_FRAME(0x5A);
			}
			else
			{
				GARTHER_FRAME(0x0A);
			}
			break;
		}
		else
		{
			g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;
			g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x0F + v_temp % 75U;
			real_size -= 2U;
		}
		iter++;
		iter %= COMPLETE_NODE.data_len;
	}

	// DmaPrintf("count %d\r\n", g_Dwin.TxCount);
#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  清除曲线
 * @param  Channel 通道号
 * @retval None
 */
void Dwin_Curve_Clear(uint16_t Channel)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x05 + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x05;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x03;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01 + Channel * 2U;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;

#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  设置变量显示颜色
 * @param  addr 变量地址
 * @param  color 16bit颜色值
 * @retval None
 */
void Dwin_Set_Color(uint16_t addr, uint16_t color)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
#if (USING_CRC)
	/*Add two bytes CRC*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x05 + 2U;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x05;
#endif
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	addr += 0x03;
#if (USING_LITTLE)
	g_Dwin.TxBuf[g_Dwin.TxCount++] = addr;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = addr >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = color;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = color >> 8;
#else
	g_Dwin.TxBuf[g_Dwin.TxCount++] = addr >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = addr;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = color >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = color;
#endif
#if (USING_CRC)
	Dwin_SendWithCRC(g_Dwin.TxBuf, g_Dwin.TxCount);
#else
	Dwin_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
#endif
}

/**
 * @brief  取得16bitCRC校验码
 * @param  ptr   当前数据串指针
 * @param  length  数据长度
 * @param  init_dat 校验所用的初始数据
 * @retval 16bit校验码
 */
#if (USING_CRC_TABLE)
uint16_t Get_Crc16(uint8_t *ptr, uint16_t length, uint16_t init_dat)
{
	uint8_t index = 0;
#if (USING_LITTLE)
	uint8_t crch = (uint8_t)init_dat;
	uint8_t crcl = init_dat >> 8U;
#else
	uint8_t crch = init_dat >> 8U;
	uint8_t crcl = (uint8_t)init_dat;
#endif

	for (uint16_t i = 0; i < length; i++)
	{
#if (USING_LITTLE)
		index = crcl ^ *ptr++;
		crcl = crch ^ CRCTABH[index];
		crch = CRCTABL[index];
#else
		index = crch ^ *ptr++;
		crch = crcl ^ CRCTABH[index];
		crcl = CRCTABL[index];
#endif
	}
	return ((crch << 8U) | crcl);
}
#else
uint16_t Get_Crc16(uint8_t *ptr, uint16_t length, uint16_t init_dat)
{
	uint16_t i = 0;
	uint16_t j = 0;
	uint16_t crc16 = init_dat;

	for (i = 0; i < length; i++)
	{
		crc16 ^= *ptr++;

		for (j = 0; j < 8; j++)
		{
			if (crc16 & 0x0001)
			{
				crc16 = (crc16 >> 1) ^ 0xa001;
			}
			else
			{
				crc16 = crc16 >> 1;
			}
		}
	}
	return (crc16);
}
#endif

/**
 * @brief  大小端数据类型交换
 * @note   对于一个单精度浮点数的交换仅仅需要2次
 * @param  pData 数据
 * @param  start 开始位置
 * @param  length  数据长度
 * @retval None
 */
void Endian_Swap(uint8_t *pData, uint8_t start, uint8_t length)
{
	uint8_t i = 0;
	uint8_t tmp = 0;
	uint8_t count = length / 2U;
	uint8_t end = start + length - 1U;

	for (; i < count; i++)
	{
		tmp = pData[start + i];
		pData[start + i] = pData[end - i];
		pData[end - i] = tmp;
	}
}
