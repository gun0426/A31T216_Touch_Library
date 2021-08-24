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

static volatile uint32_t i2c_event = 0;

static uint8_t ms_tx_data[TEST_DATA_LEN] =
{
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};
static uint8_t ms_rx_data[TEST_DATA_LEN] =
{
    0x00,
};

static uint8_t ms_burst_tx_reg = 0x8A;
static uint8_t ms_burst_tx_data[TEST_DATA_LEN] =
{
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
};

static uint8_t ms_burst_rx_reg = 0x8B;
static uint8_t ms_burst_rx_data[TEST_DATA_LEN] =
{
    0x00,
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

static void app_i2c_irq_handler(uint32_t event, void *context)
{
    i2c_event = event;
}

static void app_i2c_wait_cmpl(void)
{
    while (!i2c_event)
    {
    }

    LOG("event 0x%08x\n", i2c_event);
}

static void mainloop(void)
{
    struct i2c_cfg cfg = APP_I2C_CONFIG;

    HAL_I2C_Deinit(I2C_ID_0);
    HAL_I2C_Init(I2C_ID_0, &cfg, app_i2c_irq_handler, NULL);

    i2c_event = 0;
    HAL_I2C_MasterTransmit(I2C_ID_0, SLAVE_ADDR, ms_tx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    SystemDelay(10);

    i2c_event = 0;
    HAL_I2C_MasterReceive(I2C_ID_0, SLAVE_ADDR, ms_rx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    SystemDelay(10);

    i2c_event = 0;
    HAL_I2C_MasterBurstTransmit(I2C_ID_0, SLAVE_ADDR, &ms_burst_tx_reg, 1,
                                ms_burst_tx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    SystemDelay(10);

    i2c_event = 0;
    HAL_I2C_MasterBurstReceive(I2C_ID_0, SLAVE_ADDR, &ms_burst_rx_reg, 1,
                               ms_burst_rx_data, TEST_DATA_LEN, I2C_IO_TYPE_NON_BLOCK);
    app_i2c_wait_cmpl();

    LOG("Master Trasmit Data\n");
    hex_dump(ms_tx_data, TEST_DATA_LEN);
    LOG("Master Receive Data\n");
    hex_dump(ms_rx_data, TEST_DATA_LEN);
    LOG("Master Burst Transmit Data\n");
    LOG("REG : %02x\n", ms_burst_tx_reg);
    hex_dump(ms_burst_tx_data, TEST_DATA_LEN);
    LOG("Master Burst Receive Data\n");
    LOG("REG : %02x\n", ms_burst_rx_reg);
    hex_dump(ms_burst_rx_data, TEST_DATA_LEN);

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
    LOG("\t I2C Master Interrupt\n");
    LOG("************************************************\n");

    /* enable irq */
		__enable_irq();
		
    /* Start main loop */
    mainloop();
}

