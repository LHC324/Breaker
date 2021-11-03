/*
 * Dwin.c
 *
 *  Created on: 2021年10月23日
 *      Author: LHC
 */

#include "Dwin.h"
#include "usart.h"
#include "string.h"
#include "cmsis_os.h"

Dwin_T g_Dwin = { 0 };
DwinMap g_map[100];
uint8_t mapindex = 0;
/*定义6条链表*/
// Dwin_List DLink0, DLink1, DLink2, DLink3, DLink4, DLink5;

/*6行10列的二维数组*/
Dwin_List List_Map[LIST_SIZE] = {0};
/*记录当前波形采集的节点*/
// uint8_t g_CurNode[LIST_SIZE][LISTNODE_SIZE] = { 0 };

//#define LIST_SIZE (sizeof(List_Map) / sizeof(Dwin_List))

extern uint32_t g_Value;

/**
 * @brief  Get the time consumed by each node
 * @param  list Pointer to the first node of the current linked list
 * @retval None
 */
// static void Get_NodeTimes(Dwin_List *list)
// {
// 	/*Counter overflow, time greater than 6.5536ms*/
// 	if (list->dcb_data[list->current_node].overflows_num > OVERFLOW_COUNTS(1U, 16U, 10U))
// 	{
// 		list->dcb_data[list->current_node].consum_times = (list->dcb_data[list->current_node].consum_times +
// 		(list->dcb_data[list->current_node].overflows_num - (OVERFLOW_COUNTS(1U, 16U, 10U) + 1U)) * 65536U +
//     list->dcb_data[list->current_node].buf[list->dcb_data[list->current_node].data_len]) / 10000.0F;
// 	}
// 	else /*The counter does not overflow and the time is within 6.5536ms*/
// 	{
// 		/*Calculate the settling time*/
// 		list->dcb_data[list->current_node].consum_times =
// 			(list->dcb_data[list->current_node].buf[list->dcb_data[list->current_node].data_len] -
// 			 list->dcb_data[list->current_node].buf[0]) / 10000.0F;
// 	}
// }

/**
 * @brief  Capture error processing
 * @param  list Pointer to the first node of the current linked list
 * @retval error value
 */
// float Get_Capture_Error(Dwin_List *list)
// {
// 	float v_error = 0.0F;
// 	int32_t v_temp = 0;

// 	for(uint16_t i = 0; i < list->dcb_data[list->current_edge].data_len; i++)
// 	{
// 		v_temp = (int32_t)list->dcb_data[list->current_node].buf[i + 1] - (int32_t)list->dcb_data[list->current_node].buf[i];
// 		if(v_temp > 0)
// 		{
// 			v_error += v_temp;
// 		}
// 		else if(v_temp < 0)
// 		{
// 			v_error -= v_temp;
// 		}
// 	}
// 	return (v_error / ACCU());
// }

/**
 * @brief  多通道波形轮询显示
 * @param  None
 * @retval None
 */
void Wave_Handle(void)
{
	uint8_t i = 0, j = 0;
	// DmaPrintf("current value is %d, time consuming is , buf size is  .\r\n", 10);
	for (; i < LIST_SIZE; i++)
	{
		for (j = 0; j < LISTNODE_SIZE; j++)
		{
			// List_Map[i].dcb_data[j].consum_times += Get_Capture_Error(&List_Map[i]);
			if (List_Map[i].dcb_data[j].data_flag)
			{
#if (USING_DEBUG)
				// DmaPrintf("channel %d, node %d, overcounts %d, times is %02fms\r\n", List_Map[i].id, List_Map[i].current_node, g_Value, List_Map[i].dcb_data[j].consum_times);

				for (uint16_t k = 0; k < List_Map[i].dcb_data[j].data_len; k++)
					DmaPrintf("channel %d, node %d, counts %d, buf[%d] value %d, times %fms.\r\n",
							  List_Map[i].id, List_Map[i].current_node,  g_Value, k, List_Map[i].dcb_data[j].buf[k], List_Map[i].dcb_data[j].consum_times);

				g_Value = 0;
#else
				DmaPrintf("length %d\r\n", List_Map[i].dcb_data[j].data_len);
				/*Draw waveform*/
				Dwin_Curve_SchMd(&List_Map[i], j);
#endif
				/*Reset data processing completion flag*/
				List_Map[i].dcb_data[j].data_flag = false;
				/*Clear data length*/
				List_Map[i].dcb_data[j].data_len = 0;
				/*Clearing time consumption*/
				List_Map[i].dcb_data[j].consum_times = 0;
			}
		}
	}
}

/**
 * @brief  链表初始化
 * @param  list 首节点指针
 * @retval None
 */
// void Init_List(Dwin_List *list, uint8_t channel_id)
//{
//	uint8_t i = 0;
//	Dwin_List *p = NULL;
//	list->dcb_data.id = channel_id;
////	list->next = NULL;
//	list->dcb_data.data_flag = false;
//	list->dcb_data.data_len = 0;
//
//	for(i = 1; i < LISTNODE_SIZE; i++)
//	{
//		p = (Dwin_List *)pvPortMalloc(sizeof(Dwin_List));
//		p->dcb_data.id = channel_id;
//		p->dcb_data.data_flag = false;
//		p->dcb_data.data_len = 0;
//		list->next = p;
//		list = list->next;
//	}
//}

void Init_List(Dwin_List *list, uint8_t channel_id)
{
	uint16_t i = 0;

	for (; i < LISTNODE_SIZE; i++)
	{
		list->current_node = 0;
		list->current_edge = Falling_Edge;
		list->dcb_data[i].overtimes = 10U;
		list->id = channel_id;
		list->dcb_data[i].first_flag = false;
		list->dcb_data[i].first_value = 0;
		list->dcb_data[i].data_flag = false;
		list->dcb_data[i].timer_synflag = false;
		list->dcb_data[i].data_len = 0;
		list->dcb_data[i].overflows_num = 0;
		list->dcb_data[i].consum_times = 0.0F;
		list->dcb_data[i].error_value = 0.0F;
	}
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_SendWithCRC
*	功能说明: 带CRC的发送从站数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen)
{
	uint16_t crc;
	uint8_t buf[200];

	memcpy(buf, _pBuf, _ucLen);
	crc = Get_Crc16(_pBuf, _ucLen, 0xffff);
	buf[_ucLen++] = crc;
	buf[_ucLen++] = crc >> 8;

	// HAL_UART_Transmit(&huart1, buf, _ucLen, 0xffff);
	HAL_UART_Transmit_DMA(&huart1, buf, _ucLen);
	/*等待串口接收完成期间，进行系统调度*/
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
	{
		osDelay(1);
	}
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_Send
*	功能说明: 发送数据帧
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_Send(uint8_t *_pBuf, uint8_t _ucLen)
{
	/*Clear data in DCache*/
  	SCB_CleanDCache();
	// HAL_UART_Transmit(&huart1, _pBuf, _ucLen, 0x1000);
	HAL_UART_Transmit_DMA(&huart1, _pBuf, _ucLen);
	/*等待串口接收完成期间，进行系统调度*/
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
	{
		osDelay(1);
	}
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_AnalyzeApp
*	功能说明: 接收处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_AnalyzeApp(void)
{
	uint8_t cmd = g_Dwin.RxBuf[3];
	switch (cmd)
	{
	case READ_CMD:
	{
		DWIN_83H();
		break;
	}
	default:
		break;
	}
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_ReciveNew
*	功能说明: 接收新数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_ReciveNew(uint8_t *rxBuf, uint16_t Len)
{
	uint16_t i;
	for (i = 0; i < Len; i++)
	{
		g_Dwin.RxBuf[i] = rxBuf[i];
	}
	g_Dwin.RxCount = Len;
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_Init
*	功能说明: 初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_Init(void)
{
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_Poll
*	功能说明: 判断数据帧是否正确
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_Poll(void)
{
	if (g_Dwin.RxBuf[0] != 0x5A)
	{
		goto err_ret;
	}
	if (g_Dwin.RxBuf[1] != 0xA5)
	{
		goto err_ret;
	}

	DWIN_AnalyzeApp();

err_ret:
	g_Dwin.RxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_WRITE
*	功能说明: 发送写数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_WRITE(uint16_t slaveaddr, uint8_t *dat, uint8_t length)
{
	uint8_t i;
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = length + 3;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = WRITE_CMD;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = slaveaddr >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = slaveaddr;

	for (i = 0; i < length; i++)
	{
		g_Dwin.TxBuf[g_Dwin.TxCount++] = dat[i];
	}
	DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_READ
*	功能说明: 发送读数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_READ(uint16_t slaveaddr, uint8_t words)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x04;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = READ_CMD;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = slaveaddr >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = slaveaddr;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = words;

	DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_83H
*	功能说明: 读数据处理
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_83H(void)
{
	uint16_t Addr = ((uint16_t)g_Dwin.RxBuf[4] << 8) | g_Dwin.RxBuf[5];

	uint8_t i;
	uint8_t Payloadbuff[100]; //有效数据缓冲区
	uint8_t PayloadLength;	  //有效数据长度

	PayloadLength = g_Dwin.RxBuf[6] * 2;

	memcpy(Payloadbuff, &g_Dwin.RxBuf[7], PayloadLength);

	for (i = 0; i < mapindex + 1; i++)
	{
		if (Addr == g_map[i].addr)
		{
			// HAL_UART_Transmit(&huart1,Payloadbuff,PayloadLength,0xffff);

			g_map[i].event(Payloadbuff, PayloadLength);
			break;
		}
	}
}

/**
 * @brief  进行页面切换处理
 * @param  Page 页
 * @example 5A A5 07 82 00 84 5A 01 00 01   切到第一页
 * @retval None
 */
void DWIN_PageChange(uint16_t Page)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x07;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x84;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Page >> 8;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Page;

	DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_TouchAction
*	功能说明: 进行一次触摸动作
*	形    参:	 TouchType:触摸类型     Pos_x  x坐标    Pos_y  y坐标
*	返 回 值: 无
*	示例：5A A5 0B 82 00 D4 5A A5 00 04 00EE 008F
*********************************************************************************************************
*/
void DWIN_TouchAction(TouchType type, uint16_t Pos_x, uint16_t Pos_y)
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
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x0B;
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

	DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
}

/**
 * @brief  发送曲线点
 * @param  Channel 通道号
 * @param  *dat    数据源头指针
 * @param  length  数据长度
 * @retval None
 */
void DWIN_CURVE(uint16_t Channel, uint16_t *dat, uint16_t length)
{
	uint8_t i = 0;

	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = length * 2U + 9U;
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
	DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_CURVE_MULTICHANNEL
*	功能说明: 曲线显示多通道，但是只能是多通道一个数据
*	形    参: 5A A5 13 82 03 10 5A A5 03 00 02 01 F9 07 01 01 F7 07 00 01 00 00
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_CURVE_MULTICHANNEL(uint16_t Channelnum, DwinCurve *dat)
{
	uint8_t i = 0;

	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = Channelnum * 4 + 7U; //要计算一下
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
	DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
}

/**
 * @brief  显示单个通道上一组数据
 * @param  list 当前链表首节点指针
 * @retval None
 */
void Dwin_Curve_SchMd(Dwin_List *list, uint16_t node)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 9U + list->dcb_data[node].data_len * 2U;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x03; /*通道起始地址*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x10;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01; /*通道块数*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00; /*低字节无效*/
	g_Dwin.TxBuf[g_Dwin.TxCount++] = list->id;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = list->dcb_data[node].data_len;

	for (uint16_t j = 0; j < list->dcb_data[node].data_len; j++)
	{
#if (USING_LITTLE == 1)
		/*先对数据进行转换*/
		// Endian_Swap((uint8_t *)&list->dcb_data[node].buf[j], 0, sizeof(uint16_t));
		g_Dwin.TxBuf[g_Dwin.TxCount++] = list->dcb_data[node].buf[j];
		g_Dwin.TxBuf[g_Dwin.TxCount++] = list->dcb_data[node].buf[j] >> 8U;
#else
		g_Dwin.TxBuf[g_Dwin.TxCount++] = list->dcb_data[node].buf[j] >> 8U;
		g_Dwin.TxBuf[g_Dwin.TxCount++] = list->dcb_data[node].buf[j];
#endif
	}
	// DmaPrintf("%s\r\n", g_Dwin.TxBuf);
	// DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
	// DmaPrintf("length %d\r\n", g_Dwin.TxCount);
	SCB_CleanDCache();
	// HAL_UART_Transmit_DMA(&huart1, g_Dwin.TxBuf, g_Dwin.TxCount);
	HAL_UART_Transmit(&huart1 , g_Dwin.TxBuf, g_Dwin.TxCount, 0xFFFF);
	// for(uint16_t i = 0; i < g_Dwin.TxCount; i++)
	// 	DmaPrintf("%d\t", g_Dwin.TxBuf[i]);
	
}

/**
 * @brief  清除曲线
 * @param  Channel 通道号
 * @retval None
 */
void DWIN_CURVE_CLEAR(uint16_t Channel)
{
	g_Dwin.TxCount = 0;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x5A;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0xA5;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x05;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x82;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x03;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x01 + Channel * 2U;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;
	g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;

	DWIN_Send(g_Dwin.TxBuf, g_Dwin.TxCount);
}

/*
*********************************************************************************************************
*	函 数 名:  DWIN_InportMap
*	功能说明: 导入事件映射
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DWIN_InportMap(uint32_t addr, pfunc event)
{
	g_map[mapindex].addr = addr;
	g_map[mapindex].event = event;
	mapindex++;
}

/**
 * @brief  取得16bitCRC校验码
 * @param  ptr   当前数据串指针
 * @param  length  数据长度
 * @param  init_dat 校验所用的初始数据
 * @retval 16bit校验码
 */
uint16_t Get_Crc16(uint8_t *ptr, uint8_t length, uint16_t init_dat)
{
	uint8_t i = 0;
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
