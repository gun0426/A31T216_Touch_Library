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
#include "a31t21x_spi.h"
#include "a31t21x_gpio.h"
#include "a31t21x_scu.h"
#include "a31t21x_uart.h"

#define APP_DEVICE                      "A31T21X"
#define APP_CORE                        "ARM Cortex-M0+"
#define APP_UART_ID                     0
#define APP_UART_BAUD                   9600

#define APP_SPI_DMA_ENABLE              1
#define APP_SPI_SS_MANUAL_ENABLE        1

#define APP_SPI_CONFIG                            \
    {                                             \
        .mode = SPI_MODE_MASTER,                  \
        .freq = 1000000,			                    \
        .bit_order = SPI_BIT_ORDER_MSB_FIRST,     \
        .sync_mode = SPI_SYHC_MODE_CPOL_0_CPHA_0, \
        .data_bit = SPI_DATA_BIT_8,               \
        .ss_type = SPI_SS_MANUAL,									\
        .irq_prio = DRIVER_PRIORITY_LOWEST,       \
    };

void Port_Init(void);
void Port_Configure_CLKO(void);
void Port_Configure_UART(void);
void Port_Configure_SPI2n(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

