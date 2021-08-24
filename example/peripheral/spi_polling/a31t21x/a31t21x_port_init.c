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

void Port_Configure_SPI2n(void)
{
    /* Configure MOSI20 pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN1, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTC, PIN1, GPIO_MODE_FUNC);
    /* Configure MISO20 pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN2, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTC, PIN2, GPIO_MODE_FUNC);
    /* Configure SCK20 pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN3, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTC, PIN3, GPIO_MODE_FUNC);
    /* Configure SS20 pin to be used */
    HAL_GPIO_SetFunction(PORTC, PIN4, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTC, PIN4, GPIO_MODE_FUNC);

    /* Configure MOSI21 pin to be used */
    HAL_GPIO_SetFunction(PORTD, PIN2, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTD, PIN2, GPIO_MODE_FUNC);
    /* Configure MISO21 pin to be used */
    HAL_GPIO_SetFunction(PORTD, PIN3, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTD, PIN3, GPIO_MODE_FUNC);
    /* Configure SCK21 pin to be used */
    HAL_GPIO_SetFunction(PORTD, PIN4, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTD, PIN4, GPIO_MODE_FUNC);
    /* Configure SS21 pin to be used */
    HAL_GPIO_SetFunction(PORTD, PIN5, Pn_MUX_AF3);
    HAL_GPIO_SetMode(PORTD, PIN5, GPIO_MODE_FUNC);
}

