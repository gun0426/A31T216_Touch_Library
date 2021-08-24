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
#include "a31t21x_timer.h"
#include "a31t21x_gpio.h"
#include "a31t21x_scu.h"
#include "a31t21x_uart.h"

#define APP_DEVICE                      "A31T21X"
#define APP_CORE                        "ARM Cortex-M0+"
#define APP_UART_ID                     0
#define APP_UART_BAUD                   9600

#define APP_TIMER_CONFIG                    \
    {                                       \
        .mode = TIMER_MODE_PWM,							\
        .clk_sel = TIMER_CLK_MCCLK, 			  \
        .prescaler = 800,										\
        .irq_prio = DRIVER_PRIORITY_LOWEST, \
    }

void Port_Init(void);
void Port_Configure_CLKO(void);
void Port_Configure_TIMER2n(void);
void System_Setting_Config(void);
void Debug_Init(void);
void Assert(char *file, uint32_t line);

#endif /* _MAIN_H_ */

