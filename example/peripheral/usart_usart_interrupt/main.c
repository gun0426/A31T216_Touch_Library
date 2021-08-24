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

static volatile uint8_t app_usart_tx_done;
static volatile uint8_t app_usart_rx_done;

static void app_usart_irq_handler(uint32_t event, void *context)
{
    int32_t ret;

    if (event & USART_EVENT_RECEIVE_DONE)
    {
        /* USART received, assign received byte to be transmitted (loopback) */
        app_usart_tx_buf[0] = app_usart_rx_buf[0];

        /* Receive 1 byte again */
        ret = HAL_USART_Receive(USART_ID_10, app_usart_rx_buf, 1, USART_IO_TYPE_NON_BLOCK);
        if (ret != DRIVER_ERROR_OK)
        {
            /* error occured */
        }

        /* Transmit received byte */
        ret = HAL_USART_Transmit(USART_ID_10, app_usart_tx_buf, 1, USART_IO_TYPE_NON_BLOCK);
        if (ret != DRIVER_ERROR_OK)
        {
            /* error occured */
        }

        app_usart_rx_done = 1;
    }

    if (event & USART_EVENT_TRANSMIT_DONE)
    {
        /* do nothing */
        app_usart_tx_done = 1;
    }

    /* error handling */
}

static void mainloop(void)
{
    static const char *msg =
        "\r\n"
        "************************************************\r\n"
        "\t MCU: " APP_DEVICE "\r\n"
        "\t Core: " APP_CORE "\r\n"
        "\t " __DATE__ __TIME__ "\r\n"
        "\t USART-UART Interrupt\r\n"
        "************************************************\r\n"
        "USART-USART Interrupt Loopback - type in a character\r\n"
    ;

    struct usart_cfg cfg = APP_USART_CONFIG;

    /* Initialize USART driver */
    HAL_USART_Init(USART_ID_10, &cfg, app_usart_irq_handler, NULL);

    /* Print message */
    HAL_USART_Transmit(USART_ID_10, (uint8_t *)msg, strlen(msg), USART_IO_TYPE_NON_BLOCK);

    /* Wait for the messages to be displayed */
    SystemDelay(500);

    /* Receive 1 byte */
    HAL_USART_Receive(USART_ID_10, app_usart_rx_buf, 1, USART_IO_TYPE_NON_BLOCK);

    while (1)
    {
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

    /* enable irq */
    __enable_irq();

    /* Start main loop */
    mainloop();
}

