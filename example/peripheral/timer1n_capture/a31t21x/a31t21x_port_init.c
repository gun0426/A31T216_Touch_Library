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

void Port_Configure_TIMER1n(void)
{
    /* Configure TIMER10 pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN1, Pn_MUX_AF2);	// Capture T10C
    HAL_GPIO_SetMode(PORTE, PIN1, GPIO_MODE_FUNC);
    /* Configure TIMER11 pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN0, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTE, PIN0, GPIO_MODE_FUNC);
    /* Configure TIMER12 pin to be used */
    HAL_GPIO_SetFunction(PORTA, PIN9, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTA, PIN9, GPIO_MODE_FUNC);
    /* Configure TIMER13 pin to be used */
    HAL_GPIO_SetFunction(PORTA, PIN11, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTA, PIN11, GPIO_MODE_FUNC);
}

