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

static void app_led_irq_handler(uint32_t event, void *context)
{
    if (event & LED_EVENT_DONE)
    {
				HAL_LED_DISP_OFF();
        LOG(">> LED OPERATION DONE\n");
    }
}

static void mainloop(void)
{
		struct led_cfg cfg = APP_LED_CONFIG;

		/* Initialize LCD */
		HAL_LED_Init(&cfg, app_led_irq_handler, NULL);
		HAL_LED_SetClock(cfg.clk_sel, LED_MCCLK_MCK, 1);

		/* Configure Display Data */
		LED->COMER = 0x0000000F;				// COM / SEG Select
		LED->COMOE = 0x00001FFF;				// All COM Output Enable (COM0~COM12)
		LED->SEGOE = 0x0000FFFF;				// All SET Output Enable (SEG0~SEG15)

		LED->DISPRAM0 = 0x0000FFFF;			// COM0 [SEG15 to SEG0]
		LED->DISPRAM1 = 0x0000FFFF;
		LED->DISPRAM2 = 0x0000FFFF;
		LED->DISPRAM3 = 0x0000FFFF;
		LED->DISPRAM4 = 0x0000FFFF;
		LED->DISPRAM5 = 0x0000FFFF;
		LED->DISPRAM6 = 0x0000FFFF;
		LED->DISPRAM7 = 0x0000FFFF;
		LED->DISPRAM8 = 0x0000FFFF;
		LED->DISPRAM9 = 0x0000FFFF;
		LED->DISPRAM10 = 0x0000FFFF;
		LED->DISPRAM11 = 0x0000FFFF;
		LED->DISPRAM12 = 0x0000FFFF;		// COM12 [SEG15 to SEG0]

		LOG("LED START!!\n");

		/* Start LCD */
		HAL_LED_DISP_ON();

    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

		/* Configure LCD port */
		Port_Configure_LED();

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
    LOG("\t LED RUN\n");
    LOG("************************************************\n");

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

