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
#define APP_NUM_ADC_AVG     8

uint16_t app_adc_avg(uint16_t *value, uint8_t len)
{
    uint8_t i;
    uint16_t avg = 0;

    for (i = 0; i < len; i++)
    {
        avg += *value;
    }

    return (avg / len);
}

static void app_adc_run(void)
{
    uint32_t i, j;
    uint16_t adc_buf[APP_NUM_ADC_AVG];
    uint16_t adc_avg;

    LOG("Reading ADC\n");


    for (i = 0; i < APP_NUM_ADC_CH; i++)
    {
        /* Select the channel */
        HAL_ADC_SelectChannel(i*2+1);

        /* Start ADC and wait for conversion to be completed */
        for (j = 0; j < APP_NUM_ADC_AVG; j++)
        {
            HAL_ADC_Start(&adc_buf[j], ADC_IO_TYPE_BLOCK);
        }

        adc_avg = app_adc_avg(adc_buf, APP_NUM_ADC_AVG);
        LOG("CH[%02d]=0x%04x\n", i*2+1, adc_avg);
    }

    SystemDelay(1000);
}

static void mainloop(void)
{
    /* ADC configuration */
    struct adc_cfg cfg = APP_ADC_CONFIG;

    /* Initialize the ADC with the configuration */
    HAL_ADC_Init(&cfg, NULL, NULL);

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
    LOG("\t ADC Polling  \n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

