
#include "common.h"

#ifndef _USER_H_
#define _USER_H_

#define MCLK_1MHZ	0
#define MCLK_2MHZ	1
#define MCLK_4MHZ	2
#define MCLK_8MHZ	3
#define MCLK_16MHZ	4
#define MCLK_32MHZ	5	
#define MCLK_SPEED	MCLK_32MHZ

#define LED_DRV_EN 1
#define DBG_MSG_EN 1
#define TS_LED_TIME_DIV	1
#define TS_SLIDE_EN 	0

#define LPM_SIMULTANEOUS_SENSE	0
#define LPM_NORMAL_SENSE		1
#define LPM_SENSE				LPM_SIMULTANEOUS_SENSE
#define LPM_ENTRY_DELAY			60000

/*************************
	TOUCH CH
*************************/
#define CS00_USE	0
#define CS01_USE	1
#define CS02_USE	0
#define CS03_USE	0
#define CS04_USE	1
#define CS05_USE	0	//X
#define CS06_USE	0
#define CS07_USE	1	// SW CLK
#define CS08_USE	0	// SW DATA
#define CS09_USE	0	// UART TX
#define CS10_USE 	1	// UART RX
#define CS11_USE	0
#define CS12_USE	0
#define CS13_USE	1
#define CS14_USE	0
#define CS15_USE	0
#define CS16_USE	1
#define CS17_USE	0
#define CS18_USE	0
#define CS19_USE	1
#define CS20_USE	0
#define CS21_USE	0	// ISEG8/ICOM8
#define CS22_USE	0	// ISEG7/ICOM7
#define CS23_USE	0	// iSEG6/ICOM6,  X /* set '0' to use CS23 as a mash-shield */
/*************************
	SHIELD CH
*************************/
#define SHLD_CS00	0
#define SHLD_CS01	1
#define SHLD_CS02	0
#define SHLD_CS03	0
#define SHLD_CS04	1
#define SHLD_CS05	0
#define SHLD_CS06	0
#define SHLD_CS07	1	
#define SHLD_CS08	0	
#define SHLD_CS09	0	
#define SHLD_CS10	1	
#define SHLD_CS11	0
#define SHLD_CS12	0
#define SHLD_CS13	1
#define SHLD_CS14	0
#define SHLD_CS15	0
#define SHLD_CS16	1
#define SHLD_CS17	0
#define SHLD_CS18	0
#define SHLD_CS19	1
#define SHLD_CS20	0
#define SHLD_CS21	0
#define SHLD_CS22	0
#define SHLD_CS23	0

/*************************
	COM-SEG
*************************/
#define COM00_EN	1
#define COM01_EN	1
#define COM02_EN	1
#define COM03_EN	1
#define COM04_EN	1
#define COM05_EN	1
#define COM06_EN	0
#define COM07_EN	1
#define COM08_EN	1
#define COM09_EN	0
#define COM10_EN	0
#define COM11_EN	0
#define COM12_EN	0
#define COM13_EN	0
#define SEG00_EN	1
#define SEG01_EN	1
#define SEG02_EN	1
#define SEG03_EN	1
#define SEG04_EN	1
#define SEG05_EN	1
#define SEG06_EN	1
#define SEG07_EN	1
#define SEG08_EN	1
#define SEG09_EN	1
#define SEG10_EN	1
#define SEG11_EN	1
#define SEG12_EN	1
#define SEG13_EN	1
#define SEG14_EN	1
#define SEG15_EN	1
/*************************
	T-Type LED
*************************/
#define A1	BIT(0)
#define B1	BIT(0)
#define C1	BIT(0)
#define D1	BIT(0)
#define E1	BIT(0)
#define F1	BIT(0)
#define G1	BIT(1)
#define A2	BIT(1)
#define B2	BIT(1)
#define C2	BIT(1)
#define D2	BIT(2)
#define E2	BIT(2)
#define F2	BIT(2)
#define G2	BIT(2)
#define A3	BIT(3)
#define B3	BIT(3)
#define C3	BIT(4)
#define D3	BIT(4)
#define E3	BIT(6)
#define F3	BIT(3)
#define G3	BIT(3)
#define A4	BIT(3)
#define B4	BIT(3)
#define C4	BIT(3)
#define D4	BIT(7)
#define E4	BIT(7)
#define F4	BIT(7)
#define G4	BIT(7)
#if (1)
/*************************
	M-Type LED
*************************/
#define SEG7_A	BIT(8)	
#define SEG7_B	BIT(9)
#define SEG7_C	BIT(10)
#define SEG7_D	BIT(11) 
#define SEG7_E	BIT(12)
#define SEG7_F	BIT(13) 
#define SEG7_G	BIT(14)
#define DIGIT_0 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_F)
#define DIGIT_1 (SEG7_B|SEG7_C)
#define DIGIT_2 (SEG7_A|SEG7_B|SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_3 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_G)
#define DIGIT_4 (SEG7_B|SEG7_C|SEG7_F|SEG7_G)
#define DIGIT_5 (SEG7_A|SEG7_C|SEG7_D|SEG7_F|SEG7_G)
#define DIGIT_6 (SEG7_A|SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_7 (SEG7_A|SEG7_B|SEG7_C|SEG7_F)
#define DIGIT_8 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_9 (SEG7_A|SEG7_B|SEG7_C|SEG7_D|SEG7_F|SEG7_G)
#define DIGIT_A (SEG7_A|SEG7_B|SEG7_C|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_B (SEG7_C|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_C (SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_D (SEG7_B|SEG7_C|SEG7_D|SEG7_E|SEG7_G)
#define DIGIT_E (SEG7_A|SEG7_D|SEG7_E|SEG7_F|SEG7_G)
#define DIGIT_F (SEG7_A|SEG7_E|SEG7_F|SEG7_G)
#endif
#define SEG8	BIT(0)
#define SEG9	BIT(1)
#define SEG10	BIT(2)
#define SEG11	BIT(3)
#define SEG12	BIT(4)
#define SEG13	BIT(5)
#define SEG14	BIT(6)
#define SEG15	BIT(7)

typedef struct
{
	u8 digit[4];
	u8 preDigit[4];
} str_led;

/******************************************************************************************************************************************/
/************************************************************** don't modify *************************************************************/
#define ACT_KEY_N		(CS00_USE+CS01_USE+CS02_USE+CS03_USE+CS04_USE+CS05_USE+CS06_USE+CS07_USE+	\
						 CS08_USE+CS09_USE+CS10_USE+CS11_USE+CS12_USE+CS13_USE+CS14_USE+CS15_USE+	\
						 CS16_USE+CS17_USE+CS18_USE+CS19_USE+CS20_USE+CS21_USE+CS22_USE+CS23_USE)	

#define CHSEL_00_23		((CS23_USE<<23)+(CS22_USE<<22)+(CS21_USE<<21)+(CS20_USE<<20)+(CS19_USE<<19)+(CS18_USE<<18)+(CS17_USE<<17)+(CS16_USE<<16)+	\
						 (CS15_USE<<15)+(CS14_USE<<14)+(CS13_USE<<13)+(CS12_USE<<12)+(CS11_USE<<11)+(CS10_USE<<10)+(CS09_USE<<9)+(CS08_USE<<8)+		\
						 (CS07_USE<<7)+(CS06_USE<<6)+(CS05_USE<<5)+(CS04_USE<<4)+(CS03_USE<<3)+(CS02_USE<<2)+(CS01_USE<<1)+(CS00_USE<<0))

#define COMSEL_00_12	((COM12_EN<<12)+(COM11_EN<<11)+(COM10_EN<<10)+(COM09_EN<<9)+(COM08_EN<<8)+(COM07_EN<<7)+(COM06_EN<<6)+(COM05_EN<<5)+	\
						 (COM04_EN<<4)+(COM03_EN<<3)+(COM02_EN<<2)+(COM01_EN<<1)+(COM00_EN<<0))
#define SEGSEL_00_15	((SEG15_EN<<15)+(SEG14_EN<<14)+(SEG13_EN<<13)+(SEG12_EN<<12)+(SEG11_EN<<11)+(SEG10_EN<<10)+(SEG09_EN<<9)+(SEG08_EN<<8)+	\
						 (SEG07_EN<<7)+(SEG06_EN<<6)+(SEG05_EN<<5)+(SEG04_EN<<4)+(SEG03_EN<<3)+(SEG02_EN<<2)+(SEG01_EN<<1)+(SEG00_EN<<0))

#define RAW_Q_SIZE 7		//7

struct user_cfg
{
	s16 test[ACT_KEY_N];
	s16 del_data[ACT_KEY_N]; 
	s16 del_queue[ACT_KEY_N][RAW_Q_SIZE];
	u16 del_queue_idx;	
	u16 raw_data[ACT_KEY_N][2];	
	u16 base_data[ACT_KEY_N][2]; 
	u16 pre_data[ACT_KEY_N][2]; 
	u16 zero_sco[ACT_KEY_N];
	u8	base_hold_cnt[ACT_KEY_N];	
	u8	debounce_cnt[ACT_KEY_N];
	s16 ch_thd[ACT_KEY_N];
	u8	actv_ch_num[ACT_KEY_N];
	u32 raw_sum_data[ACT_KEY_N][2];
	u16 raw_avrg_data[ACT_KEY_N][2];
};

extern struct user_cfg user;

void led_RenewData(void);
void TS_User_Filter(void);

#endif


