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


#define SLAVE_ADDR      0x50	// Real Address : 0xA0 (0x50 << 1)
#define TEST_DATA_LEN   8

static uint8_t sl_rx_data[TEST_DATA_LEN] =
{
    0x00,
};
static uint8_t sl_tx_data[TEST_DATA_LEN] =
{
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

static uint8_t sl_tr_rx_data[TEST_DATA_LEN + 1] =
{
    0x00,
};

static uint8_t sl_tr_tx_reg = 0;
static uint8_t sl_tr_tx_data[TEST_DATA_LEN] =
{
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
};

static void hex_dump(uint8_t *data, uint32_t len)
{
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        LOG("%02x ", data[i]);
    }

    LOG("\n");
}

static void mainloop(void)
{
    struct i2c_cfg cfg = APP_I2C_CONFIG;
    int ret;

    HAL_I2C_Init(I2C_ID_0, &cfg, NULL, NULL);

    HAL_I2C_SetOwnAddress(I2C_ID_0, SLAVE_ADDR);

    ret = HAL_I2C_SlaveReceive(I2C_ID_0, sl_rx_data, TEST_DATA_LEN, I2C_IO_TYPE_BLOCK);
    if (ret)
    {
        LOG("Slave Receive error %d\n", ret);
    }

    ret = HAL_I2C_SlaveTransmit(I2C_ID_0, sl_tx_data, TEST_DATA_LEN, I2C_IO_TYPE_BLOCK);
    if (ret)
    {
        LOG("Master Transmit error %d\n", ret);
    }

    ret = HAL_I2C_SlaveReceive(I2C_ID_0, sl_tr_rx_data, TEST_DATA_LEN + 1, I2C_IO_TYPE_BLOCK);
    if (ret)
    {
        LOG("Slave Receive error %d\n", ret);
    }

    ret = HAL_I2C_SlaveTransfer(I2C_ID_0, sl_tr_tx_data, TEST_DATA_LEN, &sl_tr_tx_reg, 1, I2C_IO_TYPE_BLOCK);
    if (ret)
    {
        LOG("Slave Transfer error %d\n", ret);
    }

    LOG("Slave Receive Data\n");
    hex_dump(sl_rx_data, TEST_DATA_LEN);
    LOG("Slave Trasmit Data\n");
    hex_dump(sl_tx_data, TEST_DATA_LEN);
    LOG("Slave Burst Receive Data\n");
    LOG("REG : %02x\n", sl_tr_rx_data[0]);
    hex_dump(&sl_tr_rx_data[1], TEST_DATA_LEN);
    LOG("Slave Burst Transmit Data\n");
    LOG("REG : %02x\n", sl_tr_tx_reg);
    hex_dump(sl_tr_tx_data, TEST_DATA_LEN);

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

    /* Configure I2C pins */
    Port_Configure_I2C();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t I2C Slave Polling\n");
    LOG("************************************************\n");
			
    /* Start main loop */
    mainloop();
}

