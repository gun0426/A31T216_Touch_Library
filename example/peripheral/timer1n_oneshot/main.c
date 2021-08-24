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

static void app_timer_irq_handler(uint32_t event, void *context)
{
    if (event & TIMER_EVENT_COUNT_MATCH)
    {
        LOG("Timer expired after 1s\n");
    }
}

static void mainloop(void)
{
    struct timer_cfg cfg = APP_TIMER_CONFIG;

    /* Initialize Timer */
		HAL_TIMER_SetClock(TIMER_ID_10, cfg.clk_sel, cfg.prescaler, TIMER_MCCLK_MCK, 1);
    HAL_TIMER_Init(TIMER_ID_10, &cfg, app_timer_irq_handler, NULL);
		HAL_TIMER_SetIRQ(TIMER_ID_10, TIMER_IRQ_COUNT_MATCH, 1);

    LOG("Timer start oneshot\n");

    /* Start Timer */
		HAL_TIMER_Start(TIMER_ID_10, 10000);

    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

		/* Configure timer port */
		Port_Configure_TIMER1n();

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
    LOG("\t Timer Oneshot\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

