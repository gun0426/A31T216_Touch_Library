

/** *******************************************************************
 * @file		 debug.c
 * @brief		 : USI0-UART/500000bps
				 : DBG_Send_Data() protocol
					 - 3A(STX)
					 - xx(LEN): MODE ~ MSG
					 - 01 10 (MODE)
					 - xx(KEY)
					 - 00(Cycle)
					 - MSG
					 - xx xx(CheckSum)
					 - 0D 0A(CR/LF)
				 : DBG_RX_Check() protocol
					 - 02(STX)
					 - xx(CMD) : 'R'(Get), 'M'(Frequency), 'T'(write), 'w', 'S'
					 - MSG
					 - 03(ETX)
 * @author	 ABOV Sensor Solution Team
 *
 * Copyright(C) 2020, ABOV Semiconductor
 * All rights reserved.
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "A31T21x.h"
#include "a31t21x_uart.h"
#include "A31T21x_usart1n.h"
#include "A31T21x_gpio.h"
#include "debug.h"
#include "a31t21x_driver_common.h"
#include "common.h"
#include "touch.h"
#include "user.h"

#define DBG_TS_REG	1


void app_uart_irq_handler(uint32_t event, void *context);
uint8_t app_uart_rx_buf[8];
uint8_t app_uart_tx_buf[8];
static volatile uint8_t app_uart_tx_done;
static volatile uint8_t app_uart_rx_done;

struct dbg_cfg dbg;
struct dbg_uart uart;
static u8 s8_step = 0;
static u8 s8_buff[DBG_RX_MSG_SIZE];
static u8 s8_index = 0;

/** *******************************************************************
 * @brief		UART Interrupt call back
 * @param[in]	
 * @return
 *
 *********************************************************************/
void app_uart_irq_handler(uint32_t event, void *context)
{
	if (event & UART_EVENT_RECEIVE_DONE)
	{
		app_uart_rx_done = 1;
		uart.rx_queue[uart.rx_write_idx++] = app_uart_rx_buf[0];

		/* Additional Receive */
		app_uart_rx_done = 0;
		HAL_UART_Receive(UART_ID_1, app_uart_rx_buf, 1, UART_IO_TYPE_NON_BLOCK);
	}

	if (event & UART_EVENT_TRANSMIT_DONE)
	{
		app_uart_tx_done = 1;
		dbg.flag.busy = 0;
	}
}
/** *******************************************************************
 * @brief		DBG_Set_Baudrate
 * @param[in]	com_cnt : number of COMs to use
				duration : LED operation section in milli second.
 * @return		void
 *
 *********************************************************************/
void DBG_Set_Baudrate(u32 dbgBaud)
{
#if (0)
    uint32_t numerator;
    uint32_t denominator;
    uint32_t bdr, bfr;
    uint32_t fd;
	uint32_t cal_index1, cal_index2;

	/* Set Baudrate */
    numerator = SystemCoreClock / 2;
    denominator = 16 * dbgBaud;

	cal_index1 = (numerator / (denominator / 10));
	cal_index2 = (numerator / denominator) * 10;
	if ((cal_index1 - cal_index2) < 6)
	{
		bdr = (numerator / denominator);
	}
	else
	{
		bdr = (numerator / denominator) + 1;
	}

    fd = numerator - (bdr * denominator);
    bfr = (fd * 256) / denominator;

    UART1->BDR = bdr & 0xFFFF;
    UART1->BFR = bfr & 0xFF;    

	t_sysbit.dbg_en = 1;
#else
	dbg.baud = dbgBaud;
	t_sysbit.dbg_en = 1;
#endif
}
#if (0)
/***************************************** printf start *******************************************/
#ifdef __GNUC__
	/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf  
	 set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	#if (1)	// 테스트 필요 
	//while (!(UART1->LSR & UART_LSR_THRE)){}
	/////HAL_UART_WriteBuffer(UART1, ch);	
	UART1->THR = ch;
	#endif

	return ch;
}
#endif
/*************************************************************
 - FUNCTION & CLASS NAME
	: void db_InitReg(void) 	USART1
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void DBG_Init_UART(void) 	// ##
{
	struct uart_cfg cfg;

    /* Configure UART TXD pin to be used */
    HAL_GPIO_SetFunction(PORTF, PIN4, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTF, PIN4, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTF, PIN4, GPIO_PULL_UP);

    /* Configure UART RXD pin to be used */
    HAL_GPIO_SetFunction(PORTF, PIN5, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTF, PIN5, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTF, PIN5, GPIO_PULL_UP);

//	cfg.baudrate = BAUD_RATE;
cfg.baudrate = dbg.baud;
	cfg.data_bit = UART_DATA_BIT_8,
	cfg.parity_bit = UART_PARITY_BIT_NONE,
	cfg.stop_bit = UART_STOP_BIT_1,
	cfg.irq_prio = DRIVER_PRIORITY_LOWEST,

	HAL_UART_Init(UART_ID_1, &cfg, app_uart_irq_handler, NULL);
	HAL_UART_Receive(UART_ID_1, app_uart_rx_buf, 1, UART_IO_TYPE_NON_BLOCK);
	__enable_irq();
}
/***********************************************************************
	- Function Name
		: 
	- Description
		:
	- Parameter & Return
		:
	- Tracebility
		:
***********************************************************************/
u8 DBG_Init_UART_Param (void)
{
	u8 i;

	s8_step = DBG_RX_STEP_STX;
	s8_index = 0;
	uart.rx_write_idx = 0;
	uart.rx_read_idx = 0; 
	for (i = 0; i < DBG_RX_BUF_SIZE; i++)
	{
		uart.rx_queue[i] = 0;
	}
	for (i = 0; i < DBG_RX_MSG_SIZE; i++)
	{
		s8_buff[i] = 0;
	}
	dbg.flag.busy = 0;

	return (1);
}
/*************************************************************
 - FUNCTION & CLASS NAME
	u8 Hexstr_To_Binary(u8* mp_str)
 - DESCRIPTION	
	
 - PARAMETER & RETURN
	return : void
	input : 
 - TRACEBILITY
	
*************************************************************/
u8 Hexstr_To_Binary(u8* mp_str)
{
	u8 i;
	u8 temp = 0;

	for (i = 0; i < 2; i++)
	{
		temp <<= 4;
		
		if ((*mp_str) >= '0' && (*mp_str) <= '9')
		{
			temp |= ((*mp_str) - '0');
		}
		else if ((*mp_str) >= 'a' && (*mp_str) <= 'f')
		{
			temp |= ((*mp_str) + 10 - 'a');
		}
		else
		{
			return 0xff;
		}
		mp_str++;
	}

	return temp;
}
/***********************************************************************
	- Function Name
		: comm_do_rxjob_tb
	- Description
		: 
	- Parameter & Return
		:
	- Tracebility
		:
***********************************************************************/
//#define DATA_LEN	8		// | 02 | 52 30 31 30 30 30 32 0D | 03 |
u8 DBG_RX_Check(u8** ptr)
{
	u8 temp = 0;
	u8 result;

	result = DBG_RX_READY;

	while (uart.rx_write_idx != uart.rx_read_idx)	
	{
		temp = uart.rx_queue[uart.rx_read_idx++]; 
		uart.rx_read_idx %= DBG_RX_BUF_SIZE;
		
		switch (s8_step)
		{
			case DBG_RX_STEP_STX :						/* STX */
				if (temp == DBG_SMARTKEY_SET_STX)
				{
					s8_index = 0;
					s8_step = DBG_RX_STEP_DATA;
				}
				break;						
			case DBG_RX_STEP_DATA : 						/* MSG */
				s8_buff[s8_index++] = temp;
				if (temp == DBG_SMARTKEY_SET_ETX)
				{
					s8_step = DBG_RX_STEP_STX;
					result = DBG_RX_COMPLETE;
					*ptr = &s8_buff[0]; 				// from cmd exclude len
				}
				break;
			default :
				s8_step = DBG_RX_STEP_STX;
				break;
		}	

		if (result != DBG_RX_READY)
		{
			break;
		}
	}

	return (result);
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void DBG_Init_Variable(void)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void DBG_Init_Variable(void)
{	
	dbg.start_idx	= DBG_START_IDX;
	dbg.ch_cnt 		= DBG_CH_CNT;
	dbg.mask 		= DBG_MSK_ITEM; 
}
void Make_Packet_Byte(u8 dat)
{
	uart.tx_queue[uart.tx_write_idx++] = dat;
	uart.tx_checksum += (u16)(0x00ff & dat);	
}
void Make_Packet_Word(u16 dat)
{
	uart.tx_queue[uart.tx_write_idx++] = (u8)(dat>>8);
	uart.tx_queue[uart.tx_write_idx++] = (u8)(dat>>0);
	uart.tx_checksum += (u16)(0x00ff & (dat>>8));
	uart.tx_checksum += (u16)(0x00ff & (dat>>0));
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void DBG_Send_Data(void)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void DBG_Send_Data(void)
{
	u8 i;
	u8 len;			/* Keil:Optimization - Level 7이상에서 i 와 충돌 */
	u8 shift_bit;
	u8 mask_cnt;

	uart.tx_write_idx = 0;
	uart.tx_read_idx = 0;
	uart.tx_checksum = 0;
	uart.tx_queue[uart.tx_write_idx++] = DBG_SMARTKEY_GET_STX;	/* header[1] */

	/* Length Calc */					
	mask_cnt = 0;
	shift_bit = 0x01;
	for (i = 0; i < 8; i++)
	{
		if(dbg.mask & shift_bit)
		{
			mask_cnt++;
		}
		shift_bit<<=1;
	}

	#if (DBG_TS_REG == 1)
	len = 4+dbg.ch_cnt*mask_cnt*2+(48+38); 
	#else
	len = 4+dbg.ch_cnt*mask_cnt*2;
	#endif
	len = (len/2)|0x80;		
	Make_Packet_Byte(len);						/* length[1] */
	Make_Packet_Byte(0x00);						/* mode[2] */
	Make_Packet_Byte(0x10);
	Make_Packet_Byte(ts.detect_key);			/* key[1] */
//	Make_Packet_Byte(ts.key_num);
	Make_Packet_Byte(0x00);						/* cyle[1] */

	for (i = dbg.start_idx; i < dbg.start_idx+dbg.ch_cnt; i++) 	
	{	
		if (dbg.mask & DBG_SMARTKEY_SUM)				
		{
			Make_Packet_Word(user.raw_data[i][0]);				
		}
		if (dbg.mask & DBG_SMARTKEY_REL)				
		{				
			Make_Packet_Word(user.base_data[i][0]);
		}
		if (dbg.mask & DBG_SMARTKEY_BASE) 			
		{
			Make_Packet_Word(user.raw_data[i][1]);
		}
		if (dbg.mask & DBG_SMARTKEY_DELTA)				
		{
			Make_Packet_Word(user.base_data[i][1]);
		}
		if (dbg.mask & DBG_SMARTKEY_PER)			
		{			
			Make_Packet_Word(abs(user.del_data[i]));
		}
		if (dbg.mask & DBG_SMARTKEY_FREQ0)
		{	
			Make_Packet_Word(user.ch_thd[i]);
		}
		if (dbg.mask & DBG_SMARTKEY_FREQ1)
		{			
			Make_Packet_Word(user.ch_thd[i]*ts.noise_thd_rate/100);
		}
		if (dbg.mask & DBG_SMARTKEY_FREQ2)
		{			
			Make_Packet_Word(user.ch_thd[i]*ts.basehold_rate/100);
		}				
	}

	#if (DBG_TS_REG == 1)
	/* SCO */
	for (i = 0; i < TS_MAX_KEY; i++)		/* 24 * 2 */
	{
		Make_Packet_Word(ts_reg.SCO[i]);
	}
	/* touch Register */
	for (i = 0; i < SFR_TS_MAX+2; i++)		/* (17+2)*2 */	
	{
		Make_Packet_Word(ts_reg.tsReg[i]);	
	}
	#endif

	uart.tx_checksum = ((~uart.tx_checksum)&0x00ffff)-1;	
	uart.tx_queue[uart.tx_write_idx++] = (u8)(uart.tx_checksum>>8);
	uart.tx_queue[uart.tx_write_idx++] = (u8)(uart.tx_checksum>>0);
	uart.tx_queue[uart.tx_write_idx++] = DBG_SMARTKEY_GET_CR;
	uart.tx_queue[uart.tx_write_idx++] = DBG_SMARTKEY_GET_LF;

	HAL_UART_Transmit(UART_ID_1, uart.tx_queue, uart.tx_write_idx, UART_IO_TYPE_NON_BLOCK);  
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void Init_UART(void)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void DBG_Do_Task(void)
{
	static u8 stStep = 0;
	u8* ptr;
	u8 temp;
	u8 bin1;
	u8 bin2;
	u8 bin3;
	
	if (t_sysbit.dbg_en == 0)
	{
		return;
	}
	
	switch (stStep)
	{
		case 0 :
			DBG_Init_UART();
			DBG_Init_UART_Param();
			DBG_Init_Variable();
			stStep++;
			break;
		case 1 :
			temp = DBG_RX_Check(&ptr);	// |02| ~ |0D|03|	
			if (temp == DBG_RX_COMPLETE)
			{
				switch (*ptr)
				{
					case 'R':	/* 0x52 */
						dbg.mask 		= Hexstr_To_Binary(ptr+1);				
						dbg.start_idx	= Hexstr_To_Binary(ptr+3);	
						dbg.ch_cnt 		= Hexstr_To_Binary(ptr+5);	//printf("mask=%02x, start_idx=%02x, ch_cnt=%02x \r\n", (u16)dbg.mask, (u16)dbg.start_idx, (u16)dbg.ch_cnt);	
						break;
					#if (0)
					case 'M':	// 0x4D FREQ
						temp = Hexstr_To_Binary(ptr+1);						
						switch (temp)
						{
							case 0x10 :
								//g8_sfrIndx = 0;
								break;
							case 0x11 :
								//g8_sfrIndx = 1;
								break;
							case 0x12 :
								//g8_sfrIndx = 2;
								break;
						}	
						break;
					#endif						
					case 'T':	/* 0x54 : SFR */
						bin1 = Hexstr_To_Binary(ptr+1);	//
						bin2 = Hexstr_To_Binary(ptr+3);	
						bin3 = Hexstr_To_Binary(ptr+5);
						if ((bin1>=0x50)&&(bin1<=0x77))			/* SCO */
						{
							bin1 -= 0x50;
							bin1 = bin1/2;
							ts_reg.SCO[bin1] =  (((unsigned int)bin2<<8) + bin3)&0x1fff; 
			          		TS_SCO(bin1) = ts_reg.SCO[bin1]; 
						}
						else if ((bin1>=0x80)&&(bin1<=0x91))	/* REG */
						{
							bin1 -= 0x80;
							ts_reg.tsReg[bin1] = bin2;
							TS(bin1) = ts_reg.tsReg[bin1];
						}
						break;
					case 'w':	
						bin1 = Hexstr_To_Binary(ptr+1);
						bin2 = Hexstr_To_Binary(ptr+3);							
						t_sysbit.reinit = 1;
						break;
					case 'Z':
						break;
					default :
						break;
				}
			}
			stStep++;
			break;
		case 2 :
			if ((dbg.flag.busy == 0)&&(t_sysbit.dbg_trigger == 1))
			{			
				t_sysbit.dbg_trigger = 0;					
				dbg.flag.busy = 1;
				DBG_Send_Data();
			}			
			stStep = 1;
			break;
		default :
			stStep = 0;
			break;
	}
}

