
#include <stdlib.h>
#include "A31T21x.h"

#include "a31t21x_driver_common.h"
#include "common.h"
#include "touch.h"
#include "user.h"

struct user_cfg user;
str_led t_led;

u8 const coT4Digit[4][10][8] = { 
{
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		E1, 	F1},		// 0 : ABCDEF
	{0, 	0,		B1, 	C1, 	0,		0,		0,		0}, 		// 1 : BC
	{0, 	A1, 	B1, 	0,		D1, 	0,		E1, 	G1},		// 2 : ABDEG
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		0,		G1},		// 3 : ABCDG
	{0, 	0,		B1, 	C1, 	0,		0,		0,		F1+G1}, 	// 4 : BCFG
	{0, 	A1, 	0,		C1, 	D1, 	0,		0,		F1+G1}, 	// 5 : ACDFG
	{0, 	A1, 	0,		C1, 	D1, 	0,		E1, 	F1+G1}, 	// 6 : ACDEFG
	{0, 	A1, 	B1, 	C1, 	0,		0,		0,		F1},		// 7 : ABCF
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		E1, 	F1+G1}, 	// 8 : ABCDEFG
	{0, 	A1, 	B1, 	C1, 	D1, 	0,		0,		F1+G1}		// 9 : ABCDFG
},{
	{0, 	0,		C2, 	B2+D2,	A2+E2,	0,		F2, 	0}, 		// 0 : ABCDEF
	{0, 	0,		C2, 	B2, 	0,		0,		0,		0}, 		// 1 : BC
	{0, 	0,		0,		B2+D2,	A2+E2,	0,		0,		G2},		// 2	: ABDEG
	{0, 	0,		C2, 	B2+D2,	A2, 	0,		0,		G2},		// 3 : ABCDG
	{0, 	0,		C2, 	B2, 	0,		0,		F2, 	G2},		// 4 : BCFG
	{0, 	0,		C2, 	D2, 	A2, 	0,		F2, 	G2},		// 5	: ACDFG
	{0, 	0,		C2, 	D2, 	A2+E2,	0,		F2, 	G2},		// 6 : ACDEFG
	{0, 	0,		C2, 	B2, 	A2, 	0,		F2, 	G2},		// 7 : ABCF
	{0, 	0,		C2, 	B2+D2,	A2+E2,	0,		F2, 	G2},		// 8 : ABCDEFG
	{0, 	0,		C2, 	B2+D2,	A2, 	0,		F2, 	G2} 		// 9 : ABCDFG
},{
	{0, 	0,		0,		0,		B3, 	0,		A3+C3,	D3+E3+F3},	// 0 : ABCDEF
	{0, 	0,		0,		0,		B3, 	0,		C3, 	0}, 		// 1 : BC
	{0, 	0,		0,		0,		B3, 	G3, 	A3, 	D3+E3}, 	// 2	: ABDEG 
	{0, 	0,		0,		0,		B3, 	G3, 	A3+C3,	D3},		// 3 : ABCDG
	{0, 	0,		0,		0,		B3, 	G3, 	C3, 	F3},		// 4 : BCFG
	{0, 	0,		0,		0,		0,		G3, 	A3+C3,	D3+F3}, 	// 5	: ACDFG
	{0, 	0,		0,		0,		0,		G3, 	A3+C3,	D3+E3+F3},	// 6 : ACDEFG
	{0, 	0,		0,		0,		B3, 	0,		A3+C3,	F3},		// 7 : ABCF
	{0, 	0,		0,		0,		B3, 	G3, 	A3+C3,	D3+E3+F3},	// 8 : ABCDEFG
	{0, 	0,		0,		0,		B3, 	G3, 	A3+C3,	D3+F3}		// 9 : ABCDFG
},{
	{C4,	B4, 	A4+F4,	E4, 	0,		D4, 	0,		0}, 		// 0 : ABCDEF
	{C4,	B4, 		0,	0,		0,		0,		0,		0}, 		// 1 : BC
	{0, 	B4+G4,	A4, 	E4, 	0,		D4, 	0,		0}, 		// 2	: ABDEG
	{C4,	B4+G4,	A4, 	0,		0,		D4, 	0,		0}, 		// 3 : ABCDG
	{C4,	B4+G4,	F4, 	0,		0,		0,		0,		0}, 		// 4 : BCFG
	{C4,	G4, 	A4+F4,	0,		0,		D4, 	0,		0}, 		// 5	: ACDFG
	{C4,	G4, 	A4+F4,	E4, 	0,		D4, 	0,		0}, 		// 6 : ACDEFG
	{C4,	B4, 	A4+F4,	0,		0,		0,		0,		0}, 		// 7 : ABCF
	{C4,	B4+G4,	A4+F4,	E4, 	0,		D4, 	0,		0}, 		// 8 : ABCDEFG
	{C4,	B4+G4,	A4+F4,	0,		0,		D4, 	0,		0}			// 9 : ABCDFG
}}; 


u8 const coDigit[9][8] = {
	{0x00,0x7E,0xFF,0x81,0x81,0xFF,0x7E,0x00},	/* 0 */
	{0x00,0x02,0x02,0xFF,0xFF,0x00,0x00,0x00},	/* 1 */
	{0x00,0xC6,0xE7,0xB1,0x99,0x8F,0x86,0x00},	/* 2 */
	{0x00,0x42,0xCB,0x89,0x89,0xFF,0x76,0x00}, 	/* 3 */
	{0x00,0x30,0x3C,0x2F,0xFF,0xFF,0x20,0x00}, 	/* 4 */
	{0x00,0x4F,0xCF,0x89,0x89,0xF9,0x71,0x00},	/* 5 */
	{0x00,0x7E,0xFF,0x89,0x89,0xFB,0x72,0x00},	/* 6 */
	{0x00,0x01,0x01,0xE1,0xFD,0x1F,0x03,0x00},	/* 7 */
//	{0x00,0x76,0xFF,0x89,0x89,0xFF,0x76,0x00},	/* 8 */
//	{0x00,0x4E,0xDF,0x91,0x91,0xFF,0x7E,0x00},	/* 9 */
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}	/* nothing,	*/
};

u32 const DIGIT_HEX[16] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_B, DIGIT_C, DIGIT_D, DIGIT_E, DIGIT_F};
/*************************************************************
 - FUNCTION & CLASS NAME
	: void led_RenewData(void)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void led_RenewData(void)
{
	u16 seg_data[13];	/* COM0 ~ COM12 */
	u32* ptrDisp;

	u8 comIndx;
	u8 numIndx;
	u8 i;

	/***********  clear led data  *************/
	for (comIndx = 0; comIndx < 13; comIndx++)
	{
		seg_data[comIndx] = 0;
	}

	/***********  M-TYPE 8x8 : 0~CÇ¥½Ã ************
		SEG : 6/9/10/11/12/13/14/15
		COM : 0/1/2/3/4/5/7/8
	**********************************************/
	for (numIndx = 0; numIndx < 8; numIndx++)
	{
		if (ts.detect_flag & BIT(numIndx))
		{
			break;
		}
	}	
	/* T-Type */
	for (comIndx = 0; comIndx < 8; comIndx++)
	{
		for (i = 0; i < numIndx+1; i++)
		{
			if (i < 6)	/* 0,1,2,3,4,5 */
			{
				seg_data[comIndx] |= BIT(i);
			}
			else		/* 7,8 */
			{
				seg_data[comIndx] |= BIT(i+1);
			}
		}
	}
	/* M-Type */
	for (comIndx = 0; comIndx < 8; comIndx++)	/* COM0/1/2/3/4/5/7/8 */	
	{
		if (coDigit[numIndx][comIndx] & BIT(0))
		{
			seg_data[comIndx] |= BIT(6); 		/* SEG6 */
		}
		seg_data[comIndx] |= (u16)(coDigit[numIndx][comIndx]<<8)&0xfe00;	/* SEG9/10/11/12/13/14/15 */
	}

	/***********  renew led data  *************/
	ptrDisp = (u32*)(&LED->DISPRAM0);	
	for (comIndx = 0; comIndx < 8; comIndx++)
	{
		if (comIndx < 6)	/* COM0 ~ COM5 */
		{
			*(ptrDisp+comIndx) = seg_data[comIndx];
		}
		else				/* COM7 ~ COM8 */
		{
			*(ptrDisp+(comIndx+1)) = seg_data[comIndx];
		}
	}	
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void sort_upward(u8* mp_start, u8 len)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
u16 get_max(s16* mp_start, u8 m_offset, u8 m_len)
{
	u8 i;
	s16 max;
	
	max = 0;
	for (i=0; i<m_len; i++)
	{
		if(*(mp_start+i*m_offset) > max) 
		{
			max = *(mp_start+i*m_offset);
		}
	}

	return max;
}
/*************************************************************
 - FUNCTION & CLASS NAME
	: void TS_User_Filter(void)
	
 - DESCRIPTION
	: 
	
 - PARAMETER & RETURN
	: 
	
*************************************************************/
void TS_User_Filter(void)
{
  	u8 i;
	u8 j;
	static u8 relLevel_n[ACT_KEY_N] = {0,};
	
	for (i=0; i<ts.actv_ch_cnt; i++)
	{
user.test[i] = user.del_data[i];
		if (ts.detect_flag & BIT(i))
		{
			user.del_queue[i][user.del_queue_idx++] = user.del_data[i];
			user.del_queue_idx %= RAW_Q_SIZE;			
			if (abs(user.del_data[i]) < (ts.thd_buf[i]*ts.release_rate/100))
			{
				relLevel_n[i]++;

				if (relLevel_n[i] >= 2)
				{
					relLevel_n[i] = 0;
					for (j=0; j<RAW_Q_SIZE; j++)
					{
						user.del_queue[i][j] = user.del_data[i];
					}
				}
			}
			else
			{
				relLevel_n[i] = 0;
			}
			user.del_data[i] = get_max(&user.del_queue[i][0], 1 ,RAW_Q_SIZE);
		}
		else
		{
			relLevel_n[i] = 0;
		}
	}	
}

