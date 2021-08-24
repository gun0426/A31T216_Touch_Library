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


static uint8_t write_buf[APP_FLASH_REGION_SIZE];
static uint8_t read_buf[APP_FLASH_REGION_SIZE];

static enum debug_cmd_status cmd_erase(int argc, char *argv[])
{
    int32_t ret;

    ret = HAL_FLASH_Erase(APP_FLASH_REGION_ADDR);

    LOG("Erase done %d\n", ret);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_write(int argc, char *argv[])
{
    int32_t ret;
    uint32_t i;

    for (i = 0; i < APP_FLASH_REGION_SIZE; i++)
    {
        write_buf[i] = i;
    }

    ret = HAL_FLASH_Write(APP_FLASH_REGION_ADDR, write_buf, APP_FLASH_REGION_SIZE);

    LOG("Write done %d\n", ret);
    LOG_HEX((uint8_t *)APP_FLASH_REGION_ADDR, APP_FLASH_REGION_SIZE);

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_verify(int argc, char *argv[])
{
    HAL_FLASH_Read(APP_FLASH_REGION_ADDR, read_buf, APP_FLASH_REGION_SIZE);

    if (memcmp(write_buf, read_buf, APP_FLASH_REGION_SIZE) == 0)
    {
        LOG("Verify done\n");
    }
    else
    {
        LOG("Verify error\n");
    }

    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "erase", cmd_erase, "erase" },
    { "write", cmd_write, "write" },
    { "verify", cmd_verify, "verify" },
};

static void mainloop(void)
{
    LOG("Flash test address = 0x%08x, size = %d\n",
        APP_FLASH_REGION_ADDR, APP_FLASH_REGION_SIZE);

    HAL_FLASH_Init();

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
    LOG("\t Code Flash\n");
    LOG("************************************************\n");

    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));

    /* Start main loop */
    mainloop();
}

