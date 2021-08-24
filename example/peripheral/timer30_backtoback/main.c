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

static void app_timer30_irq_handler(uint32_t event, void *context)
{
    if (event & TIMER30_EVENT_BOTTOM_MATCH)
    {
        LOG("pwm bottom match\n");
    }
    if (event & TIMER30_EVENT_PERIOD_MATCH)
    {
        LOG("pwm period\n");
    }
    if (event & TIMER30_EVENT_A_MATCH)
    {
        LOG("pwm a match\n");
    }
    if (event & TIMER30_EVENT_B_MATCH)
    {
        LOG("pwm b match\n");
    }
    if (event & TIMER30_EVENT_C_MATCH)
    {
        LOG("pwm c match\n");
    }
}

static void mainloop(void)
{
    struct timer30_cfg cfg = APP_TIMER_CONFIG;

    /* Initialize Timer */
		HAL_TIMER30_SetClock(TIMER30_ID_30, cfg.clk_sel, cfg.prescaler, TIMER30_MCCLK_MCK, 1);
    HAL_TIMER30_Init(TIMER30_ID_30, &cfg, app_timer30_irq_handler, NULL);
		HAL_TIMER30_SetIRQ(TIMER30_ID_30, TIMER30_IRQ_BOTTOM_MATCH, 1);

    LOG("Timer30 start PWM\n");

    /* Start Timer */
    HAL_TIMER30_StartPWM(TIMER30_ID_30, TIMER30_PWM_START_LOW, 1000, 2000, 4000, 5000, TIMER30_PWM_EVENT_NONE);

    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

		/* Configure timer port */
		Port_Configure_TIMER30();

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
    LOG("\t Timer30 Backtoback Mode\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

