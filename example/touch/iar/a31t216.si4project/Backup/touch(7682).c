
/** *******************************************************************
 * @file		touch.c
 * @brief		touch library(include API)
 * @author		ABOV Sensor Solution Team
 *
 * Copyright(C) 2020, ABOV Semiconductor
 * All rights reserved.
 *
 **********************************************************************/
#include <stdlib.h>
#include "A31T21x.h"
#include "a31t21x_gpio.h"
#include "a31t21x_driver_common.h"
#include "common.h"
#include "user_timer.h"
#include "touch.h"


struct ts_param ts;
struct ts_reg ts_reg;

void (*func_p_user_filter)(void);

/** *******************************************************************
 * @brief		Touch Interrupt Service Routine
				- Interrupt occurs whenever processing for one channel is completed.
				- in touch/LED time division operation mode
				: If the LED is working, this data is discarded and the touch operation for the 
				  next channel does not proceed until the LED operation is finished.
				: If the LED is not working, it performs a touch operation for the next channel
				- in touch/LED independent operation mode
				: If the LED is not working, it performs a touch operation for the next channel
 * @param[in]	
 * @return	void
 *
 *********************************************************************/
void TOUCH_IRQHandler(void)
{
 	u8 addrIndx;
	
	if (t_sysbit.time_div != 0)
	{
		//if (LED->LEDCON1 & BIT(0))	/* LEDST 1 : LED Driver in operation */
		if (TS(SFR_TS_CON) & BIT(3))
		{
			ts.flag.ts_status = TS_BREAK_TIME;
			NVIC_DisableIRQ(TOUCH_IRQn);
TS(SFR_SHLD_CON) &= ~BIT(24);		/* CH & MESH Disable, Re-Enable in LED ISR */		
			TS(SFR_TS_CON) &= ~(BIT(3)+BIT(0));				
			return;
		}
	}

	/* get touch data */
	addrIndx = ts.ch_num_buf[ts.actv_ch_idx]; 
	ts.raw_buf[ts.actv_ch_idx][0] = (TSRAW(addrIndx*2));
	ts.raw_buf[ts.actv_ch_idx][1] = (TSRAW(addrIndx*2+1));

	TS(SFR_TS_CON) = 0x00;
	
	ts.actv_ch_idx++;
	if (ts.actv_ch_idx == ts.actv_ch_cnt)
	{
		ts.flag.ts_status = TS_COMPLETE;
		ts.actv_ch_idx = 0;
		NVIC_DisableIRQ(TOUCH_IRQn);
	}
	else
	{
		TS(SFR_TS_CH_SEL) = 0;
		addrIndx = ts.ch_num_buf[ts.actv_ch_idx];
		TS(SFR_TS_CH_SEL) |= BIT(addrIndx);				
		TS(SFR_TS_CON) = _TS_START;	
	}
}
/** *******************************************************************
 * @brief		Set raw_sum_cnt register of T418 Touch IP
 * @param[in]	raw_sum_cnt : value to be entered in raw_sum_cnt
 * @return		void
 *
 *********************************************************************/
void TS_Set_Mode(u8 mode)
{
	ts.mode = mode;
}
/** *******************************************************************
 * @brief		Set raw_sum_cnt register of T418 Touch IP
 * @param[in]	raw_sum_cnt : value to be entered in raw_sum_cnt
 * @return		void
 *
 *********************************************************************/
void TS_Set_Shield(u8 ch_shld, u8 mesh_shld)
{
	ts.flag.mesh_shld_en = 0;
	ts.flag.ch_shld_en = 0;
	if ((ch_shld == CH_SHIELD_EN) || (mesh_shld == MESH_SHIELD_EN))
	{		
		if (ch_shld == CH_SHIELD_EN)
		{
			ts.flag.ch_shld_en = 1;
			TS(SFR_SHLD_CON) = ts.shield_ch_bit;
		}
		else
		{
			TS(SFR_SHLD_CON) = 0;
		}
		if (mesh_shld == MESH_SHIELD_EN)
		{
			HAL_GPIO_SetFunction(PORTE, PIN1, Pn_MUX_AF5);
		    HAL_GPIO_SetMode(PORTE, PIN1, GPIO_MODE_FUNC);
			TS(SFR_SHLD_CON) &= ~ BIT(23);	/* CS23 CH-Shield Disable */
			TS(SFR_SHLD_CON) |= 0x80000000;		/* CS23 Mash-Shield enable */
			ts.flag.mesh_shld_en = 1;
		}
		TS(SFR_SHLD_CON) |= BIT(24);			/* 1: CH Shield & Mesh Shield enable */
	}
	else
	{
		TS(SFR_SHLD_CON) &= ~ BIT(24);		/* 1: CH Shield & Mesh Shield enable */	
//TS(SFR_SHLD_CON) |= BIT(30);		
	}
}
/** *******************************************************************
 * @brief		Set raw_sum_cnt register of T418 Touch IP
 * @param[in]	raw_sum_cnt : value to be entered in raw_sum_cnt
 * @return		void
 *
 *********************************************************************/
void TS_Set_Sum_Count(u8 raw_sum_cnt)
{
	TS(SFR_TS_SUM_CNT)	= raw_sum_cnt;	
}
/** *******************************************************************
 * @brief		All channels set to one THD value
 * @param[in]	thd : the value to be set   
 * @return		void
 *
 *********************************************************************/
void TS_Set_Common_THD(s16 thd)
{
	u8 i;	
	for (i = 0; i < ts.actv_ch_cnt; i++)
	{
		ts.thd_buf[i] = thd;
	}
}
/** *******************************************************************
 * @brief		Set threshold of each channel
 * @param[in]	ch : channel to be set
 * @param[in]	thd : the value to be set   
 * @return		void
 *
 *********************************************************************/
void TS_Set_CH_THD(u8 ch_idx, s16 thd)
{
	ts.thd_buf[ch_idx] = thd;
}
/** *******************************************************************
 * @brief		Set the noise threshold
 * @param[in]	rate : set the value as rate % of threshold 
 * @return		void
 *
 *********************************************************************/
void TS_Set_NoiseTHD_Rate(u8 rate)
{
	ts.noise_thd_rate = rate;
}
/** *******************************************************************
 * @brief		Set the DIFF value to stop baseline tracking.
 * @param[in]	rate : set the value as rate % of threshold 
 * @return		void
 *
 *********************************************************************/
void TS_Set_BaseHold_Rate(u8 rate)
{
	ts.basehold_rate = rate;
}
#if (0)	/* old version */
/** *******************************************************************
 * @brief		Set two parameters for how to follow the baseline
 * @param[in]	track_step : the size of the value to follow at once
 * @param[in]	track_delay : baseline update interval
 * @return		void
 *
 *********************************************************************/
void TS_Set_Base_Tracking(u8 track_step, u16 track_delay)
{
	ts.track_step = track_step;
	ts.track_delay = track_delay;
}
#endif
/** *******************************************************************
 * @brief		Set two parameters for how to follow the baseline
 * @param[in]	track_step : the size of the value to follow at once
 * @param[in]	track_delay : baseline update interval
 * @return		void
 *
 *********************************************************************/
void TS_Set_Base_Tracking(u8 iir_1, u8 iir_2, u16 period)
{
	ts.base_track_iir_1 = iir_1;
	ts.base_track_iir_2 = iir_2;
	ts.base_track_period = period;
}
/** *******************************************************************
 * @brief		Set the release judgement level
 * @param[in]	rate : set the value as rate % of threshold
 * @return		void
 *
 *********************************************************************/
void TS_Set_Release_Rate(u8 rate)
{
	ts.release_rate = rate;
}
/** *******************************************************************
 * @brief		Set the reverse(touch data > baseline)	judgement level
 * @param[in]	rate : set the value as rate % of threshold
 * @return		void
 *
 *********************************************************************/
void TS_Set_Reverse_Rate(u8 rate)
{
	ts.reverse_rate = rate;
}
/** *******************************************************************
 * @brief		Set the holding time to judge the reverse situation.
 * @param[in]	time : if the reverse condition is maintained during
					 this time, it is judged as a reverse condition
					 - reverse condition : touch data > baseline + (th * reverseRate/100)
 * @return		void
 *
 *********************************************************************/
void TS_Set_Reverse_Time(u16 time)
{
	ts.reverse_time = time;
}
/** *******************************************************************
 * @brief		Set the debounce value for determining touch 
					 detection or touch release.
 * @param[in]	detect_cnt : detection debounce
 * @param[in]	release_cnt : release debounce
 * @return		void
 *
 *********************************************************************/
void TS_Set_Debounce(u8 detect_cnt, u8 release_cnt)
{
	ts.debounce_cnt_detect = detect_cnt;
	ts.debounce_cnt_release = release_cnt;
}
/** *******************************************************************
 * @brief		Set the offset count value of the IIR filter
 * @param[in]	offset_cnt : IIR offset count value
 * @return		void
 *
 *********************************************************************/
void TS_Set_IIR(u8 offset_cnt)
{
	ts.iir_offset_cnt = offset_cnt;
}
/** *******************************************************************
 * @brief		
 * @param[in]	
 * @return		void
 *
 *********************************************************************/
void TS_Set_Init_Delay(uint16_t delay)
{
	ts.init_delay = delay;
}
/** *******************************************************************
 * @brief		
 * @param[in]	
 * @return		void
 *
 *********************************************************************/
void TS_Set_Inact_Port(uint8_t state)
{
 	if (state < 3)
 	{
		ts.inact_port = state;
	}
	else
	{
		ts.inact_port = OUT_LOW;
	}
}
/** *******************************************************************
 * @brief		
 * @param[in]	
 * @return		void
 *
 *********************************************************************/
void TS_Set_Comp_RefV(uint16_t ref_v)
{
	ts.comp_ref = ref_v/5;
}
/** *******************************************************************
 * @brief		
 * @param[in]	
 * @return		void
 *
 *********************************************************************/
void TS_Set_Hisense_Level(uint16_t vhs, uint16_t ref_v)
{
	ts.vhs = vhs/5;
	ts.comp_ref = ref_v/5;
}
u32 TS_Get_Key(void)
{
	return ts.detect_key;
}
/***********touch api end**************/

/** *******************************************************************
 * @brief		Initialize variables related to touch
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Init_Variable(void)
{
	u8 i;
	u8 j;

	ts.step = 0;
	ts.actv_ch_idx = 0;
	for (i=0; i<ts.actv_ch_cnt; i++)
	{
		ts.hold_cnt_buf[i] = 0;
		ts.debounce_cnt_buf[i] = 0;
	}
	ts.flag.base_init = 0;
	ts.detect_flag = 0;
	ts.detect_key = 0;
	ts.reverse_ch_idx = 0xff;

	for (i=0; i<ts.actv_ch_cnt; i++)
	{
		ts.ch_num_buf[i] = 0;
	}
	j = 0;
	for (i=0; i<ts.actv_ch_cnt; i++)
	{
		for (j=j; j<TS_MAX_KEY; j++)
		{
			if (ts.actv_ch_bit & ((u32)1<<j))
			{
				ts.ch_num_buf[i] = j;
				j++;
				break;
			}
		}
	}
}
/** *******************************************************************
 * @brief		Set registers for touch operation.
			- Operating Mode : Normal
			- Inactive CH I/O : ouput low
			- VCOMP : 0380 - 4.38V
			- Frequency : 4MHz
			- +++
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Init_Register(void)
{
	u8 i;
	u8 j;
	u32 temp;

	/* touch register set */
	TS(SFR_TS_CON)		= 0x00;
	TS(SFR_TS_MODE)	= (0<<15)|	/* 1:Channel adjust path enable 														*/
						  (1<<8) |	/* 1:LED FLAG enable */
						  (1<<7) |	/* 1:GPIO Mode Alternative, 0:TS_CH_SEL( TS_CH_SEL 여러개일 때 무조건 0으로 			*/
						  (0<<6) |	/* 1:Gain Calibration Capacitor Enable													*/
						  (1<<5) |	/* 1:S1 Sweep Disable																	*/
						  (0<<2) |	/* 0:normal,  1:high sense,  2:ch-adjust,  3:offset calib,	4:simultaneous sense(#GUN)	*/
						  (0<<0);	/* 0:input, 1:output low, 2:output high 												*/
TS(SFR_TS_MODE) += ts.inact_port;				  

if (t_sysbit.time_div == 0)
{
	TS(SFR_TS_MODE) &= ~BIT(8);
}

						  
	TS(SFR_TS_VHS) = 0x03FF;
	/////TS(SFR_TS_SUM_CNT) 	= 7;				
	TS(SFR_TS_S1_WIDTH) 	= 0xff; 	//0x30; 	/*																						*/
	TS(SFR_TS_SLP_CON)	= (7<<4) |	/* Cap 7:10.5pF, stpe:(+/-)1.5pF, 8:12.0pF,  6:9.0pF								*/
							  (4<<0);	/* Res 0:open,	1:0,  2:5K,  4:100K,  8:200K,  6:33K,  12:67K,	10:40K,  14:28K 	*/
	TS(SFR_TS_VREF)		= ts.comp_ref;	//0x0300; 	/* ### GUN_MARK : CS */							  
	TS(SFR_TS_TRIM) 	= 0x08;
	TS(SFR_TS_CLK_CFG)= 	(0<<7) |	/* 0:TS_CLK, 1:MCLK */
							(0<<4) |	/* 0:Both FreqA and FreqB 1:FreqA Only, 2,3:FreqB Only */
							(0<<3) |	/* 1:Clock Output Enable */
							(2<<0);	/* 0:TS_CLK/1(16MHz), 1:8M, 2:4M, 3:2M, 4:1M, 5:0.5M, 6:0.254, 7:0.125M*/
	TS(SFR_TRIM_OSC)	= 0x20; 	/* FB: touch clock 16MHz, 0x20:16MHz, 0x21:+0.25%, 0x1f:-0.25% */
	TS(SFR_DELTA_OSC)	= 0x1f; 	/* FA: 0x01~0x1F->FB+, 0x3F~0x20->FB- */
	TS(SFR_TLED)		= 0x00; 		 


	j = 0;
 	for (i = 0; i < SFR_TS_MAX+2; i++)	/* CH_SEL, SHLD_CON */	
	{
		if ((i == 3) || (i == 16))
		{
			temp = TS(SFR_TS_CON+i);
			ts_reg.tsReg[j++] 	= (u16)((temp >> 16) & 0x0000ffff);
			ts_reg.tsReg[j++]	= (u16)((temp >> 0) & 0x0000ffff);
		}
		else
		{
			ts_reg.tsReg[j++] 	= (u16)TS(SFR_TS_CON+i);	/* SHLD_CON, CH_SEL */
		}		
	}
        asm("nop");
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void TS_Get_Single_CH(u8 indx)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void TS_Get_Single_CH(u8 idx)
{
	u8 addr_idx;

	addr_idx = ts.ch_num_buf[idx];
	
	/* touch channel select */
	TS(SFR_TS_CH_SEL) |= BIT(addr_idx);
	
	/* touch sensing */
	TS(SFR_TS_CON) = _TS_START;

	while(!(TS(SFR_TS_CON)&TS_IF));
	ts.raw_buf[idx][0] = (TSRAW(ts.ch_num_buf[idx]*2));
	ts.raw_buf[idx][1] = (TSRAW(ts.ch_num_buf[idx]*2+1));
	TS(SFR_TS_CON) = 0x00;

	/* touch channel de-select */
	TS(SFR_TS_CH_SEL) &= ~BIT(addr_idx);
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void TS_Start_Touch(u8 indx)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void TS_Start_Touch(u8 idx)
{
	u8 addr_idx;
	
	addr_idx = ts.ch_num_buf[idx];
	/* touch channel select */
	TS(SFR_TS_CH_SEL) = 0;
	TS(SFR_TS_CH_SEL) |= BIT(addr_idx);

	/* touch sensing */
	t_sysbit.dbg_trigger = 0;
	NVIC_ClearPendingIRQ(TOUCH_IRQn);	
	NVIC_EnableIRQ(TOUCH_IRQn);
	
	TS(SFR_TS_CON) = _TS_START;
//PC->OUTDR ^= (1<<6);	
}
#if (0)
/*************************************************************
 - FUNCTION & CLASS NAME
	: void TS_Config_Mode(void)
	
 - DESCRIPTION
	: normal mode - 외부 Cap 측정, SCO Cap 사용 안함  
	: hisense mode - SCO Cap 사용, CAPN 방전 시 SCO Cap만큼 방전 후, Step으로 방전 함. 
	: 1step - 0.025pF * 4095 = 102.3pF
	
 - PARAMETER & RETURN
	: 
*************************************************************/
#define SCO_BIT 12		// 12bit + 1bit(Extra:51.2pF)
#define SCO_MAX (pow(2,SCO_BIT)-1)
void TS_Config_Mode(void)
{
	u8 i;
	u16 bitDigit;
	
	/*------------------------------
	   measure parasitic capacitance 
		  1. SCO init value  = 0
	   2. Normal Mode Operation
	   3. get touch raw-data(target value)
	------------------------------*/
	for (i = 0; i < TS_MAX_KEY; i++)
	{
		TS_SCO(i) = 0x0000;
	}
	TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2)); 	/* normal mode: 외부 Cap 측정 */
	/* touch sensing : target value */	
	for (i = 0; i < ts.actv_ch_cnt; i++)
	{
		TS_Get_Single_CH(i);
		ts.sco_buf[i] = ts.raw_buf[i][0];
	}
	/*------------------------------
	   search & set SCO value
	   1. SCO init value  = 2^12/2 = 0x0800
	   2. Cal Mode Operation
	------------------------------*/
	//for (i=0; i<TS_MAX_KEY; i++)			// 200804-
	for (i=0; i<ts.actv_ch_cnt; i++)	//200804+
	{
		//TS_SCO(i) = 0x0800; 	// 51.2pF(102.4/2), 2^12/2	// 200804-
		TS_SCO(ts.ch_num_buf[i]) = 0x0800;		// 200804+
	}
	
	TS(SFR_TS_MODE) |= (BIT(3)+BIT(2)); 			/* calibration mode : step 0.025pF */

	bitDigit = 0x0400;
	
	for (;;)
	{
		for (i=0; i<ts.actv_ch_cnt; i++)
		{
			/* judge : ts.sco_buf[i] 보다 */
			TS_Get_Single_CH(i);
			if (ts.raw_buf[i][0] > ts.sco_buf[i])
			{
				TS_SCO(ts.ch_num_buf[i]) += bitDigit;
			}
			else
			{
				TS_SCO(ts.ch_num_buf[i]) -= bitDigit;
			}	
		}
		bitDigit >>= 1;
		if (bitDigit == 0)
		{																									
			break;
		}
	}

	if (ts.mode == ADJUST_MODE)
	{
		ts.max_sco = 0;
		for (i=0; i<ts.actv_ch_cnt; i++)
		{
			if (TS_SCO(ts.ch_num_buf[i]) > ts.max_sco)
			{
				ts.max_sco = TS_SCO(ts.ch_num_buf[i]);
			}
		}
		for (i=0; i<ts.actv_ch_cnt; i++)
		{
			TS_SCO(ts.ch_num_buf[i]) = ts.max_sco-TS_SCO(ts.ch_num_buf[i]);
		}	
		TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2)); 	/* adjust mode */
		TS(SFR_TS_MODE) |= BIT(3);				/* adjust mode */
	}
	else if (ts.mode == NORMAL_MODE)
	{
		TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2)); 	/* normal mode */
	}
	else if (ts.mode == HISENSE_MODE)
	{
		TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2));	/* high sense mode */
		TS(SFR_TS_MODE) |= BIT(2);				/* high sense mode */
	}
	else if (ts.mode == SIMULT_MODE)
	{
		TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2));	/* simultaneous mode */
		TS(SFR_TS_MODE) |= BIT(4);				/* simultaneous mode */
	}

	for (i = 0; i < TS_MAX_KEY; i++)
	{
		ts_reg.SCO[i] = TS_SCO(i);
	}
	t_sysbit.cal_ok = 1;
}
#endif
/*************************************************************
 - FUNCTION & CLASS NAME
	: void TS_Config_Mode(void)
	
 - DESCRIPTION
	: normal mode - 외부 Cap 측정, SCO Cap 사용 안함  
	: hisense mode - SCO Cap 사용, CAPN 방전 시 SCO Cap만큼 방전 후, Step으로 방전 함. 
	: 1step - 0.025pF * 4095 = 102.3pF
	
 - PARAMETER & RETURN
	: 
*************************************************************/
#define SCO_BIT 12		// 12bit + 1bit(Extra:51.2pF)
#define SCO_MAX (pow(2,SCO_BIT)-1)
void TS_Config_Mode(void)
{
	u8 i;
	u16 bit_digit;
	
	/* *****************************
	  TS_SCO(x) = 0;
	  Normal Mode
	*******************************/
	for (i = 0; i < TS_MAX_KEY; i++)
	{
		TS_SCO(i) = 0x0000;
	}
	TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2)); 	/* normal mode: 외부 Cap 측정 */

	/* ********************************************************************************************
	  Hisense Mode
	  : Adjust the internal capacitor so that it becomes the same value as the external capacitance
	**********************************************************************************************/
	if (ts.mode == HISENSE_MODE)
	{
		for (i = 0; i < ts.actv_ch_cnt; i++)
		{
			TS_Get_Single_CH(i);
			ts.sco_buf[i] = ts.raw_buf[i][0];
		}
		/* *******************************************************
		   search & set SCO value
		   1. SCO init value  = 2^12/2 = 0x0800
		   2. Offset-cal Mode Operation
		   3. Set the SCO to be the same value as t_ts.sco_buf[i] 
		   4. Switch to hisense mode
		*********************************************************/
		for (i=0; i<ts.actv_ch_cnt; i++)
		{
			TS_SCO(ts.ch_num_buf[i]) = 0x0800;
		}		
		TS(SFR_TS_MODE) |= (BIT(3)+BIT(2)); 			/* offset-cal mode : step 0.025pF */
		bit_digit = 0x0400; 	
		for (;;)
		{
			for (i=0; i<ts.actv_ch_cnt; i++)
			{
				TS_Get_Single_CH(i);
				if (ts.raw_buf[i][0] > ts.sco_buf[i])
				{
					TS_SCO(ts.ch_num_buf[i]) += bit_digit;
				}
				else
				{
					TS_SCO(ts.ch_num_buf[i]) -= bit_digit;
				}	
			}
			bit_digit >>= 1;
			if (bit_digit == 0)
			{																									
				break;
			}
		}	
TS(SFR_TS_VHS)		= ts.vhs;		//0x0280;	/* 3.200V */
TS(SFR_TS_VREF) 	= ts.comp_ref;	//0x0221; 	/* 2.725V */	
		TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3));		/* high sense mode */
		TS(SFR_TS_MODE) |= BIT(2);				/* high sense mode */
	}
	/* ************************************************************************
	  Adjust Mode
	  1. Find the channel with the largest parasitic capacitance.(max_cap_idx)
	  2. Switch to adjust mode
	  3. Get the max_cap_idx channel's touch data(t_ts.sco_buf[0])
	  4. SCO is adjusted so that the data value of the other channel is the same as t_ts.sco_buf[0]
	**************************************************************************/
	else if (ts.mode == ADJUST_MODE)
	{
		ts.max_cap_idx = 0;
		TS_Get_Single_CH(0);
		ts.sco_buf[0] = ts.raw_buf[0][0];
		for (i = 1; i < ts.actv_ch_cnt; i++)
		{
			TS_Get_Single_CH(i);
			if (ts.raw_buf[i][0] < ts.sco_buf[0])
			{
				ts.sco_buf[0] = ts.raw_buf[i][0];
				ts.max_cap_idx = i;
			}
		}
		/*------------------------------
			3. get max_cap ch's data in adjust mode
		------------------------------*/
		TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(2));		/* adjust mode */
		TS(SFR_TS_MODE) |= BIT(3);				/* adjust mode */
		TS_Get_Single_CH(ts.max_cap_idx);
		ts.sco_buf[0] = ts.raw_buf[ts.max_cap_idx][0];

		for (i = 0; i < ts.actv_ch_cnt; i++)
		{
			if (i != ts.max_cap_idx)
			{
				TS_SCO(ts.ch_num_buf[i]) = 0x0800;	
			}
		}
		bit_digit = 0x0400;
		for (;;)
		{
			for (i = 0; i < ts.actv_ch_cnt; i++)
			{
				if (i == ts.max_cap_idx)
				{
					continue;
				}
				TS_Get_Single_CH(i);
				if (ts.raw_buf[i][0] > ts.sco_buf[0])
				{
					TS_SCO(ts.ch_num_buf[i]) += bit_digit;
				}
				else
				{
					TS_SCO(ts.ch_num_buf[i]) -= bit_digit;
				}	
			}
			bit_digit >>= 1;
			if (bit_digit == 0)
			{																									
				break;
			}
		}
	}
	else{}
	
	for (i = 0; i < TS_MAX_KEY; i++)
	{
		ts_reg.SCO[i] = TS_SCO(i);
	}	
	//g_sys_flag.calib_ok = 1;
	
}
/** *******************************************************************
 * @brief		Initialize the baseline with the current touch data
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Init_Base(void)
{
	u8 i;
	if (UT_Get_Status() & UT_INIT_DELAY)
	{
		if (UT_Is_Expired(UT_INIT_DELAY) == 1)
		{
			for (i = 0; i < ts.actv_ch_cnt; i++)
			{
				ts.base_buf[i][0]= ts.raw_buf[i][0];
				ts.base_buf[i][1]= ts.raw_buf[i][1];
				ts.pre_buf[i][0] = ts.raw_buf[i][0];
				ts.pre_buf[i][1] = ts.raw_buf[i][1];	
			}
			ts.flag.base_init = 1;
		}
	}
	else
	{
		UT_Set_Timer(UT_INIT_DELAY, ts.init_delay);
	}
}
#if (0) /* old version */
/** *******************************************************************
 * @brief		Updata the baseline
			- ts.track_delay : update tracking period
			- ts.track_step : update tracking step
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Update_Base(void)
{
 	u8 i;
 	u8 j;
	
	if (UT_Get_Status() & UT_BASE_HOLD)
	{
		if (UT_Is_Expired(UT_BASE_HOLD) == 1)
		{
			UT_Set_Timer(UT_BASE_HOLD, ts.track_delay);
		}
		else
		{
			return;
		}
	}
	else
	{
		UT_Set_Timer(UT_BASE_HOLD, ts.track_delay);
		return;
	}
	
	for (i=0; i<ts.actv_ch_cnt; i++)
	{		
		if (!(BIT(i) & ts.detect_flag))
		{	
			if (ts.hold_cnt_buf[i] == 0)
			{		
				for (j=0; j<2; j++)
				{
					if (ts.raw_buf[i][j] > ts.base_buf[i][j])
					{
						ts.base_buf[i][j] += ts.track_step;
					}
					else
					{
						ts.base_buf[i][j] -= ts.track_step;
					}
				}
			}
			else
			{
				ts.hold_cnt_buf[i]--;
			}
		}
	}
}
#endif
#if (1)		// IIR
/** *******************************************************************
 * @brief		Updata the baseline
			- ts.track_delay : update tracking period
			- ts.track_step : update tracking step
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Update_Base(void)
{
	u8 i;
	u8 j;
	u8 iir_n;
	u16 base_temp;
	
	if (UT_Get_Status() & UT_BASE_HOLD)
	{
		if (UT_Is_Expired(UT_BASE_HOLD) == 1)
		{
			for (i = 0; i < ts.actv_ch_cnt; i++)
			{
				for (j = 0; j < 2; j++)
				{
					ts.raw_avrg_buf[i][j] = (u16)(ts.raw_sum_buf[i][j]/ts.raw_sum_cnt);
					if (!(BIT(i) & ts.detect_flag))
					{
						if (ts.hold_cnt_buf[i] == 0)
						{
							if (ts.delta_buf[i] < ts.thd_buf[i]*ts.basehold_rate/100)
							{
								if (ts.delta_buf[i] < ts.thd_buf[i]*ts.noise_thd_rate/100)
								{
									iir_n = ts.base_track_iir_1;
								}
								else
								{
									iir_n = ts.base_track_iir_2;
								}
								base_temp = ((((u32)ts.base_buf[i][j]*iir_n)+ ts.raw_avrg_buf[i][j])/(iir_n+1));
								if (abs(ts.base_buf[i][j]-ts.raw_buf[i][j]) >  abs(base_temp-ts.raw_buf[i][j]))
								{
									ts.base_buf[i][j] = base_temp;
								}
							}
						}
						else
						{
							ts.hold_cnt_buf[i]--;
						}
					}
					ts.raw_sum_buf[i][j] = 0;
				}
			}
			ts.raw_sum_cnt = 0;
			UT_Set_Timer(UT_BASE_HOLD, ts.base_track_period);
		}
		else
		{
			for (i = 0; i < ts.actv_ch_cnt; i++)
			{
				for (j = 0; j < 2; j++)
				{
					ts.raw_sum_buf[i][j] += ts.raw_buf[i][j];
				}
			}
			ts.raw_sum_cnt++;
		}
	}
	else
	{
		for (i = 0; i < ts.actv_ch_cnt; i++)
		{
			for (j = 0; j < 2; j++)
			{
				ts.raw_sum_buf[i][j] = 0;
			}
		}
		ts.raw_sum_cnt = 0;
		UT_Set_Timer(UT_BASE_HOLD, ts.base_track_period);
	}
}
#endif
/*************************************************************
 - FUNCTION & CLASS NAME
	: u16 get_abs(s16 m_value)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
/*
u16 get_abs(s16 value)
{
	if (value < 0)
	{
		return (u16)(-value);
	}
	else
	{
		return (u16)value;
	}
}
*/
/** *******************************************************************
 * @brief		
 			- raw_data is IIR filtered.
			- calculate diff_data
			- calculate pre_data
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Make_Data(void)
{
 	u8 i;
	s16 temp[2];
	
	for (i = 0; i < ts.actv_ch_cnt; i++)
	{
		temp[0] = (s16)(ts.raw_buf[i][0]-ts.pre_buf[i][0]);
		temp[1] = (s16)(ts.raw_buf[i][1]-ts.pre_buf[i][1]);
		if (ts.iir_offset_cnt != 0)
		{
			ts.raw_buf[i][0] = ((((u32)ts.pre_buf[i][0]*ts.iir_offset_cnt)+ts.raw_buf[i][0])/(ts.iir_offset_cnt+1));
	     	ts.raw_buf[i][1] = ((((u32)ts.pre_buf[i][1]*ts.iir_offset_cnt)+ts.raw_buf[i][1])/(ts.iir_offset_cnt+1));
	   	}
		if (abs(temp[0]) < abs(temp[1]))
		{
			ts.delta_buf[i] = (s16)(ts.base_buf[i][0]-ts.raw_buf[i][0]);
		}
		else
		{
			ts.delta_buf[i] = (s16)(ts.base_buf[i][1]-ts.raw_buf[i][1]);
		}
		ts.pre_buf[i][0] = ts.raw_buf[i][0];
		ts.pre_buf[i][1] = ts.raw_buf[i][1];
	}	
}
/** *******************************************************************
 * @brief		Determine key detection and release.
			Judge the reverse situation
			
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Judge_Key(void)
{
 	u8 i;
	u8 j;
u8 reverse;		

	for (i = 0; i < ts.actv_ch_cnt; i++)
	{
		if (ts.detect_flag & BIT(i))	/* release check */		
		{
			if (ts.delta_buf[i] < ts.thd_buf[i]*ts.release_rate/100)
			{
				ts.debounce_cnt_buf[i]--;
				if (ts.debounce_cnt_buf[i] == 0)
				{
					ts.detect_flag &= ~ BIT(i);	
					ts.detect_key &= ~ BIT(i);
					ts.hold_cnt_buf[i] = 3;	
				}
			}
			else
			{
				ts.debounce_cnt_buf[i] = ts.debounce_cnt_release+1;		
			}
		}
		else						/* press check */
		{
			if (ts.delta_buf[i] > ts.thd_buf[i])	
			{
				ts.debounce_cnt_buf[i]++;
				ts.hold_cnt_buf[i] = 3;			
				if (ts.debounce_cnt_buf[i] > ts.debounce_cnt_detect)
				{
					ts.debounce_cnt_buf[i] = ts.debounce_cnt_release+1;	
					ts.detect_flag |= BIT(i);
					ts.detect_key |= BIT(i);	
				}
			}
			else
			{
				ts.debounce_cnt_buf[i] = 0;
			}
		}	

#if (1)
		/*==================
			Reverse Or 
		==================*/
		/* check if the current channel is in reverse */		
		reverse = 0;	

		#if (0)	
		if (ts.flag.slide_en == 0)
		{
			if(	(abs(ts.base_buf[i][0]-ts.raw_buf[i][0]) > (u16)(ts.thd_buf[i]*ts.reverse_rate/100)) || \
				(abs(ts.base_buf[i][1]-ts.raw_buf[i][1]) > (u16)(ts.thd_buf[i]*ts.reverse_rate/100)))
			{
				reverse = 1;
			}
		}
		else	
		{
			if(	(ts.raw_buf[i][0] > ts.base_buf[i][0] + (u16)(ts.thd_buf[i]*ts.reverse_rate/100)) || \
				(ts.raw_buf[i][1] > ts.base_buf[i][1] + (u16)(ts.thd_buf[i]*ts.reverse_rate/100))) 
			{
				reverse = 1;
			}
		}
		#else
		if(	(ts.raw_buf[i][0] > ts.base_buf[i][0] + (u16)(ts.thd_buf[i]*ts.reverse_rate/100)) || \
				(ts.raw_buf[i][1] > ts.base_buf[i][1] + (u16)(ts.thd_buf[i]*ts.reverse_rate/100))) 
		{
			reverse = 1;
		}
		#endif
		if ((ts.detect_flag!=0) || (ts.debounce_cnt_buf[i]!=0))	//if ((det_flag!=0) || (ts.debounce_cnt_buf[i]!=0))
		{	/* Any key is recognized or in process of recognition */
			ts.reverse_ch_idx = 0xff;
		}
		else	
		{	/* No key is recognized */
			if (ts.reverse_ch_idx == 0xff)
			{	/* there is no reverse channel */
				if (reverse == 1)		
				{
					ts.reverse_ch_idx = i;	/* save the reverse channel */					
					if (ts.raw_buf[i][0] > ts.base_buf[i][0])
					{
						UT_Set_Timer(UT_REVERSE, 100);	
					}
					else
					{
						UT_Set_Timer(UT_REVERSE, ts.reverse_time);	
					}		
				}
			}
			else
			{	/* reverse situation, and it is reverse channel */
				if (i == ts.reverse_ch_idx)
				{	/* check if the reverse state is maintained for ts.reverse_time */
					if (reverse == 1)			
					{
						if (UT_Is_Expired(UT_REVERSE) == 1)
						{	/* if the reverse state is maintained for ts.reverse_time */			
							for (j = 0; j < ts.actv_ch_cnt; j++)
							{	/* the baseline value is adjusted to the rawdata value */
								ts.base_buf[j][0] = ts.raw_buf[j][0];
								ts.base_buf[j][1] = ts.raw_buf[j][1];
ts.raw_sum_buf[j][0] = 0;
ts.raw_sum_buf[j][1] = 0;
ts.raw_sum_cnt = 0;							
							}
						}
					}
					else
					{	/* if the reverse state is not maintained for ts.reverse_time */	
						ts.reverse_ch_idx = 0xff;
						UT_Clear_Timer(UT_REVERSE);
					}
				}
				/* reverse situation, but it is not reverse channel */
				else
				{
					/* there is nothing to do. */
				}
			}
		}	
#endif		
	}

#if (1)		// 200729-
ts.key_num = 0;
for (i = 0; i < ts.actv_ch_cnt; i++)
{
	if (ts.detect_flag & BIT(i))
	{
		ts.key_num = i+1;
		break;
	}
}
#endif

}
/** *******************************************************************
 * @brief		Touch Task
			- initialize variables
			- set touch register
			- touch sensing
			- judge key detect & release
			- update baseline
 * @param[in]	void
 * @return	void
 *
 *********************************************************************/
void TS_Do_Task(void)
{	
	static u8 base_delay_n = 0;
	u8 i;

 	if (t_sysbit.lpm_mode == 1)
	{
		if (t_sysbit.lpm_simul == 1)
		{
			TS(SFR_TS_CON) = _TS_START;				
			while (!(TS(SFR_TS_CON)&BIT(2)));
			ts.simultaneous_raw = (TSRAW(ts.ch_num_buf[0]*2));		
			TS(SFR_TS_CON) = 0x00;
			//if (ts.simultaneous_raw > ts.simultaneous_base+10)
			if (ts.simultaneous_raw > ts.simultaneous_base+5000)
			{
				base_delay_n = 0;
				ts.flag.wakeup = 1;
			}
			else
			{				
				ts.simultaneous_base = ts.simultaneous_raw;		
				base_delay_n++;
				/* initialize raw, base */
				if (base_delay_n >= 5)		/* 300x50 = 15000ms */
				{	
					base_delay_n = 0;				
					TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2));		/* normal mode */
					TS(SFR_TS_CON) = _TS_START;					
					while (!(TS(SFR_TS_CON)&BIT(2)));	
					for (i = 0; i < ts.actv_ch_cnt; i++)
					{
						ts.raw_buf[i][0]  = (TSRAW(ts.ch_num_buf[i]*2));
						ts.raw_buf[i][1]  = (TSRAW(ts.ch_num_buf[i]*2+1));
						ts.base_buf[i][0] = ts.raw_buf[i][0];
						ts.base_buf[i][1] = ts.raw_buf[i][1];
					}				
					TS(SFR_TS_CON) = 0;
					TS(SFR_TS_MODE) &= ~(BIT(4)+BIT(3)+BIT(2));		/* simultaneous mode */
					TS(SFR_TS_MODE) |= BIT(4);						/* simultaneous mode */				
				}				
			}
		}
t_sysbit.dbg_trigger = 1;					
		return;
	}
	
	switch (ts.step)
	{
		case 0 :	/* Initilaize */
			TS_Init_Variable();
			TS_Init_Register();			
			ts.step++;
			break;
		case 1 :	/* Calibration */
			if ((t_sysbit.led_en == 1)&&(t_sysbit.led_io != 1))
			{
				break;
			}
			TS_Config_Mode();	
			t_sysbit.cal_ok = 1;
			ts.flag.ts_status = 0;
			ts.step++;
			break;
		case 2 :	/* Touch Sensing */
			if ((t_sysbit.led_en == 0) || (t_sysbit.time_div == 0)) 
			{		
				TS_Start_Touch(ts.actv_ch_idx);
				ts.step++;
			}
			else
			{
 				if ((t_sysbit.led_en == 1) && (t_sysbit.led_end == 1))
 				{					
					ts.step++;	
				}
			}
			break;
		case 3 :
			if (ts.flag.ts_status == TS_BREAK_TIME)			// timeout for led operation 
			{
				ts.flag.ts_status = 0;
				t_sysbit.led_end = 0;				
				ts.step--;
			}
			else if (ts.flag.ts_status == TS_COMPLETE)		// all ch complete
			{			
				ts.flag.ts_status = 0;
				t_sysbit.led_end = 0;
				ts.step++;		
			}
			break;
		case 4 :	/* Initialize Baseline */
			if (ts.flag.base_init == 0)
			{
				TS_Init_Base();
				if (ts.flag.base_init == 0)	// GUN_FIXED
				{
					ts.step = 2;
				}
			}
			else
			{
				ts.step++;
			}
			break;
		case 5	:	/* Manipulate touch data */				
			TS_Make_Data();	
			func_p_user_filter();
			t_sysbit.dbg_trigger = 1;
			ts.step++;
			break;
		case 6 :
			TS_Judge_Key();
			ts.step++;
			break;
		case 7 :
			TS_Update_Base();			
			ts.step++;
		default :
			ts.step = 2;
			break;
	}
}

