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

void app_spi_irq_handler(uint32_t event, void *context)
{
    if ((event & SPI_EVENT_TRANSFER_DONE) || (event & SPI_EVENT_DMA_TRANSFER_DONE))
    {
        complete = 1;
    }

    if (event & SPI_EVENT_OVERRUN)
    {
        LOG("SPI_EVENT_OVERRUN\n");
    }

    if (event & SPI_EVENT_UNDERRUN)
    {
        LOG("SPI_EVENT_UNDERRUN\n");
    }
}

struct spi_cfg app_spi_cfg = APP_SPI_CONFIG;

static enum debug_cmd_status cmd_master_send(int argc, char *argv[])
{
    uint8_t len;

    if (argc < 2)
    {
        return DEBUG_CMD_INVALID;
    }

    len = (uint8_t)atoi(argv[1]);

    app_spi_cfg.mode = SPI_MODE_MASTER;

    HAL_SPI_Init(SPI_ID_20, &app_spi_cfg, app_spi_irq_handler, NULL);

#if (APP_SPI_DMA_ENABLE == 1)
    HAL_SPI_SetTransmitDMA(SPI_ID_20, DMA_ID_2, 1);
    HAL_SPI_SetReceiveDMA(SPI_ID_20, DMA_ID_3, 1);
#endif

#if (APP_SPI_SS_MANUAL_ENABLE == 1)
    HAL_SPI_SSManual(SPI_ID_20, SPI_SS_STATE_LOW);
#endif

    complete = 0;
    HAL_SPI_Transmit(SPI_ID_20, send_val, len, SPI_IO_TYPE_NON_BLOCK);
    while (!complete)
    {
    }

#if (APP_SPI_SS_MANUAL_ENABLE == 1)
		SystemDelay(200);	// ##
    HAL_SPI_SSManual(SPI_ID_20, SPI_SS_STATE_HIGH);
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

    app_spi_cfg.mode = SPI_MODE_MASTER;

    HAL_SPI_Init(SPI_ID_20, &app_spi_cfg, app_spi_irq_handler, NULL);

#if (APP_SPI_DMA_ENABLE == 1)
    HAL_SPI_SetTransmitDMA(SPI_ID_20, DMA_ID_2, 1);
    HAL_SPI_SetReceiveDMA(SPI_ID_20, DMA_ID_3, 1);
#endif

#if (APP_SPI_SS_MANUAL_ENABLE == 1)
    HAL_SPI_SSManual(SPI_ID_20, SPI_SS_STATE_LOW);
#endif

    complete = 0;
		HAL_SPI_Transfer(SPI_ID_20, send_val, len, recv_val, len, SPI_IO_TYPE_NON_BLOCK);	// ##
    while (!complete)
    {
    }

#if (APP_SPI_SS_MANUAL_ENABLE == 1)
    HAL_SPI_SSManual(SPI_ID_20, SPI_SS_STATE_HIGH);
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

    app_spi_cfg.mode = SPI_MODE_SLAVE;

    HAL_SPI_Init(SPI_ID_20, &app_spi_cfg, app_spi_irq_handler, NULL);

#if (APP_SPI_DMA_ENABLE == 1)
    HAL_SPI_SetTransmitDMA(SPI_ID_20, DMA_ID_2, 1);
    HAL_SPI_SetReceiveDMA(SPI_ID_20, DMA_ID_3, 1);
#endif

    complete = 0;
    HAL_SPI_Transmit(SPI_ID_20, send_val, len, SPI_IO_TYPE_NON_BLOCK);
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

    app_spi_cfg.mode = SPI_MODE_SLAVE;

    HAL_SPI_Init(SPI_ID_20, &app_spi_cfg, app_spi_irq_handler, NULL);

#if (APP_SPI_DMA_ENABLE == 1)
    HAL_SPI_SetTransmitDMA(SPI_ID_20, DMA_ID_2, 1);
    HAL_SPI_SetReceiveDMA(SPI_ID_20, DMA_ID_3, 1);
#endif

    complete = 0;
    HAL_SPI_Receive(SPI_ID_20, recv_val, len, SPI_IO_TYPE_NON_BLOCK);
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

    /* Configure SPI pins */
    Port_Configure_SPI2n();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t SPI dma interrupt test\n");
    LOG("************************************************\n");
		
    /* Initialze command line processing */
    debug_cmd_init(test_commands, DEBUG_CMD_LIST_COUNT(test_commands));
		
    /* enable irq */
    __enable_irq();
		
    /* Start main loop */
    mainloop();
}

