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


static uint8_t app_uart_rx_buf[1];
static uint8_t app_uart_tx_buf[1];

static void mainloop(void)
{
    static const char *msg =
        "\r\n"
        "************************************************\r\n"
        "\t MCU: " APP_DEVICE "\r\n"
        "\t Core: " APP_CORE "\r\n"
        "\t " __DATE__ __TIME__ "\r\n"
        "\t UART Polling\r\n"
        "************************************************\r\n"
        "UART poll Loopback - type in a character\r\n"
    ;

    struct uart_cfg cfg = APP_UART_CONFIG;

    int32_t ret;

    /* Initialize UART driver */
    HAL_UART_Init(UART_ID_0, &cfg, NULL, NULL);

    /* Print message */
    HAL_UART_Transmit(UART_ID_0, (uint8_t *)msg, strlen(msg), UART_IO_TYPE_BLOCK);

    while (1)
    {
        /* Receive 1 byte */
        ret = HAL_UART_Receive(UART_ID_0, app_uart_rx_buf, 1, UART_IO_TYPE_BLOCK);
        if (ret != DRIVER_ERROR_OK)
        {
            continue;
        }

        /* Assign received byte to be transmitted (loopback) */
        app_uart_tx_buf[0] = app_uart_rx_buf[0];

        /* Transmit received byte */
        ret = HAL_UART_Transmit(UART_ID_0, app_uart_tx_buf, 1, UART_IO_TYPE_BLOCK);
        if (ret != DRIVER_ERROR_OK)
        {
            continue;
        }
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Configure UART pins */
    Port_Configure_UART();

    /* Start main loop */
    mainloop();
}

