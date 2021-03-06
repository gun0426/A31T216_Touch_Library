/** *******************************************************************
 * @file		led_driver.c
 * @brief		Describes LED driver register setting and operation
 * @author		ABOV Sensor Solution Team
 *
 * Copyright(C) 2020, ABOV Semiconductor
 * All rights reserved.
 *
 **********************************************************************/
#include "A31T21x.h"
#include "A31T21x_gpio.h"
#include "A31T21x_scu.h"
#include "A31T21x_led.h"
#include "common.h"
#include "a31t21x_driver_common.h"
#include "touch.h"
#include "led_driver.h"
#include "user.h"


#define	LED_CCSTRIM					(*(volatile unsigned long *)0x40006074)
u8 led_lib_ver = LED_LIB_VER;
str_ledx led;	
void (*fpRenewLED) (void);


/** *******************************************************************
 * @brief		LED Interrupt call back
 * @param[in]	
 * @return
 *
 *********************************************************************/
void app_led_irq_handler(uint32_t event, void *context)
{
	if (event & LED_EVENT_DONE)
	{
		LED->SR &= ~(1<<2);
		t_sysbit.led_end = 1;
		if ((t_sysbit.cal_ok == 1) && (t_sysbit.time_div != 0))
		{
			TS(SFR_SHLD_CON) |= BIT(24); 	
			TS_Start_Touch(ts.actv_ch_idx);
		}
	}
}
/** *******************************************************************
 * @brief		Set the LED operation duration
 * @param[in]	com_cnt : number of COMs to use
				duration : LED operation section in milli second.
 * @return		void
 *
 *********************************************************************/
void LED_Set_Actv_Time(u8 com_cnt, u8 slot_ms, u8 period)
{
	struct led_cfg ledcfg = APP_LED_CONFIG;

	HAL_LED_Init(&ledcfg, app_led_irq_handler, NULL);


	/* *************************************************
	slotMs = (1/16M)*(PRESD+1)*(COMPWID+1)*COM_N*1000
	slotMs*16000/((PRESD+1)*COM_N)-1 = COMPWID
	if PRESD = 127, then (16000>>7) = 125;
	************************************************* */
#if MCLK_SPEED == MCLK_16MHZ
	LED->PRESD		= 127;		
	LED->COMPWID	= (u8)(((u16)slot_ms*125/(u16)com_cnt)-1);	
	LED->LEDPD		= (u32)(period * 16000);
#endif	
#if MCLK_SPEED == MCLK_32MHZ
	LED->PRESD		= 255;		
	LED->COMPWID	= (u8)(((u16)slot_ms*125/(u16)com_cnt)-1);	
	LED->LEDPD		= (u32)(period * 32000); 
#endif

	if (t_sysbit.time_div != 0)
	{
		led.flag.op_mode = 2;
	}
	else
	{
		led.flag.op_mode = 0;
	}	
	t_sysbit.led_en = 1;
}
/** *******************************************************************
 * @brief		Set the LED seg current.
 * @param[in]	current_indx : define the current amount of the SEG-pads.
 * @return		void
 *
 *********************************************************************/
void LED_Set_Current(u8 currLev)
{
	/*----------------------------
	0 : 8.47 mA
	1 : 13.61 mA
	2 : 22.09 mA
	3 : 26.20 mA
	----------------------------*/
	if (currLev > 3)
	{
		led.flag.seg_current = 3;
	}
	else
	{
		led.flag.seg_current = currLev;
	}
}
/** *******************************************************************
 * @brief		When the LED is in the inactive state, it sets the SEG/
				COM I/O state.
 * @param[in]	none
 * @return		void
 *
 *********************************************************************/
void LED_Init_IO(void)
{
 	/* ****************************************************************************
		SEG00-PE7, SEG01-PE6, SEG02-PE5, SEG03-PE4, SEG04-PE3, SEG05-PE2,SEG06-PE1, 
		SEG07-PE0, SEG08-PD5, SEG09-PD4, SEG10-PD3, SEG11-PD2, SEG12-PD1,SEG13-PD0
		SEG14-PC4, SEG15-PC3
	******************************************************************************/
#if (0)	/* Hisense Mode ???? floating */
	if (led.seg_sel & BIT(0))	/* PE7 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN7);
	}
	if (led.seg_sel & BIT(1))	/* PE6 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN6);
	}
	if (led.seg_sel & BIT(2))	/* PE5 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN5);
	}
	if (led.seg_sel & BIT(3))	/* PE4 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN4);
	}
	if (led.seg_sel & BIT(4))	/* PE3 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN3);
	}
	if (led.seg_sel & BIT(5))	/* PE2 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN2);
	}
	if (led.seg_sel & BIT(6))	/* PE1 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN1);
	}
	if (led.seg_sel & BIT(7))	/* PE0 */
	{
		HAL_GPIO_ClearPin(PORTE, PIN0);
	}
	if (led.seg_sel & BIT(8))	/* PD5 */
	{
		HAL_GPIO_ClearPin(PORTD, PIN5);
	}
	if (led.seg_sel & BIT(9))	/* PD4 */
	{
		HAL_GPIO_ClearPin(PORTD, PIN4);
	}	
	if (led.seg_sel & BIT(10))	/* PD3 */
	{
		HAL_GPIO_ClearPin(PORTD, PIN3);
	}
	if (led.seg_sel & BIT(11))	/* PD2 */
	{
		HAL_GPIO_ClearPin(PORTD, PIN2);
	}
	if (led.seg_sel & BIT(12))	/* PD1 */
	{
		HAL_GPIO_ClearPin(PORTD, PIN1);
	}
	if (led.seg_sel & BIT(13))	/* PD0 */
	{	
		HAL_GPIO_ClearPin(PORTD, PIN0);
	}
	if (led.seg_sel & BIT(14))	/* PC4 */
	{
		HAL_GPIO_ClearPin(PORTC, PIN4);
	}
	if (led.seg_sel & BIT(15))	/* PC3 */
	{
		HAL_GPIO_ClearPin(PORTC, PIN3);
	}
#endif	
	/* ****************************************************************************
		COM00-PE7, COM01-PE6, COM02-PE5, COM03-PE4, COM04-PE3, COM05-PE2,COM06-PE1, 
		COM07-PE0, COM08-PD5, COM09-PD4, COM10-PD3, COM11-PD2, COM12-PD1
	******************************************************************************/
	if (led.com_sel & BIT(0))	/* PE7 */
	{
		HAL_GPIO_SetPin(PORTE, PIN7);
	}
	if (led.com_sel & BIT(1))	/* PE6 */
	{
		HAL_GPIO_SetPin(PORTE, PIN6);
	}
	if (led.com_sel & BIT(2))	/* PE5 */
	{
		HAL_GPIO_SetPin(PORTE, PIN5);
	}
	if (led.com_sel & BIT(3))	/* PE4 */
	{
		HAL_GPIO_SetPin(PORTE, PIN4);
	}
	if (led.com_sel & BIT(4))	/* PE3 */
	{
		HAL_GPIO_SetPin(PORTE, PIN3);
	}
	if (led.com_sel & BIT(5))	/* PE2 */
	{
		HAL_GPIO_SetPin(PORTE, PIN2);
	}
	if (led.com_sel & BIT(6))	/* PE1 */
	{
		HAL_GPIO_SetPin(PORTE, PIN1);
	}
	if (led.com_sel & BIT(7))	/* PE0 */
	{
		HAL_GPIO_SetPin(PORTE, PIN0);
	}
	if (led.com_sel & BIT(8))	/* PD5 */
	{
		HAL_GPIO_SetPin(PORTD, PIN5);
	}
	if (led.com_sel & BIT(9))	/* PD4 */
	{
		HAL_GPIO_SetPin(PORTD, PIN4);
	}
	if (led.com_sel & BIT(10))	/* PD3 */
	{
		HAL_GPIO_SetPin(PORTD, PIN3);
	}
	if (led.com_sel & BIT(11))	/* PD2 */
	{
		HAL_GPIO_SetPin(PORTD, PIN2);
	}
	if (led.com_sel & BIT(12))	/* PD1 */
	{
		HAL_GPIO_SetPin(PORTD, PIN1);
	}

	t_sysbit.led_io = 1;
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void LED_Do_Task(void)
	
 - DESCRIPTION
	: Auto		- auto iteration continuously.
	: HandShake 	- touch end-signal make led-start.
	: StopCount 	- re-start after (STPD+1) count from led operation end. (***)
	: SmartShare	- StopCount(Base)+HandShake
	: Alone 		- 1 iteration and stop.
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void LED_Init_Reg(void)	
{
#if ((COM00_EN == 1) || (SEG00_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN7, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN7, GPIO_MODE_FUNC);			
	}
#endif
#if ((COM01_EN == 1) || (SEG01_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN6, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN6, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM02_EN == 1) || (SEG02_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN5, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN5, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM03_EN == 1) || (SEG03_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN4, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN4, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM04_EN == 1) || (SEG04_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN3, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN3, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM05_EN == 1) || (SEG05_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN2, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN2, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM06_EN == 1) || (SEG06_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN1, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN1, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM07_EN == 1) || (SEG07_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTE, PIN0, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTE, PIN0, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM08_EN == 1) || (SEG08_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTD, PIN5, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTD, PIN5, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM09_EN == 1) || (SEG09_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTD, PIN4, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTD, PIN4, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM10_EN == 1) || (SEG10_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTD, PIN3, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTD, PIN3, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM11_EN == 1) || (SEG11_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTD, PIN2, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTD, PIN2, GPIO_MODE_FUNC);	
	}
#endif
#if ((COM12_EN == 1) || (SEG12_EN == 1))
	{
		HAL_GPIO_SetFunction(PORTD, PIN1, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTD, PIN1, GPIO_MODE_FUNC);	
	}
#endif
#if (SEG13_EN == 1)
	{
		HAL_GPIO_SetFunction(PORTD, PIN0, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTD, PIN0, GPIO_MODE_FUNC);	
	}
#endif
#if (SEG14_EN == 1)
	{
		HAL_GPIO_SetFunction(PORTC, PIN4, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTC, PIN4, GPIO_MODE_FUNC);	
	}
#endif
#if (SEG15_EN == 1)
	{
		HAL_GPIO_SetFunction(PORTC, PIN3, Pn_MUX_AF5);
		HAL_GPIO_SetMode(PORTC, PIN3, GPIO_MODE_FUNC);	
	}
#endif

	LED->LEDCON1 = 	(led.flag.op_mode<<2) |		/* 0:Auto, 1:HandShake, 2:StopCount, 3:SmartShare, 4:Alone			*/
			  		(1<<1) |					/* Enable                                                           */
			  		(0<<0);						/* Start			                                                */
	LED->LEDCON2 = 	(1<<7) |					/* 1: overlap time = 1/8MHz Fix. 125ns                              */
			  		(7<<4) |					/* overlap time, 0:64us, 1:32, 2:24, 3:21, 4:12, 5:9, 6:6, 7:3us    */
			  		(0<<3);						/* 0:SEG floating, 1: SEG LOW                                       */
	LED->LEDCON3 = 	(5<<4) |					/* sst time, 0:1us, 1:2, 2:4, 3:6, ... 15:32us			*/
			 		(5<<0);						/* clear time, 0:1us, 1:2, 2:4, 3:6, ... 15:32us    	*/

	//LED->COMDRIVE		/* LED Current : GPIO N-MOS PAD + */

	LED->SR = 0;
	LED->SR |= (1<<1);
	LED->COMOE  = led.com_sel;
	LED->SEGOE = led.seg_sel;
	LED->COMER = led.com_sel; 

	LED->COMDIMM0 = 0;
	LED->COMDIMM1 = 0;
	LED->COMDIMM2 = 0;
	LED->COMDIMM3 = 0;

	LED->DISPRAM0 = LED_INIT_DATA;
	LED->DISPRAM1 = LED_INIT_DATA;
	LED->DISPRAM2 = LED_INIT_DATA;
	LED->DISPRAM3 = LED_INIT_DATA;
	LED->DISPRAM4 = LED_INIT_DATA;
	LED->DISPRAM5 = LED_INIT_DATA;
	LED->DISPRAM6 = LED_INIT_DATA;
	LED->DISPRAM7 = LED_INIT_DATA;
	LED->DISPRAM8 = LED_INIT_DATA;
	LED->DISPRAM9 = LED_INIT_DATA;
	LED->DISPRAM10 = LED_INIT_DATA;
	LED->DISPRAM11 = LED_INIT_DATA;	
	LED->DISPRAM12 = LED_INIT_DATA;

	LED->PORTCTRL = 0x0000;	/* 0: floating, 1: Output */

	LED_CCSTRIM = led.flag.seg_current;	/* ### GUN */

	NVIC_SetPriority(LED_IRQn, 2);
	NVIC_EnableIRQ(LED_IRQn);
	__enable_irq();
	
	LED->LEDCON1 |= BIT(0);		/* LED Start ! */
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void LED_Do_Task(void)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void LED_Do_Task(void)
{
	static u8 staStep = 0;

	if (t_sysbit.led_en == 0)
	{
		return;
	}
	switch (staStep)
	{
		case 0:
			LED_Init_IO();
			staStep++;
			break;
		case 1:
			if (t_sysbit.cal_ok == 1)
			{
				LED_Init_Reg();
				staStep++;	
			}
			break;
		case 2:
			fpRenewLED();
			break;
		default:
			staStep = 0;
			break;
	}
}

