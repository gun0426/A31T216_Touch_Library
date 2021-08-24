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


#define APP_NUM_ADC_CH      4

static volatile uint32_t adc_irq_single_flg;

static void app_adc_irq_handler(uint32_t event, void *context)
{
    if (event & ADC_EVENT_SINGLE)
    {
        /* ADC single conversion event */
        LOG("ADC single conversion\n");
        adc_irq_single_flg = 1;
    }

    if (event & ADC_EVENT_TRIGGER)
    {
        /* ADC trigger conversion event*/
        LOG("ADC DMA conversion \n");
        adc_irq_single_flg = 1;
    }
}

static void app_adc_run(void)
{
    uint32_t i;
    uint16_t adc_buf;

    LOG("Reading ADC\n");

    for (i = 0; i < APP_NUM_ADC_CH; i++)
    {
        /* Select the channel -> ch 1, 3, 5, 7 */
        HAL_ADC_SelectChannel(i*2 + 1);

        adc_irq_single_flg = 0;

        /* Start ADC and wait for conversion to be completed */
        HAL_ADC_Start(&adc_buf, ADC_IO_TYPE_NON_BLOCK);

        while (adc_irq_single_flg == 0)
        {
        }

				HAL_ADC_GetData(&adc_buf);
				LOG("CH[%02d]=0x%04x\n", i*2+1, adc_buf);
    }
    SystemDelay(1000);
}

static void mainloop(void)
{
    /* ADC configuration */
    struct adc_cfg cfg = APP_ADC_CONFIG;

    /* Initialize the ADC with the configuration */
    HAL_ADC_Init(&cfg, app_adc_irq_handler, NULL);

    while (1)
    {
        app_adc_run();
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Initialize ADC port */
    Port_Configure_ADC();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t ADC interrupt\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

