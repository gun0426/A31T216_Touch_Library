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


static void app_lcd_irq_handler(uint32_t event, void *context)
{
    if (event & LCD_EVENT_DONE)
    {
        LOG(">> LCD OPERATION DONE\n");
    }
}

static void mainloop(void)
{
		struct lcd_cfg cfg = APP_LCD_CONFIG;

		/* Initialize LCD */
		HAL_LCD_Init(&cfg, app_lcd_irq_handler, NULL);
		HAL_LCD_SetClock(LCD_CLK_MCCLK, LCD_MCCLK_MCK, 1);

		/* Configure Display Data */
		LCD->DR0 = 0xFF; LCD->DR1 = 0x5A; LCD->DR2 = 0x00; LCD->DR3 = 0xA5;

		LOG("LCD START!!\n");

		/* Start LCD */
		HAL_LCD_DISP_ON();

    while (1)
    {
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

		/* Configure LCD port */
		Port_Configure_LCD();

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
    LOG("\t LCD RUN\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

