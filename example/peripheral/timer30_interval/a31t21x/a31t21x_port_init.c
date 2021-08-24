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

void Port_Configure_TIMER30(void)
{
    /* Configure PWM30AA pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN7, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTE, PIN7, GPIO_MODE_FUNC);
    /* Configure PWM30AB pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN6, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTE, PIN6, GPIO_MODE_FUNC);
    /* Configure PWM30BA pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN5, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTE, PIN5, GPIO_MODE_FUNC);
    /* Configure PWM30BB pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN4, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTE, PIN4, GPIO_MODE_FUNC);
    /* Configure PWM30CA pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN3, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTE, PIN3, GPIO_MODE_FUNC);
    /* Configure PWM30CB pin to be used */
    HAL_GPIO_SetFunction(PORTE, PIN2, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTE, PIN2, GPIO_MODE_FUNC);
}

