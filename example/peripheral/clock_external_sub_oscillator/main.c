/**
 *******************************************************************************
 * @file        main.c
 * @brief       Application main
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
#include "debug_log.h"


static struct app_gpio_def app_btn = APP_GPIO_BTN;
static struct app_gpio_def app_led = APP_GPIO_LED;

static void mainloop(void)
{
    /* Set mode as input */
    HAL_GPIO_SetMode(app_btn.port, app_btn.pin, GPIO_MODE_INPUT);

    /* Set pull up */
    HAL_GPIO_SetPull(app_btn.port, app_btn.pin, GPIO_PULL_UP);

    /* Set mode to output */
    HAL_GPIO_SetMode(app_led.port, app_led.pin, GPIO_MODE_OUTPUT);

    /* Set output type to push pull */
    HAL_GPIO_SetOutput(app_led.port, app_led.pin, GPIO_OUTPUT_PUSH_PULL);

    while (1)
    {
        HAL_GPIO_SetPin(app_led.port, app_led.pin);
        for(uint32_t i=0; i<0xFF; i++);

        HAL_GPIO_ClearPin(app_led.port, app_led.pin);
        for(uint32_t i=0; i<0xFF; i++);
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

		/* Configure clock output port */
		Port_Configure_CLKO();

    /* Configure system settings */
    System_Setting_Config();

    /* Start main loop */
    mainloop();
}

