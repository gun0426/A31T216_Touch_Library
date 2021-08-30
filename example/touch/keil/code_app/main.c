/**********************************************************************
* @file		main.c
* @brief	Contains all macro definitions and function prototypes
* 			support for A31T21x firmware library
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/
#include <stdio.h>
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Change_SysClock(enum system_clock sysclk);
/* Private variables ---------------------------------------------------------*/
str_flag t_sysbit;


/**********************************************************************
 * @brief		SysTick_Configure
 * @param[in]	None
 * @return 	None
 **********************************************************************/
void SysTick_Configure(void)
{
	/*1msec interrupt */
	SysTick_Config(SystemCoreClock/1000);
}

/** *******************************************************************
 * @brief		Specifies the variable location for library use.
 * @param[in]	none
 * @return		void
 *
 *********************************************************************/
void Init_Library(void)
{
	ts.actv_ch_bit = (	((u32)CS00_USE<<0)+	((u32)CS01_USE<<1)+	((u32)CS02_USE<<2)+	((u32)CS03_USE<<3)+	\
						((u32)CS04_USE<<4)+	((u32)CS05_USE<<5)+	((u32)CS06_USE<<6)+	((u32)CS07_USE<<7)+	\
						((u32)CS08_USE<<8)+	((u32)CS09_USE<<9)+	((u32)CS10_USE<<10)+	((u32)CS11_USE<<11)+	\
						((u32)CS12_USE<<12)+	((u32)CS13_USE<<13)+	((u32)CS14_USE<<14)+	((u32)CS15_USE<<15)+	\
						((u32)CS16_USE<<16)+	((u32)CS17_USE<<17)+	((u32)CS18_USE<<18)+	((u32)CS19_USE<<19)+	\
						((u32)CS20_USE<<20)+	((u32)CS21_USE<<21)+	((u32)CS22_USE<<22)+	((u32)CS23_USE<<23));
						
	ts.shield_ch_bit= (	((u32)SHLD_CS00<<0)+	((u32)SHLD_CS01<<1)+	((u32)SHLD_CS02<<2)+	((u32)SHLD_CS03<<3)+	\
						((u32)SHLD_CS04<<4)+	((u32)SHLD_CS05<<5)+	((u32)SHLD_CS06<<6)+	((u32)SHLD_CS07<<7)+	\
						((u32)SHLD_CS08<<8)+	((u32)SHLD_CS09<<9)+	((u32)SHLD_CS10<<10)+	((u32)SHLD_CS11<<11)+	\
						((u32)SHLD_CS12<<12)+	((u32)SHLD_CS13<<13)+	((u32)SHLD_CS14<<14)+	((u32)SHLD_CS15<<15)+	\
						((u32)SHLD_CS16<<16)+	((u32)SHLD_CS17<<17)+	((u32)SHLD_CS18<<18)+	((u32)SHLD_CS19<<19)+	\
						((u32)SHLD_CS20<<20)+	((u32)SHLD_CS21<<21)+	((u32)SHLD_CS22<<22)+	((u32)SHLD_CS23<<23));
	
	ts.step 			= 0;
	ts.slide_touch 		= 0;
	ts.flag.slide_en 	= TS_SLIDE_EN;
	ts.actv_ch_cnt 		= ACT_KEY_N;
	ts.delta_buf 		= &user.del_data[0];
	ts.raw_buf 			= &user.raw_data[0];
	ts.base_buf 		= &user.base_data[0];
	ts.pre_buf 			= &user.pre_data[0];
	ts.sco_buf 			= &user.zero_sco[0];
	ts.thd_buf 			= &user.ch_thd[0];
	ts.hold_cnt_buf 	= &user.base_hold_cnt[0];	
	ts.debounce_cnt_buf = &user.debounce_cnt[0];
	ts.ch_num_buf 		= &user.actv_ch_num[0];
	func_p_user_filter 	= TS_User_Filter;

	ts.raw_sum_buf = &user.raw_sum_data[0];
	ts.raw_avrg_buf = &user.raw_avrg_data[0];	

#if LED_DRV_EN == 1	// ###GUN
	fpRenewLED = led_RenewData;
	led.flag.seg_current = 1;
	led.flag.op_mode = 2;
	led.com_sel = COMSEL_00_12;
	led.seg_sel = SEGSEL_00_15;
#endif

	t_sysbit.cal_ok = 0;
	t_sysbit.led_io = 0;
	t_sysbit.time_div = TS_LED_TIME_DIV;	
}
/** *******************************************************************
 * @brief		set LED I/O
 * @param[in]	none
 * @return		void
 *
 *********************************************************************/
void Set_TS_IO (void)
{
	#if (CS00_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTA, PIN8, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTA, PIN8, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS01_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTA, PIN9, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTA, PIN9, GPIO_MODE_FUNC);
	}
	#endif	/* GUN */
	#if (CS02_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTA, PIN10, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTA, PIN10, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS03_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTA, PIN11, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTA, PIN11, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS04_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTB, PIN0, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTB, PIN0, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS05_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTB, PIN1, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTB, PIN1, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS06_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTB, PIN2, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTB, PIN2, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS07_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTB, PIN4, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTB, PIN4, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS08_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTB, PIN5, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTB, PIN5, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS09_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTB, PIN10, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTB, PIN10, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS10_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTB, PIN11, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTB, PIN11, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS11_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTC, PIN0, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTC, PIN0, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS12_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTC, PIN1, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTC, PIN1, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS13_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTC, PIN2, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTC, PIN2, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS14_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTC, PIN3, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTC, PIN3, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS15_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTC, PIN4, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTC, PIN4, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS16_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTD, PIN0, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTD, PIN0, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS17_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTD, PIN1, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTD, PIN1, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS18_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTD, PIN2, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTD, PIN2, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS19_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTD, PIN3, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTD, PIN3, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS20_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTD, PIN4, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTD, PIN4, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS21_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTD, PIN5, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTD, PIN5, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS22_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTE, PIN0, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTE, PIN0, GPIO_MODE_FUNC);
	}
	#endif
	#if (CS23_USE == 1)
	{
	    HAL_GPIO_SetFunction(PORTE, PIN1, Pn_MUX_AF5);
	    HAL_GPIO_SetMode(PORTE, PIN1, GPIO_MODE_FUNC);
	}
	#endif

	/* CAPN */
	HAL_GPIO_SetFunction(PORTA, PIN0, Pn_MUX_AF5);
	HAL_GPIO_SetMode(PORTA, PIN0, GPIO_MODE_FUNC);

	/* Touch/LED Block Set */
	SCU->PER2 &= ~(1<<25);
	SCU->PCER2 &= ~(1<<25);
	SCU->PER2 |= (1<<25);
	SCU->PCER2 |= (1<<25);

	SCU->PER2 &= ~(1<<29);
	SCU->PCER2 &= ~(1<<29);
	SCU->PER2 |= (1<<29);
	SCU->PCER2 |= (1<<29);
}
/** *******************************************************************
 * @brief		
 * @param[in]	none
 * @return		void
 *
 *********************************************************************/
void Set_Library (void)
{
	////////////////  DON'T TOUCH ME   //////////////////////
	TS_Set_Mode(NORMAL_MODE); 				
	if (ts.mode == HISENSE_MODE)
	{
		TS_Set_Hisense_Level(3200, 2725);	/* VHS, VREF */	
	}
	else
	{
		TS_Set_Comp_RefV(3840);				/* 3840/5 = 0x300 */		
	}	
	///////////////////////////////////////////////////////////////
	
	TS_Set_Shield(CH_SHIELD_DIS, MESH_SHIELD_DIS);
	TS_Set_Sum_Count(8);
	TS_Set_Inact_Port(OUT_LOW);			/* 0:FLOATING, 1:OUT-LOW, 2:OUT-HIGH */
	TS_Set_Common_THD(100); 	
	TS_Set_CH_THD(0, 284*0.7);			/* 0: CS1 when CS1 @CS1,CS4,CS7,CS10,CS13,CS16,CS19,CS23 are in use */
	TS_Set_CH_THD(1, 150*0.7);			/* 1: CS4 */
	TS_Set_CH_THD(2, 300*0.7);			/* 2: CS7 */
	TS_Set_CH_THD(3, 274*0.7);			/* 3: CS10 */
	TS_Set_CH_THD(4, 185*0.7);			/* 4: CS13 */
	TS_Set_CH_THD(5, 170*0.7);			/* 5: CS16 */
	TS_Set_CH_THD(6, 130*0.7);			/* 6: CS19 */
	TS_Set_NoiseTHD_Rate(30);
	TS_Set_BaseHold_Rate(70);
	TS_Set_Base_Tracking(1, 20, 500);	/* 1st.param if < noise THD, 2nd.param if > noise THD, 3rd.param: period */
	TS_Set_Release_Rate(50);			/* 50% of THD */
	TS_Set_Reverse_Rate(50);			/* 50% of THD */
	TS_Set_Reverse_Time(100); 			/* ms */
	TS_Set_Debounce(0, 0);
	TS_Set_IIR(0);
	TS_Set_Init_Delay(300);				/* 300ms */
	#if LED_DRV_EN == 1
	LED_Set_Current(1);					/* 0, 1, 2, 3 */
	LED_Set_Actv_Time(8, 8, 15);
	#endif
	#if DBG_MSG_EN == 1
	DBG_Set_Baudrate(500000);
	#endif

	UT_Run_Timer(); 					/* use systick, touch library works with user-timer. */
}
/** *******************************************************************
 * @brief		
 * @param[in]	none
 * @return		void
 *
 *********************************************************************/
void Change_SysClock(enum system_clock sysclk)
{
	struct scu_clock_cfg cfg;

	switch (sysclk)
	{
		case SYS_CLK_32MHZ :
			cfg.mclk_sel = SCU_CLOCK_HSI;
			cfg.mclk_div = SCU_CLOCK_DIV_1;
			cfg.mclk_hz = 32000000;
			break;
		case SYS_CLK_16MHZ :
			cfg.mclk_sel = SCU_CLOCK_HSI;
			cfg.mclk_div = SCU_CLOCK_DIV_2;
			cfg.mclk_hz = 16000000;
			break;
		case SYS_CLK_8MHZ :
			cfg.mclk_sel = SCU_CLOCK_HSI;
			cfg.mclk_div = SCU_CLOCK_DIV_4;
			cfg.mclk_hz = 8000000;
			break;
		case SYS_CLK_4MHZ :
			cfg.mclk_sel = SCU_CLOCK_HSI;
			cfg.mclk_div = SCU_CLOCK_DIV_8;
			cfg.mclk_hz = 4000000;
			break;
		case SYS_CLK_2MHZ :
			cfg.mclk_sel = SCU_CLOCK_HSI;
			cfg.mclk_div = SCU_CLOCK_DIV_16;
			cfg.mclk_hz = 2000000;
			break;
		case SYS_CLK_1MHZ :
			cfg.mclk_sel = SCU_CLOCK_HSI;
			cfg.mclk_div = SCU_CLOCK_DIV_32;
			cfg.mclk_hz = 1000000;
			break;
	}
	/* Update Main Clock */
	HAL_SCU_UpdateMainClock(&cfg);
}
/** *******************************************************************
 * @brief		
 * @param[in]	none
 * @return		void
 *
 *********************************************************************/
#define WAKEUP_PERIOD	0.3
void Check_LPM_Entry(void)
{
	/* LPM Mode */
	if(t_sysbit.lpm_mode == 1)
	{
		if (ts.flag.wakeup == 1)
		{
			ts.flag.wakeup = 0;		
			/* System clock change */
#if MCLK_SPEED == MCLK_16MHZ
			Change_SysClock(SYS_CLK_16MHZ);
#endif
#if MCLK_SPEED == MCLK_32MHZ
			Change_SysClock(SYS_CLK_32MHZ);
#endif

#if LED_DRV_EN == 1
			/* LED Start */
			LED->LEDCON1 = (led.flag.op_mode<<2) |		/* 0:Auto, 1:HandShake, 2:StopCount, 3:SmartShare, 4:Alone */
							(1<<1) |					/* Enable 	*/
							(0<<0);						/* Stop		*/	
			LED->LEDCON1 |= BIT(0); 					/* Start	*/	
#endif			
			TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2));
			ts.step = 2;
			ts.actv_ch_idx = 0;
			TS(SFR_TS_MODE) |= BIT(7);	/* GPIO Mode Alterative */				
			TS(SFR_TS_CON) = 0;			
			NVIC_SetPriority(TOUCH_IRQn, 3);
			NVIC_ClearPendingIRQ(TOUCH_IRQn);			
			NVIC_EnableIRQ(TOUCH_IRQn);
			t_sysbit.lpm_mode = 0;		
		}
		else
		{	
			/* Watchdog timer setting */
			struct wdt_cfg cfg;

			HAL_SCU_EnableClock(SCU_CLOCK_LSI);
			HAL_SCU_DisableResetSource(~(1<<3));
			HAL_SCU_EnableWakeupSource((1<<1));

			cfg.clk_src = WDT_CLK_SRC_WDTRC;
			cfg.clk_div = WDT_CLK_DIV_4;					/* fwdtsrc(31250)/4 = fwdt = 7812Hz */
			cfg.mode = WDT_MODE_IRQ,
      		cfg.window_count = 0x00FFFFFF,
			cfg.irq_prio = DRIVER_PRIORITY_LOWEST,

			HAL_WDT_Init(&cfg, NULL, NULL);
			HAL_WDT_Start(7812 * WAKEUP_PERIOD);

			/* Enter DeepSlee Mode */
			HAL_PWR_EnterDeepSleepMode();
				
			/* After wakeup */
#if MCLK_SPEED == MCLK_16MHZ
			Change_SysClock(SYS_CLK_16MHZ);
#endif
#if MCLK_SPEED == MCLK_32MHZ
			Change_SysClock(SYS_CLK_32MHZ);
#endif

			/* WDT OFF */
			WDT->CR = 0			
				|(0x5A69<<16)
				|(0x25<<10)
				|(0x1A<<4);			
		}
	}
	/* Active Mode */
	else
	{
		if (UT_Get_Status() & UT_8)
		{
			if (UT_Is_Expired(UT_8) == 1)
			{
				/*******************************
					switch to LPM mode	
				*******************************/
#if LED_DRV_EN == 1				
				/* LED Stop */
				LED->LEDCON1 &= ~(BIT(1)+BIT(0));		/* LED Stop -> I/O GPIO */
#endif				
				NVIC_DisableIRQ(TOUCH_IRQn);
TS(SFR_TS_MODE) &= ~BIT(7);				
				TS(SFR_TS_CH_SEL) = CHSEL_00_23;
				TS(SFR_TS_CON) = 0;					
				TS(SFR_TS_MODE) 	&= ~(BIT(4)+BIT(3)+BIT(2));	/* simultaneous mode */	
				TS(SFR_TS_MODE) 	|= BIT(4);						/* simultaneous mode */	
				TS(SFR_TS_CON) 	= _TS_START; 		
				while (!(TS(SFR_TS_CON)&BIT(2)));	
				ts.simultaneous_raw = (TSRAW(ts.ch_num_buf[0]*2));
				ts.simultaneous_base = ts.simultaneous_raw;
				ts.flag.wakeup = 0;
				t_sysbit.lpm_simul = 1;
				TS(SFR_TS_CON) = 0x00;
				t_sysbit.lpm_mode = 1;
while (dbg.flag.busy == 1){}				
				/* System clock change */
#if MCLK_SPEED == MCLK_16MHZ
				Change_SysClock(SYS_CLK_16MHZ);
#endif
#if MCLK_SPEED == MCLK_32MHZ
				Change_SysClock(SYS_CLK_32MHZ);
#endif
			}
		}
		else
		{
			UT_Set_Timer(UT_8, LPM_ENTRY_DELAY);
		}

		if(ts.detect_key != 0)
		{
			UT_Set_Timer(UT_8, LPM_ENTRY_DELAY);
		}
	}
}
/** *******************************************************************
 * @brief		main function
 * @param[in]	none
 * @return		void
 *
 *********************************************************************/
int main (void)
{
	e_task task;

	/* Initialize all port */
	Port_Init();

	/* Configure system settings */
	System_Setting_Config();

#if MCLK_SPEED == MCLK_16MHZ
	Change_SysClock(SYS_CLK_16MHZ);
#endif
#if MCLK_SPEED == MCLK_32MHZ
	Change_SysClock(SYS_CLK_32MHZ);
#endif

	/*****************************	
 		TOUCH/LED Configuration 		
 	*****************************/
	SysTick_Config(SystemCoreClock/1000);	/* 1ms tick */	
	Set_TS_IO();
	Init_Library();
	Set_Library();
	/*-------------------------*/

	/* Global Interrupt Enable */
	__enable_irq();
		
	task = TASK_LED;
	while(1)
	{
		switch(task)
		{
			case TASK_LED :
				#if LED_DRV_EN == 1
				LED_Do_Task();
				#endif
				break;			
			case TASK_TOUCH :
				TS_Do_Task();
				//Check_LPM_Entry();
				break;			
			case TASK_DEBUG :
				#if DBG_MSG_EN == 1
				DBG_Do_Task();
				#endif
				break;			
			default :
				break;
		}
		if ((t_sysbit.lpm_mode == 0)||(task != TASK_TOUCH))
		{
			task++;
			if(task == TASK_MAX)
			{
				task = TASK_LED;
			}
		}
	}
}


