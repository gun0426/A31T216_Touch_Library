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


static void app_wt_irq_handler(uint32_t event, void *context)
{
    if (event & WT_EVENT_COUNT_MATCH)
    {
        LOG("WT expired after 1s\n");
    }
}

static void mainloop(void)
{
    struct wt_cfg cfg = APP_WT_CONFIG;

    /* Initialize Timer */
    HAL_WT_Init(&cfg, app_wt_irq_handler, NULL);
		HAL_WT_SetClock(cfg.clk_sel, WT_MCCLK_MCK, 1);
		HAL_WT_SetIRQ(WT_IRQ_COUNT_MATCH, 1);

    LOG("WT start \n");

    /* Start Timer */
    HAL_WT_Start(500);

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
    LOG("\t WT Interrupt\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

