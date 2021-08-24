/**********************************************************************
* @file		main_conf.h
* @brief	Contains all macro definitions and function prototypes
* 			support for firmware library on A31T21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_


/* Includes ------------------------------------------------------------------*/
#include "a31t21x.h"
#include "a31t21x_pwr.h"
#include "a31t21x_scu.h"
#include "a31t21x_gpio.h"
//#include "A31T21x_hal_fmc.h"
//#include "A31T21x_hal_dmac.h"
#include "a31t21x_wdt.h"
//#include "A31T21x_hal_wt.h"
#include "a31T21x_timer.h"
//#include "A31T21x_hal_timer2n.h"
//#include "A31T21x_hal_timer30.h"
#include "A31T21x_usart1n.h"
#include "A31T21x_uart.h"
//#include "A31T21x_hal_spi.h"
//#include "A31T21x_hal_i2c.h"
//#include "A31T21x_hal_adc.h"
//#include "A31T21x_hal_lcd.h"
//#include "A31T21x_hal_crc.h"
#include "A31T21x_led.h"


#include "common.h"
#include "a31t21x_driver_common.h"
#include "debug.h"
#include "user.h"
#include "led_driver.h"
#include "touch.h"
#include "user_timer.h"


#ifdef __cplusplus
extern "C"
{
#endif	


#if (1)
/* Private typedef -----------------------------------------------------------*/
enum system_clock
{
	SYS_CLK_32MHZ,
	SYS_CLK_16MHZ,
	SYS_CLK_8MHZ,
	SYS_CLK_4MHZ,
	SYS_CLK_2MHZ,
	SYS_CLK_1MHZ
};
typedef enum
{	
	TASK_LED = 0,
	TASK_TOUCH,
	TASK_DEBUG,
	TASK_MAX
} e_task;
#endif


/* Public variables ---------------------------------------------------------*/
extern uint32_t SystemCoreClock;		/*!< System Clock Frequency (Core Clock)  */
extern uint32_t SystemPeriClock;			/*!< System Clock Frequency (Peri Clock)  */
/* Public function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if (0)
#define APP_LED_CONFIG												\
    {																					\
        .tPRESD		= 15,												\
				.tCOMPWID	= 99,												\
				.tLEDPD		= 1000,											\
				.tOVERLAP	= 0,												\
				.tSR_INTE	= 1,												\
				.tMODE		= LED_AUTO_MODE,						\
				.clk_sel	= LED_CLK_MCCLK,						\
				.irq_prio	= DRIVER_PRIORITY_LOWEST,		\
    }
 
#define APP_UART_CONFIG												\
		{																					\
        .baudrate = 115200,										\
        .data_bit = UART_DATA_BIT_8,					\
        .parity_bit = UART_PARITY_BIT_NONE,		\
        .stop_bit = UART_STOP_BIT_1,					\
        .irq_prio = DRIVER_PRIORITY_LOWEST,		\
    }
#endif       
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Port_Init(void);
void System_Setting_Config(void);


#ifdef __cplusplus
}
#endif


#endif /* _MAIN_H_ */


