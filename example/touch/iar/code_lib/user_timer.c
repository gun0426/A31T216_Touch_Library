/** *******************************************************************
 * @file		 user_timer.c
 * @brief		 
 * @author	 ABOV Sensor Solution Team
 *
 * Copyright(C) 2020, ABOV Semiconductor
 * All rights reserved.
 *
 **********************************************************************/
#include "A31T21x.h"
#include "common.h"
#include "user_timer.h"

u32 ut_tick;

static u16 ut_set_flag;
static u16 ut_expire_flag;
static u32 ut_timer_1;
static u32 ut_timer_2;
static u32 ut_timer_3;
static u32 ut_timer_4;
static u32 ut_timer_5;
static u32 ut_timer_6;
static u32 ut_timer_7;
static u32 ut_timer_8;


void UT_Run_Timer(void)
{
	ut_timer_1 = 0;
	ut_timer_2 = 0;
	ut_timer_3 = 0;
	ut_timer_4 = 0;
	ut_timer_5 = 0;
	ut_timer_6 = 0;
	ut_timer_7 = 0;
	ut_timer_8 = 0;
	ut_set_flag = 0;
	ut_expire_flag = 0;
}

#if (0)
void Timer1_ISR(void)
{
	g_systick++;
}
#endif
#if (0)
/*************************************************************
  - FUNCTION & CLASS NAME
	 void UT_Clear_Timer(u8 utimer)
	 
  - DESCRIPTION
	 
  - PARAMETER & RETURN
	 return : 
	 input1 : 
	 input2 :
	 
  - TRACEBILITY
	 
*************************************************************/
void ut_InitParam(void)
{
	ut_timer_1 = 0;
	ut_timer_2 = 0;
	ut_timer_3 = 0;
	#if (0)
	ut_timer_4 = 0;
	ut_timer_5 = 0;
	ut_timer_6 = 0;
	ut_timer_7 = 0;
	ut_timer_8 = 0;
	#endif
	ut_set_flag = 0;
	ut_expire_flag = 0;
}
#endif

void SysTick_Handler(void)
{
	ut_tick++;
}

/*************************************************************
 - FUNCTION & CLASS NAME
	void UT_Set_Timer(eUTIDType utimer, u32 time_ms)
	
 - DESCRIPTION
	
 - PARAMETER & RETURN
	return : 
	input1 : 
	input2 :
	
 - TRACEBILITY
	
*************************************************************/
void UT_Set_Timer(u16 utid, u32 timeout)
{
	u32 ticks;

	ticks = ut_tick;
	
	switch (utid)
	{	
		case UT_INIT_DELAY : 
			ut_timer_1 = ticks + timeout;
			break;
		case UT_REVERSE : 
			ut_timer_2 = ticks + timeout;
			break;
		case UT_BASE_HOLD: 
			ut_timer_3 = ticks + timeout;
			break;	
		case UT_4: 
			ut_timer_4 = ticks + timeout;
			break;
		case UT_5: 
			ut_timer_5 = ticks + timeout;
			break;
		case UT_6: 
			ut_timer_6 = ticks + timeout;
			break;
		case UT_7: 
			ut_timer_7 = ticks + timeout;
			break;
		case UT_8: 
			ut_timer_8 = ticks + timeout;
			break;
		default: 
			break;
	}
	/* set timer-set flag and clear time-out flag */
	if(utid)
	{
		ut_set_flag |= utid;
		ut_expire_flag &= ~utid;
	}
}
#if (0)
/*************************************************************
 - FUNCTION & CLASS NAME
	void UT_Set_Timer(eUTIDType utimer, u32 time_ms)
	
 - DESCRIPTION
	
 - PARAMETER & RETURN
	return : 
	input1 : 
	input2 :
	
 - TRACEBILITY
	
*************************************************************/
u16 ut_GetElaps(u16 m_utid)
{
	u16 elap_ms;
	u32 dwCapt_ms;
	u32 dwTicks;

	dwTicks = g_systick;
	
	switch (m_utid)
	{	
		case UT_INIT_DELAY : 
			dwCapt_ms = s32_Capture_1;
			break;
		case UT_REVERSE : 
			dwCapt_ms = s32_Capture_2;
			break;
		case UT_BASE_HOLD: 
			dwCapt_ms = s32_Capture_3;
			break;				
		case UT_4: 
			dwCapt_ms = s32_Capture_4;
			break;
		case UT_5: 
			dwCapt_ms = s32_Capture_5;
			break;
		case UT_6: 
			dwCapt_ms = s32_Capture_6;
			break;
		case UT_7: 
			dwCapt_ms = s32_Capture_7;
			break;
		case UT_INIT_DELAY: 
			dwCapt_ms = s32_Capture_8;
			break;
		case UT_9: 
			dwCapt_ms = s32_Capture_9;
			break;
		default: 
			break;
	}
	
	if (dwTicks > dwCapt_ms)
	{
		if (dwTicks - dwCapt_ms > 65535)
		{
			elap_ms = 0xffff;
		}
		else
		{
			elap_ms = (u16)(dwTicks - dwCapt_ms);
		}
	}
	else
	{
		if (dwCapt_ms - dwTicks > 65535)
		{
			elap_ms = 0xffff;
		}
		else
		{
			elap_ms = (u16)(dwCapt_ms - dwTicks);
		}
	}

	return elap_ms;
}
#endif
#if (1)
/*************************************************************
  - FUNCTION & CLASS NAME
	 u8 UT_Get_Status(void)
	 
  - DESCRIPTION
	 
  - PARAMETER & RETURN
	 return : 
	 input1 : 
	 input2 :
	 
  - TRACEBILITY
	 
 *************************************************************/
u16 UT_Get_Status(void)
{
	return ut_set_flag;
}
#endif
/*************************************************************
  - FUNCTION & CLASS NAME
	 void UT_Clear_Timer(u8 utimer)
	 
  - DESCRIPTION
	 
  - PARAMETER & RETURN
	 return : 
	 input1 : 
	 input2 :
	 
  - TRACEBILITY
	 
*************************************************************/
void UT_Clear_Timer(u16 utid)
{
	/* clear user-timer */
	switch (utid)
	{		
		case UT_INIT_DELAY : 
			ut_timer_1 = 0;
			break;	
		case UT_REVERSE : 
			ut_timer_2 = 0;
			break;		
		case UT_BASE_HOLD : 
			ut_timer_3 = 0;
			break;		
		case UT_4 : 
			ut_timer_4 = 0;
			break;
		case UT_5 : 
			ut_timer_5 = 0;
			break;	
		case UT_6 : 
			ut_timer_6 = 0;
			break;		
		case UT_7 : 
			ut_timer_7 = 0;
			break;			   
		case UT_8 : 
			ut_timer_8 = 0;
			break;
		default: 
			break;
	}
	
	/* clear timer-set flag and time-out flag */
	if (utid)
	{
		ut_set_flag &= ~utid;
		ut_expire_flag &= ~utid;
	}
}
/*************************************************************
  - FUNCTION & CLASS NAME
	 void UT_Check_Expire(void)
	 
  - DESCRIPTION
	 
  - PARAMETER & RETURN
	 return : 
	 input1 : 
	 input2 :
	 
  - TRACEBILITY
	 
*************************************************************/
static void UT_Check_Expire(void)
{
	u16 wFlag = 0;
	u32 systick;

	systick = ut_tick;

	if (ut_set_flag)
	{
		if (ut_set_flag & UT_INIT_DELAY)
		{
			if((ut_timer_1 <= systick) && ((systick-ut_timer_1) < 0x80000000))
			{
				ut_timer_1 = 0;
				wFlag |= UT_INIT_DELAY;
			}
		}
		if (ut_set_flag & UT_REVERSE)
		{
			if((ut_timer_2 <= systick) && ((systick-ut_timer_2) < 0x80000000))
			{
				ut_timer_2 = 0;
				wFlag |= UT_REVERSE;
			}
		}
		if (ut_set_flag & UT_BASE_HOLD)
		{
			if((ut_timer_3 <= systick) && ((systick-ut_timer_3) < 0x80000000))
			{
				ut_timer_3 = 0;
				wFlag |= UT_BASE_HOLD;
			}
		}
		if (ut_set_flag & UT_4)
		{
			if((ut_timer_4 <= systick) && ((systick-ut_timer_4) < 0x80000000))
			{
				ut_timer_4 = 0;
				wFlag |= UT_4;
			}
		}
		if (ut_set_flag & UT_5)
		{
			if((ut_timer_5 <= systick) && ((systick-ut_timer_5) < 0x80000000))
			{
				ut_timer_5 = 0;
				wFlag |= UT_5;
			}
		}
		if (ut_set_flag & UT_6)
		{
			if((ut_timer_6 <= systick) && ((systick-ut_timer_6) < 0x80000000))
			{
				ut_timer_6 = 0;
				wFlag |= UT_6;
			}
		}
		if (ut_set_flag & UT_7)
		{
			if((ut_timer_7 <= systick) && ((systick-ut_timer_7) < 0x80000000))
			{
				ut_timer_7 = 0;
				wFlag |= UT_7;
			}
		}
		if (ut_set_flag & UT_8)
		{
			if((ut_timer_8 <= systick) && ((systick-ut_timer_8) < 0x80000000))
			{
				ut_timer_8 = 0;
				wFlag |= UT_8;
			}
		}
		/* user-timer-out-flag set */
		if (wFlag)
		{
			ut_expire_flag |= wFlag;
		}
	}

}
/*************************************************************
  - FUNCTION & CLASS NAME
	 u8 UT_Is_Expired(u8 utimer)
	 
  - DESCRIPTION
	 
  - PARAMETER & RETURN
	 return : 
	 input1 : 
	 input2 :
	 
  - TRACEBILITY
	 
*************************************************************/
u8 UT_Is_Expired(u16 utid)
{
	u8 rtn;

	UT_Check_Expire();

	rtn = 0;
	/* if time-out flag is setted */
	if (ut_expire_flag)
	{
		if (utid)
		{
			if ((ut_expire_flag & utid) == utid)
			{
				rtn = 1;
				ut_expire_flag &= ~utid;	// clear time-out flag 
				ut_set_flag &= ~utid;
				UT_Clear_Timer(utid); 			
			}
		}
	}
	
	return(rtn);
}
#if (0)
/*************************************************************
 - FUNCTION & CLASS NAME
	void ut_StartStopwatch(u16 m_utid, u32 m_ms)
	
 - DESCRIPTION
	
 - PARAMETER & RETURN
	return : 
	input1 : 
	input2 :
	
 - TRACEBILITY
	
*************************************************************/
void ut_SW_Start(u16 m_utid)	// SW: STOP WATCH
{
	u32 dwTicks;

	dwTicks = g_systick;
	
	switch (m_utid)
	{	
		case UT_INIT_DELAY : 
			ut_timer_1 = dwTicks;
			break;
		case UT_REVERSE : 
			ut_timer_2 = dwTicks;
			break;
		case UT_BASE_HOLD: 
			ut_timer_3 = dwTicks;
			break;				
		case UT_4: 
			ut_timer_4 = dwTicks;
			break;
		case UT_5: 
			ut_timer_5 = dwTicks;
			break;
		case UT_6: 
			ut_timer_6 = dwTicks;
			break;
		case UT_7: 
			ut_timer_7 = dwTicks;
			break;
		case UT_8: 
			ut_timer_8 = dwTicks;
			break;
		case UT_9: 
			s32_timer_9 = dwTicks;
			break;
		default: 
			break;
	}
	/* set timer-set flag and clear time-out flag */
	if(m_utid)
	{
		ut_set_flag |= m_utid;
		ut_expire_flag &= ~m_utid;
	}
}
/*************************************************************
 - FUNCTION & CLASS NAME
	void ut_StartStopwatch(u16 m_utid, u32 m_ms)
	
 - DESCRIPTION
	
 - PARAMETER & RETURN
	return : 
	input1 : 
	input2 :
	
 - TRACEBILITY
	
*************************************************************/
void ut_SW_GetLap(u16 m_utid)
{
	u32 dwTicks;

	dwTicks = g_systick;
	
	switch (m_utid)
	{	
		case UT_INIT_DELAY : 
			ut_timer_1 = dwTicks;
			
			break;
		case UT_REVERSE : 
			ut_timer_2 = dwTicks;
			break;
		case UT_BASE_HOLD: 
			ut_timer_3 = dwTicks;
			break;				
		case UT_4: 
			ut_timer_4 = dwTicks;
			break;
		case UT_5: 
			ut_timer_5 = dwTicks;
			break;
		case UT_6: 
			ut_timer_6 = dwTicks;
			break;
		case UT_7: 
			ut_timer_7 = dwTicks;
			break;
		case UT_8: 
			ut_timer_8 = dwTicks;
			break;
		case UT_9: 
			s32_timer_9 = dwTicks;
			break;
		default: 
			break;
	}
	/* set timer-set flag and clear time-out flag */
	if(m_utid)
	{
		ut_set_flag |= m_utid;
		ut_expire_flag &= ~m_utid;
	}
}
#endif

