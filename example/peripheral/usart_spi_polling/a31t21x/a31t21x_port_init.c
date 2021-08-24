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

void Port_Configure_UART(void)
{
    /* Configure UART TXD pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN4, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTB, PIN4, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTB, PIN4, GPIO_PULL_UP);

    /* Configure UART RXD pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN5, Pn_MUX_AF1);
    HAL_GPIO_SetMode(PORTB, PIN5, GPIO_MODE_FUNC);
    HAL_GPIO_SetPull(PORTB, PIN5, GPIO_PULL_UP);
}

void Port_Configure_USART(void)
{
    /* Configure MOSI10 pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN0, Pn_MUX_AF2);
    HAL_GPIO_SetMode(PORTB, PIN0, GPIO_MODE_FUNC);
    /* Configure MISO10 pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN1, Pn_MUX_AF2);
    HAL_GPIO_SetMode(PORTB, PIN1, GPIO_MODE_FUNC);
    /* Configure SCK10 pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN2, Pn_MUX_AF2);
    HAL_GPIO_SetMode(PORTB, PIN2, GPIO_MODE_FUNC);
    /* Configure SS10 pin to be used */
    HAL_GPIO_SetFunction(PORTB, PIN3, Pn_MUX_AF2);
    HAL_GPIO_SetMode(PORTB, PIN3, GPIO_MODE_FUNC);
#if (APP_USART_SS_MANUAL_ENABLE == 1)
    HAL_GPIO_SetMode(PORTB, PIN3, GPIO_MODE_OUTPUT);
		HAL_GPIO_SetPin(PORTB, PIN3);
#endif
}

