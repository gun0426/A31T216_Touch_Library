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
    uint32_t count;

    if (event & TIMER_EVENT_CAPTURE_CAPDR)
    {
        HAL_TIMER_GetCaptureCount(TIMER_ID_10, &count, TIMER_CAPTURE_REG_CAPDR);
        LOG("timer capture CAPDR:%d\n", count);
    }
}

static void simulate_capture_input(void)
{
    struct timer_cfg cfg = APP_TIMER_SIM_CONFIG;

		HAL_TIMER_SetClock(TIMER_ID_11, cfg.clk_sel, cfg.prescaler, TIMER_MCCLK_MCK, 1);
    HAL_TIMER_Init(TIMER_ID_11, &cfg, NULL, NULL);
    HAL_TIMER_StartPWM(TIMER_ID_11, TIMER_PWM_START_LOW, 4000, 10000, TIMER_PWM_EVENT_NONE);
}

static void mainloop(void)
{
    struct timer_cfg cfg = APP_TIMER_CONFIG;

    /* Simulate input clock */
    simulate_capture_input();

    /* Initialize Timer */
		HAL_TIMER_SetClock(TIMER_ID_10, cfg.clk_sel, cfg.prescaler, TIMER_MCCLK_MCK, 1);
    HAL_TIMER_Init(TIMER_ID_10, &cfg, app_timer_irq_handler, NULL);
		HAL_TIMER_SetIRQ(TIMER_ID_10, TIMER_IRQ_CAPTURE_CAPDR, 1);

    LOG("Timer start capture\n");

    /* Start Timer */
    HAL_TIMER_StartCapture(TIMER_ID_10, TIMER_CAPTURE_CLEAR_BOTH);

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
    LOG("\t Timer Capture\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

