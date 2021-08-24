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


static uint8_t app_usart_rx_buf[1];
static uint8_t app_usart_tx_buf[1];

static void mainloop(void)
{
    static const char *msg =
        "\r\n"
        "************************************************\r\n"
        "\t MCU: " APP_DEVICE "\r\n"
        "\t Core: " APP_CORE "\r\n"
        "\t " __DATE__ __TIME__ "\r\n"
        "\t USART-USART Polling\r\n"
        "************************************************\r\n"
        "USART-USART poll Loopback - type in a character\r\n"
    ;

    struct usart_cfg cfg = APP_USART_CONFIG;

    int32_t ret;

    /* Initialize USART driver */
    HAL_USART_Init(USART_ID_10, &cfg, NULL, NULL);

    /* Print message */
    HAL_USART_Transmit(USART_ID_10, (uint8_t *)msg, strlen(msg), USART_IO_TYPE_BLOCK);

    while (1)
    {
        /* Receive 1 byte */
        ret = HAL_USART_Receive(USART_ID_10, app_usart_rx_buf, 1, USART_IO_TYPE_BLOCK);
        if (ret != DRIVER_ERROR_OK)
        {
            continue;
        }

        /* Assign received byte to be transmitted (loopback) */
        app_usart_tx_buf[0] = app_usart_rx_buf[0];

        /* Transmit received byte */
        ret = HAL_USART_Transmit(USART_ID_10, app_usart_tx_buf, 1, USART_IO_TYPE_BLOCK);
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

    /* Configure USART pins */
    Port_Configure_USART();

    /* Start main loop */
    mainloop();
}

