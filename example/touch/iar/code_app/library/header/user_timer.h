/*************************************************************
 - 파일명	: user_timer.h
 - 내용 	: 사용자 타이머설정에 관한 함수 선언 및 기타 설정값을 기술한다. 
 - 작성자	: 허영건 
 - 날짜 	: 2013.02.10 ~
 - 수정 	: 2013.02.10 - 파일 생성 
		: 
*************************************************************/
#include "common.h"

#ifndef __USERTIMER_H__
#define __USERTIMER_H__

typedef enum 
{
	UT_MIN				=0x00,
	UT_INIT_DELAY		=0x01,	
	UT_REVERSE			=0x02,	
	UT_BASE_HOLD		=0x04,
#if (1)	
	UT_4				=0x08,
	UT_5				=0x10,	
	UT_6				=0x20,
	UT_7				=0x40,
	UT_8				=0x80,
#endif	
	UT_MAX				=0xff
} eUTIDType;


void UT_Set_Timer(u16 m_utid, u32 time_ms);
u16 UT_Get_Status(void);
void UT_Clear_Timer(u16 m_utid);
unsigned char UT_Is_Expired(u16 m_utid);
void UT_Run_Timer(void);
void Timer1_ISR(void);

extern u32 ut_tick;

#endif	// __USERTIMER_H__

