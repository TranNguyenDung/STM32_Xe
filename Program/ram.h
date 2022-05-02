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
extern unsigned char r1_buff2[16];

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
//addr module
extern unsigned char addr_module;
extern unsigned char addr_module_bk;

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
extern unsigned char ras_run_seq;
extern unsigned short ras_run_tm;
extern unsigned short ras_run_tot;
extern unsigned char system_shutdown_flg;
extern unsigned char ras_ok_flg;

//target stop
extern unsigned char signal_stop_flg;
extern unsigned char signal_run_flg;
extern unsigned char ls_add_pass[256];

//
extern unsigned char send2pc_flg;
extern unsigned short send2pc_tm;
extern unsigned char send2pc_addr;
extern unsigned char send2pc_addr_bk;
extern unsigned char send2pc_cmd;


