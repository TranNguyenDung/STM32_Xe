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
unsigned char r1_buff2[16];

//Uart2 with PC
unsigned char t2_buff[1048];
unsigned char r2_buff[1048];
unsigned short t2_len;
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

unsigned short one1sec_tm;
unsigned short led_tm;

//IR
struct st_IR_Format
{
	unsigned char addr;
	unsigned char cmd;
	unsigned char data[3];
};
struct st_IR_Format IR_Format;
//addr module
unsigned char addr_module;
unsigned char addr_module_bk;

//struct fact
struct st_fact
{
	//unsigned char res01[8];
	unsigned char header[2];
	unsigned char res01[6];

	unsigned char res02[8];
  unsigned char res03[8];
	unsigned char res04[8];
  unsigned char res05[8];
	unsigned char res06[8];
	
  unsigned char res07[8];
	unsigned char res08[8];
	unsigned char res09[8];
	unsigned char res10[8];
	unsigned char res11[8];

	unsigned char res12[8];
	unsigned char res13[8];
	unsigned char res14[8];
	unsigned char res15[8];
	unsigned char res16[8];

	unsigned char res17[8];
	unsigned char res18[8];
	unsigned char res19[8];
	unsigned char res20[8];
	unsigned char res21[8];

	unsigned char res22[8];
	unsigned char res23[8];
	unsigned char res24[8];
	unsigned char res25[8];
	unsigned char res26[8];	

	unsigned char duty_ch3[2];
	unsigned char duty_ch2[2];
	unsigned char duty_ch4[2];
	unsigned char duty_ch1[2];
	unsigned char duty_ch5[2];
	unsigned char address;
	unsigned char bootloader;
	unsigned char res28[4];
	
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
unsigned char ras_run_seq;
unsigned short ras_run_tm;
unsigned short ras_run_tot;
unsigned char system_shutdown_flg;
unsigned char ras_ok_flg;

//target stop
unsigned char signal_stop_flg;
unsigned char signal_run_flg;
unsigned char ls_add_pass[256];

//
unsigned char send2pc_flg;
unsigned short send2pc_tm;
unsigned char send2pc_addr;
unsigned char send2pc_addr_bk;
unsigned char send2pc_cmd;


