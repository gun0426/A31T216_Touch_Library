/**
 *******************************************************************************
 * @file        a31t21x_port_init.c
 * @brief       Application specific port configuration
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "main.h"
#include "a31t21x_gpio.h"

void Port_Init(void)
{
		HAL_GPIO_Init();
}

void Port_Configure_CLKO(void)
{
    /* Configure CLKO - PD0 */
    HAL_GPIO_SetFunction(PORTD, PIN0, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTD, PIN0, GPIO_MODE_FUNC);
		HAL_SCU_EnableClockOut(0);
}

void Port_Configure_TIMER2n(void)
{
    /* Configure TIMER20 pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN0, Pn_MUX_AF2);	// Capture T20C
    HAL_GPIO_SetMode(PORTC, PIN0, GPIO_MODE_FUNC);
    /* Configure TIMER21 pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN1, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTC, PIN1, GPIO_MODE_FUNC);
}

