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

#define RX_BUF_SIZE	 128U
#define TX_BUF_SIZE  5120U

#define WRITE_CMD	 0x82		//写
#define READ_CMD	 0x83		//读

#define PAGE_CHANGE_CMD  0x84	//页面切换
#define TOUCH_CMD	     0xD4   //触摸动作

typedef void (*pfunc)(uint8_t*,uint8_t);

typedef enum
{
	press,			//按下
	uplift,			//抬起
	longpress,		//长按
	press_uplift,	//按下和抬起
}TouchType;

typedef struct
{
	uint8_t RxBuf[RX_BUF_SIZE];
	uint32_t RxCount;

	uint8_t TxBuf[TX_BUF_SIZE];
	uint32_t TxCount;
}Dwin_T;

typedef struct
{
	uint32_t addr;
	pfunc event;
}DwinMap;

typedef struct
{
	uint8_t channel;
	uint16_t val;
}DwinCurve;

typedef enum
{
	D0

}Listx;




extern Dwin_T g_Dwin;
extern DwinMap g_map[100];

extern Dwin_List List_Map[LIST_SIZE];

void DWIN_WRITE(uint16_t slaveaddr,uint8_t* dat,uint8_t length);
void DWIN_READ(uint16_t slaveaddr,uint8_t words);
void DWIN_CURVE(uint16_t Channel,uint16_t* dat,uint16_t num);
void DWIN_CURVE_CLEAR(uint16_t Channel);
void DWIN_CURVE_MULTICHANNEL(uint16_t Channelnum,DwinCurve* dat);
void Dwin_Curve_SchMd(Dwin_List *list, uint16_t node);

void DWIN_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen);
void DWIN_Send(uint8_t *_pBuf, uint8_t _ucLen);
uint16_t Get_Crc16(uint8_t *ptr, uint8_t length, uint16_t IniDat);
void Endian_Swap(uint8_t *pData, uint8_t start, uint8_t length);

void DWIN_AnalyzeApp(void);
void DWIN_ReciveNew(uint8_t * rxBuf,uint16_t Len);
void DWIN_InportMap(uint32_t addr,pfunc event);
void DWIN_Init(void);
void DWIN_Poll(void);

void DWIN_83H(void);
void DWIN_PageChange(uint16_t Page);
void DWIN_TouchAction(TouchType type,uint16_t Pos_x,uint16_t Pos_y);

void Wave_Handle(void);
void Init_List(Dwin_List *list, uint8_t channel_id);

#endif /* INC_DWIN_H_ */
