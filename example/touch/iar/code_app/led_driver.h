
#include "common.h"

#ifndef __LEDDRV_H__
#define __LEDDRV_H__

#define LED_LIB_VER 	1	/* 201126 */

#define LED_INIT_DATA	0xFFFF
#define SEG(x)	((u16)1<<x)
#define COM(x)	((u8)1<<x)
//---------------


#define APP_LED_CONFIG												\
	{																					\
		.tPRESD 	= 15,												\
				.tCOMPWID	= 99,												\
				.tLEDPD 	= 1000, 										\
				.tOVERLAP	= 0,												\
				.tSR_INTE	= 1,												\
				.tMODE		= LED_AUTO_MODE,						\
				.clk_sel	= LED_CLK_MCCLK,						\
				.irq_prio	= DRIVER_PRIORITY_LOWEST,		\
	}



typedef struct {
	u16	com_sel;
	u16 seg_sel;
	u8  com_cnt; 
	u8  slot_ms;
	u8  period;
	struct{
		u8 seg_current	:2;
		u8 op_mode	:3;
		u8 rsrvd	:3;
	}flag;
} str_ledx;


extern str_ledx led;
extern void (*fpRenewLED)(void);

void LED_Do_Task(void);
void LED_Set_Actv_Time(u8 comCnt, u8 slotMs, u8 period);
void LED_Set_Current(u8 currLev);
void LedDriver_ISR(void);


#endif	// __LEDDRV_H__


