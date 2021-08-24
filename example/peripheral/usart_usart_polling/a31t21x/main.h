/**
 *******************************************************************************
 * @file        main.h
 * @brief       Application common
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/


#ifndef _MAIN_H_
#define _MAIN_H_

#include "a31t21x.h"
#include "a31t21x_gpio.h"
#include "a31t21x_scu.h"
#include "a31t21x_uart.h"
#include "a31t21x_usart1n.h"

#define APP_DEVICE                      "A31T21X"
#define APP_CORE                        "ARM Cortex-M0+"
#define APP_UART_ID                     0
#define APP_UART_BAUD                   9600

#define APP_USART_CONFIG														\
    {																								\
				.mode = USART_MODE_SYNC,					  				\
        .baudrate = 115200,													\
        .data_bit = USART_DATA_BIT_8,   	 				  \
        .parity_bit = USART_PARITY_BIT_NONE,				\
        .stop_bit = USART_STOP_BIT_1,      				  \
        .sync_mode = USART_SYHC_MODE_CPOL_0_CPHA_0,	\
        .master_sel = USART_SEL_MASTER,      			  \
        .irq_prio = DRIVER_PRIORITY_LOWEST, 				\
    };


void Port_Init(void);
void Port_Configure_CLKO(void);
void Port_Configure_USART(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

