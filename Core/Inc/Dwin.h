/*
 * Dwin.h
 *
 *  Created on: Nov 19, 2020
 *      Author: play
 */

#ifndef INC_DWIN_H_
#define INC_DWIN_H_

#include "main.h"

/*STC使用大端模式存储数据，不需要交换数据；STM32使用小端模式存储数据*/
#define USING_LITTLE 1
/*数据是否启用CRC检验*/
#define USING_CRC 1
/*CRC校验使用查表方式*/
#define USING_CRC_TABLE 1

#define RX_BUF_SIZE 128U
#define TX_BUF_SIZE 1024U
/*写*/
#define WRITE_CMD 0x82
/*读*/
#define READ_CMD 0x83
/*页面切换*/
#define PAGE_CHANGE_CMD 0x84
/*触摸动作*/
#define TOUCH_CMD 0xD4
/*时耗变量显示地址*/
#define TIMECOUNSUM_ADDR 0x4000
/*历史记录变量显示地址*/
#define RECORDS_ADDR 0x4030

/*半波形通道的长度*/
#define WAVE_CH_WIDTH 295U
/*波形通道的高度*/
#define WAVE_CH_HEIGHT 100U
/*量化比例*/
#define RATIO() (65535U / (WAVE_CH_HEIGHT - 20U))
/*单个数据块尺寸*/
#define SINGLE_SIZE 100U
/*数据帧头和帧尾固定长度*/
#define FIXED_SIZE 20U

#define GARTHER_FRAME(value)                            \
	do                                                  \
	{                                                   \
		for (uint16_t i = 0; i < FIXED_SIZE; i++)       \
		{                                               \
			if (i % 2)                                  \
			{                                           \
				g_Dwin.TxBuf[g_Dwin.TxCount++] = value; \
			}                                           \
			else                                        \
			{                                           \
				g_Dwin.TxBuf[g_Dwin.TxCount++] = 0x00;  \
			}                                           \
		}                                               \
	} while (0)

typedef void (*pfunc)(uint8_t *, uint8_t);

typedef enum
{
	press,		  //按下
	uplift,		  //抬起
	longpress,	  //长按
	press_uplift, //按下和抬起
} TouchType;

typedef struct
{
	uint8_t RxBuf[RX_BUF_SIZE];
	uint32_t RxCount;

	uint8_t TxBuf[TX_BUF_SIZE];
	uint32_t TxCount;
} Dwin_T;

typedef struct
{
	uint32_t addr;
	pfunc event;
} DwinMap;

typedef struct
{
	uint8_t channel;
	uint16_t val;
} DwinCurve;

extern Dwin_T g_Dwin;
extern DwinMap g_map[100];

extern Dwin_List List_Map[LIST_SIZE];

void Dwin_Write(uint16_t start_addr, uint8_t *dat, uint16_t length);
void Dwin_Read(uint16_t slaveaddr, uint16_t words);
void Dwin_Curve(uint16_t Channel, uint16_t *dat, uint16_t length);
void Dwin_Curve_Clear(uint16_t Channel);
void Dwin_Curve_MuitiChannel(uint16_t Channelnum, DwinCurve *dat);
void Dwin_Curve_SchMd(Dwin_List *list);

void Dwin_SendWithCRC(uint8_t *_pBuf, uint16_t _ucLen);
void Dwin_Send(uint8_t *_pBuf, uint16_t _ucLen);
uint16_t Get_Crc16(uint8_t *ptr, uint16_t length, uint16_t IniDat);
void Endian_Swap(uint8_t *pData, uint8_t start, uint8_t length);

void DWIN_AnalyzeApp(void);
void DWIN_ReciveNew(uint8_t *rxBuf, uint16_t Len);
void DWIN_InportMap(uint32_t addr, pfunc event);
void DWIN_Init(void);
void DWIN_Poll(void);

void Dwin_83H(void);
void Dwin_PageChange(uint16_t Page);
void Dwin_TouchAction(TouchType type, uint16_t Pos_x, uint16_t Pos_y);

bool Wave_Handle(void);
void Init_List(Dwin_List *list, uint8_t channel_id);
void Report_TimeConsum(void);

#endif /* INC_DWIN_H_ */
