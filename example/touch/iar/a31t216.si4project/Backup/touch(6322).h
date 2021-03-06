
#include "common.h"

#ifndef __TOUCH_H__
#define __TOUCH_H__

#define NORMAL_MODE 	0
#define HISENSE_MODE	1	
#define ADJUST_MODE 	2
#define OFFCAL_MODE 	3
#define SIMULT_MODE		4

#define FLOATING	0
#define OUT_LOW		1
#define OUT_HIGH	2

#define TS_BREAK_TIME	0x01
#define TS_COMPLETE 	0x02

struct ts_param
{
	u8 	step;
	s16 *delta_buf;
	s16 *thd_buf;			
	u16 (*raw_buf)[2];
	u16 (*base_buf)[2];
	u16 (*pre_buf)[2];
	u16 *sco_buf;
	u8  *hold_cnt_buf;	
	u8  *debounce_cnt_buf;
	u8  *ch_num_buf;
	u32 shield_ch_bit;
	u32	detect_key; 	
	u32 detect_flag;
	u8  actv_ch_idx;
	u8  actv_ch_cnt;
	u32 actv_ch_bit;
	u8  mode;
	u8  max_cap_idx;
//	u16 max_sco;
	u8  debounce_cnt_detect;
	u8  debounce_cnt_release;
	u8  iir_offset_cnt;
	u16 reverse_time;
	u8  reverse_rate;
	u8  release_rate;
//	u8  track_step;
//	u16 track_delay;
	u16 init_delay;
	u8  reverse_ch_idx;
	u8	slide_touch;
	u16 simultaneous_raw;
	u16 simultaneous_base;
	u8	key_num;
u8  base_track_iir_1;
u8  base_track_iir_2;
u16 base_track_period;
u32 (*raw_sum_buf)[2];
u16 (*raw_avrg_buf)[2];
u16 raw_sum_cnt;
u8  noise_thd_rate;
u8  basehold_rate;
u8  inact_port;
u16 comp_ref;
u16 vhs;

	struct{
		u8 base_init	:1;
		u8 ts_status	:2;
		u8 wakeup		:1;
		u8 mesh_shld_en	:1;
		u8 ch_shld_en	:1;
		u8 slide_en		:1;
		u8 rsvd			:2;
	} flag;
};
struct ts_reg
{
	u16 SCO[TS_MAX_KEY];
	u16 tsReg[SFR_TS_MAX+2];
	#if (0)
	u8 con;
	u8 mode;
	u8 sumCnt;
	u8 chsel_h;
	u8 chsel_m;
	u8 chsel_l;
	u8 s1width;
	u8 slpCon;
	u8 reserved;
	u8 trim;
	u8 clkCfg;
	u8 trimOsc;
	u8 deltaOsc;
	u8 tled;
	u8 vhs_h;
	u8 vhs_l;		
	u8 vref_h;
	u8 vref_l;
	#endif
} ;

extern struct ts_param ts;
extern struct ts_reg ts_reg;
extern void (*func_p_user_filter)(void);

void TS_Do_Task(void);
void TS_Start_Touch(u8 indx);
void TS_Set_Mode(u8 op_mode);
void TS_Set_Shield(u8 ch_shld, u8 mesh_shld);
void TS_Set_Sum_Count(u8 raw_sum_cnt);
void TS_Set_Common_THD(s16 thd);
void TS_Set_CH_THD(u8 ch_idx, s16 thd);
void TS_Set_NoiseTHD_Rate(u8 rate);
void TS_Set_BaseHold_Rate(u8 rate);
void TS_Set_Base_Tracking(u8 iir_1, u8 iir_2, u16 period);
void TS_Set_Release_Rate(u8 rate);
void TS_Set_Reverse_Rate(u8 rate);
void TS_Set_Debounce(u8 detect_cnt, u8 release_cnt);
void TS_Set_IIR(u8 offset_cnt);
void TS_Set_Init_Delay(uint16_t delay);
void TS_Set_Reverse_Time(u16 time);		
u32 TS_Get_Key(void);
void Touch_ISR(void);
void TS_Set_Inact_Port(uint8_t state);
void TS_Set_Comp_RefV(uint16_t ref_v);
void TS_Set_Hisense_Level(uint16_t vhs, uint16_t ref_v);




#endif	// __TOUCH_H__
