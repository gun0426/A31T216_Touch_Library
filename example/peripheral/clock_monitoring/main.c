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

static void app_sysclock_failure_handler(uint32_t event, void *context)
{
    if (event & SCU_CLOCK_FAIL_EVENT_MCLK)
    {
        LOG("Main Clock fail\n");
        HAL_SCU_DisableClockMonitor(SCU_MON_CLOCK_MCLK);
    }

    if (event & SCU_CLOCK_FAIL_EVENT_LSE)
    {
        LOG("Low Speed External OSC fail\n");
        HAL_SCU_DisableClockMonitor(SCU_MON_CLOCK_LSE);
    }

    if (event & SCU_CLOCK_FAIL_EVENT_HSE)
    {
        LOG("High Speed External OSC fail\n");
        HAL_SCU_DisableClockMonitor(SCU_MON_CLOCK_HSE);
    }
}

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

    /* Enable HSE clock */
    HAL_SCU_EnableClock(SCU_CLOCK_HSE);

    /* Enable LSI clock */
    HAL_SCU_EnableClock(SCU_CLOCK_LSI);

    /* Start monitoring the clock */
    HAL_SCU_EnableClockMonitor(SCU_MON_CLOCK_HSE, app_sysclock_failure_handler, NULL, 3);

    LOG("HSE clock disable after 1s\n");
    SystemDelay(1000);

    /* Disable HSE to for the clock to fail, clock monitoring reports this event */
    HAL_SCU_DisableClock(SCU_CLOCK_HSE);

    SystemDelay(1000);

    while (1)
    {
        LOG("GPIO LED Set\n");
        HAL_GPIO_SetPin(app_led.port, app_led.pin);
        SystemDelay(500);

        LOG("GPIO LED Clear\n");
        HAL_GPIO_ClearPin(app_led.port, app_led.pin);
        SystemDelay(500);
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
    LOG("\t Clock Setting - HSE Monitoring Enable\n");
    LOG("************************************************\n");

		/* Enable global interrupt */
		__enable_irq();

    /* Start main loop */
    mainloop();
}

