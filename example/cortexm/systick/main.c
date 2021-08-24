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


static volatile uint32_t tick_count;

/*
 * Systick is special.
 * Application can declare SysTick_Handler directly from the main application.
 */
void SysTick_Handler(void)
{
    /* Simply increment count */
    tick_count++;

    /* Every 1000 ticks, log the count value */
    if ((tick_count % 1000) == 0)
    {
        LOG("tick = %d\n", tick_count);
    }
}

static void mainloop(void)
{
    uint32_t ret;

    /* Enable SysTick interrupt */
    NVIC_SetPriority(SysTick_IRQn, DRIVER_PRIORITY_LOWEST);
    NVIC_EnableIRQ(SysTick_IRQn);

    /* Configure systick for every 1 msec */
    ret = SysTick_Config(SystemCoreClock / 1000);
    if (ret != 0)
    {
        LOG("error configuring systick\n");
    }

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
    LOG("\t Systick\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

