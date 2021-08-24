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
#include "debug_cmd.h"


static struct app_gpio_def app_led = APP_GPIO_LED;

void app_lvi_irq_handler(uint32_t event, void *context)
{
    /*
     * Application specified low voltage is detected
     * If needed, make the system into a safe state according to the application requirement.
     */
    LOG("LVI interrupt\n");
}

static void mainloop(void)
{
    struct lvi_cfg cfg =
    {
        .level = PWR_LV_3_04V,
        .irq_prio = DRIVER_PRIORITY_LOWEST,
    };

    /* initialize LVI */
    HAL_LVI_Init(&cfg, app_lvi_irq_handler, NULL);

    /* Set mode to output */
    HAL_GPIO_SetMode(app_led.port, app_led.pin, GPIO_MODE_OUTPUT);

    /* Set output type to push pull */
    HAL_GPIO_SetOutput(app_led.port, app_led.pin, GPIO_OUTPUT_PUSH_PULL);
				
    /* LED on for a while to signal application start */
    HAL_GPIO_SetPin(app_led.port, app_led.pin);
    SystemDelay(3000);

    while (1)
    {
        LOG("GPIO LED Set\n");
        HAL_GPIO_SetPin(app_led.port, app_led.pin);
        SystemDelay(100);

        LOG("GPIO LED Clear\n");
        HAL_GPIO_ClearPin(app_led.port, app_led.pin);
        SystemDelay(100);
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
    LOG("\t LVI (Low-Voltage Indicator) control  \n");
    LOG("************************************************\n");
    
	  /* enable irq */
	  __enable_irq();
	
    /* Start main loop */
    mainloop();
}

