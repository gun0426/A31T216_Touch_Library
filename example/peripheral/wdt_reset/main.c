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


static void app_reset_reason(void)
{
    uint32_t reset_reason;

    HAL_SCU_GetResetReason(&reset_reason);
    LOG("Reset Reason: 0x%x\n", reset_reason);
}

static void mainloop(void)
{
    struct wdt_cfg cfg = APP_WDT_CONFIG;
    uint32_t wdt_feed = 0;
    uint32_t wdt_counter;

    /* Initialize Watchdog */
    HAL_WDT_Init(&cfg, NULL, NULL);

    /* Start Watchdog: e.g. 4 seconds */
    HAL_WDT_Start(31250);

    while (1)
    {
        /* feed the watchdog for a while */
        if (wdt_feed < 4)
        {
            wdt_feed++;
            HAL_WDT_Feed();
        }

        HAL_WDT_GetCounter(&wdt_counter);
        LOG("WDT: %d\n", wdt_counter);

        SystemDelay(1000);
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
    LOG("\t Watchdog Timer Reset\n");
    LOG("************************************************\n");

    /* Find the reset reason */
    app_reset_reason();

    /* Start main loop */
    mainloop();
}

