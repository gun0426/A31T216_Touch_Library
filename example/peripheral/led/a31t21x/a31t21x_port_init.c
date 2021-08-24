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

void Port_Configure_LED(void)
{
    /* COM port setting - ICOM0 to ICOM2 */
    HAL_GPIO_SetFunction(PORTE, PIN7, Pn_MUX_AF5);	// ICOM0
    HAL_GPIO_SetMode(PORTE, PIN7, GPIO_MODE_FUNC);
    HAL_GPIO_SetFunction(PORTE, PIN6, Pn_MUX_AF5);	// ICOM1
    HAL_GPIO_SetMode(PORTE, PIN6, GPIO_MODE_FUNC);
    HAL_GPIO_SetFunction(PORTE, PIN5, Pn_MUX_AF5);	// ICOM2
    HAL_GPIO_SetMode(PORTE, PIN5, GPIO_MODE_FUNC);

		/* SEG port setting - ISEG13 to ISEG15 */
    HAL_GPIO_SetFunction(PORTD, PIN0, Pn_MUX_AF5);	// ISEG13
    HAL_GPIO_SetMode(PORTD, PIN0, GPIO_MODE_FUNC);
    HAL_GPIO_SetFunction(PORTC, PIN4, Pn_MUX_AF5);	// ISEG14
    HAL_GPIO_SetMode(PORTC, PIN4, GPIO_MODE_FUNC);
    HAL_GPIO_SetFunction(PORTC, PIN3, Pn_MUX_AF5);	// ISEG15
    HAL_GPIO_SetMode(PORTC, PIN3, GPIO_MODE_FUNC);
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

