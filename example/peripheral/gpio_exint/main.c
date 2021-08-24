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

static void app_gpio_irq_handler(void *context)
{
    uint8_t value;

    HAL_GPIO_ReadPin(app_btn.port, app_btn.pin, &value);

    LOG("GPIO Button Value=%d\n", value);

    if (value)
    {
        HAL_GPIO_SetPin(app_led.port, app_led.pin);
    }
    else
    {
        HAL_GPIO_ClearPin(app_led.port, app_led.pin);
    }
}

static void mainloop(void)
{
    /* Set mode as input */
    HAL_GPIO_SetMode(app_btn.port, app_btn.pin, GPIO_MODE_INPUT);

    /* Set pull up */
    HAL_GPIO_SetPull(app_btn.port, app_btn.pin, GPIO_PULL_UP);

    /* Set debouncing clock */
    HAL_GPIO_SetDebounceClock(app_btn.port, APP_GPIO_DBC_CLK_SRC, APP_GPIO_DBC_CLK_DIV);

    /* Set debouncing enable */
    HAL_GPIO_SetDebounce(app_btn.port, app_btn.pin, 1);

    /* Set interrupt handler */
    HAL_GPIO_SetIRQHandler(app_btn.port, app_btn.pin, app_gpio_irq_handler, NULL);

    /* Enable interrupt */
    HAL_GPIO_EnableIRQ(app_btn.port, app_btn.pin,
                       GPIO_IRQ_TRIG_EDGE,
                       GPIO_IRQ_FALLING_AND_RISING,
                       DRIVER_PRIORITY_LOWEST);

    /* Set mode to output */
    HAL_GPIO_SetMode(app_led.port, app_led.pin, GPIO_MODE_OUTPUT);

    /* Set output type to push pull */
    HAL_GPIO_SetOutput(app_led.port, app_led.pin, GPIO_OUTPUT_PUSH_PULL);

    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t GPIO External Interrupt\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

