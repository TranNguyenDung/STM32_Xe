//Note at tao code
//1:Uart
//#include "stm32h7xx_hal.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "program.h"
#include "ram.h"
#include "table.h"
#include "flashV2.h"
#include "const_def.h"
#include <string.h>
/*---------------------------------------------------------------*/

//----------------------------------------------
/*
main program
  //init_fact();
	//dmy = 0;
	if(read_fact() == 0){
		init_fact();
		#if 0
		write_fact();
		#endif // 0
	}
	fact_bk = fact;
  //htim2.Init.Prescaler = (unsigned short)(fact.clk_Prescaler_scan[0] << 8) | fact.clk_Prescaler_scan[1];
  //htim5.Init.Prescaler = (unsigned short)(fact.clk_Prescaler_scan[0] << 8) | fact.clk_Prescaler_scan[1];
*/
//----------------------------------------------

//Main program
void program(void)
{
	//unsigned short i;
	//uint32_t data;
	//unsigned char tmp;
	//unsigned char dmy;
	//unsigned char *pos;
	//char data[50] = {0};
	//unsigned char buff[256];
	//unsigned char flg;

#if 0//Read fact
	dmy = read_fact();
	//dmy = 0;
	if(dmy == 0){
		init_fact();
		#if 0
		write_fact();
		#endif // 0
	}
#endif//Read fact

	//timer 10ms for freqency loop code
	//Freq Tim1: 120MHz
	HAL_TIM_Base_Start_IT(&htim1);

	//With communication module IR
	HAL_UART_Receive_IT(&huart1,&r1_buff2[0],sizeof(r1_buff2));

	//with communication PC
	HAL_UART_Receive_IT(&huart2,&r2_buff2[0],sizeof(r2_buff2));

	//with communication bluetooth
	HAL_UART_Receive_IT(&huart3,&r3_buff2[0],sizeof(r3_buff2));
	
	//interrup RGB
	//HAL_TIM_Base_Start_IT(&htim2);
	//HAL_TIM_Base_Start_IT(&htim5);

	clockCouter = (72 * 10 ^ 6);//(72 * 10 ^ 6) / (htim2.Init.Prescaler + 1);
	//HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	//HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
	//HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
	//HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_4);
	//HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_1);
	HAL_TIM_Base_Start(&htim2);
	//HAL_TIM_Base_Start_IT(&htim3);
		
#if 0//TEST FLASH
	//write 256 byte to flash
	Flash_Erase(ADDR_FACT);
	pos = &buff[0];
	for(i = 0;i < 256;i++){
		*(pos + i) = i;
	}
	//Flash_Write_Page(ADDR_FACT,pos);
	flash_write_pages_tm32f(ADDR_FACT,pos,256);
	pos = &buff[0];
	for(i = 0;i < 256;i++){
		*(pos + i) = 0;
	}
	Flash_Read_page_stm32f(ADDR_FACT,pos,256);
#endif	//TEST FLASH

#if 0//PWM TEST
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	//HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
	//HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
#endif//PWM TEST

	init_data();
	init_drive();
	//test_crc16();

	//Read address module
	read_fact_read_address();
	//read_fact_write_address(48);
	addr_module = fact.address;
	
	while(1){
		while(htim1_flg == 0);//10ms frequency
		htim1_flg = 0;
		REFREST_WWDR
		
		opera();
		uart2_with_due();
		mem_set();
		serial2_comm();//Xử lý nhận data IR
		serial3_communication();//Xử lý nhận data blue
		get_status_rasp();
		drive_xe();
		pwm_drive_opera();
	}
}
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//INIT data
/*----------------------------------------------------------------*/
#if 1
//Init Data
void init_data(void)
{
	//car_run_mode = CAR_RUN_CDIPDNADN;
	car_run_mode = CAR_RUN_MERGE_LINE;
	car_run_mode_bk = car_run_mode;
	car_back_flg = 0;
	car_not_get_ir_flg = 0;
	car_start_flg = 0;
	
	fact_write_tm = 0;
	car_run_ena = 0;
	car_run_up_down_flg_bk = CAR_RUN_DOWN;
	car_run_up_down_flg = CAR_RUN_DOWN;
	//car_run_up_down_flg = CAR_RUN_UP;
	rgb_pwm_tot = 1;
	rgb_status = RGB_STATUS_FULL_CHIP;
	car_start_run_flg = 1;
	grb_stanby_tm = 100;
	//rgb_pwm();
}

//Init fact
void init_fact(void)
{
	unsigned char i;
	unsigned long dmy;

	fact.header[0] = 0x55;
	fact.header[1] = 0xAA;
	
	for(i = 0; i < 5; i++){
		dmy =5000;
		fact.ch[i].red_thr[0] = (dmy & 0xFF000000) >> 24;
		fact.ch[i].red_thr[1] = (dmy & 0x00FF0000) >> 16;
		fact.ch[i].red_thr[2] = (dmy & 0x0000FF00) >> 8;
		fact.ch[i].red_thr[3] = (dmy & 0x000000FF) >> 0;
		fact.ch[i].red_thr_chk_flg = 1;
		
		dmy = 5000;
		fact.ch[i].green_thr[0] = (dmy & 0xFF000000) >> 24;
		fact.ch[i].green_thr[1] = (dmy & 0x00FF0000) >> 16;
		fact.ch[i].green_thr[2] = (dmy & 0x0000FF00) >> 8;
		fact.ch[i].green_thr[3] = (dmy & 0x000000FF) >> 0;
		//fact.ch1.green_thr_chk_flg = 1;
		fact.ch[i].green_thr_chk_flg = 0;
		
		dmy = 5000;
		fact.ch[i].blue_thr[0] = (dmy & 0xFF000000) >> 24;
		fact.ch[i].blue_thr[1] = (dmy & 0x00FF0000) >> 16;
		fact.ch[i].blue_thr[2] = (dmy & 0x0000FF00) >> 8;
		fact.ch[i].blue_thr[3] = (dmy & 0x000000FF) >> 0;
		//fact.ch[i].blue_thr_chk_flg = 1;
		fact.ch[i].blue_thr_chk_flg = 0;
		
		dmy = 5000;
		fact.ch[i].nofiter_thr[0] = (dmy & 0xFF000000) >> 24;
		fact.ch[i].nofiter_thr[1] = (dmy & 0x00FF0000) >> 16;
		fact.ch[i].nofiter_thr[2] = (dmy & 0x0000FF00) >> 8;
		fact.ch[i].nofiter_thr[3] = (dmy & 0x000000FF) >> 0;
		//fact.ch[i].nofiter_thr_chk_flg = 1;
		fact.ch[i].nofiter_thr_chk_flg = 0;

		fact.ch[i].value_at_over_thr = 1;

		//Offset
		dmy = 500;
		fact.ch[i].offset[0] = (dmy & 0xFF00) >> 8;
		fact.ch[i].offset[1] = (dmy & 0x00FF) >> 0;
	}
	
	//frequency scan RGB
	dmy = 5;
	fact.clk_Prescaler_scan[0] = (dmy & 0xFF00) >> 8;
	fact.clk_Prescaler_scan[1] = (dmy & 0x00FF) >> 0;

	fact.duty_ch3[0] = 50;//L
	fact.duty_ch3[1] = 50;//R

	fact.duty_ch2[0] = 50;//L
	fact.duty_ch2[1] = 50;//R

	fact.duty_ch4[0] = 50;//L
	fact.duty_ch4[1] = 50;//R

	fact.duty_ch1[0] = 50;//L
	fact.duty_ch1[1] = 50;//R

	fact.duty_ch5[0] = 50;//L
	fact.duty_ch5[1] = 50;//R

	fact.rgb_scale = FREQ_LOW;
	
	
	fact.address = 0xFF;
	
	// 0xAA not next app address
	// 0x55 next app address
	fact.bootloader = 0x55;

	fact.footer[0] = 0x55;
	fact.footer[1] = 0xAA;
	
	fact_bk = fact;
}

//init drive
void init_drive()
{
	drive.freq[0] = (htim3.Instance->ARR & 0xFF00) >> 8;
	drive.freq[1] = (htim3.Instance->ARR & 0x00FF) >> 0;
	drive.freq_bk[0] = drive.freq[0];
	drive.freq_bk[1] = drive.freq[1];

	drive.left_up_on = 0;
	drive.left_up_on_bk = 0;
	//drive.left_duty = htim3.Init.Period / 2;
	//drive.left_duty = 30;
	drive.left_up_duty = 18;
	//drive.left_duty_bk = htim3.Init.Period / 2;
	//drive.left_duty_bk = 30;
	drive.left_up_duty_bk = 18;
	drive.left_up_value[0] = 0;
	drive.left_up_value[1] = 0;

	drive.left_down_on = 0;
	drive.left_down_on_bk = 0;
	//drive.left_duty1 = htim3.Init.Period / 2;
	//drive.left_duty1 = 30;
	drive.left_down_duty = 18;
	//drive.left_duty_bk1 = htim3.Init.Period / 2;
	//drive.left_duty_bk1 = 30;
	drive.left_down_duty_bk = 18;
	drive.left_down_value[0] = 0;
	drive.left_down_value[1] = 0;

	drive.right_up_on = 0;
	drive.right_up_on_bk = 0;
	//drive.right_duty = htim3.Init.Period / 2;
	//drive.right_duty = 30;
	drive.right_up_duty = 18;
	//drive.right_duty_bk = htim3.Init.Period / 2;
	//drive.right_duty_bk = 30;
	drive.right_up_duty_bk = 18;
	drive.right_up_value[0] = 0;
	drive.right_up_value[1] = 0;

	drive.right_down_on = 0;
	drive.right_down_on_bk = 0;
	//drive.right_duty1 = htim3.Init.Period / 2;
	//drive.right_duty1 = 30;
	drive.right_down_duty = 18;
	//drive.right_duty_bk1 = htim3.Init.Period / 2;
	//drive.right_duty_bk1 = 30;
	drive.right_down_duty_bk = 18;
	drive.right_down_value[0] = 0;
	drive.right_down_value[1] = 0;

	drive.duty_set = 18;
	drive.step_duty = 1;
	drive.duty_limit = 18;

	drive_bk = drive;

	drive_tm = 100;
}
#endif // 1
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//System opera/timer...
/*----------------------------------------------------------------*/
#if 1
#define COUNT_DOWN(x)  			if(x>0){x--;}
#define COUNT_DOWN_LIMIT(x,lm)  if(x>lm){x--;}
//Couter tm run on interrupt
void timer_couter(void)
{
	COUNT_DOWN(one1sec_tm)
	COUNT_DOWN(ir_scan_tm)//couter resend to CAR
	COUNT_DOWN(ir_run_tm)
	COUNT_DOWN(send_due_tm)//couter resend to due
	COUNT_DOWN(scan_rgb_tm)
	COUNT_DOWN(ir_send_must_ok_tm)//re-send to Dua via IR
	COUNT_DOWN(rgb_pwm_tot)
	COUNT_DOWN(driver_run_more_tm)//Run more space for face to face module IR
	COUNT_DOWN(car_run_mode_tm)
	COUNT_DOWN(car_run_mode_seq_tm)
	COUNT_DOWN(rgb_wait_chk_tm)
	COUNT_DOWN(drive_tm)
	COUNT_DOWN(grb_stanby_tm)
	COUNT_DOWN(car_tm)
	COUNT_DOWN(ras_blink_tot)
	
	COUNT_DOWN_LIMIT(r1_tm,1)//TimeOut receiver module IR
	COUNT_DOWN_LIMIT(r3_tot,1)
	COUNT_DOWN_LIMIT(bootloader_tm,1)
	COUNT_DOWN_LIMIT(fact_write_tm,1)
}

//
void opera(void)
{
	unsigned char i;

	if(one1sec_tm == 0){
		if(updatebootloader_flg && (bootloader_error_flg == 0)){
			one1sec_tm = 20;
			//Blink LED
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}		
		else if(bootloader_error_flg == 1){
			one1sec_tm = 5;
			//Blink LED
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);

			//
			//HAL_GPIO_TogglePin(LED_TEST1_GPIO_Port,LED_TEST1_Pin);
			//HAL_GPIO_TogglePin(LED_TEST2_GPIO_Port,LED_TEST2_Pin);

			//HAL_GPIO_TogglePin(LED_PHAT01_GPIO_Port,LED_PHAT01_Pin);
			//HAL_GPIO_TogglePin(LED_PHAT02_GPIO_Port,LED_PHAT02_Pin);
		}
		// Error bootloader 
		else{
			one1sec_tm = 100;
			//Blink LED
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
	}
	
	if(ras_blink ^ ras_blink_bk){
		ras_blink_bk = ras_blink;
		ras_blink_tot = 200;
	}

	if(ras_th_bk ^ ras_th){
		ras_th_bk = ras_th;
		if(ras_th_bk == 0){
			HAL_GPIO_WritePin(RAS_SET_TH_H_GPIO_Port,RAS_SET_TH_H_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RAS_SET_TH_L_GPIO_Port,RAS_SET_TH_L_Pin,GPIO_PIN_RESET);			
		}
		else if(ras_th_bk == 1){
			HAL_GPIO_WritePin(RAS_SET_TH_H_GPIO_Port,RAS_SET_TH_H_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(RAS_SET_TH_L_GPIO_Port,RAS_SET_TH_L_Pin,GPIO_PIN_RESET);	
		}
		else{
			if(ras_th != 2){
				ras_th = 2;
				ras_th_bk = 2;
			}
			HAL_GPIO_WritePin(RAS_SET_TH_H_GPIO_Port,RAS_SET_TH_H_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(RAS_SET_TH_L_GPIO_Port,RAS_SET_TH_L_Pin,GPIO_PIN_SET);		
		}
	}
	
#if 0
	// Goto bootloader
	if(fact.bootloader == JIMP_BOOTLOADER){
		if(bootloader_tm == 1){
			bootloader_tm = 0;
			//HAL_DeInit();
			REFREST_WWDR
			flash_jump_to_bootloader();
		}
	}
	else{
		if(bootloader_tm){
			bootloader_tm = 0;
		}
	}
#endif
	
	//Uart1 time out next to process
	if(r1_tm == 1){
		r1_tm = 0;
		r1_flg = 1;
	}
	
	if(r3_tot == 1){
		r3_tot = 0;
		r3_flg = 1;
	}
	
	if(flash_erase_flg){
		flash_erase_flg = 0;
		//erase_sector(FLASH_FACT_BANK_SECTOR);
	}

#if 0
	//flash test
	if(flash_erase_flg){
	flash_erase_flg = 0;
		//save_to_flash(&tmp);
		erase_sector(FLASH_FACT_BANK2_SECTOR);
	}

	if(flash_write_flg){
		flash_write_flg = 0;
		//i = 0;
		//data[i++] = 0x01020304;
		//data[i++] = 0x05060708;
		//data[i++] = 0x090A0B0C;
		//data[i++] = 0x0D0E0F10;
		//data[i++] = 0x11121314;
		//data[i++] = 0x15161719;
		//data[i++] = 0x1A1B1C1D;
		//data[i++] = 0x1E1F2021;
		//flash_write1(&data,0x081E0000,32);
		//flash_write_v2(&data[0],0x081E0000,1);
		//flash_write_v2(&data[0],0x08100000,1);
		//flash_write_v2(&data[0],0x08120000,1);
		//fact_write_page();
	}

	if(flash_read_flg){
		flash_read_flg = 0;
		//data = Flash_Read(0x08100000 | (1 << 1) << 16);
		//read_fact();
		//flash_read(&tmp[0],0);
		__nop();
	}
#endif // 0

	if(car_run_mode ^ car_run_mode_bk){
		car_run_mode_bk = car_run_mode;
		setup_run_th(car_run_mode);
	}

	if(mode_test_flg){
		mode_test_flg = 0;
		i = 0;
		//uart2_send_string("TT~AU02",strlen("TT~AU02"));
		buff_test[i++] = 'T';
		buff_test[i++] = 'T';
		buff_test[i++] = '~';
		buff_test[i++] = 'A';
		buff_test[i++] = 'U';
		buff_test[i++] = buff_file[0];
		buff_test[i++] = buff_file[1];
		uart2_send_string(&buff_test[0],i);
	}

#if 0
	if(mode_test2_flg){
		if(drive.left_up_on == 0){
			drive.left_up_on = 1;
		}
		if(drive.right_up_on == 0){
			drive.right_up_on = 1;
		}
	}
	else{
		if(drive.left_up_on){
			drive.left_up_on = 0;
		}
		if(drive.right_up_on){
			drive.right_up_on = 0;
		}
	}
#endif
}

//get status rasp
void get_status_rasp()
{
	ras_left_h = HAL_GPIO_ReadPin(RAS_LEFT_H_GPIO_Port,RAS_LEFT_H_Pin);
	ras_left_l = HAL_GPIO_ReadPin(RAS_LEFT_L_GPIO_Port,RAS_LEFT_L_Pin);
	ras_center = HAL_GPIO_ReadPin(RAS_CENTER_GPIO_Port,RAS_CENTER_Pin);
	ras_right_l = HAL_GPIO_ReadPin(RAS_RIGHT_L_GPIO_Port,RAS_RIGHT_L_Pin);
	ras_right_h = HAL_GPIO_ReadPin(RAS_RIGHT_H_GPIO_Port,RAS_RIGHT_H_Pin);
	ras_stop = HAL_GPIO_ReadPin(RAS_STOP_GPIO_Port,RAS_STOP_Pin);
	ras_blink = HAL_GPIO_ReadPin(RAS_BLINK_GPIO_Port,RAS_BLINK_Pin);
}
#endif // 1
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//Memory
/*----------------------------------------------------------------*/
#if 1
//mem flash
void mem_set(void)
{
	if(fact_write_tm == 1){
		fact_write_tm = 0;
		REFREST_WWDR
		write_fact();
		REFREST_WWDR
	}
	
	//reset get new value
	if((fact.clk_Prescaler_scan[0] ^ fact_bk.clk_Prescaler_scan[0]) ||
		 (fact.clk_Prescaler_scan[1] ^ fact_bk.clk_Prescaler_scan[1])){
		//write_fact();
		while(1);//wait WDT Reset
	}
}

// Read fact
unsigned char read_fact()
{
	struct st_fact tmp;
	unsigned char flg;
	unsigned char *pos;
	unsigned char *pos2;
	unsigned short i;
	
	flg = 1;
	pos = &tmp.header[0];
	flash_read(FACT_ADDRESS,pos,sizeof(tmp));
	
	if(tmp.header[0] != 0x55){
		flg = 0;
	}
	else if(tmp.header[1] != 0xAA){
		flg = 0;
	}
	else if(tmp.footer[0] != 0x55){
		flg = 0;
	}
	else if(tmp.footer[1] != 0xAA){
		flg = 0;
	}
	else{
		flg = 1;
	}
	
	if(flg){
		pos2 = &fact.header[0];
		for(i=0;i<sizeof(tmp);i++){
			*(pos2 + i) = *(pos + i);
		}
	}
	
	return flg;
}

// Write fact
void write_fact()
{
	unsigned char *pos;
	
	flash_eraseV2(FACT_ADDRESS);
	pos = &fact.header[0];
	flash_writeV2(FACT_ADDRESS,pos,sizeof(fact));
}

// Read fact & write address
void read_fact_write_address(unsigned char address)
{
	unsigned char flg;

	flg = read_fact();
	if(flg == 0){
		init_fact();
	}
	fact.address = address;
	addr_module = fact.address;
	REFREST_WWDR
	write_fact();
	REFREST_WWDR
}

// read fact & read address
void read_fact_read_address()
{
	unsigned char flg;
	flg = read_fact();
	if(flg == 0){
		init_fact();
		//write_fact();
	}
	fact_bk = fact;
	opera();
	addr_module = fact.address;
	fact_bk = fact;
	REFREST_WWDR
}

#endif // 1
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//With RGB & PWM => set speed CAR
/*----------------------------------------------------------------*/
#if 1
// drive xe
void drive_xe()
{
	if(ras_blink_tot == 0){
		car_stop();
	}
	else if(ras_stop && 
		ras_left_h && 
		ras_left_l && 
		ras_center && 
		ras_right_l && 
		ras_right_h){
		car_stop();
	}
	else if((ras_stop == 1) || 
	   ((ras_left_h == 0) &&
		(ras_left_l == 0) && 
		(ras_center == 0) && 
		(ras_right_l == 0) && 
		(ras_right_h == 0))){
		car_stop();
	}
	else if((ras_stop == 0) &&
			((ras_left_h == 1) ||
			 (ras_left_l == 1) ||
			 (ras_center == 1) ||
			 (ras_right_l == 1) ||
			 (ras_right_h == 1))){
		if(ras_left_h){
			//RASP on Left High
			right_up(0);
			left_up(50);
			//     |
			// OOOOO
		}
		else if(ras_left_l){
			//RASP on Left Low
			right_up(40);
			left_up(50);
			//     |
            //  OOOOO
		}
		else if(ras_center){
			//RASP Center
			right_up(40);
			left_up(40);
			//     |
            //   OOOOO
		}
		else if(ras_right_l){
			//RASP on Right Low
			right_up(50);
			left_up(40);
			//     |
            //    OOOOO
		}
		else if(ras_right_h){
			//RASP on Right High
			right_up(50);
			left_up(0);
			//     |
			//     OOOOO
		}
	}
	else{
		// Stop
		car_stop();
	}

}

//grb set PWM
void rgb_pwm()
{
	unsigned long tmp;
	unsigned char flg;
	unsigned char i;
	unsigned char i_bk;

	//------------------------------------------------------
	// Receive data from module IR
	if(ir_new_data_type == IR_CMD_STOP){
		if(driver_run_more_tm == 0){
			//Stop PWM
			if(drive.left_up_on){
				drive.left_up_on = 0;
			}
			if(drive.left_down_on){
				drive.left_down_on = 0;
			}
			if(drive.right_up_on){
				drive.right_up_on = 0;
			}
			if(drive.right_down_on){
				drive.right_down_on = 0;
			}	
		}
	}
	else{
		//When new value
		if(rgb_pwm_tot && (grb_stanby_tm == 0)){

			//-----------------------------------------------
			#if 1//Tìm Chênh lệch lớn nhất

			if(test_motor_flg == 0){
				max_chenhlech = 0;
				i_bk = 0;
				flg = 0;
				//Stop
				if((chn_flg[1] != 0) && (chn_flg[2] != 0) && (chn_flg[3] != 0)){

					if(rgb_status == RGB_STATUS_FULL_CHIP){
						// Up
						if(car_run_up_down_flg_bk == CAR_RUN_UP){
							if(car_run_up_down_flg == CAR_RUN_DOWN){
								rgb_chk_ch3_flg = 1;
							}

							if(car_seq == 0){
								car_seq = 1;
								car_tm = 50;
							}

							//car_run_up_down_flg = car_run_ena ? CAR_STOP : CAR_RUN_DOWN;
							car_run_up_down_flg = car_run_ena ? CAR_RUN_DOWN : CAR_RUN_DOWN;
						}
						// Down
						else{
							if(car_run_up_down_flg == CAR_RUN_UP){
								rgb_chk_ch3_flg = 1;
							}
							if(car_seq == 0){
								car_seq = 1;
								car_tm = 50;
							}
							car_run_up_down_flg = car_run_ena ? CAR_RUN_UP : CAR_STOP;

							// Start Power
							if(car_start_run_flg){
								rgb_chk_ch3_flg = 0;
								car_start_run_flg = 0;
								car_run_up_down_flg = CAR_STOP;
							}
						}
					}

					//Update status
					if(rgb_status != RGB_STATUS_FULL_CHIP){
						rgb_status = RGB_STATUS_FULL_CHIP;
					}
				}
				//---------------------------------------------
				else{
					if(car_seq){
						car_seq = 0;
					}
					//Update status
					if(rgb_status != RBB_STATUS_SEE_HIGH_LEVEL){
						rgb_status = RBB_STATUS_SEE_HIGH_LEVEL;
					}
					if(car_run_up_down_flg_bk ^ car_run_up_down_flg){
						car_run_up_down_flg_bk = car_run_up_down_flg;
					}
					rgb_wait_seq = 0;
					flg = 0;
					//Chon độ lệch tần số lớn nhất
					for(i = 0; i < sizeof(chn_flg);i++){
						tmp = 0;
						if(chn_flg[i]){
							if(chn_chenhlech[i] > max_chenhlech + tmp){
								max_chenhlech = chn_chenhlech[i];
								i_bk = i;
								flg = 1;
							}
						}
					}

					// Init detect line
					if(line_detect_flg){
						line_detect_flg = 0;
					}

					//Check & set
					if(flg){
						if(i_bk == 0){
							rgb_chk_ch1_flg = 1;
						}
						else if(i_bk == 1){
							rgb_chk_ch2_flg = 1;
						}
						else if(i_bk == 2){
							rgb_chk_ch3_flg = 1;
						}
						else if(i_bk == 3){
							rgb_chk_ch4_flg = 1;
						}
						else if(i_bk == 4){
							rgb_chk_ch5_flg = 1;
						}
					}
					//Stop PWM Out line
					else{
					}
				}
			}

			#endif // 1//Tìm Chênh lệch lớn nhất
			//-----------------------------------------------

			#if 1//Get Fact data PWM to data PWM
			// Init PWM
			car_init();
			if(car_tm == 0){
				// run UP
				if(car_run_up_down_flg == CAR_RUN_UP){
					//Cho mục đích lấy thông tin RGB
					if(adjust_on_flg){
						//Stop drive when Adjust
						if(adjust_set_mode == 0){
							//Stop drive when Adjust
							car_stop();
						}
						else if(adjust_set_mode == 1){
							left_down(fact.duty_ch3[L]);
						}
						else if(adjust_set_mode == 2){
							left_up(fact.duty_ch3[L]);
						}
						else if(adjust_set_mode == 3){
							right_down(fact.duty_ch3[R]);
						}
						else{
							right_up(fact.duty_ch3[R]);
						}
					}
					else if(rgb_chk_ch3_flg){
						left_up(fact.duty_ch3[L]);
						right_up(fact.duty_ch3[R]);
					}
					else if(rgb_chk_ch2_flg){
						left_up(fact.duty_ch2[L]);
						right_up(fact.duty_ch2[R]);
					}
					else if(rgb_chk_ch4_flg){
						left_up(fact.duty_ch4[L]);
						right_up(fact.duty_ch4[R]);
					}
					else{		
						car_stop();
					}
					car_adj_line_tm = 0;
					car_adj_line_left_right = 0;
				}
				// run Down
				else if(car_run_up_down_flg == CAR_RUN_DOWN){
					//Cho mục đích lấy thông tin RGB
					if(adjust_on_flg){
						// Stop drive when Adjust
						if(adjust_set_mode == 0){
							car_stop();
						}
						else if(adjust_set_mode == 1){
							left_down(fact.duty_ch3[L]);
						}
						else if(adjust_set_mode == 2){
							left_up(fact.duty_ch3[L]);
						}
						else if(adjust_set_mode == 3){
							right_down(fact.duty_ch3[R]);
						}
						else{
							right_up(fact.duty_ch3[R]);
						}
					}
					else{
						// Run Down[LEFT = RIGHT]
						left_down(fact.duty_ch3[L]);
						right_down(fact.duty_ch3[R]);
					}
				}
				// Stop
				else{
					car_stop();
				}			
			}
			else{
				car_stop();
			}
			#endif // 1//Get Fact data PWM to data PWM
			send_status_car();
			//-----------------------------------------------
		}
		else{// Not Get Signal RGB
		    car_stop();
		}
	}
}
// Send status to PC
void send_status_car() 
{
	unsigned char buff[5];
	if(rgb_status_flg){
		if(grb_stats_tm == 0){
			buff[0] = rgb_chk_ch1_flg ? 0xAA : 0x00;
			buff[1] = rgb_chk_ch2_flg ? 0xAA : 0x00;
			buff[2] = rgb_chk_ch3_flg ? 0xAA : 0x00;
			buff[3] = rgb_chk_ch4_flg ? 0xAA : 0x00;
			buff[4] = rgb_chk_ch5_flg ? 0xAA : 0x00;
			uart2_send_buff(&buff[0],sizeof(buff));
			//Time 1sec
			grb_stats_tm = 100;
		}
		else{
			grb_stats_tm--;
		}
	}
}

// Car stop
void car_stop()
{
	left_stop();
	right_stop();
}

// Car Run Down
void car_run_down_sp1()
{


}

// Car Run UP
void car_run_up_sp1()
{

}

// Right Up
void right_up(unsigned char duty)
{
	if(duty > 99){
		duty = 99;
	}
	drive.right_up_on = 1;
	drive.right_up_duty = duty;
}

// Right Down
void right_down(unsigned char duty)
{
	if(duty > 99){
		duty = 99;
	}

	drive.right_down_on = 1;
	drive.right_down_duty = duty;
}

// left Up
void left_up(unsigned char duty)
{
	if(duty > 99){
		duty = 99;
	}
	drive.left_up_on = 1;
	drive.left_up_duty = duty;
}

// Left down
void left_down(unsigned char duty)
{
	if(duty > 99){
		duty = 99;
	}
	drive.left_down_on = 1;
	drive.left_down_duty = duty;
}

// Left stop
void left_stop()
{
	if(drive.left_up_on){
		drive.left_up_on = 0;
	}
	if(drive.left_down_on){
		drive.left_down_on = 0;
	}
}

// Right stop
void right_stop()
{
	if(drive.right_up_on){
		drive.right_up_on = 0;
	}
	if(drive.right_down_on){
		drive.right_down_on = 0;
	}
}

// Car init
void car_init()
{
	//Init PWM
	drive.left_up_on = 0;
	drive.left_down_on = 0;
	drive.right_up_on = 0;
	drive.right_down_on = 0;
}

//Thiết lập chạy trường hợp CDVL - CD&PD - CDI&PDN&ADN
void setup_run_th(unsigned char mode)
{
	car_run_mode = mode;
	car_run_mode_seq = 0;
	car_run_mode_tm = 0;
	line_detect_ct = 0;
}

//Từ data thiết lập thanh ghi điều khiển(Tốc độ)
void pwm_drive_opera(void)
{
	unsigned char duty;
	unsigned short tmp;
	if(drive_tm == 0){
		if(drive.left_up_on){
			if(drive.left_up_on ^ drive.left_up_on_bk){
				if(drive.left_up_duty){
					drive.left_up_on_bk = drive.left_up_on;
					drive.left_up_duty_bk = drive.left_up_duty;
				
					if((drive.freq[0] ^ drive.freq_bk[0]) || (drive.freq[1] ^ drive.freq_bk[1])){
						drive.freq_bk[0] = drive.freq[0];
						drive.freq_bk[1] = drive.freq[1];
						tmp = drive.freq[0];
						tmp <<= 8;
						tmp |= drive.freq[1];
						set_freq(tmp);
					}
					duty = drive.left_up_duty;
					set_duty(duty,TIM_CHANNEL_3);
					HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);			
				}
			}
			else if(drive.left_up_duty ^ drive.left_up_duty_bk){
				drive.left_up_duty_bk = drive.left_up_duty;
				duty = drive.left_up_duty;
				set_duty(duty,TIM_CHANNEL_3);
			}
			//enable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_LEFT_EN_GPIO_Port,PWM_LEFT_EN_Pin) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(PWM_LEFT_EN_GPIO_Port,PWM_LEFT_EN_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.left_up_on ^ drive.left_up_on_bk){
				drive.left_up_on_bk = drive.left_up_on;
				//set_duty(drive.left_duty,TIM_CHANNEL_1);
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_3);			
			}
		}

		//WHEEL LEFT1
		if(drive.left_down_on){
			if(drive.left_down_on ^ drive.left_down_on_bk){
				if(drive.left_down_duty){
					drive.left_down_on_bk = drive.left_down_on;
					drive.left_down_duty_bk = drive.left_down_duty;

					if((drive.freq[0] ^ drive.freq_bk[0]) || (drive.freq[1] ^ drive.freq_bk[1])){
						drive.freq_bk[0] = drive.freq[0];
						drive.freq_bk[1] = drive.freq[1];
						tmp = drive.freq[0];
						tmp <<= 8;
						tmp |= drive.freq[1];
						set_freq(tmp);
					}
					duty = drive.left_down_duty;
					set_duty(duty,TIM_CHANNEL_4);
					HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
				}
			}
			else if(drive.left_down_duty ^ drive.left_down_duty_bk){
				drive.left_down_duty_bk = drive.left_down_duty;
				duty = drive.left_down_duty;
				set_duty(duty,TIM_CHANNEL_4);
			}
			//enable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_LEFT_EN_GPIO_Port,PWM_LEFT_EN_Pin) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(PWM_LEFT_EN_GPIO_Port,PWM_LEFT_EN_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.left_down_on ^ drive.left_down_on_bk){
				drive.left_down_on_bk = drive.left_down_on;
				//set_duty(drive.left_duty,TIM_CHANNEL_1);
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
			}
		}

		//WHEEL RIGHT
		if(drive.right_up_on){
			if(drive.right_up_on ^ drive.right_up_on_bk){
				//if(drive.right_duty){}
				drive.right_up_on_bk = drive.right_up_on;
				drive.right_up_duty_bk = drive.right_up_duty;
				
				if((drive.freq[0] ^ drive.freq_bk[0]) || (drive.freq[1] ^ drive.freq_bk[1])){
					drive.freq_bk[0] = drive.freq[0];
					drive.freq_bk[1] = drive.freq[1];
					tmp = drive.freq[0];
					tmp <<= 8;
					tmp |= drive.freq[1];
					set_freq(tmp);
				}
				duty = drive.right_up_duty;
				set_duty(duty,TIM_CHANNEL_2);
				HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);			
			
			}
			else if(drive.right_up_duty ^ drive.right_up_duty_bk){
				drive.right_up_duty_bk = drive.right_up_duty;
				duty = drive.right_up_duty;
				set_duty(duty,TIM_CHANNEL_2);
			}
			//enable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_RIGHT_EN_GPIO_Port,PWM_RIGHT_EN_Pin) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(PWM_RIGHT_EN_GPIO_Port,PWM_RIGHT_EN_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.right_up_on ^ drive.right_up_on_bk){
				drive.right_up_on_bk = drive.right_up_on;
				//set_duty(drive.right_duty,TIM_CHANNEL_2);
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);			
			}
		}


		//WHEEL RIGHT
		if(drive.right_down_on){
			if(drive.right_down_on ^ drive.right_down_on_bk){
				//if(drive.right_duty1){}
				drive.right_down_on_bk = drive.right_down_on;
				drive.right_down_duty_bk = drive.right_down_duty;

				if((drive.freq[0] ^ drive.freq_bk[0]) || (drive.freq[1] ^ drive.freq_bk[1])){
					drive.freq_bk[0] = drive.freq[0];
					drive.freq_bk[1] = drive.freq[1];
					tmp = drive.freq[0];
					tmp <<= 8;
					tmp |= drive.freq[1];
					set_freq(tmp);
				}
				duty = drive.right_down_duty;
				set_duty(duty,TIM_CHANNEL_1);
				HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
			
			}
			else if(drive.right_down_duty ^ drive.right_down_duty_bk){
				drive.right_down_duty_bk = drive.right_down_duty;

				duty = drive.right_down_duty;
				set_duty(duty,TIM_CHANNEL_1);
			}
			//enable PWM ENA			
			if(HAL_GPIO_ReadPin(PWM_RIGHT_EN_GPIO_Port,PWM_RIGHT_EN_Pin) == GPIO_PIN_RESET){
				HAL_GPIO_WritePin(PWM_RIGHT_EN_GPIO_Port,PWM_RIGHT_EN_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.right_down_on ^ drive.right_down_on_bk){
				drive.right_down_on_bk = drive.right_down_on;
				//set_duty(drive.right_duty,TIM_CHANNEL_2);
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
			}
		}

		if((drive.right_up_on == 0) && 
		   (drive.right_down_on == 0)){
			//disable PWM ENA		
			if(HAL_GPIO_ReadPin(PWM_RIGHT_EN_GPIO_Port,PWM_RIGHT_EN_Pin) == GPIO_PIN_SET){
				HAL_GPIO_WritePin(PWM_RIGHT_EN_GPIO_Port,PWM_RIGHT_EN_Pin,GPIO_PIN_RESET);
			}
		}
		if((drive.left_up_on == 0) && 
		   (drive.left_down_on == 0)){
			//disable PWM ENA		
			if(HAL_GPIO_ReadPin(PWM_LEFT_EN_GPIO_Port,PWM_LEFT_EN_Pin) == GPIO_PIN_SET){
				HAL_GPIO_WritePin(PWM_LEFT_EN_GPIO_Port,PWM_LEFT_EN_Pin,GPIO_PIN_RESET);
			}
		}
	}
}

//Set duty
//duty(0% <-> 100%)
void set_duty(unsigned char duty,unsigned long channel)
{
	unsigned long ccnt;
	unsigned long value;
	ccnt = htim3.Instance->ARR;

	value = ccnt * duty / 100;

	if(channel == TIM_CHANNEL_1){
		htim3.Instance->CCR1 = value;
		//htim.Instance->CCR1 = value;
	}
	else if(channel == TIM_CHANNEL_2){
		htim3.Instance->CCR2 = value;
		//htim.Instance->CCR2 = value;
	}
	else if(channel == TIM_CHANNEL_3){
		htim3.Instance->CCR3 = value;
		//htim.Instance->CCR3 = value;
	}
	else if(channel == TIM_CHANNEL_4){
		htim3.Instance->CCR4 = value;
		//htim.Instance->CCR4 = value;
	}

}

//set freqency PWM
void set_freq(unsigned short value)
{
	htim3.Instance->ARR = value;
}
#endif // 1
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//With IR Module
/*----------------------------------------------------------------*/
#if 1
//Process data UART2 IR
void serial2_comm(void)
{
	if(r1_flg){
		r1_flg = 0;
		if((r1_buff[0] == IR_CMD_RUN) && (r1_buff[1] == 0x0D) && (r1_buff[2] == 0x0A)){
			ir_new_data_flg = 1;
			ir_new_data_type = IR_CMD_RUN;
			//SEND SYSTEM I AM RUN
			IR_Send(IR_CMD_OK);
		}
		else if((r1_buff[0] == IR_CMD_STOP) && (r1_buff[1] == 0x0D) && (r1_buff[2] == 0x0A)){
			ir_new_data_flg = 1;
			ir_new_data_type = IR_CMD_STOP;
			//driver_run_more_tm = 100;//1 giay
			//driver_run_more_tm = 50;//1 giay
			driver_run_more_tm = 0;//1 giay
			//SEND SYSTEM I AM STOP
			IR_Send(IR_CMD_OK);
		}
		//IR get OK then stop send
		else if((r1_buff[0] == IR_CMD_OK) && (r1_buff[1] == 0x0D) && (r1_buff[2] == 0x0A)){
			//ir_new_data_flg = 1;
			//ir_new_data_type = IR_CMD_OK;
			//Finsh send to
			ir_send_must_ok_flg = 0;
			ir_send_must_ok_type = 0;
			ir_send_must_ok_tm = 0;
			set_ir_send_must_ok(0,OFF,0);
		}
		//IR get TH1 - Công đức vô lượng
		else if((r1_buff[0] == IR_CMD_TH1_CDVL) && (r1_buff[1] == 0x0D) && (r1_buff[2] == 0x0A)){
			car_run_mode = CAR_RUN_CDVL;
		}
		//IR get TH1 - Công đức bằng Phước Đức
		else if((r1_buff[0] == IR_CMD_TH2_CDBPD) && (r1_buff[1] == 0x0D) && (r1_buff[2] == 0x0A)){
			car_run_mode = CAR_RUN_CDBPD;
		}
		//IR get TH1 - Công đức Ít - Phước Đức Nhiều - Ác Đức Nhiều
		else if((r1_buff[0] == IR_CMD_TH3_CDIPDNADN) && (r1_buff[1] == 0x0D) && (r1_buff[2] == 0x0A)){
			car_run_mode = CAR_RUN_CDIPDNADN;
		}

		else{
			//ir_new_data_flg = 0;
			//ir_new_data_type = 0;
		}

		r1_pos = 0;
	}
}

//send data to module IR
void IR_Send(unsigned char data)
{
	unsigned char* pos;
	unsigned char i;

	pos = &t1_buff[0];
	i = 0;
	*(pos + i++) = 0xFA;//250
	*(pos + i++) = 0xF1;//241
	*(pos + i++) = data;//xx
	*(pos + i++) = 0x0D;//13
	*(pos + i++) = 0x0A;//10
	HAL_UART_Transmit_IT(&huart1,pos,i);
}

//send ir send must ok
//value [len = 1] => forever send
//value [len != 1 && len > 0 then send len]
void set_ir_send_must_ok(unsigned char type,unsigned char on_off,unsigned char len)
{
	if(on_off){
		ir_send_must_ok_flg = 1;
		ir_send_must_ok_type = type;
		ir_send_must_ok_tm = 0;
		ir_send_must_ok_tmr = len + 1;
	}
	else{
		ir_send_must_ok_flg = 0;
		ir_send_must_ok_type = 0;
		ir_send_must_ok_tm = 0;
		ir_send_must_ok_tmr = 0;
	}
}
#endif // 1
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//With PC
/*----------------------------------------------------------------*/
//Communication with PC
#if 1

void uart2_with_due(void)
{
	unsigned char* pos;
	//unsigned char* pos2;
	unsigned char data;
	unsigned char flg;
	//unsigned short i;
	unsigned char tmp;
	struct st_IR_Format ir_tmp;
	unsigned char tmp_buff[4];
	unsigned char cmd;
	unsigned char tm;
	
	if(r2_flg){
		pos = &r2_buff[3];

		//Get addr
		if(ascii2bin(pos,&tmp,1)){
			//check addr
			if((tmp == addr_module) || (tmp == 0xFF)){
				pos += 2;
				//SEND INFO
				if(string_compera(pos,"VER#")){
					send_version();
				}
				// Send info
				else if(string_compera(pos,"INFO#")){
					//u2_send_str("BAN QUYEN CHUA THIEN TONG TAN DIEU",sizeof("BAN QUYEN CHUA THIEN TONG TAN DIEU"));
					u2_send_addr_data_str("BAN QUYEN CHUA THIEN TONG TAN DIEU",sizeof("BAN QUYEN CHUA THIEN TONG TAN DIEU"));
				}
				// Write fact to flash on MCU
				else if(string_compera(pos,"FACTW#")){
					if(get_fact()){
						//uart2_send_nak();
						u2_send_addr_data_str("OK",strlen("OK"));
					}
					else{
						//uart2_send_ack();
						u2_send_addr_data_str("NG",strlen("NG"));
					}
				}
				// Read fact struct
				else if(string_compera(pos,"FACTR#")){
					if(read_fact()){
						//uart2_send_buff(&fact.header[0],sizeof(fact));
						send_fact(addr_module);
					}
					else{
						//uart2_send_ack();
						u2_send_addr_data_str("NG",strlen("NG"));
					}
				}
				// Send to PC value RGB now
				else if(string_compera(pos,"GETRGB#")){
					pos += strlen("GETRGB");
					if(ascii2bin(pos,&data,1)){
						if(data < 5){
							
						}
						else{
							u2_send_str("NG",2);
						}
					}
					else{
						u2_send_str("NG",2);
					}
				}
				//Adjist on
				else if(string_compera(pos,"ADJON#")){
					adjust_on_flg = 1;
					uart2_send_nak();
				}
				//Adjist off
				else if(string_compera(pos,"ADJOFF#")){
					adjust_on_flg = 0;
					uart2_send_nak();
				}
				// Set Adjust 1
				else if(string_compera(pos,"ADJSET1#")){
					adjust_set_mode = 1;
					uart2_send_nak();
				}
				// Set Adjust 2
				else if(string_compera(pos,"ADJSET2#")){
					adjust_set_mode = 2;
					uart2_send_nak();
				}
				//Bootloader
				else if(string_compera(pos,"BOOTLOADERXE#")){
					//uart2_send_nak();
					flg = read_fact();
					if(flg == 0){
						init_fact();
						//write_fact();
					}
					// 0xAA not next app address
					// 0x55 next app address
					fact.bootloader = JIMP_BOOTLOADER;
					write_fact();
					REFREST_WWDR
					//flash_jump_to_bootloader();
					bootloader_tm = 100;
				}
				//CHECK MODULE
				else if(string_compera(pos,"CHK#")){
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//ADDR MODULE WRITE
				else if(string_compera(pos,"ADDRW#")){//FORMAT WHILE ADDR [TT~ADDRWaa] "aa" là địa chỉ
					pos += strlen("ADDRW");
					if(ascii2bin(pos,&tmp,1)){
						flg = read_fact();
						if(flg == 0){
							init_fact();
							//write_fact();
						}
						fact.address = tmp;
						write_fact();
						addr_module = tmp;
						u2_send_addr_data_str("OK",sizeof("OK"));
					}
					else{
						u2_send_addr_data_str("NG",sizeof("NG"));
					}
				}
				//ADDR MODULE READ
				else if(string_compera(pos,"ADDRR#")){
					//uart2_send_nak();
					flg = read_fact();
					if(flg == 0){
						init_fact();
						//write_fact();
					}
					tmp = fact.address;
					write_fact();
					addr_module = fact.address;
					uart2_send_buff(&tmp,1);
				}
				//IR Send to car data from Due
				else if(string_compera(pos,"IRSEND#")){
					pos += strlen("IRSEND");
					if(ascii2bin(pos,&ir_tmp.data[0],3)){
						//addr_module;
						IR_Format.addr = 0xFA;//địa chỉ chung của module
						IR_Format.cmd = 0xF1;//cmd
						IR_Format.data[0] = ir_tmp.data[0];
						IR_Format.data[1] = ir_tmp.data[1];
						IR_Format.data[2] = ir_tmp.data[2];
						IR_Send(IR_Format.data[0]);
						u2_send_addr_data_str("OK",sizeof("OK"));
					}
					else{
						u2_send_addr_data_str("NG",sizeof("NG"));
					}
				}
				//SEND SCAN IR ON
				else if(string_compera(pos,"SCANON#")){
					//pos += strlen("SCANON");
					ir_scan_flg = 1;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//SEND SCAN IR OFF
				else if(string_compera(pos,"SCANOFF#")){
					//pos += strlen("SCANOFF");
					ir_scan_flg = 0;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//SEND SCAN IR
				else if(string_compera(pos,"SCAN#")){
					//pos += strlen("SCAN");
					ir_scan_flg = 1;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//CHECK IR SEND
				else if(string_compera(pos,"IRCHK#")){
					ir_chk_flg = 1;
					ir_run_tm = 0;
					//CHECK FINISH THEN SEND OK
					//u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//SEND CAR RUN 
				else if(string_compera(pos,"RUN#")){
					ir_run_flg = 1;
					//ir_run_OnOff = 1;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				// Car run
				else if(string_compera(pos,"CARRUN#")){
					car_run_ena = 1;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				// car stop
				else if(string_compera(pos,"CARSTOP#")){
					car_run_ena = 0;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//DUE SEND OF
				else if(string_compera(pos,"OK#")){
				}
				// Get Main Key
				else if(string_compera(pos,"MAINKEY#")){
				    pos += 7;
					cmd = *pos;
					pos++;
					if(ascii2bin(pos,&tm,1)){
						if(cmd == IR_CMD_CHECK){
							u2_send_addr_data_str("OK",strlen("OK"));
						}
						else{
							u2_send_addr_data_str("NG",strlen("NG"));
						}
					}
					else{
						u2_send_addr_data_str("NG",strlen("NG"));
					}
                }
				//Other
				else{
					u2_send_str("NG",2);
				}
			}
			else{
			// Nothing
			}
		}
		else{
			pos = &r2_buff[3];
			//Bootloader
			if(string_compera(pos,"BOOTLOADERXE#")){
				//uart2_send_nak();
				flg = read_fact();
				if(flg == 0){
					init_fact();
					//write_fact();
				}
				REFREST_WWDR
				//while(huart2.gState == HAL_UART_STATE_BUSY);
				REFREST_WWDR
				// 0xAA not next app address
				// 0x55 next app address
				fact.bootloader = 0xAA;
				write_fact();
				REFREST_WWDR
				//HAL_DeInit();
				//flash_jump_to_bootloader();
				bootloader_tm = 100;
			}
			// Update Bootloader
			else if(string_compera(pos,"BOOTLOADERTHIS#")){
				updatebootloader_flg = 1;
				bootloader_error_flg = 0;
				//bootloader_start_addr = 0;
				u2_send_addr_data_str("OK",sizeof("OK"));
			}
			//Size program
			else if(string_compera(pos,"PROGRAMINFO#")){
				if(updatebootloader_flg){
					pos += strlen("PROGRAMINFO");
					//Get Basic address
					if(ascii2bin(pos,&tmp_buff[0],4)){

						bootloader_basic_addr = tmp_buff[0];

						bootloader_basic_addr <<= 8;
						bootloader_basic_addr |= tmp_buff[1];

						bootloader_basic_addr <<= 8;
						bootloader_basic_addr |= tmp_buff[2];

						bootloader_basic_addr <<= 8;
						bootloader_basic_addr |= tmp_buff[3];

						//Get Len
						pos += 8;
						if(ascii2bin(pos,&tmp_buff[0],4)){

							bootloader_len = tmp_buff[0];

							bootloader_len <<= 8;
							bootloader_len |= tmp_buff[1];

							bootloader_len <<= 8;
							bootloader_len |= tmp_buff[2];

							bootloader_len <<= 8;
							bootloader_len |= tmp_buff[3];

							//bootloader_addr = bootloader_basic_addr;
							bootloader_addr = START_ADDR;

							// Send reply
							//uart2_send_nak();
						}
						else{
							//uart2_send_ack();
							updatebootloader_flg = 0;
						}
					}
					else{
						//uart2_send_ack();
						updatebootloader_flg = 0;
					}
				}
				else{
					//uart2_send_ack();
					updatebootloader_flg = 0;
				}
			}
			else if(string_compera(pos,"ERASE#")){
				if(updatebootloader_flg){
					REFREST_WWDR
					flash_erase_bootloader(START_ADDR,16);
					REFREST_WWDR
					//u2_send_addr_data_str("OK",sizeof("OK"));
				}
				else{
					//u2_send_addr_data_str("NG",sizeof("NG"));
				}
			}
			// Write data to flash
			else if(string_compera(pos,"PG#")){
				if(updatebootloader_flg){
					// Write data to flash
					pos += strlen("PG");
					if(get_data_flash(pos)){

						// Send reply
						//uart2_send_nak();
					}
					else{
						//uart2_send_ack();
					}
				}
			}
			//Jum to Application
			else if(string_compera(pos,"END#")){
				if(updatebootloader_flg){
					updatebootloader_flg = 0;
					//Send reply
					//uart2_send_nak();

					if(START_ADDR + bootloader_len != bootloader_addr){
						bootloader_error_flg = 1;
					}
				}
			}
		}
		r2_flg = 0;
		r2_pos = 0;
	}
}

// Get data flash
unsigned char get_data_flash(unsigned char *pos)
{
	unsigned long data;
	unsigned short i;
	unsigned char flg;
	uint32_t *pos2;

	flg = 0;

	// Check checksum
	flg = check_checksume16(pos,1026);
	if(flg == 0){
		goto END_ERROR;
	}
	// Check CRC
	flg = check_crc16(pos,1028);
	if(flg == 0){
		goto END_ERROR;
	}

	pos2 = &bootloader_buff[0];
	for(i = 0;i < 256;i++){
		data = *(pos + i * 4 + 3);
		data <<= 8;
		data |= *(pos + i * 4 + 2);
		data <<= 8;
		data |= *(pos + i * 4 + 1);
		data <<= 8;
		data |= *(pos + i * 4 + 0);

		*(pos2 + i) = (uint32_t) data;
	}

	if(flash_write(bootloader_addr,pos2,256) != FLASH_OK){
		flg = 0;
		goto END_ERROR;
	}

	bootloader_addr += 1024;

	flg = 1;
END_ERROR:
	return flg;
}

//Vesion
void send_version()
{
	
	unsigned char *pos;
	unsigned char buff[64];
	unsigned char tmp[64];
	unsigned char i;
	unsigned char j;
	
	// Copy table to buff
	for(i=0;i<24;i++){
		tmp[i] = model_tab[i];
	}
	
	//Get ponter
	pos = &buff[0];
	i = 0;
	*(pos + i++) = 'T';
	*(pos + i++) = 'T';
	*(pos + i++) = '~';
	*(pos + i++) = 'V';
	*(pos + i++) = 'E';
	*(pos + i++) = 'R';
	for(j=0;j<22;j++){
		*(pos + i++) = tmp[j];
	}
	
	bin2ascii(&tmp[22],(pos + i),2);
	
	i += 4;
	
	//uart2_send_buff(pos,i);
	uart2_send_string(pos,i);
}

//get fact data
unsigned char get_fact()
{
	struct st_fact tmp;
	unsigned char *s_pos;
	unsigned char *d_pos;
	//unsigned short i;
	//unsigned short j;
	unsigned char flg;

	//get data to tmp
	//TT~FFFACTW...
	s_pos = &r2_buff[10];
	d_pos = &tmp.header[0];
	//if ok then retun 1/ not ok then return 0
	flg = ascii2bin(s_pos,d_pos,sizeof(tmp));

	//check data
	//flg = 1;
	if(tmp.header[0] != 0x55){
		flg = 0;
	}
	else if(tmp.header[1] != 0xAA){
		flg = 0;
	}
	else if(tmp.footer[0] != 0x55){
		flg = 0;
	}
	else if(tmp.footer[1] != 0xAA){
		flg = 0;
	}
	else{
		//copy data from tmp to fact
		fact = tmp;
		write_fact();
	}
	return flg;
}

// Send fact
void send_fact(unsigned char addr)
{
	unsigned short i;
	//unsigned char tmp;
	unsigned char* d_pos;
	
	//uart2_send_buff(&fact.header[0],sizeof(fact));
	d_pos = &t2_buff[0];
	i = 0;
	*(d_pos + i++) = 'T';
	*(d_pos + i++) = 'T';
	*(d_pos + i++) = '~';
	bin2ascii_v2(&addr,(d_pos + i),1,&i);
	*(d_pos + i++) = 'F';
	*(d_pos + i++) = 'A';
	*(d_pos + i++) = 'C';
	*(d_pos + i++) = 'T';
	bin2ascii_v2(&fact.header[0],(d_pos + i),sizeof(fact),&i);
	*(d_pos + i++) = 0x0D;
	*(d_pos + i++) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,d_pos,i);
}

//send buff hex convert to ascii
void uart2_send_buff(unsigned char* s_pos,unsigned short len)
{
	unsigned char* d_pos;
	unsigned short i;
	i = len * 2;
	d_pos = &t2_buff[0];
	bin2ascii(s_pos,d_pos,len);
	*(d_pos + i++) = 0x0D;
	*(d_pos + i++) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,d_pos,i);
}

//send tring and hex
void uart2_send_plus(unsigned char* str_pos,unsigned short str_len,unsigned char* hex_pos,unsigned short hex_len)
{
	unsigned char* d_pos;
	unsigned short i;
	unsigned short j;
	j = 0;
	d_pos = &t2_buff[0];
	//string copy
	for(i = 0;i < str_len;i++){
		*(d_pos + j++) = *(str_pos + i);
	}
	//Hex copy
	bin2ascii(hex_pos,(d_pos + j),hex_len);
	
	*(d_pos + str_len + hex_len * 2 + 0) = 0x0D;
	*(d_pos + str_len + hex_len * 2 + 1) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,d_pos,str_len + hex_len * 2 + 2);
}

//uart2 send string
void uart2_send_string(unsigned char *s_pos,unsigned short len)
{
	unsigned char* d_pos;
	unsigned short i;
	d_pos = &t2_buff[0];
	for(i = 0;i < len;i++){
		*(d_pos + i) = *(s_pos + i);
	}
	*(d_pos + i++) = 0x0D;
	*(d_pos + i++) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,d_pos,i);
}

//send nak/ok
void uart2_send_nak()
{
	unsigned char* pos;
	unsigned char i;
	pos = &t2_buff[0];
	i = 0;
	*(pos + i++) = 'O';
	*(pos + i++) = 'K';
	*(pos + i++) = 0x0D;
	*(pos + i++) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,pos,i);
}

//send ack/ng
void uart2_send_ack(void)
{
	unsigned char* pos;
	unsigned char i;
	pos = &t2_buff[0];
	i = 0;
	*(pos + i++) = 'N';
	*(pos + i++) = 'G';
	*(pos + i++) = 0x0D;
	*(pos + i++) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,pos,i);
}

//uart2 send and address
void u2_send_addr_data_str(char* s_pos,unsigned short len)
{
	unsigned char* d_pos;
	unsigned short i;
	unsigned short j;
	unsigned char tmp[2];

	d_pos = &t2_buff[0];
	i = 0;
	j = 0;
	*(d_pos + i++) = 'T';
	*(d_pos + i++) = 'T';
	*(d_pos + i++) = '~';
	bin2ascii(&addr_module,&tmp[0],1);
	*(d_pos + i++) = tmp[0];
	*(d_pos + i++) = tmp[1];

	for(j = 0;j < len;j++){
		*(d_pos + i++) = *(s_pos + j);
	}
	*(d_pos + i++) = 0x0D;
	*(d_pos + i++) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,d_pos,i);
}

//u2 send string
//uart2 send and address
void u2_send_str(char *s_pos,unsigned short len)
{
	unsigned char *d_pos;
	unsigned short i,j;

	d_pos = &t2_buff[0];
	i = 0;
	*(d_pos + i++) = 'T';
	*(d_pos + i++) = 'T';
	*(d_pos + i++) = '~';

	for(j = 0;j < len;j++){
		*(d_pos + i++) = *(s_pos + j);
	}
	*(d_pos + i++) = 0x0D;
	*(d_pos + i++) = 0x0A;
	HAL_UART_Transmit_IT(&huart2,d_pos,i);
}
#endif
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//Bluetooth
/*----------------------------------------------------------------*/
#if 1

//with module Bluetooth
void serial3_communication()
{
	unsigned short i;
	unsigned char *pos;
	//unsigned char dmy;

	if(r3_flg){
		r3_flg = 0;
		//Clear buff
		pos = &bluetooth_buff[0];
		for(i = 0;i < sizeof(bluetooth_buff);i++){
			*(pos + i) = 0;
		}

		//s1_send_buff(&bluetooth_buff[0],sizeof(bluetooth_buff));
		utf8_to_ascii(&r3_buff[0],r3_pos,&bluetooth_buff[0],&bluetooth_len);//Data type utf8 sang kiểu ascii không dấu
		Upper(&bluetooth_buff[0],bluetooth_len);//Chử in hoa hóa
		//s1_send_buff(&bluetooth_buff[0], bluetooth_len);
		bluetooth_flg = 1;

		//clear buff r3_buff
        //Serial.println("â");
		for(i = 0;i < sizeof(r3_buff);i++){
			if(r3_buff[i]){
				r3_buff[i] = 0;
			}
			else{
				//break;
			}
		}
		r3_pos = 0;

		//HAL_UART_DeInit(&huart3);
		HAL_UART_Receive_IT(&huart3,&r3_buff2[0],128);
	}
	//--------------------------------------------------
}
#endif // 1
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//UART interrupt
/*----------------------------------------------------------------*/
#if 1
//Interrupt timer 10ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim->Instance == TIM1){
		htim1_flg = 1;
		timer_couter();
		
	}
}

//Interrupt UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	unsigned short i;
	unsigned char *pos;
	//Module IR
	if(huart->Instance == USART1)
	{
		pos = &r1_buff2[0];
		for(i = 0;i < sizeof(r1_buff2);i++){
			*(pos + i) = 0;
		}
		HAL_UART_Receive_IT(&huart1,&r1_buff2[0],sizeof(r1_buff2));	//Kich hoat UART nhan du lieu ngat moi luc
	}
	//With PC
	if(huart->Instance == USART2)	//UART hien tai la uart3
	{
		pos = &r2_buff2[0];
		for(i = 0;i < sizeof(r2_buff2);i++){
			*(pos + i) = 0;
		}
		HAL_UART_Receive_IT(&huart2,&r2_buff2[0],sizeof(r2_buff2));
	}
	//With bluetooth
	if(huart->Instance == USART3)	//UART hien tai la uart3
	{
		pos = &r3_buff2[0];
		for(i = 0;i < sizeof(r3_buff2);i++){
			*(pos + i) = 0;
		}
		HAL_UART_Receive_IT(&huart3,&r3_buff2[0],sizeof(r3_buff2));
	}

}

//irq uart1 rx it
unsigned char uart1_rx_it(unsigned char data)
{
	if(r1_flg == 0){
		r1_buff[r1_pos++] = data;
		//r1_flg = 1;
		r1_tm = 5;
	}

	return 1;
}

//irq uart2 rx it
unsigned char uart2_rx_it(unsigned char data)
{
	unsigned char flg;
	
	//Status free
	flg = 0;
	if(r2_flg == 0){
		if(r2_pos == 0){
			if(data == 'T'){
				r2_buff[r2_pos++] = data;
			}
		}
		else if(r2_pos == 1){
			if(data == 'T'){
				r2_buff[r2_pos++] = data;
			}
			else{
				r2_pos = 0;
			}
		}
		else if(r2_pos == 2){
			if(data == '~'){
				r2_buff[r2_pos++] = data;
			}
			else{
				r2_pos = 0;
			}
		}
		else{
			r2_buff[r2_pos++] = data;
			#if 1
			if((r2_buff[3] == 'P') && (r2_buff[4] == 'G')){
				//[TT~PG] + [1024] + [2 BYTE CHECKSUM] + [2 BYTE CRC]
				if(r2_pos == 1033){
					r2_flg = 1;
					flg = 1;
				}
				else if(r2_pos > 1033){
					flg = 1;
					r2_pos = 0;
				}
			}
			else{
				if(r2_pos >= sizeof(r2_buff)){
					r2_pos = 0;
					flg = 1;
					//HAL_UART_Receive_IT(&huart2,&uart2_rx[0],sizeof(uart2_rx));
				}
				else if((r2_buff[r2_pos - 2] == 0x0D) && (r2_buff[r2_pos - 1] == 0x0A)){
					r2_flg = 1;
					flg = 1;
					//HAL_UART_Receive_IT(&huart2,&uart2_rx[0],sizeof(uart2_rx));
				}
			}
			#endif // 0


		}
	}
	return flg;
}

//uart for bluetooth
unsigned char uart3_rx_it(unsigned char data)
{
	r3_buff[r3_pos++] = data;
	r3_tot = 50;
	return 0;
}

#if 0
//For STM32F103
/**
  * @brief  Receives an amount of data in non blocking mode
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef* huart)
{
	uint16_t* tmp;
	unsigned char data;

	/* Check that a Rx process is ongoing */
	if(huart->RxState == HAL_UART_STATE_BUSY_RX){
		if(huart->Init.WordLength == UART_WORDLENGTH_9B){
			tmp = (uint16_t*) huart->pRxBuffPtr;
			if(huart->Init.Parity == UART_PARITY_NONE){
				*tmp = (uint16_t) (huart->Instance->DR & (uint16_t) 0x01FF);
				huart->pRxBuffPtr += 2U;
			}
			else{
				*tmp = (uint16_t) (huart->Instance->DR & (uint16_t) 0x00FF);
				huart->pRxBuffPtr += 1U;
			}
		}
		else{
			if(huart->Init.Parity == UART_PARITY_NONE){
				//*huart->pRxBuffPtr++ = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
				data = (uint8_t) (huart->Instance->DR & (uint8_t) 0x00FF);
				*huart->pRxBuffPtr++ = data;
			}
			else{
				*huart->pRxBuffPtr++ = (uint8_t) (huart->Instance->DR & (uint8_t) 0x007F);
			}
		}

		if(--huart->RxXferCount == 0U){
			/* Disable the IRDA Data Register not empty Interrupt */
			__HAL_UART_DISABLE_IT(huart,UART_IT_RXNE);

			/* Disable the UART Parity Error Interrupt */
			__HAL_UART_DISABLE_IT(huart,UART_IT_PE);
			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			__HAL_UART_DISABLE_IT(huart,UART_IT_ERR);

			/* Rx process is completed, restore huart->RxState to Ready */
			huart->RxState = HAL_UART_STATE_READY;

			HAL_UART_RxCpltCallback(huart);

			return HAL_OK;
		}

		if(huart->Instance == USART1){
			if(uart1_rx_it(data)){
				/* Disable the IRDA Data Register not empty Interrupt */
				__HAL_UART_DISABLE_IT(huart,UART_IT_RXNE);

				/* Disable the UART Parity Error Interrupt */
				__HAL_UART_DISABLE_IT(huart,UART_IT_PE);
				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
				__HAL_UART_DISABLE_IT(huart,UART_IT_ERR);

				/* Rx process is completed, restore huart->RxState to Ready */
				huart->RxState = HAL_UART_STATE_READY;

				HAL_UART_RxCpltCallback(huart);

				return HAL_OK;
			}
		}

		if(huart->Instance == USART2){
			if(uart2_rx_it(data)){
				/* Disable the IRDA Data Register not empty Interrupt */
				__HAL_UART_DISABLE_IT(huart,UART_IT_RXNE);

				/* Disable the UART Parity Error Interrupt */
				__HAL_UART_DISABLE_IT(huart,UART_IT_PE);
				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
				__HAL_UART_DISABLE_IT(huart,UART_IT_ERR);

				/* Rx process is completed, restore huart->RxState to Ready */
				huart->RxState = HAL_UART_STATE_READY;

				HAL_UART_RxCpltCallback(huart);

				return HAL_OK;
			}
		}

		if(huart->Instance == USART3){
			if(uart3_rx_it(data)){
				/* Disable the IRDA Data Register not empty Interrupt */
				__HAL_UART_DISABLE_IT(huart,UART_IT_RXNE);

				/* Disable the UART Parity Error Interrupt */
				__HAL_UART_DISABLE_IT(huart,UART_IT_PE);
				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
				__HAL_UART_DISABLE_IT(huart,UART_IT_ERR);

				/* Rx process is completed, restore huart->RxState to Ready */
				huart->RxState = HAL_UART_STATE_READY;

				HAL_UART_RxCpltCallback(huart);

				return HAL_OK;
			}
		}
		return HAL_OK;
	}
	else{
		return HAL_BUSY;
	}
}
#endif // 0

#if 0
//For STM32F7
/**
  * @brief RX interrrupt handler for 7 or 8 bits data word length .
  * @param huart UART handle.
  * @retval None
  */
static void UART_RxISR_8BIT(UART_HandleTypeDef *huart)
{
	uint16_t uhMask = huart->Mask;
	uint16_t  uhdata;
	unsigned char data;

	/* Check that a Rx process is ongoing */
	if(huart->RxState == HAL_UART_STATE_BUSY_RX){
		uhdata = (uint16_t) READ_REG(huart->Instance->RDR);
		data = (uint8_t) (uhdata & (uint8_t) uhMask);


		#if 1
		* huart->pRxBuffPtr = (uint8_t) (uhdata & (uint8_t) uhMask);
		huart->pRxBuffPtr++;
		huart->RxXferCount--;

		if(huart->RxXferCount == 0U){
			/* Disable the UART Parity Error Interrupt and RXNE interrupts */
			CLEAR_BIT(huart->Instance->CR1,(USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));

			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			CLEAR_BIT(huart->Instance->CR3,USART_CR3_EIE);

			/* Rx process is completed, restore huart->RxState to Ready */
			huart->RxState = HAL_UART_STATE_READY;

			/* Clear RxISR function pointer */
			huart->RxISR = NULL;

			#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
			/*Call registered Rx complete callback*/
			huart->RxCpltCallback(huart);
			#else
			/*Call legacy weak Rx complete callback*/
			HAL_UART_RxCpltCallback(huart);
			#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
		}

		//----------------------------------
		if(huart->Instance == USART1){
			if(uart1_rx_it(data)){
				/* Disable the UART Parity Error Interrupt and RXNE interrupts */
				CLEAR_BIT(huart->Instance->CR1,(USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));

				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
				CLEAR_BIT(huart->Instance->CR3,USART_CR3_EIE);

				/* Rx process is completed, restore huart->RxState to Ready */
				huart->RxState = HAL_UART_STATE_READY;

				/* Clear RxISR function pointer */
				huart->RxISR = NULL;

				/*Call legacy weak Rx complete callback*/
				HAL_UART_RxCpltCallback(huart);

				//return HAL_OK;
			}
		}

		if(huart->Instance == USART2){
			if(uart2_rx_it(data)){
				/* Disable the UART Parity Error Interrupt and RXNE interrupts */
				CLEAR_BIT(huart->Instance->CR1,(USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));

				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
				CLEAR_BIT(huart->Instance->CR3,USART_CR3_EIE);

				/* Rx process is completed, restore huart->RxState to Ready */
				huart->RxState = HAL_UART_STATE_READY;

				/* Clear RxISR function pointer */
				huart->RxISR = NULL;

				/*Call legacy weak Rx complete callback*/
				HAL_UART_RxCpltCallback(huart);

				//return HAL_OK;
			}
		}

		if(huart->Instance == USART3){
			if(uart3_rx_it(data)){
				/* Disable the UART Parity Error Interrupt and RXNE interrupts */
				CLEAR_BIT(huart->Instance->CR1,(USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));

				/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
				CLEAR_BIT(huart->Instance->CR3,USART_CR3_EIE);

				/* Rx process is completed, restore huart->RxState to Ready */
				huart->RxState = HAL_UART_STATE_READY;

				/* Clear RxISR function pointer */
				huart->RxISR = NULL;

				/*Call legacy weak Rx complete callback*/
				HAL_UART_RxCpltCallback(huart);

				//return HAL_OK;
			}
		}

	}
	else{
		/* Clear RXNE interrupt flag */
		__HAL_UART_SEND_REQ(huart,UART_RXDATA_FLUSH_REQUEST);
	}
	#endif // 0


}

#endif

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
{
	//I implement this .. but eventually you can decide with different approach
	//Many macros/proc on this code .. is just to be a hint (mem/rec about this possibility)
	__HAL_UART_CLEAR_OREFLAG(huart);
	__HAL_UART_CLEAR_NEFLAG(huart);
	__HAL_UART_CLEAR_FEFLAG(huart);
	/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_DISABLE_IT(huart,UART_IT_ERR);
	//The most important thing when UART framing error occur/any error is restart the RX process 
	if(huart->Instance == USART1){
		//Restarting the RX, .. 1 byte. .. u8DATUartShortRxBuffer is My own rx buffer 
		HAL_UART_Receive_IT(&huart1,&r1_buff2[0],sizeof(r1_buff2));
	}
	if(huart->Instance == USART2){
		//Restarting the RX, .. 1 byte. 
		HAL_UART_Receive_IT(&huart2,&r2_buff2[0],sizeof(r2_buff2));
	}
	if(huart->Instance == USART3){
		//Restarting the RX, .. 1 byte. 
		HAL_UART_Receive_IT(&huart3,&r3_buff2[0],sizeof(r3_buff2));

	}
}
#endif // 1
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
//Other
/*----------------------------------------------------------------*/
#if 1
/// so sánh string
unsigned char string_compera(unsigned char* s_pos,char* d_pos)
{
	unsigned char i;
	unsigned char flg;
	i = 0;
	flg = 1;
	while(*(d_pos + i) != '#'){
		if(*(s_pos + i) != *(d_pos + i)){
			flg = 0;
			break;
		}
		i++;
	}
	return flg;
}

//bin2ascii
void bin2ascii(unsigned char* s_pos,unsigned char* d_pos,unsigned short len_s_pos)
{
	unsigned char data;
	unsigned char dmy;
	unsigned short i;
	unsigned short j;

	for(i = 0,j = 0;i < len_s_pos;i++,j += 2){
		data = *(s_pos + i);
		dmy = (data & 0xF0) >> 4;
		if(dmy < 10){
			*(d_pos + j) = dmy + '0';
		}
		else{
			*(d_pos + j) = dmy + 'A' - 10;
		}

		dmy = (data & 0x0F);
		if(dmy < 10){
			*(d_pos + j + 1) = dmy + '0';
		}
		else{
			*(d_pos + j + 1) = dmy + 'A' - 10;
		}
	}
}

// bin 2 ascii vesion 2
void bin2ascii_v2(unsigned char* s_pos,unsigned char* d_pos,unsigned short len_s_pos,unsigned short *necxt_pos)
{
	unsigned char data;
	unsigned char dmy;
	unsigned short i;
	unsigned short j;

	for(i=0,j=0;i<len_s_pos;i++,j+=2){
		data = *(s_pos + i);
		dmy = (data & 0xF0) >> 4;
		if(dmy < 10){
			*(d_pos + j) = dmy + '0';
		}
		else{
			*(d_pos + j) = dmy + 'A' - 10;
		}

		dmy = (data & 0x0F);
		if(dmy < 10){
			*(d_pos + j + 1) = dmy + '0';
		}
		else{
			*(d_pos + j + 1) = dmy + 'A' - 10;
		}
	}
	*necxt_pos += len_s_pos * 2;
}

//2 byte ascii to 1byte bin
unsigned char ascii2bin(unsigned char* s_pos,unsigned char* d_pos,unsigned short len_d_pos)
{
	//len num byte want get
	unsigned char flg;
	unsigned char data;
	unsigned char dmy;
	unsigned short i;
	unsigned short j;
	flg = 1;
	for(i = 0,j = 0;i < len_d_pos;i++,j += 2){
		dmy = 0;
		data = *(s_pos + j);
		if(data > '/' && data < ':'){//0 <-> 9
			dmy |= ((data - '0') << 4);
		}
		else if(data > '@' && data < 'G'){//A <-> F
			dmy |= ((data - 'A' + 10) << 4);
		}
		else{
			flg = 0;
			break;
		}

		data = *(s_pos + j + 1);
		if(data > '/' && data < ':'){//0 <-> 9
			dmy |= data - '0';
		}
		else if(data > '@' && data < 'G'){//A <-> F
			dmy |= data - 'A' + 10;
		}
		else{
			flg = 0;
			break;
		}
		*(d_pos + i) = dmy;
	}

	return flg;
}

//utf8_to_ascii()
//Input: Data type UFT-8
//Output: ascci không dấu 
/*-------------------------------------------*/
unsigned char utf8_to_ascii(unsigned char *s_pos,unsigned short s_pos_len,unsigned char *d_pos,unsigned short *d_pos_len)
{
	unsigned short i;
	unsigned short j;
	unsigned long tmp;
	unsigned short data;
	unsigned char flg;
	unsigned char chr;
	unsigned short d_pos_ct;
	flg = 1;
	d_pos_ct = 0;
	for(i = 0;i < s_pos_len;i++){//Format 0b1110xxxx-10xxxxxx-10xxxxxx
		if((*(s_pos + i + 0) & 0xF0) == 0xE0){//type utf-8 type 3byte
			if(*(s_pos + i + 1) & 0x80){//type utf-8
				if(*(s_pos + i + 2) & 0x80){//type utf-8
					tmp = *(s_pos + i + 0);
					tmp <<= 8;
					tmp |= *(s_pos + i + 1);
					tmp <<= 8;
					tmp |= *(s_pos + i + 2);
					//s1_send_long(&tmp);
					data = (unsigned short) ((tmp & 0x0F0000) >> 4);
					data |= (unsigned short) ((tmp & 0x3C00) >> 2);
					data |= (unsigned short) ((tmp & 0x0300) >> 2);
					data |= (unsigned short) (tmp & 0x3F);
					if((data >= 0x1EA0) && (data <= 0X1EF9)){
						//s1_send_work(&data);
						i += 2;
						j = 0;
						while(utf8_vi_3byte_tab[j] != 0xFFFF){
							if(utf8_vi_3byte_tab[j] == data){
								chr = (unsigned char)utf8_vi_3byte_tab[j + 1];
								//s1_send_buff(&chr,1);
								*(d_pos + d_pos_ct++) = chr;
								break;
							}
							j += 2;
						}
					}
					else{
						flg = 0;
					}
				}
				else{//Error
					flg = 0;
				}
			}
			else{//Error
				flg = 0;
			}
		}
		else if((*(s_pos + i + 0) & 0xE0) == 0xC0){//type utf-8 type 2byte
			if(*(s_pos + i + 1) & 0x80){//type utf-8
			  //Format 0b110xxxxx-10xxxxxx
				tmp = *(s_pos + i + 0);
				tmp <<= 8;
				tmp |= *(s_pos + i + 1);
				data = (unsigned short) ((tmp & 0x1F00) >> 2);
				data |= (unsigned short) ((tmp & 0x3F));
				if((data >= 0x00C0) && (data <= 0x01B0)){
					//s1_send_work(&data);
					i += 1;
					j = 0;
					while(utf8_vi_2byte_tab[j] != 0xFFFF){
						if(utf8_vi_2byte_tab[j] == data){
							chr = (unsigned char)utf8_vi_2byte_tab[j + 1];
							//s1_send_buff(&chr,1);
							*(d_pos + d_pos_ct++) = chr;
							break;
						}
						j += 2;
					}
				}
			}
			else{
				flg = 0;
			}
		}
		else{
			*(d_pos + d_pos_ct++) = *(s_pos + i);
		}
	}
	*d_pos_len = d_pos_ct;
	//s1_send_buff(d_pos, d_pos_ct);
	return flg;
}

//Support type ascii
void Upper(unsigned char *string,unsigned short s_pos_len)
{
	const char OFFSET = 'a' - 'A';
	unsigned short i;

	for(i = 0;i < s_pos_len;i++){
		*(string + i) = (*(string + i) >= 'a' && *(string + i) <= 'z') ? *(string + i) -= OFFSET : *(string + i);
	}
}

//Add CRC16
void add_crc16(unsigned char *pos,unsigned short len)
{
	unsigned long data;
	unsigned long crc;
	//unsigned short i;
	unsigned char bit_pos;
	unsigned short byte_pos;

	//0b1 0001 0000 - 0010 0001
	crc = 0x11021;//x16 + x12 + x5 + 1
	data = 0;
	*(pos + len - 1) = 0;
	*(pos + len - 2) = 0;
	bit_pos = 0x80;
	byte_pos = 0;
	while(byte_pos < len){
		data <<= 1;
		if(*(pos + byte_pos) & bit_pos){
			data |= 0x01;
		}
		
		if(data & 0x00010000){
			data ^= crc;
		}
		bit_pos >>= 1;
		if(bit_pos == 0){
			bit_pos = 0x80;
			byte_pos++;
		}
	}

	*(pos + len - 2) = (data & 0xFF00) >> 8;
	*(pos + len - 1) = (data & 0xFF);
}

//Check CRC16
unsigned char check_crc16(unsigned char *pos,unsigned short len)
{
	unsigned long data;
	unsigned long crc;
	//unsigned short i;
	unsigned char bit_pos;
	unsigned short byte_pos;

	//0b1 0001 0000 - 0010 0001
	crc = 0x00011021;//x16 + x12 + x5 + 1
	data = 0;
	//*(pos + len - 1) = 0;
	//*(pos + len - 2) = 0;
	bit_pos = 0x80;
	byte_pos = 0;
	while(byte_pos < len){
		data <<= 1;
		if(*(pos + byte_pos) & bit_pos){
			data |= 0x01;
		}
		
		if(data & 0x00010000){
			data ^= crc;
		}
		bit_pos >>= 1;
		if(bit_pos == 0){
			bit_pos = 0x80;
			byte_pos++;
		}
	}

	//*(pos + len - 1) = (data & 0xFF00) >> 8;
	//*(pos + len - 2) = (data & 0xFF);
	if(data == 0){
		return 1;
	}
	else{
		return 0;
	}
}

//Add CheckSum16
void add_checksume16(unsigned char *pos,unsigned short len)
{
	unsigned short i;
	unsigned short checksum;

	checksum = 0;
	*(pos + len - 1) = 0;
	*(pos + len - 2) = 0;
	for(i = 0;i < len - 2;i++){
		checksum += *(pos + i);
	}

	*(pos + len - 2) = (checksum & 0xFF00) >> 8;
	*(pos + len - 1) = (checksum & 0xFF) >> 0;
}

// test crc
unsigned char test_crc16()
{
	unsigned short i;
	unsigned char flg;

	for(i = 0;i < sizeof(t1_buff);i++){
		t1_buff[i] = (unsigned char)i;
	}
	add_crc16(&t1_buff[0],sizeof(t1_buff) - 2);
	flg = check_crc16(&t1_buff[0],sizeof(t1_buff) - 2);
	add_checksume16(&t1_buff[0],sizeof(t1_buff));
	flg = check_checksume16(&t1_buff[0],sizeof(t1_buff));
	return flg;
}

//CheckSum16
unsigned char check_checksume16(unsigned char *pos,unsigned short len)
{
	unsigned short i;
	unsigned short checksum;
	unsigned short data;

	checksum = 0;
	//*(pos + len - 1) = 0;
	//*(pos + len - 2) = 0;
	data = *(pos + len - 2);
	data <<= 8;
	data |= *(pos + len - 1);

	for(i = 0;i < len - 2;i++){
		checksum += *(pos + i);
	}

	if(checksum == data){
		return 1;
	}
	else{
		return 0;
	}
	
}

// Char to long
unsigned long char2long(unsigned char *pos)
{
	unsigned long data;
	data = 0;

	data = *(pos + 0);
	data <<= 8;
	data |= *(pos + 1);
	data <<= 8;
	data |= *(pos + 2);
	data <<= 8;
	data |= *(pos + 3);

	return data;
}

//Mem copy
void memcopy(unsigned char* d_pos,unsigned char* s_pos,unsigned short len)
{
	unsigned short i;
	for(i = 0;i < len;i++){
		*(d_pos + i) = *(s_pos + i);
	}
}

// check other
unsigned char chk_other(unsigned char* pos,unsigned char* pos2,unsigned short len)
{
	unsigned short i;
	unsigned char flg;
	flg = 0;
	for(i = 0;i < len;i++){
		if(*(pos + i) ^ *(pos2 + i)){
			flg = 1;
			break;
		}
	}
	return flg;
}
#endif // 1
/*----------------------------------------------------------------*/






