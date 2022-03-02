/* 
 * File:   ram.h
 * Author: FI
 *
 * Created on September 27, 2018, 8:52 PM
 */
// #include "stm32h7xx_hal.h"
 #include "stm32f1xx_hal.h"
 
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern IWDG_HandleTypeDef hiwdg;

//extern SPI_HandleTypeDef hspi1;

extern unsigned char htim1_flg;

//Uart1 with IR module
extern unsigned char t1_buff[16];
extern unsigned char r1_buff[16];
extern unsigned short t1_len;
extern unsigned short t1_num;
extern unsigned short t1_flg;
extern unsigned short r1_len;
extern unsigned short r1_pos;
extern unsigned char r1_flg;
extern unsigned short r1_tm;
extern unsigned char r1_buff2[128];

//Uart2 with PC
extern unsigned char t2_buff[1048];
extern unsigned char r2_buff[1048];
extern unsigned short t2_len;
extern unsigned short t2_flg;
extern unsigned short r2_pos;
extern unsigned char r2_flg;
extern unsigned char r2_buff2[1048];

//Serial3/uart3 with bluetooth
extern unsigned char t3_buff[128];
extern unsigned char r3_buff[128];
extern unsigned short t3_len;
extern unsigned short t3_flg;
extern unsigned short r3_len;
extern unsigned short r3_tot;
extern unsigned char r3_flg;
extern unsigned char r3_buff2[128];
extern unsigned short r3_pos;

extern unsigned char bluetooth_buff[128];
extern unsigned short bluetooth_len;
extern unsigned char bluetooth_flg;


//SPI
extern unsigned char spi_tx_buff[32];
extern unsigned char spi_rx_buff[32];
extern unsigned char buff_index;

extern unsigned short one1sec_tm;
extern unsigned short led_tm;

//IR
struct st_IR_Format
{
	unsigned char addr;
	unsigned char cmd;
	unsigned char data[3];
};
extern struct st_IR_Format IR_Format;
extern unsigned char ir_scan_flg;
extern unsigned char ir_scan_OnOff;
extern unsigned short ir_scan_tm;

extern unsigned char ir_run_flg;
extern unsigned char ir_run_OnOff;
extern unsigned short ir_run_tm;
extern unsigned char ir_chk_flg;
extern unsigned char ir_for_car_run_th1_flg;
extern unsigned char ir_for_car_run_th2_flg;
extern unsigned char ir_for_car_run_th3_flg;

extern unsigned char ir_send_must_ok_flg;
extern unsigned char ir_send_must_ok_type;
extern unsigned short ir_send_must_ok_tm;
extern unsigned short ir_send_must_ok_tmr;
extern unsigned char ir_new_data_flg;
extern unsigned char ir_new_data_type;
extern unsigned short driver_run_more_tm;

//send to due
extern unsigned char send_due_flg;
extern unsigned char send_due_type;
extern unsigned short send_due_tm;

//addr module
extern unsigned char addr_module;

//Couter frequency RGB
extern unsigned int clockCouter;
extern unsigned long cnt_bk1;
extern unsigned long cnt_bk2;
extern unsigned long cnt_bk3;
extern unsigned long cnt_bk4;
extern unsigned char s0_flg;
extern unsigned char s0_bk_flg;
extern unsigned char s1_flg;
extern unsigned char s1_bk_flg;
extern unsigned char s2_flg;
extern unsigned char s2_bk_flg;
extern unsigned char s3_flg;
extern unsigned char s3_bk_flg;

extern unsigned long Green_ct;
extern unsigned long freq_G;
extern unsigned long Red_ct;
extern unsigned long freq_R;
extern unsigned long Blue_ct;
extern unsigned long freq_B;
extern unsigned long nofiter_ct;
extern unsigned long freq_nofiter;

extern unsigned char scan_rgb_seq;
extern unsigned char scan_rgb_seq_bk;
extern unsigned char scan_rgb_tm;
extern unsigned char scan_rgb_tm_set;
extern unsigned char rgb_chk_ch1_flg;
extern unsigned char rgb_chk_ch2_flg;
extern unsigned char rgb_chk_ch3_flg;
extern unsigned char rgb_chk_ch4_flg;
extern unsigned char rgb_chk_ch5_flg;
extern unsigned char pwm_180_flg;
extern unsigned short rgb_pwm_flg;
extern unsigned short rgb_pwm_tot;
extern unsigned char rgb_status_flg;
extern unsigned short grb_stats_tm;
extern unsigned short grb_stanby_tm;

// Channel RGB 
extern unsigned char chn_flg[5];
extern unsigned char rgb_chn_sort[5];
extern unsigned short rgb_wait_chk_tm;
extern unsigned char rgb_wait_seq;
extern unsigned char rgb_debug[8];
extern unsigned char rgb_status;
extern unsigned char test_motor_flg;

//threshold rgb check
struct st_rgb_threshold
{
	unsigned char red_thr[4];
	unsigned char red_thr_chk_flg;

	unsigned char green_thr[4];
	unsigned char green_thr_chk_flg;

	unsigned char blue_thr[4];
	unsigned char blue_thr_chk_flg;

	unsigned char nofiter_thr[4];
	unsigned char nofiter_thr_chk_flg;

	unsigned char value_at_over_thr;
	unsigned char offset[4];

	unsigned char res2[7];

	unsigned char offset_at_max[4];
	unsigned char res3[4];
};

//struct fact
struct st_fact
{
	//unsigned char res01[8];
	unsigned char header[2];
	unsigned char res01[6];

	//unsigned char res02[8];
	//unsigned char res03[8];
	//unsigned char res04[8];
	//unsigned char res05[8];
	//unsigned char res06[8];
	struct st_rgb_threshold ch[5];

	//unsigned char res07[8];
	//unsigned char res08[8];
	//unsigned char res09[8];
	//unsigned char res10[8];
	//unsigned char res11[8];
	//struct st_rgb_threshold ch2;

	//unsigned char res12[8];
	//unsigned char res13[8];
	//unsigned char res14[8];
	//unsigned char res15[8];
	//unsigned char res16[8];
	//struct st_rgb_threshold ch3;

	//unsigned char res17[8];
	//unsigned char res18[8];
	//unsigned char res19[8];
	//unsigned char res20[8];
	//unsigned char res21[8];
	//struct st_rgb_threshold ch4;

	//unsigned char res22[8];
	//unsigned char res23[8];
	//unsigned char res24[8];
	//unsigned char res25[8];
	//unsigned char res26[8];
	//struct st_rgb_threshold ch5;

	//Prescaler htim2 for capture
	unsigned char clk_Prescaler_scan[2];
	unsigned char duty_ch3[2];
	unsigned char duty_ch2[2];
	unsigned char duty_ch4[2];
	unsigned char duty_ch1[2];
	unsigned char duty_ch5[2];
	unsigned char rgb_scale;

	unsigned char address;
	unsigned char bootloader;
	unsigned char res28;
	unsigned char res29[8];
	unsigned char res30[8];
	unsigned char res31[8];

	//unsigned char res32[8];
	unsigned char dmy[2];
	unsigned char footer[2];
	unsigned char crc[2];
	unsigned char checksum[2];
};
extern struct st_fact fact;
extern struct st_fact fact_bk;
extern unsigned char fact_write_flg;
extern unsigned char fact_write_tm;
extern unsigned short bootloader_tm;
extern unsigned char updatebootloader_flg;
extern unsigned long bootloader_len;
extern unsigned long bootloader_len_chk;
extern unsigned long bootloader_addr;
extern unsigned long bootloader_basic_addr;
extern uint32_t bootloader_buff[256];
extern unsigned char bootloader_error_flg;

struct st_pwm_drive
{
	unsigned char freq[2];
	unsigned char freq_bk[2];

	unsigned char left_up_on;
	unsigned char left_up_on_bk;
	unsigned char left_up_duty;//%
	unsigned char left_up_duty_bk;//%
	unsigned char left_up_value[2];

	unsigned char left_down_on;
	unsigned char left_down_on_bk;
	unsigned char left_down_duty;//%
	unsigned char left_down_duty_bk;//%
	unsigned char left_down_value[2];

	unsigned char right_up_on;
	unsigned char right_up_on_bk;
	unsigned char right_up_duty;//%
	unsigned char right_up_duty_bk;//%
	unsigned char right_up_value[2];

	unsigned char right_down_on;
	unsigned char right_down_on_bk;
	unsigned char right_down_duty;//%
	unsigned char right_down_duty_bk;//%
	unsigned char right_down_value[2];

	unsigned char duty_set;
	unsigned char step_duty;
	unsigned char duty_limit;//duty_limit > step_duty
};
extern struct st_pwm_drive drive;
extern struct st_pwm_drive drive_bk;
extern unsigned short drive_tm;

// Adjust mode
extern unsigned char adjust_on_flg;
extern unsigned char adjust_set_mode;

//flash test
extern unsigned char flash_erase_flg;
extern unsigned char flash_write_flg;
extern unsigned char flash_read_flg;

//RUN mode CDVL - CDBPD - CDIPDNADN
extern unsigned char car_run_mode_ena_flg;
extern unsigned char car_run_mode;
extern unsigned char car_run_mode_bk;
extern unsigned char car_run_mode_seq;
extern unsigned char car_run_mode_seq_tm;
extern unsigned short car_run_mode_tm;
extern unsigned char line_detect_ct;
extern unsigned char line_detect_flg;
extern unsigned char car_not_get_ir_flg;
extern unsigned char car_back_flg;
extern unsigned char car_start_flg;
extern unsigned char car_run_up_down_flg;
extern unsigned char car_run_up_down_flg_bk;
extern unsigned char car_run_ena;
extern unsigned char car_start_run_flg;
extern unsigned short car_adj_line_tm;
extern unsigned char car_adj_line_left_right;
extern unsigned long max_chenhlech;
extern unsigned long chn_chenhlech[5];
extern unsigned long rgb_chn_ct[5];
extern unsigned long rgb_chn_ct_cal[5];
extern unsigned long rgb_chn_ct_cal_max[5];
extern unsigned short car_tm;
extern unsigned char car_seq;

//Test
extern unsigned char mode_test_flg;
extern unsigned char buff_test[8];
extern unsigned char buff_file[2];
extern unsigned char mode_test2_flg;

// rasp
extern unsigned char ras_left_h;
extern unsigned char ras_left_l;
extern unsigned char ras_center;
extern unsigned char ras_right_l;
extern unsigned char ras_right_h;
extern unsigned char  ras_stop;
extern unsigned char ras_blink;
extern unsigned char ras_blink_bk;
extern unsigned short ras_blink_tot;
extern unsigned char ras_th;
extern unsigned char ras_th_bk;


