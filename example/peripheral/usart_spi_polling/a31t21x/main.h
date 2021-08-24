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

#define APP_USART_SS_MANUAL_ENABLE        1

#define APP_USART_SPI_CONFIG                    	  \
    {                                            		\
        .mode = USART_MODE_SPI,          	      	  \
        .baudrate = 1000000,			                  \
        .data_bit = USART_DATA_BIT_8,            		\
        .bit_order = USART_BIT_ORDER_MSB_FIRST, 	  \
        .sync_mode = USART_SYHC_MODE_CPOL_1_CPHA_1, \
        .ss_type = USART_SS_MANUAL,									\
				.master_sel = USART_SEL_MASTER,							\
        .irq_prio = DRIVER_PRIORITY_LOWEST,     	  \
    };

void Port_Init(void);
void Port_Configure_CLKO(void);
void Port_Configure_UART(void);
void Port_Configure_USART(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

