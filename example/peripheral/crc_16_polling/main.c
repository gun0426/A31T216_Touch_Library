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

#define APP_CRC_DATA_STRING     1
#define APP_CRC_DATA_RAM        2

/**
 * CRC input data to test can be selected
 */
#define APP_CRC_DATA_TYPE       APP_CRC_DATA_STRING


#if (APP_CRC_DATA_TYPE == APP_CRC_DATA_STRING)

static uint32_t test_string[] =
{
    '0', '1', '2', '3'
};

#define APP_CRC_DATA            test_string
#define APP_CRC_SIZE            sizeof(test_string)/sizeof(uint32_t)

#elif (APP_CRC_DATA_TYPE == APP_CRC_DATA_RAM)

#define APP_CRC_DATA            (uint32_t *)0x20000000
#define APP_CRC_SIZE            0x010

#endif


static void app_crc16(void)
{
    uint32_t value;
    struct crc_cfg cfg = APP_CRC_CONFIG;
	
    LOG("CRC16\n");

    HAL_CRC_Init(&cfg, NULL, NULL);
    HAL_CRC_Compute(0xFFFF, APP_CRC_DATA, APP_CRC_SIZE, &value);

    LOG("HW = 0x%08x\n", value);

    /*
     * Note: CRC implementation is differnt from standard
     * Use software for CRC
     */
		value = HAL_CRC_SW_Generate(0xFFFF, APP_CRC_DATA, APP_CRC_SIZE, cfg.poly, cfg.bit);
    LOG("SW = 0x%08x\n", value);
}

static void mainloop(void)
{
    app_crc16();

    while (1)
    {
        /* if there is a command ready, process it */
        debug_cmd_process();
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
    LOG("\t CRC 16 Polling\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

