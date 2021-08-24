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

#include <stdlib.h>

#include "main.h"
#include "debug_log.h"
#include "debug_cmd.h"


volatile uint8_t complete = 0;
static uint8_t send_val[256];
static uint8_t recv_val[256];

void app_usart_irq_handler(uint32_t event, void *context)
{
    if ((event & USART_EVENT_TRANSMIT_DONE))
    {
        complete = 1;
        LOG("SPI_EVENT_DONE\n");
    }

    if (event & USART_EVENT_OVERFLOW_ERROR)
    {
        LOG("SPI_EVENT_OVERRUN\n");
    }
}

struct usart_cfg app_usart_cfg = APP_USART_SPI_CONFIG;

static enum debug_cmd_status cmd_master_send(int argc, char *argv[])
{
    uint8_t len;

    if (argc < 2)
    {
        return DEBUG_CMD_INVALID;
    }

    len = (uint8_t)atoi(argv[1]);

    app_usart_cfg.mode = USART_MODE_SPI;
		app_usart_cfg.master_sel = USART_SEL_MASTER;

    HAL_USART_Init(USART_ID_10, &app_usart_cfg, app_usart_irq_handler, NULL);

#if (APP_USART_SS_MANUAL_ENABLE == 1)
    HAL_GPIO_ClearPin(PORTB, PIN3);
#endif

    complete = 0;
		HAL_USART_Transmit(USART_ID_10, send_val, len, USART_IO_TYPE_NON_BLOCK);
    while (!complete)
    {
    }

#if (APP_USART_SS_MANUAL_ENABLE == 1)
    HAL_GPIO_SetPin(PORTB, PIN3);
#endif

    LOG("master send val :");
    LOG_HEX(send_val, len);
    LOG("\n");

    return DEBUG_CMD_SUCCESS;
}


static enum debug_cmd_status cmd_master_recv(int argc, char *argv[])
{
    uint8_t len;

    if (argc < 2)
    {
        return DEBUG_CMD_INVALID;
    }

    len = (uint8_t)atoi(argv[1]);

    app_usart_cfg.mode = USART_MODE_SPI;
		app_usart_cfg.master_sel = USART_SEL_MASTER;

    HAL_USART_Init(USART_ID_10, &app_usart_cfg, app_usart_irq_handler, NULL);

#if (APP_USART_SS_MANUAL_ENABLE == 1)
    HAL_GPIO_ClearPin(PORTB, PIN3);
#endif

    complete = 0;
		HAL_USART_Receive(USART_ID_10, recv_val, len, USART_IO_TYPE_NON_BLOCK);
    while (!complete)
    {
    }

#if (APP_USART_SS_MANUAL_ENABLE == 1)
    HAL_GPIO_SetPin(PORTB, PIN3);
#endif

    LOG("master recv val :");
    LOG_HEX(recv_val, len);
    LOG("\n");

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_slave_send(int argc, char *argv[])
{
    uint8_t len;

    if (argc < 2)
    {
        return DEBUG_CMD_INVALID;
    }

    len = (uint8_t)atoi(argv[1]);

    app_usart_cfg.mode = USART_MODE_SPI;
		app_usart_cfg.master_sel = USART_SEL_SLAVE;

    HAL_USART_Init(USART_ID_10, &app_usart_cfg, app_usart_irq_handler, NULL);

    complete = 0;
		HAL_USART_Transmit(USART_ID_10, send_val, len, USART_IO_TYPE_NON_BLOCK);
    while (!complete)
    {
    }

    LOG("slave send val :");
    LOG_HEX(send_val, len);
    LOG("\n");

    return DEBUG_CMD_SUCCESS;
}

static enum debug_cmd_status cmd_slave_recv(int argc, char *argv[])
{
    uint8_t len;

    if (argc < 2)
    {
        return DEBUG_CMD_INVALID;
    }

    len = (uint8_t)atoi(argv[1]);

    app_usart_cfg.mode = USART_MODE_SPI;
		app_usart_cfg.master_sel = USART_SEL_SLAVE;

    HAL_USART_Init(USART_ID_10, &app_usart_cfg, app_usart_irq_handler, NULL);

    complete = 0;
		HAL_USART_Receive(USART_ID_10, recv_val, len, USART_IO_TYPE_NON_BLOCK);
    while (!complete)
    {
    }

    LOG("slave recv val :");
    LOG_HEX(recv_val, len);
    LOG("\n");
    return DEBUG_CMD_SUCCESS;
}

static const struct debug_cmd test_commands[] =
{
    { "ms", cmd_master_send, "send value to slave", "\targ1 = send len\n" },
    { "mr", cmd_master_recv, "recv value from slave", "\targ1 = recv len\n" },
    { "ss", cmd_slave_send, "send value to master", "\targ1 = send len\n" },
    { "sr", cmd_slave_recv, "recv value from master", "\targ1 = recv len\n" },
};

static void mainloop(void)
{
    uint8_t i;

    for (i = 1; i < 255; i++)
    {
        send_val[i - 1] = i;
    }

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

    /* Configure USART_SPI pins */
    Port_Configure_USART();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t USART_SPI interrupt test\n");
    LOG("************************************************\n");
		
    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));
		
    /* enable irq */
    __enable_irq();
		
    /* Start main loop */
    mainloop();
}

