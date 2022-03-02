/* 
 * File:   ram.c
 * Author: FI
 *
 * Created on September 27, 2018, 8:52 PM
 */
//#include "stm32h7xx_hal.h"
#include "stm32f1xx_hal.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
IWDG_HandleTypeDef hiwdg;

//SPI_HandleTypeDef hspi1;

unsigned char htim1_flg;

//Uart1 with IR module
unsigned char t1_buff[16];
unsigned char r1_buff[16];
unsigned short t1_len;
unsigned short t1_num;
unsigned short t1_flg;
unsigned short r1_len;
unsigned short r1_pos;
unsigned char r1_flg;
unsigned short r1_tm;
unsigned char r1_buff2[128];

//Uart2 with PC
unsigned char t2_buff[1048];
unsigned char r2_buff[1048];
unsigned short t2_len;
unsigned short t2_num;
unsigned short t2_flg;
unsigned short r2_pos;
unsigned char r2_flg;
unsigned char r2_buff2[1048];

//Serial3/uart3 with bluetooth
unsigned char t3_buff[128];
unsigned char r3_buff[128];
unsigned short t3_len;
unsigned short t3_flg;
unsigned short r3_len;
unsigned short r3_tot;
unsigned char r3_flg;
unsigned char r3_buff2[128];
unsigned short r3_pos;

unsigned char bluetooth_buff[128];
unsigned short bluetooth_len;
unsigned char bluetooth_flg;

//SPI
unsigned char spi_tx_buff[32];
unsigned char spi_rx_buff[32];
unsigned char buff_index;

unsigned short one1sec_tm;
unsigned short led_tm;

//IR
struct st_IR_Format{
	unsigned char addr;
	unsigned char cmd;
	unsigned char data[3];
};
struct st_IR_Format IR_Format;
unsigned char ir_scan_flg;
unsigned char ir_scan_OnOff;
unsigned short ir_scan_tm;

unsigned char ir_run_flg;
unsigned char ir_run_OnOff;
unsigned short ir_run_tm;
unsigned char ir_chk_flg;
unsigned char ir_for_car_run_th1_flg;
unsigned char ir_for_car_run_th2_flg;
unsigned char ir_for_car_run_th3_flg;

unsigned char ir_send_must_ok_flg;
unsigned char ir_send_must_ok_type;
unsigned short ir_send_must_ok_tm;
unsigned short ir_send_must_ok_tmr;
unsigned char ir_new_data_flg;
unsigned char ir_new_data_type;
unsigned short driver_run_more_tm;

//send to due
unsigned char send_due_flg;
unsigned char send_due_type;
unsigned short send_due_tm;

//addr module
unsigned char addr_module;

//Couter frequency RGB
unsigned int clockCouter;
unsigned long cnt_bk1;
unsigned long cnt_bk2;
unsigned long cnt_bk3;
unsigned long cnt_bk4;
unsigned char s0_flg;
unsigned char s0_bk_flg;
unsigned char s1_flg;
unsigned char s1_bk_flg;
unsigned char s2_flg;
unsigned char s2_bk_flg;
unsigned char s3_flg;
unsigned char s3_bk_flg;

unsigned long Green_ct;
unsigned long freq_G;
unsigned long Red_ct;
unsigned long freq_R;
unsigned long Blue_ct;
unsigned long freq_B;
unsigned long nofiter_ct;
unsigned long freq_nofiter;

unsigned char scan_rgb_seq;
unsigned char scan_rgb_seq_bk;
unsigned char scan_rgb_tm;
unsigned char scan_rgb_tm_set;
unsigned char rgb_chk_ch1_flg;
unsigned char rgb_chk_ch2_flg;
unsigned char rgb_chk_ch3_flg;
unsigned char rgb_chk_ch4_flg;
unsigned char rgb_chk_ch5_flg;
unsigned char pwm_180_flg;
unsigned short rgb_pwm_flg;
unsigned short rgb_pwm_tot; 
unsigned char rgb_status_flg;
unsigned short grb_stats_tm;
unsigned short grb_stanby_tm;

// Channel RGB 
unsigned char chn_flg[5];
unsigned char rgb_chn_sort[5];
unsigned short rgb_wait_chk_tm;
unsigned char rgb_wait_seq;
unsigned char rgb_debug[8];
unsigned char rgb_status;
unsigned char test_motor_flg;

//threshold rgb check
//40byte
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
	
  //unsigned char res07[8];
	//unsigned char res08[8];
	//unsigned char res09[8];
	//unsigned char res10[8];
	//unsigned char res11[8];

	//unsigned char res12[8];
	//unsigned char res13[8];
	//unsigned char res14[8];
	//unsigned char res15[8];
	//unsigned char res16[8];

	//unsigned char res17[8];
	//unsigned char res18[8];
	//unsigned char res19[8];
	//unsigned char res20[8];
	//unsigned char res21[8];

	//unsigned char res22[8];
	//unsigned char res23[8];
	//unsigned char res24[8];
	//unsigned char res25[8];
	//unsigned char res26[8];	
	struct st_rgb_threshold ch[5];



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
struct st_fact fact;
struct st_fact fact_bk;
unsigned char fact_write_flg;
unsigned char fact_write_tm;
unsigned short bootloader_tm;
unsigned char updatebootloader_flg;
unsigned long bootloader_len;
unsigned long bootloader_len_chk;
unsigned long bootloader_addr;
unsigned long bootloader_basic_addr;
uint32_t bootloader_buff[256];
unsigned char bootloader_error_flg;

//control speed & vector drive
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
struct st_pwm_drive drive;
struct st_pwm_drive drive_bk;
unsigned short drive_tm;

// Adjust mode
unsigned char adjust_on_flg;
unsigned char adjust_set_mode;

//flash test
unsigned char flash_erase_flg;
unsigned char flash_write_flg;
unsigned char flash_read_flg;

//RUN mode CDVL - CDBPD - CDIPDNADN
unsigned char car_run_mode_ena_flg;
unsigned char car_run_mode;
unsigned char car_run_mode_bk;
unsigned char car_run_mode_seq;
unsigned char car_run_mode_seq_tm;
unsigned short car_run_mode_tm;
unsigned char line_detect_ct;
unsigned char line_detect_flg;
unsigned char car_not_get_ir_flg;
unsigned char car_back_flg;
unsigned char car_start_flg;
unsigned char car_run_up_down_flg;
unsigned char car_run_up_down_flg_bk;
unsigned char car_run_ena;
unsigned char car_start_run_flg;
unsigned short car_adj_line_tm;
unsigned char car_adj_line_left_right;
unsigned long max_chenhlech;
unsigned long chn_chenhlech[5];
unsigned long rgb_chn_ct[5];
unsigned long rgb_chn_ct_cal[5];
unsigned long rgb_chn_ct_cal_max[5];
unsigned short car_tm;
unsigned char car_seq;

//Test
unsigned char mode_test_flg;
unsigned char buff_test[8];
unsigned char buff_file[2];
unsigned char mode_test2_flg;

// rasp
unsigned char ras_left_h;
unsigned char ras_left_l;
unsigned char ras_center;
unsigned char ras_right_l;
unsigned char ras_right_h;
unsigned char ras_stop;
unsigned char ras_blink;
unsigned char ras_blink_bk;
unsigned short ras_blink_tot;
unsigned char ras_th;
unsigned char ras_th_bk;




