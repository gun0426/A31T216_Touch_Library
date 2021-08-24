/** *******************************************************************
 * @file		debug.h
 * @brief		
 * @author		ABOV Sensor Solution Team
 *
 * Copyright(C) 2020, ABOV Semiconductor
 * All rights reserved.
 *
 **********************************************************************/
#include "common.h"

#ifndef __DEBUG_H__
#define __DEBUG_H__

#define BAUD_RATE		115200	//115200 //9600

enum dbg_smartkey
{
	DBG_SMARTKEY_SUM	= 0x01,
	DBG_SMARTKEY_REL	= 0x02,
	DBG_SMARTKEY_BASE	= 0x04,
	DBG_SMARTKEY_DELTA	= 0x08,
	DBG_SMARTKEY_PER	= 0x10,
	DBG_SMARTKEY_FREQ0	= 0x20,
	DBG_SMARTKEY_FREQ1	= 0x40,
	DBG_SMARTKEY_FREQ2	= 0x80
};
#define DBG_MSK_ITEM	(DBG_SMARTKEY_SUM+DBG_SMARTKEY_REL+DBG_SMARTKEY_BASE+DBG_SMARTKEY_DELTA)
#define DBG_START_IDX	0
#define DBG_CH_CNT		4
struct dbg_cfg
{
	u8 start_idx;
	u8 ch_cnt;
	u8 mask;
	u32 baud;
	struct{
		u8 busy    	:1;
		u8 fla_dbg	:1;
		u8 rsvd    	:6;
	} flag;
} ;

#define DBG_SMARTKEY_GET_STX	0x3A
#define DBG_SMARTKEY_GET_CR		0x0D
#define DBG_SMARTKEY_GET_LF		0x0A
#define DBG_SMARTKEY_SET_STX	0x02
#define DBG_SMARTKEY_SET_ETX	0x03
#define DBG_RX_BUF_SIZE			100
#define DBG_TX_BUF_SIZE			200
#define DBG_RX_MSG_SIZE 		25
struct dbg_uart
{	
	u16 rx_write_idx;
	u16 rx_read_idx;
	u8  rx_queue[DBG_RX_BUF_SIZE];
	u16 tx_write_idx;
	u16 tx_read_idx;
	u16 tx_checksum;
	u8  tx_queue[DBG_TX_BUF_SIZE];
};
enum dbg_rx_step
{
	DBG_RX_STEP_STX = 0,
	DBG_RX_STEP_CMD,
	DBG_RX_STEP_LENGTH,
	DBG_RX_STEP_DATA,
	DBG_RX_STEP_CHKSUM,
	DBG_RX_STEP_ETX,
	DBG_RX_STEP_ERR = 0xff
};
#define DBG_RX_READY 		0
#define DBG_RX_COMPLETE		1
#define DBG_RX_ERR			2

extern struct dbg_cfg dbg;
extern struct dbg_uart uart;
void DBG_Do_Task(void);
void Make_Packet_Byte(u8 dat);
void Make_Packet_Word(u16 dat);
void DBG_Set_Baudrate(u32 dbgBaud);
void USI0RX_ISR(void);
void USI0TX_ISR(void);

/* I2C */
#define I2C_MY_ADDR 0x84
#define I2CBUFFERSIZE	250
extern volatile unsigned char I2C_Buffer[I2CBUFFERSIZE];


#endif	// __DEBUG_H__
