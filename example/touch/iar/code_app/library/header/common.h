
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define _A31T21X_

#define ON			1
#define OFF			0
#define TS_MAX_KEY 24
#define CH_SHIELD_EN		1
#define CH_SHIELD_DIS		0
#define MESH_SHIELD_EN		1
#define MESH_SHIELD_DIS		0

/* TYPE DEFINE */
typedef unsigned long  	u32;
typedef unsigned int 		u16;
typedef unsigned char  	u8;
typedef signed long  		s32;
typedef signed int 		s16;
typedef signed char  		s8;

////#define BIT(n)	((u32)1 << (n))
//#define ABS(x) 	(x<0)?-(x):(x)

typedef struct
{
		u16 reinit	:1;
		u16 cal_ok	:1; 
		u16 led_end	:1;
		u16 dbg_trigger	:1;
		u16 led_en	:1;
		u16 dbg_en	:1;
		u16 led_io 	:1; 
		u16 time_div	:1;	
		u16 lpm_mode	:1;
		u16 lpm_simul   :1;	
		u16 reserved	:6;
}str_flag;

/* Touch */ 															
//=============================================================
//=============================================================

#define SFR_TS_CON			0x00		//0x0120
#define SFR_TS_MODE 		0x01		//0x0124
#define SFR_TS_SUM_CNT		0x02		//0x0128
#define SFR_TS_CH_SEL		0x03		//0x012C
#define SFR_TS_S1_WIDTH 	0x04		//0x0130
#define SFR_TS_SLP_CON		0x05		//0x0134
#define SFR_TS_TRIM 		0x06		//0x0138
#define SFR_TS_CLK_CFG		0x07		//0x013C
#define SFR_TRIM_OSC		0x08		//0x0140
#define SFR_DELTA_OSC		0x09		//0x0144
#define SFR_TLED			0x0A		//0x0148
#define SFR_TS_VHS			0x0B		//0x014C
#define SFR_TS_VREF 		0x0C		//0x0150
#define SFR_TS_RESERVE1 	0x0D		//0x0154
#define SFR_TS_RESERVE2 	0x0E		//0x0158
#define SFR_TS_RESERVE3 	0x0F		//0x015C
#define SFR_SHLD_CON		0x10		//0x0160
#define SFR_TS_MAX			0x11

typedef struct touch_T214
{
	__IO uint32_t RAW[48];				//0x0000~0x00BC
	__IO uint32_t SCO[24];				//0x00C0~0x011C
	__IO uint32_t TS[SFR_TS_MAX];		//0x0120~0x0150
} touch_T214;

#define bSFR						0x40003600UL
#define tSFR						((touch_T214*)bSFR)
#define TS(X)						(tSFR->TS[X])
#define TS_SCO(X)	(tSFR->SCO[X])
#define TSRAW(X)		(tSFR->RAW[X])

//#define T356_SCO_SIZE (3)
#define T418_SCO_SIZE (_MAX_KEY)
//#define SFR_A356_SIZE (SFR_TS_MAX+(T356_SCO_SIZE*2)+1)//protocol상 짝수 값을 위해.
#define SFR_T418_SIZE (SFR_TS_MAX+(T418_SCO_SIZE*2)/*+1*/)//protocol상 짝수 값을 위해.

typedef enum{
	TS_RUN = 0x01,
	 TS_IF = 0x04, //Touch Sensor Interrupt Flag
//	  BGR_EN = 0x08, //BGR enable
	OSC_EN = 0x10, //Oscillator enable
}TS_CON_TYPE;
#define _TS_START (OSC_EN|TS_RUN)

typedef enum{
	PORT_0 = 0x01, //Low, (00 : floating)
	PORT_1 = 0x02, //High
	WDT_STOP = 0x04, //only TS block is waked up by WDT. MCU is still in stop mode.
	SAP_0 = 0x10, //TS
	SAP_1 = 0x20, // ADC. The result of ADC is stored only at SUM_CH0 register
	SC_GAIN = 0x40, //SC_GAIN enable
	SREF = 0x80,//: External reference offset enable
}TS_MODE_TYPE;

typedef enum{
	TSCLKDIV0 = 0x01, //Touch Sensor Clock Divider
	TSCLKDIV1 = 0x02, //000 : OSCts / 1 (27MHz),2,4,8,16,32,64,128
	TSCLKDIV2 = 0x04, //
	TSCLKOE = 0x08, //Divided Touch Sensor Clock Output Enable
	ACLKDIV0 = 0x10, //ADC Clock Divider
	ACLKDIV1 = 0x20, //000 : OSCsys / 1 (16MHz),,2,4,8,16,32,64,128   
	ACLKDIV2 = 0x40, //010 : OSCsys / 4 (4MHz, default)
	ACLKSEL = 0x80, //default(0) 0: Touch clock, 1 : System clock
}TS_CLK_CFG_TYPE;

typedef enum{
	SR_OPEN = 0x00,
	SR_2_5k = 0x01,
	SR_5_0k = 0x02,
	SR_10_0k = 0x04,
	SR_20_0K = 0x08,
	SR_1_3K = 0x0F,
}TS_SVREF_TYPE; //VREF resistor select, 0x0F = 2.6k 

typedef enum{
	SCI_0_2pF = 0x00,
	SCI_0_6pF = 0x01,
	SCI_1_0pF = 0x02,
	SCI_1_4pF = 0x03,
	SCI_1_8pF = 0x04,
	SCI_2_2pF = 0x05,
	SCI_2_6pF = 0x06,
	SCI_3_0pF = 0x07,
}TS_SCI_TYPE;//Input Capacitor Selection

//SCC : Touch Sensor Conversion Capacitor Selection Register
typedef enum{
	SCC_1pF  = 0x00,
	SCC_3pF = 0x01,
	SCC_5pF = 0x02,
	SCC_7pF = 0x03,
	SCC_9pF = 0x04,
	SCC_11pF = 0x05,
	SCC_13pF = 0x06,
	SCC_15pF = 0x07,
}TS_SCC_TYPE;//Conversion Capacitor Selection


extern str_flag t_sysbit;


#endif // __GLOBAL_H__
