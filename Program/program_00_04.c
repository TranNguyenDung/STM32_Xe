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
/*----------------------------------------------------------------*/


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
	unsigned char flg;

	
	//----------------------------------------------
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
	//----------------------------------------------

	
	//----------------------------------------------
	//timer 10ms for freqency loop code
	//Freq Tim1: 120MHz
	HAL_TIM_Base_Start_IT(&htim1);

	//With communication module IR
	HAL_UART_Receive_IT(&huart1,&r1_buff2[0],128);

	//with communication PC
	HAL_UART_Receive_IT(&huart2,&r2_buff2[0],sizeof(r2_buff2));

	//with communication bluetooth
	HAL_UART_Receive_IT(&huart3,&r3_buff2[0],128);
	
	
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
	rgb_ch[0].ccr_bk = __HAL_TIM_GetCounter(&htim2);
	rgb_ch[1].ccr_bk = __HAL_TIM_GetCounter(&htim2);
	rgb_ch[2].ccr_bk = __HAL_TIM_GetCounter(&htim2);
	rgb_ch[3].ccr_bk = __HAL_TIM_GetCounter(&htim2);
	rgb_ch[4].ccr_bk = __HAL_TIM_GetCounter(&htim2);

	//set pwm 
	//----------------------------------------------
		


	//----------------------------------------------
#if 0//Reset WDT
	IWDG->KR = 0xAAAA; // Writing 0xAAAA in the Key register prevents watchdog reset
	IWDG->KR = 0xCCCC; // Start the independent watchdog timer
	//TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_1s);
#endif	//Reset WDT
	//----------------------------------------------
	


	//----------------------------------------------
#if 0//Write address module
	tmp = 37;
	Flash_Erase(ADDR_MODULE);
	Flash_Write_Int(ADDR_MODULE,tmp);
#endif //Write address module

	//Read address module
	//tmp = (unsigned char) Flash_Read_Int(ADDR_MODULE);
	//addr_module = tmp;
	//----------------------------------------------




	//----------------------------------------------
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
	//----------------------------------------------

	//----------------------------------------------
	//tmp = 0xAA;
	//save_to_flash(&tmp);
	//data = 0x0000;
	//flash_write1(&data,0x081E0000,32);
	//----------------------------------------------

	//----------------------------------------------
#if 0//PWM TEST
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	//HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
	//HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
#endif//PWM TEST
	//----------------------------------------------

	//----------------------------------------------
	init_data();
	init_drive();
	//----------------------------------------------
	
	//----------------------------------------------
	//GPIO_PIN_SET/GPIO_PIN_RESET = 0U,
	//HAL_GPIO_WritePin(GPIOA,PWM_ENA_Pin,GPIO_PIN_SET);
	//----------------------------------------------

	//----------------------------------------------
	#if 0//For Test CRC16
	for(i = 0;i < sizeof(t1_buff);i++){
		t1_buff[i] = i;
	}
	add_crc16(&t1_buff[0],sizeof(t1_buff) - 2);
	flg = check_crc16(&t1_buff[0],sizeof(t1_buff) - 2);
	if(flg){
		flg = 1;
	}
	else{
		flg = 0;
	}
	
	add_checksume16(&t1_buff[0],sizeof(t1_buff));
	flg = check_checksume16(&t1_buff[0],sizeof(t1_buff));
	if(flg){
		flg = 1;
	}
	else{
		flg = 0;
	}
	#endif // 1//For Test CRC16
	//----------------------------------------------

	//Read address module
#if 1	// Bootloader check
	flg = read_fact();
	if(flg == 0){
		init_fact();
		//write_fact();
	}
	fact_bk = fact;
	opera();
	addr_module = fact.address;
#endif
	REFREST_WWDR
	
#if 0// Write address
	flg = read_fact();
	if(flg == 0){
		init_fact();
	}
	fact.address = 38;
	addr_module = fact.address;
	write_fact();
#endif	
	//-------------------------------
	while(1){
		//10ms frequency
		while(htim1_flg == 0);
		htim1_flg = 0;

		REFREST_WWDR
		
		//timer_couter();
		opera();

		//Xử lý nhận data từ mạch hệ thống
		uart2_with_due();

		rgb_opera();

		//Xử lý bộ nhớ cài đặt
		mem_set();

		//Xử lý nhận data IR
		serial2_comm();

		//Xử lý nhận data blue
		serial3_communication();
		
		check_rgb_value_tot();
	}
	//-------------------------------
}
/*----------------------------------------------------------------*/




/*------------------------------------------------------------------*/
//INIT data
/*------------------------------------------------------------------*/
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
	rgb_pwm();
}

//Init fact
void init_fact(void)
{
	unsigned char i;
	unsigned long dmy;

	fact.header[0] = 0x55;
	fact.header[1] = 0xAA;
	//-------------------------
	
	for(i = 0; i < 5; i++){
		dmy =5000;
		fact.ch[i].red_thr[0] = (dmy & 0xFF000000) >> 24;
		fact.ch[i].red_thr[1] = (dmy & 0x00FF0000) >> 16;
		fact.ch[i].red_thr[2] = (dmy & 0x0000FF00) >> 8;
		fact.ch[i].red_thr[3] = (dmy & 0x000000FF) >> 0;
		fact.ch[i].red_thr_chk_flg = 1;
		//-------------------------
		dmy = 5000;
		fact.ch[i].green_thr[0] = (dmy & 0xFF000000) >> 24;
		fact.ch[i].green_thr[1] = (dmy & 0x00FF0000) >> 16;
		fact.ch[i].green_thr[2] = (dmy & 0x0000FF00) >> 8;
		fact.ch[i].green_thr[3] = (dmy & 0x000000FF) >> 0;
		//fact.ch1.green_thr_chk_flg = 1;
		fact.ch[i].green_thr_chk_flg = 0;
		//-------------------------
		dmy = 5000;
		fact.ch[i].blue_thr[0] = (dmy & 0xFF000000) >> 24;
		fact.ch[i].blue_thr[1] = (dmy & 0x00FF0000) >> 16;
		fact.ch[i].blue_thr[2] = (dmy & 0x0000FF00) >> 8;
		fact.ch[i].blue_thr[3] = (dmy & 0x000000FF) >> 0;
		//fact.ch[i].blue_thr_chk_flg = 1;
		fact.ch[i].blue_thr_chk_flg = 0;
		//-------------------------
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
	
	//--------------------------------------------------
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
//Couter tm run on interrupt
void timer_couter(void)
{
	unsigned char i;
	
	if(one1sec_tm){
		one1sec_tm--;	
	}

	//TimeOut receiver module IR
	if(r1_tm > 1){
		r1_tm--;
	}
	
	//couter resend to CAR
	if(ir_scan_tm){
		ir_scan_tm--;
	}

	if(ir_run_tm){
		ir_run_tm--;
	}

	//couter resend to due
	if(send_due_tm){
		send_due_tm--;
	}
	
	if(scan_rgb_tm){
		scan_rgb_tm--;
	}

	//re-send to Dua via IR
	if(ir_send_must_ok_tm){
		ir_send_must_ok_tm--;
	}

	if(r3_tot > 1){
		r3_tot--;
	}

	if(rgb_pwm_tot){
		rgb_pwm_tot--;
	}

	//Run more space for face to face module IR
	if(driver_run_more_tm){
		driver_run_more_tm--;
	}

	if(car_run_mode_tm){
		car_run_mode_tm--;
	}

	if(car_run_mode_seq_tm){
		car_run_mode_seq_tm--;
	}

	if(rgb_wait_chk_tm){
		rgb_wait_chk_tm--;
	}
	
	if(bootloader_tm > 1){
		bootloader_tm--;
	}	

	if(fact_write_tm > 1){
		fact_write_tm--;
	}

	for(i = 0;i < sizeof(rgb_ch) / sizeof(rgb_ch[0]);i++){
		if(rgb_ch[i].freq_red_value_tot){
			rgb_ch[i].freq_red_value_tot--;
		}
		if(rgb_ch[i].freq_green_value_tot){
			rgb_ch[i].freq_green_value_tot--;
		}
		if(rgb_ch[i].freq_blue_value_tot){
			rgb_ch[i].freq_blue_value_tot--;
		}
		if(rgb_ch[i].freq_nofiter_value_tot){
			rgb_ch[i].freq_nofiter_value_tot--;
		}	
	}

	if(drive_tm){
		drive_tm--;
	}

	if(grb_stanby_tm){
		grb_stanby_tm--;
	}
}

//
void opera(void)
{
	//uint32_t data[8];
	//unsigned char tmp;
	unsigned char i;
	//unsigned long data;
	//unsigned char tmp[256];

	if(one1sec_tm == 0){
		one1sec_tm = 100;
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		//HAL_GPIO_TogglePin(LED_TEST1_GPIO_Port,LED_TEST1_Pin);
		//HAL_GPIO_TogglePin(LED_TEST2_GPIO_Port,LED_TEST2_Pin);
		//uart2_send_string("NAME",4);
	}
	
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

#endif // 1
/*----------------------------------------------------------------*/



/*----------------------------------------------------------------*/
//With RGB & PWM => set speed CAR
/*----------------------------------------------------------------*/
#if 1
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
		//Điều khiển động
		#if 1
		//When new value
		//rgb_pwm_tot = 1;
		if(rgb_pwm_tot && (grb_stanby_tm == 0)){

			//------------------------------------------------------------------------------
			#if 1//channel = RGB_CH3;
			//Channel 3
			get_chenh_lech_freq_channel(RGB_CH3);
			#endif // 1//channel = RGB_CH3;

			//------------------------------------------------------------------------------
			#if 1 // channel = RGB_CH2;
			get_chenh_lech_freq_channel(RGB_CH2);
			#endif // 1 // channel = RGB_CH2;

			//------------------------------------------------------------------------------
			#if 1//channel = RGB_CH4;
			get_chenh_lech_freq_channel(RGB_CH4);
			#endif // channel = RGB_CH4;

			#if 1//Tìm Chênh lệch lớn nhất
			max_chenhlech = 0;
			i_bk = 0;
			flg = 0;

			//Stop
			if((chn_flg[1] != 0) && (chn_flg[2] != 0) && (chn_flg[3] != 0)){
			//if((chn_flg[1] == 0) && (chn_flg[2] == 0) && (chn_flg[3] == 0)){
				if(rgb_status == RGB_STATUS_FULL_CHIP){
					//Up
					if(car_run_up_down_flg_bk == CAR_RUN_UP){
						if(car_run_up_down_flg == CAR_RUN_DOWN){
							rgb_chk_ch3_flg = 1;
						}
						#if 1//For debug
						car_run_up_down_flg = CAR_RUN_DOWN;
						#endif // 1//For debug

					}
					else{//car_run_up_down_flg_bk == CAR_RUN_DOWN
						if(car_run_up_down_flg == CAR_RUN_UP){
							rgb_chk_ch3_flg = 1;
						}
						#if 1//For debug
						if(car_run_ena){
							car_run_up_down_flg = CAR_RUN_UP;
						}
						else{
							car_run_up_down_flg = CAR_STOP;
						}
						#endif // 1//For debug

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
				#if 0
				if(car_run_mode ^ CAR_RUN_CDIPDNADN){
						car_run_mode = CAR_RUN_CDIPDNADN;
				}
				#endif // 0

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
				//Stop PWM khi xe ddi ra ngoài vạch màu
				else{
				}//Stop PWM
			}

			#endif // 1//Tìm Chênh lệch lớn nhất
			//-----------------------------------------------

			#if 1//Get Fact data PWM to data PWM
			//Init PWM
			car_init();

			//Car run UP
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
						left_up(fact.duty_ch3[R]);
					}
					else if(adjust_set_mode == 3){
						right_down(fact.duty_ch3[R]);
					}
					else{
						right_up(fact.duty_ch3[L]);
					}
				}
				#if 1
				// Tốc độ xe khi là con mắt RGB chính giửa
				else if(rgb_chk_ch3_flg){
					left_up(fact.duty_ch3[R]);
					right_up(fact.duty_ch3[L]);
				}
				// Tốc độ xe khi là con mắt RGB bên phải nhẹ
				else if(rgb_chk_ch2_flg){
					left_up(fact.duty_ch2[R]);
					right_up(fact.duty_ch2[L]);
				}

				// Tốc độ xe khi là con mắt RGB bên trái nhẹ
				else if(rgb_chk_ch4_flg){
					left_up(fact.duty_ch4[R]);
					right_up(fact.duty_ch4[L]);
				}
				#endif

				//Khi xe chạy ra ngoài đường đi
				else{		
					car_stop();
					//left_up(fact.duty_ch3[R]);
					//right_up(fact.duty_ch3[L]);
				}
				
				car_adj_line_tm = 0;
				car_adj_line_left_right = 0;
			}
			// Car run down
			else if(car_run_up_down_flg == CAR_RUN_DOWN){//car_run_up_down_flg == CAR_RUN_DOWN

				//Cho mục đích lấy thông tin RGB
				if(adjust_on_flg){
					if(adjust_set_mode == 0){
						//Stop drive when Adjust
						car_stop();
					}
					else if(adjust_set_mode == 1){
						left_down(fact.duty_ch3[L]);
					}
					else if(adjust_set_mode == 2){
						left_up(fact.duty_ch3[R]);
					}
					else if(adjust_set_mode == 3){
						right_down(fact.duty_ch3[R]);
					}
					else{
						right_up(fact.duty_ch3[L]);
					}
				}
				#if 0
				// Tốc độ xe khi là con mắt RGB chính giửa
				else if(rgb_chk_ch3_flg){
					if(car_adj_line_tm){
						if(car_adj_line_left_right == 1){//rgb_chk_ch2_flg
							left_down(fact.duty_ch2[L]);
							right_down(fact.duty_ch2[R]);
						}
						else{//car_adj_line_left_right // rgb_chk_ch4_flg
							left_down(fact.duty_ch4[L]);
							right_down(fact.duty_ch4[R]);
						}
						car_adj_line_tm--;
					}
					else{
						car_adj_line_left_right = 0;
						
						left_down(fact.duty_ch3[R]);
						right_down(fact.duty_ch3[L]);
					}

				}
				// Tốc độ xe khi là con mắt RGB bên phải nhẹ
				else if(rgb_chk_ch2_flg){
					left_down(fact.duty_ch2[R]);
					right_down(fact.duty_ch2[L]);

					if(car_adj_line_tm > 20){
						car_adj_line_tm = 21;
					}
					else{
						car_adj_line_tm++;
					}
					car_adj_line_left_right = 1;	
				}

				// Tốc độ xe khi là con mắt RGB bên trái nhẹ
				else if(rgb_chk_ch4_flg){
					left_down(fact.duty_ch4[R]);
					right_down(fact.duty_ch4[L]);

					if(car_adj_line_tm > 20){
						car_adj_line_tm = 21;
					}
					else{
						car_adj_line_tm++;
					}
					car_adj_line_left_right = 2;	
				}
				#endif

				//Khi xe chạy ra ngoài đường đi
				else{
					//car_stop();
					left_down(fact.duty_ch3[R]);
					right_down(fact.duty_ch3[L]);
				}
			}
			else{
				car_stop();
			}


			#endif // 1//Get Fact data PWM to data PWM

			send_status_car();
			//-----------------------------------------------
		}
		else{//Trường hợp module không trả về tín hiệu xung
		    car_stop();
		}
#endif

	}
}

// Check channel RGB
void get_chenh_lech_channel(unsigned char channel)
{
	unsigned long tmp;
	unsigned long offset;

	offset = char2long(&fact.ch[channel].offset[0]);
	if(fact.ch[channel].red_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].red_thr[0]);
		tmp += offset;

		if(rgb_ch[channel].freq_red_ct < tmp){
			chn_flg[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = rgb_ch[channel].freq_red_ct - tmp;
			chn_flg[channel] = 1;
		}
	}

	if(fact.ch[channel].green_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].green_thr[0]);
		tmp += offset;

		if(rgb_ch[channel].freq_green_ct < tmp){
			chn_flg[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = rgb_ch[channel].freq_green_ct - tmp;
			chn_flg[channel] = 1;
		}
	}

	if(fact.ch[channel].blue_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].blue_thr[0]);
		tmp += offset;

		if(rgb_ch[channel].freq_blue_ct < tmp){
			chn_flg[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = rgb_ch[channel].freq_blue_ct - tmp;
			chn_flg[channel] = 1;
		}
	}

	if(fact.ch[channel].nofiter_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].nofiter_thr[0]);
		tmp += offset;

		if(rgb_ch[channel].freq_nofiter_ct < tmp){
			chn_flg[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = rgb_ch[channel].freq_nofiter_ct - tmp;
			chn_flg[channel] = 1;
		}
	}
}

// Check channel RGB
void get_chenh_lech_freq_channel(unsigned char channel)
{
	unsigned long tmp;
	unsigned long offset;

	offset = char2long(&fact.ch[channel].offset[0]);
	if(fact.ch[channel].red_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].red_thr[0]);
		tmp -= offset;

		if(rgb_ch[channel].freq_red_ave > tmp){
			chn_flg[channel] = 0;
			chn_chenhlech[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = tmp - rgb_ch[channel].freq_red_ave;
			chn_flg[channel] = 1;
		}
	}

	if(fact.ch[channel].green_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].green_thr[0]);
		tmp -= offset;

		if(rgb_ch[channel].freq_green_ave > tmp){
			chn_flg[channel] = 0;
			chn_chenhlech[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = tmp - rgb_ch[channel].freq_green_ave;
			chn_flg[channel] = 1;
		}
	}

	if(fact.ch[channel].blue_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].blue_thr[0]);
		tmp -= offset;

		if(rgb_ch[channel].freq_blue_ave > tmp){
			chn_flg[channel] = 0;
			chn_chenhlech[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = tmp - rgb_ch[channel].freq_blue_ave;
			chn_flg[channel] = 1;
		}
	}

	if(fact.ch[channel].nofiter_thr_chk_flg){
		tmp = char2long(&fact.ch[channel].nofiter_thr[0]);
		tmp -= offset;

		if(rgb_ch[channel].freq_nofiter_ave > tmp){
			chn_flg[channel] = 0;
			chn_chenhlech[channel] = 0;
		}
		else{
			chn_chenhlech[channel] = tmp - rgb_ch[channel].freq_nofiter_ave;
			chn_flg[channel] = 1;
		}
	}
}

// Send status to PC
void send_status_car() 
{
	unsigned char buff[5];


	if(rgb_status_flg){
		if(grb_stats_tm == 0){
			if(rgb_chk_ch3_flg){
				buff[2] = 0xAA;
			}
			else{
				buff[2] = 0;
			}
			if(rgb_chk_ch2_flg){
				buff[1] = 0xAA;
			}
			else{
				buff[1] = 0;
			}
			if(rgb_chk_ch1_flg){
				buff[0] = 0xAA;
			}
			else{
				buff[0] = 0;
			}
			if(rgb_chk_ch4_flg){
				buff[3] = 0xAA;
			}
			else{
				buff[3] = 0;
			}
			if(rgb_chk_ch5_flg){
				buff[4] = 0xAA;
			}
			else{
				buff[4] = 0;
			}

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
	unsigned long duty;
	unsigned long tmp;
	//PWM TEST
	//HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	//HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
	//HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
	//WHEEL LEFT
	//HAL_GPIO_WritePin(GPIOA, RGB_LED_Pin|PWM_ENA_Pin, GPIO_PIN_RESET);
	if(drive_tm == 0){
		if(drive.left_up_on){
			if(drive.left_up_on ^ drive.left_up_on_bk){
				if(drive.left_up_duty){
					drive.left_up_on_bk = drive.left_up_on;
					drive.left_up_duty_bk = drive.left_up_duty;
				
					if((drive.freq[0] ^ drive.freq_bk[0]) || (drive.freq[1] ^ drive.freq_bk[1])){
						drive.freq_bk[0] = drive.freq[0];
						drive.freq_bk[1] = drive.freq[1];
						tmp = drive.freq[0] << 8;
						tmp |= drive.freq[1];
						set_freq(tmp);
					}
					duty = drive.left_up_duty;
					set_duty(&htim3,duty,TIM_CHANNEL_3);
					HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);			
				}
			}
			else if(drive.left_up_duty ^ drive.left_up_duty_bk){
				drive.left_up_duty_bk = drive.left_up_duty;
				duty = drive.left_up_duty;
				set_duty(&htim3,duty,TIM_CHANNEL_3);
			}
			//enable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_ENA_GPIO_Port,PWM_ENA_Pin) == GPIO_PIN_RESET){
				//GPIO_PIN_SET/GPIO_PIN_RESET = 0U,
				HAL_GPIO_WritePin(PWM_ENA_GPIO_Port,PWM_ENA_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.left_up_on ^ drive.left_up_on_bk){
				drive.left_up_on_bk = drive.left_up_on;
				//set_duty(&htim3,drive.left_duty,TIM_CHANNEL_1);
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
						tmp = drive.freq[0] << 8;
						tmp |= drive.freq[1];
						set_freq(tmp);
					}
					duty = drive.left_down_duty;
					set_duty(&htim3,duty,TIM_CHANNEL_4);
					HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
				}
			}
			else if(drive.left_down_duty ^ drive.left_down_duty_bk){
				drive.left_down_duty_bk = drive.left_down_duty;
				duty = drive.left_down_duty;
				set_duty(&htim3,duty,TIM_CHANNEL_4);
			}
			//enable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_ENA_GPIO_Port,PWM_ENA_Pin) == GPIO_PIN_RESET){
				//GPIO_PIN_SET/GPIO_PIN_RESET = 0U,
				HAL_GPIO_WritePin(PWM_ENA_GPIO_Port,PWM_ENA_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.left_down_on ^ drive.left_down_on_bk){
				drive.left_down_on_bk = drive.left_down_on;
				//set_duty(&htim3,drive.left_duty,TIM_CHANNEL_1);
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
					tmp = drive.freq[0] << 8;
					tmp |= drive.freq[1];
					set_freq(tmp);
				}
				duty = drive.right_up_duty;
				set_duty(&htim3,duty,TIM_CHANNEL_2);
				HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);			
			
			}
			else if(drive.right_up_duty ^ drive.right_up_duty_bk){
				drive.right_up_duty_bk = drive.right_up_duty;
				duty = drive.right_up_duty;
				set_duty(&htim3,duty,TIM_CHANNEL_2);
			}
			//enable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_ENA_GPIO_Port,PWM_ENA_Pin) == GPIO_PIN_RESET){
				//GPIO_PIN_SET/GPIO_PIN_RESET = 0U,
				HAL_GPIO_WritePin(PWM_ENA_GPIO_Port,PWM_ENA_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.right_up_on ^ drive.right_up_on_bk){
				drive.right_up_on_bk = drive.right_up_on;
				//set_duty(&htim3,drive.right_duty,TIM_CHANNEL_2);
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
					tmp = drive.freq[0] << 8;
					tmp |= drive.freq[1];
					set_freq(tmp);
				}
				duty = drive.right_down_duty;
				set_duty(&htim3,duty,TIM_CHANNEL_1);
				HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
			
			}
			else if(drive.right_down_duty ^ drive.right_down_duty_bk){
				drive.right_down_duty_bk = drive.right_down_duty;

				duty = drive.right_down_duty;
				set_duty(&htim3,duty,TIM_CHANNEL_1);
			}
			//enable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_ENA_GPIO_Port,PWM_ENA_Pin) == GPIO_PIN_RESET){
				//GPIO_PIN_SET/GPIO_PIN_RESET = 0U,
				HAL_GPIO_WritePin(PWM_ENA_GPIO_Port,PWM_ENA_Pin,GPIO_PIN_SET);
			}
		}
		else{
			if(drive.right_down_on ^ drive.right_down_on_bk){
				drive.right_down_on_bk = drive.right_down_on;
				//set_duty(&htim3,drive.right_duty,TIM_CHANNEL_2);
				HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
			}
		}

		if((drive.left_up_on == 0) && 
		   (drive.left_down_on == 0) && 
		   (drive.right_up_on == 0) && 
		   (drive.right_down_on == 0)){
			//disable PWM ENA
			if(HAL_GPIO_ReadPin(PWM_ENA_GPIO_Port,PWM_ENA_Pin) == GPIO_PIN_SET){
				//GPIO_PIN_SET/GPIO_PIN_RESET = 0U,
				HAL_GPIO_WritePin(PWM_ENA_GPIO_Port,PWM_ENA_Pin,GPIO_PIN_RESET);
			}
		}	
	}

}

//Set duty
//duty(0% <-> 100%)
void set_duty(TIM_HandleTypeDef *htim,unsigned char duty,unsigned long channel)
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
	//unsigned char *pos;
	//unsigned char i;

	//
	if(r1_flg){
		r1_flg = 0;
		//---------------------------------------
		//Serial.write(&r2_buff[0],r2_pos);
		//pos = &r1_buff[0];
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


		//---------------------------------------
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
		else if((r1_buff[0] == IR_CMD_NG) && (r1_buff[1] == 0x0D) && (r1_buff[2] == 0x0A)){
			//ir_new_data_flg = 1;
			//ir_new_data_type = IR_CMD_NG;
		}


		//---------------------------------------
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


		//---------------------------------------
		else{
			//ir_new_data_flg = 0;
			//ir_new_data_type = 0;
		}

		r1_pos = 0;
	}

	//SEND DATA
	if(ir_send_must_ok_flg){
		if(ir_send_must_ok_tm == 0){
			//Send to speed [VE PHAT GIOI]
			if(ir_send_must_ok_type == IR_CMD_VE_PG){
				IR_Send(IR_CMD_VE_PG);
				//Serial.println("IR_CMD_VE_PG");
			}
			//Send to speed [THUOC CONG THUC]
			else if(ir_send_must_ok_type == IR_CMD_THUOC_CT){
				IR_Send(IR_CMD_THUOC_CT);
				//Serial.println("IR_CMD_THUOC_CT");
			}
			//Send to speed [KHONG THUOC CONG THUC]
			else if(ir_send_must_ok_type == IR_CMD_KHONG_THUOC_CT){
				IR_Send(IR_CMD_KHONG_THUOC_CT);
				//Serial.println("IR_CMD_KHONG_THUOC_CT");
			}
			//Send to speed [BUONG]
			else if(ir_send_must_ok_type == IR_CMD_BUONG){
				IR_Send(IR_CMD_BUONG);
				//Serial.println("IR_CMD_BUONG");
			}
			//Send cmd KHONG VE
			else if(ir_send_must_ok_type == IR_CMD_KHONG_VE_PG){
				IR_Send(IR_CMD_KHONG_VE_PG);
				//Serial.println("IR_CMD_KHONG_VE_PG");
			}
			ir_send_must_ok_tm = 50;//500ms	

			if(ir_send_must_ok_tmr > 1){
				ir_send_must_ok_tmr--;
				if(ir_send_must_ok_tmr == 1){
					ir_send_must_ok_tmr = 0;
					ir_send_must_ok_flg = 0;
					ir_send_must_ok_tm = 0;
				}
			}
		}
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
	unsigned char* pos2;
	unsigned char data;
	unsigned char flg;
	unsigned short i;
	unsigned char tmp;
	struct st_IR_Format ir_tmp;
	unsigned char tmp_buff[4];

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
				else if(string_compera(pos,"INFO#")){
					u2_send_str("BAN QUYEN CHUA THIEN TONG TAN DIEU",sizeof("BAN QUYEN CHUA THIEN TONG TAN DIEU"));
				}
				//-----------------------------------------------
				// Write fact to flash on MCU
				else if(string_compera(pos,"FACTW#")){
					if(get_fact()){
						uart2_send_nak();
					}
					else{
						uart2_send_ack();
					}
				}
				// Read fact struct
				else if(string_compera(pos,"FACTR#")){
					if(read_fact()){
						uart2_send_buff(&fact.header[0],sizeof(fact));
					}
					else{
						uart2_send_ack();
					}
				}
				//-----------------------------------------------
				// Send to PC value RGB now
				else if(string_compera(pos,"GETRGB#")){
					pos += strlen("GETRGB");
					if(ascii2bin(pos,&data,1)){
						if(data < 5){
							//send_couter_rgb(data);
							send_freq_rgb(data);
						}
						else{
							u2_send_str("NG",2);
						}
					}
					else{
						u2_send_str("NG",2);
					}
				}

				//-----------------------------------------------
				//RGB status ON
				else if(string_compera(pos,"RGBSTON#")){
					rgb_status_flg = 1;
					uart2_send_nak();
				}
				//RGB status OFF
				else if(string_compera(pos,"RGBSTOFF#")){
					rgb_status_flg = 0;
					uart2_send_nak();
				}

				//-----------------------------------------------
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


				//-----------------------------------------------
				//BLU
				else if(string_compera(pos,"BLUE#")){
					//[TT~BLUE] + [bbbbbbbbb...] + [enter 2 byte]
					pos += 4;
					r3_flg = 1;
					pos2 = &r3_buff[0];
					r3_pos = r2_pos - 9;
					for(i = 0;i < r3_pos; i++){
						*(pos2 + i) = *(pos + i);
					}
					uart2_send_nak();

				}

				//-----------------------------------------------
				//Car run TH1
				else if(string_compera(pos,"CARRUNTH1#")){
					car_run_mode = CAR_RUN_CDVL;
					uart2_send_nak();
				}
				//Car run TH2
				else if(string_compera(pos,"CARRUNTH2#")){
					car_run_mode = CAR_RUN_CDBPD;
					uart2_send_nak();
				}
				//Car run TH3
				else if(string_compera(pos,"CARRUNTH3#")){
					car_run_mode = CAR_RUN_CDIPDNADN;
					uart2_send_nak();
				}


				//Back Car
				else if(string_compera(pos,"CARBACK#")){
					car_back_flg = 1;
					car_not_get_ir_flg = 1;
					uart2_send_nak();
				}
				//Start Car
				else if(string_compera(pos,"CARSTART#")){
					car_start_flg = 1;
					car_back_flg = 0;
					car_not_get_ir_flg = 0;
					uart2_send_nak();
				}
				//-----------------------------------------------
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

				//-----------------------------------------
				//CHECK MODULE
				else if(string_compera(pos,"CHK#")){
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//-----------------------------------------
				//Setting
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

				//-----------------------------------------

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

				//Cồng Đức Vô Lượng(Về Phật Giới Trường Hợp 1)
				else if(string_compera(pos,"VPGTH1#")){
					ir_for_car_run_th1_flg = 1;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//Công Đức Bằng Phước Đức(Về Phật Giới trường hợp 2)
				else if(string_compera(pos,"VPGTH2#")){
					ir_for_car_run_th2_flg = 1;
					u2_send_addr_data_str("OK",sizeof("OK"));
				}
				//Công Đức Ít - Phước Đức Nhiều - Ác Đức Nhiều(Về Phật Giới trường hợp 3)
				else if(string_compera(pos,"VPGTH3#")){
					ir_for_car_run_th3_flg = 1;
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

				//-----------------------------------------
				//DUE SEND OF
				else if(string_compera(pos,"OK#")){
					send_due_flg = 0;
					send_due_tm = 0;
					send_due_type = TYPE_SEND_DUE_NULL;
				}
				//Other
				else{
					u2_send_str("NG",2);
				}
			}
			else{
			
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

	//send to due if have event car
	if(send_due_flg){
		if(send_due_tm == 0){
			//KHI XE DEN DIEM VA GUI LENH LEN
			if(send_due_type == TYPE_SEND_DUE_CAR){
				u2_send_addr_data_str("CAR",sizeof("CAR"));
			}
			//KHI ONG THAN TRUONG HOP CONG DUC VA PHUOC DUC BANG NHAU HOI
			else if(send_due_type == TYPE_SEND_DUE_VE_PG){
				u2_send_addr_data_str("VE",sizeof("VE"));
			}
			//KHI ONG THAN HOI CO THUOC CONG DUC KHONG
			else if(send_due_type == TYPE_SEND_DUE_THUOC_CT){
				u2_send_addr_data_str("THUOC",sizeof("THUOC"));
			}
			//KHI ONG THAN HOI CO THUOC CONG DUC KHONG
			else if(send_due_type == TYPE_SEND_DUE_KHONG_THUOC_CT){
				u2_send_addr_data_str("KHONG",sizeof("KHONG"));
			}
			//KHI THANH - THANH - DONG TOC NIU KEO LAI THE GIOI LUAN HOI
			else if(send_due_type == TYPE_SEND_DUE_BUONG){
				u2_send_addr_data_str("BUONG",sizeof("BUONG"));
			}
			send_due_tm = 150;//1s
		}

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
	for(j=0;j<22;j++){
		*(pos + i++) = tmp[j];
	}
	
	bin2ascii(&tmp[22],(pos + 22),2);
	
	i += 4;
	
	//uart2_send_buff(pos,i);
	uart2_send_string(pos,i);
}

//send couter rgb
void send_couter_rgb(unsigned char channel)
{
	unsigned char *pos;
	unsigned char buff[64];
	unsigned char i;

	i = 0;
	//red
	buff[i++] = (rgb_ch[channel].freq_red_ct & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_red_ct & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_red_ct & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_red_ct & 0xFF) >> 0;
	//green
	buff[i++] = (rgb_ch[channel].freq_green_ct & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_green_ct & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_green_ct & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_green_ct & 0xFF) >> 0;
	//blue
	buff[i++] = (rgb_ch[channel].freq_blue_ct & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_blue_ct & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_blue_ct & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_blue_ct & 0xFF) >> 0;
	//no fiter
	buff[i++] = (rgb_ch[channel].freq_nofiter_ct & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_nofiter_ct & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_nofiter_ct & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_nofiter_ct & 0xFF) >> 0;
	pos = &buff[0];
	uart2_send_buff(pos,i);
}

//send freq rgb
void send_freq_rgb(unsigned char channel)
{
	unsigned char *pos;
	unsigned char buff[64];
	unsigned char i;

	i = 0;
	//red
	buff[i++] = (rgb_ch[channel].freq_red_ave & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_red_ave & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_red_ave & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_red_ave & 0xFF) >> 0;
	//green
	buff[i++] = (rgb_ch[channel].freq_green_ave & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_green_ave & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_green_ave & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_green_ave & 0xFF) >> 0;
	//blue
	buff[i++] = (rgb_ch[channel].freq_blue_ave & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_blue_ave & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_blue_ave & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_blue_ave & 0xFF) >> 0;
	//no fiter
	buff[i++] = (rgb_ch[channel].freq_nofiter_ave & 0xFF000000) >> 24;
	buff[i++] = (rgb_ch[channel].freq_nofiter_ave & 0xFF0000) >> 16;
	buff[i++] = (rgb_ch[channel].freq_nofiter_ave & 0xFF00) >> 8;
	buff[i++] = (rgb_ch[channel].freq_nofiter_ave & 0xFF) >> 0;
	pos = &buff[0];
	uart2_send_buff(pos,i);
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


	//dmy = strlenght("MUON VE PHAT GIOI");
	//Serial.println(dmy);

	if(bluetooth_flg){
		bluetooth_flg = 0;

		//s0_send_buff(&bluetooth_buff[0], bluetooth_len);
		//s1_send_buff(&bluetooth_buff[0],bluetooth_buff);
		//VỀ PHẬT GIỚI
		//KHÔNG VỀ PHẬT GIỚI
		//KHÔNG MUỐN VỀ PHẬT GIỚI
		//ĐI HƯỠNG PHƯỚC ĐỨC
		//KHÔNG MUỐN ĐI HƯỠNG PHƯỚC
		//MUỐN GIẢI THOÁT


		//ĐI LUÂN HỒI
		//KHÔNG MUỐN ĐI LUÂN HỒI
		//dmy = strlenght("MUON VE PHAT GIOI");
		//Serial.println(dmy);
		//Serial1.println("Xin Chào");
		#if 1
		pos = &bluetooth_buff[0];
		if(str_compare(pos,"KHONG MUON VE PHAT GIOI",bluetooth_len)){
			//Serial.println("TH01");
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		else if(str_compare(pos,"MUON VE PHAT GIOI",bluetooth_len)){
			//Serial.println("TH02");
		  //Serial.println("TH08");	
			set_ir_send_must_ok(IR_CMD_VE_PG,ON,10);
		}
		else if(str_compare(pos,"KHONG MUON DE PHAT GIOI",bluetooth_len)){
			//Serial.println("TH03");
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		else if(str_compare(pos,"MUON DE PHAT GIOI",bluetooth_len)){
			//Serial.println("TH04");
			//Serial.println("TH08");	
			set_ir_send_must_ok(IR_CMD_VE_PG,ON,10);
		}
		else if(str_compare(pos,"KHONG MUON GIAI THOAT",bluetooth_len)){
			//Serial.println("TH05");
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		else if(str_compare(pos,"MUON GIAI THOAT",bluetooth_len)){
			//Serial.println("TH06");
			//Serial.println("TH08");	
			set_ir_send_must_ok(IR_CMD_VE_PG,ON,10);
		}
		//----------------------------------------------
		//BUONG TH2: CONG DUC = PHUOC DUC: KHONG VE
		else if(str_compare(pos,"KHONG VE",bluetooth_len)){
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: KHONG VE
		else if(str_compare(pos,"KHONG MUON VE",bluetooth_len)){
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: KHONG VE
		else if(str_compare(pos,"KHONG MUN VE",bluetooth_len)){
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: KHONG VE
		else if(str_compare(pos,"KHONG MON VE",bluetooth_len)){
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: KHONG VE
		else if(str_compare(pos,"KHONG ZE",bluetooth_len)){
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: VE
		else if(str_compare(pos,"VE",bluetooth_len)){
			//Serial.println("TH08");	
			set_ir_send_must_ok(IR_CMD_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: VE
		else if(str_compare(pos,"MUON VE",bluetooth_len)){
			//Serial.println("TH08");	
			set_ir_send_must_ok(IR_CMD_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: VE
		else if(str_compare(pos,"MON VE",bluetooth_len)){
			//Serial.println("TH08");	
			set_ir_send_must_ok(IR_CMD_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: VE
		else if(str_compare(pos,"MUN VE",bluetooth_len)){
			//Serial.println("TH08");	
			set_ir_send_must_ok(IR_CMD_VE_PG,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: KHONG THUOC
		else if(str_compare(pos,"KHONG THUOC",bluetooth_len)){
			//Serial.println("TH07");
			set_ir_send_must_ok(IR_CMD_KHONG_THUOC_CT,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: THUOC 
		else if(str_compare(pos,"THUOC",bluetooth_len)){
			//Serial.println("TH08");
			set_ir_send_must_ok(IR_CMD_THUOC_CT,ON,10);
		}

		//BUONG TH2: CONG DUC = PHUOC DUC: CO BIET 
		else if(str_compare(pos,"KHONG BIET",bluetooth_len)){
			//Serial.println("TH08");
			set_ir_send_must_ok(IR_CMD_KHONG_THUOC_CT,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: CO BIET 
		else if(str_compare(pos,"CO BIET",bluetooth_len)){
			//Serial.println("TH08");
			set_ir_send_must_ok(IR_CMD_THUOC_CT,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: CO BIET 
		else if(str_compare(pos,"BIET",bluetooth_len)){
			//Serial.println("TH08");
			set_ir_send_must_ok(IR_CMD_THUOC_CT,ON,10);
		}
		//BUONG TH2: CONG DUC = PHUOC DUC: CO BIET 
		else if(str_compare(pos,"BIEC",bluetooth_len)){
			//Serial.println("TH08");
			set_ir_send_must_ok(IR_CMD_THUOC_CT,ON,10);
		}

		//----------------------------------------------

		//BUONG TH3: CONG DUC = PHUOC DUC: 
		else if(str_compare(pos,"BUONG",bluetooth_len)){
			//SEND TO DUE
			set_ir_send_must_ok(IR_CMD_BUONG,ON,10);
		}
		//TH BUONG MEO MIENG THANH [BUON]
		else if(str_compare(pos,"BUON",bluetooth_len)){
			//SEND TO DUE
			set_ir_send_must_ok(IR_CMD_BUONG,ON,10);
		}
		//BUM
		//TH BUONG MEO MIENG THANH [BUM]
		else if(str_compare(pos,"BUM",bluetooth_len)){
			//SEND TO DUE
			set_ir_send_must_ok(IR_CMD_BUONG,ON,10);
		}
		//PHONE
		else if(str_compare(pos,"PHONE",bluetooth_len)){
			//SEND TO DUE
			set_ir_send_must_ok(IR_CMD_BUONG,ON,10);
		}
		//------------------------------------------------------
		else{
			//Serial.println("TH09");
		}
		#endif
	}

}
#endif // 1

/*----------------------------------------------------------------*/






/*----------------------------------------------------------------*/
//UART interrupt
/*----------------------------------------------------------------*/
#if 1

//Interrupt timer
//Interrupt timer 10ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim->Instance == TIM1){
		htim1_flg = 1;
		timer_couter();
		rgb_pwm();
		pwm_drive_opera();
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






/*---------------------------------------------------------------*/
//RGB Detect
/*---------------------------------------------------------------*/
#if 1

//scan_rgb_seq
void rgb_opera(void)
{
	//unsigned char i;
	//unsigned long* pos;
	//Chuan Bi
	if(scan_rgb_seq == 0){
		//set_scale_freq(FREQ_HIGH);
		set_scale_freq(fact.rgb_scale);
		s2_flg = 0;
		s3_flg = 0;	
		//scan_rgb_seq_bk = 0;
		scan_rgb_seq++;
		scan_rgb_tm = 0;
		scan_rgb_tm_set = 10;
		//disable PWM ENA
		if(HAL_GPIO_ReadPin(RGB_LED_GPIO_Port,RGB_LED_Pin) == GPIO_PIN_RESET){
			//GPIO_PIN_SET/GPIO_PIN_RESET = 0U,
			HAL_GPIO_WritePin(RGB_LED_GPIO_Port,RGB_LED_Pin,GPIO_PIN_SET);
		}
	}
	//set scan red
	else if(scan_rgb_seq == 1){
		if(scan_rgb_tm == 0){
			if(fact.ch[RGB_CH1].red_thr_chk_flg){
				if(scan_rgb_seq_bk ^ scan_rgb_seq){
					scan_rgb_seq_bk = scan_rgb_seq;
					scan_rgb_tm = scan_rgb_tm_set;
				
					//set_scale_freq(FREQ_HIGH);
					set_scale_freq(fact.rgb_scale);
					set_fiter(RED_SELECT);
					//init_st_rgb_channel(RGB_CH1,RED_SELECT);
					init_st_rgb_channel(RGB_CH2,RED_SELECT);
					init_st_rgb_channel(RGB_CH3,RED_SELECT);
					init_st_rgb_channel(RGB_CH4,RED_SELECT);
					//init_st_rgb_channel(RGB_CH5,RED_SELECT);
				}			
			}
			else{
				scan_rgb_seq++;
			}
		}
	}
	//set scan blue
	else if(scan_rgb_seq == 2){
		if(scan_rgb_tm == 0){
			if(fact.ch[RGB_CH1].blue_thr_chk_flg){
				if(scan_rgb_seq_bk ^ scan_rgb_seq){
					scan_rgb_seq_bk = scan_rgb_seq;
					scan_rgb_tm = scan_rgb_tm_set;

					//set_scale_freq(FREQ_HIGH);
					set_scale_freq(fact.rgb_scale);
					set_fiter(BLUE_SELECT);
					//init_st_rgb_channel(RGB_CH1,BLUE_SELECT);
					init_st_rgb_channel(RGB_CH2,BLUE_SELECT);
					init_st_rgb_channel(RGB_CH3,BLUE_SELECT);
					init_st_rgb_channel(RGB_CH4,BLUE_SELECT);
					//init_st_rgb_channel(RGB_CH5,BLUE_SELECT);
				}			
			}
			else{
				scan_rgb_seq++;
			}


		}
	}
	//set scan green
	else if(scan_rgb_seq == 3){
		if(scan_rgb_tm == 0){
			if(fact.ch[RGB_CH1].green_thr_chk_flg){
				if(scan_rgb_seq_bk ^ scan_rgb_seq){
					scan_rgb_seq_bk = scan_rgb_seq;
					scan_rgb_tm = scan_rgb_tm_set;

					//set_scale_freq(FREQ_HIGH);
					set_scale_freq(fact.rgb_scale);
					set_fiter(GREEN_SELECT);
					//init_st_rgb_channel(RGB_CH1,GREEN_SELECT);
					init_st_rgb_channel(RGB_CH2,GREEN_SELECT);
					init_st_rgb_channel(RGB_CH3,GREEN_SELECT);
					init_st_rgb_channel(RGB_CH4,GREEN_SELECT);
					//init_st_rgb_channel(RGB_CH5,GREEN_SELECT);
				}			
			}
			else{
				scan_rgb_seq++;
			}

		}
	}
	//set scan no fiter
	else if(scan_rgb_seq == 4){
		if(scan_rgb_tm == 0){
			if(fact.ch[RGB_CH1].nofiter_thr_chk_flg){
				if(scan_rgb_seq_bk ^ scan_rgb_seq){
					scan_rgb_seq_bk = scan_rgb_seq;
					scan_rgb_tm = scan_rgb_tm_set;

					//set_scale_freq(FREQ_HIGH);
					set_scale_freq(fact.rgb_scale);
					set_fiter(NOFITER_SELECT);
					//init_st_rgb_channel(RGB_CH1,NOFITER_SELECT);
					init_st_rgb_channel(RGB_CH2,NOFITER_SELECT);
					init_st_rgb_channel(RGB_CH3,NOFITER_SELECT);
					init_st_rgb_channel(RGB_CH4,NOFITER_SELECT);
					//init_st_rgb_channel(RGB_CH5,NOFITER_SELECT);
				}
			}
			else{
				scan_rgb_seq = 1;
			}

		}
	}
	else{
		//set_scale_freq(FREQ_HIGH);
		set_scale_freq(fact.rgb_scale);
		scan_rgb_seq = 0;
		scan_rgb_tm = 0;
	}
	
	//-----S0 SET---------
	if(s0_flg ^ s0_bk_flg){
		s0_bk_flg = s0_flg;
		if(s0_flg){
			//S0 set on
			HAL_GPIO_WritePin(S0_GPIO_Port,S0_Pin,GPIO_PIN_SET);
		}
		else{
			//S0 set off
			HAL_GPIO_WritePin(S0_GPIO_Port,S0_Pin,GPIO_PIN_RESET);
		}
	}
	//-----S1 SET---------
	if(s1_flg ^ s1_bk_flg){
		s1_bk_flg = s1_flg;
		if(s1_flg){
			//S0 set on
			HAL_GPIO_WritePin(S1_GPIO_Port,S1_Pin,GPIO_PIN_SET);
		}
		else{
			//S0 set off
			HAL_GPIO_WritePin(S1_GPIO_Port,S1_Pin,GPIO_PIN_RESET);
		}
	}
	//-----S2 SET---------
	if(s2_flg ^ s2_bk_flg){
		s2_bk_flg = s2_flg;
		if(s2_flg){
			//S0 set on
			HAL_GPIO_WritePin(S2_GPIO_Port,S2_Pin,GPIO_PIN_SET);
		}
		else{
			//S0 set off
			HAL_GPIO_WritePin(S2_GPIO_Port,S2_Pin,GPIO_PIN_RESET);
		}
	}
	//-----S3 SET---------
	if(s3_flg ^ s3_bk_flg){
		s3_bk_flg = s3_flg;
		if(s3_flg){
			//S3 set on
			HAL_GPIO_WritePin(S3_GPIO_Port,S3_Pin,GPIO_PIN_SET);
		}
		else{
			//S0 set off
			HAL_GPIO_WritePin(0,S3_Pin,GPIO_PIN_RESET);
		}
	}
	
#if 0
	//-----------------------------
	//Set pin LEFT & RIGHT to MEGA
	//HAL_GPIO_TogglePin(LED_TEST1_GPIO_Port,LED_TEST1_Pin);
	//HAL_GPIO_TogglePin(LED_TEST2_GPIO_Port,LED_TEST2_Pin);
	if(rgb_ch[0].freq_red_ave > 7000){
		HAL_GPIO_WritePin(LED_TEST1_GPIO_Port,LED_TEST1_Pin,1);
	}
	else{
		HAL_GPIO_WritePin(LED_TEST1_GPIO_Port,LED_TEST1_Pin,0);
	}
	
	if(rgb_ch[1].freq_red_ave > 7000){
		HAL_GPIO_WritePin(LED_TEST2_GPIO_Port,LED_TEST2_Pin,1);
	}
	else{
		HAL_GPIO_WritePin(LED_TEST2_GPIO_Port,LED_TEST2_Pin,0);
	}
#endif	
}

//init struct grb
void init_st_rgb_channel(unsigned char channel,unsigned char rgb)
{
	unsigned long *pos;
	unsigned char i;
	//Red
	if(rgb == RED_SELECT){
		pos = &rgb_ch[channel].freq_red[0];
		rgb_ch[channel].red_pos = 0;
		rgb_ch[channel].red_flg = 0;
	}
	//Green
	else if(rgb == GREEN_SELECT){
		pos = &rgb_ch[channel].freq_green[0];
		rgb_ch[channel].green_pos = 0;
		rgb_ch[channel].green_flg = 0;
	}
	//Blue
	else if(rgb == BLUE_SELECT){
		pos = &rgb_ch[channel].freq_blue[0];
		rgb_ch[channel].blue_pos = 0;
		rgb_ch[channel].blue_flg = 0;
	}
	else{//rgb == NOFITER_SELECT
		pos = &rgb_ch[channel].freq_nofiter[0];
		rgb_ch[channel].nofiter_pos = 0;
		rgb_ch[channel].nofiter_flg = 0;
	}
	
	for(i = 0;i < sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter[0]);i++){
		*(pos + i) = 0;
	}
}

//OUTPUT_FREQUENCY_SCALING
//set scale frequency
void set_scale_freq(unsigned char scale)
{
	#if 1
	if(scale == POWER_DOWM){//off frequency(0%)
		s0_flg = 0;
		s1_flg = 0;
	}
	else if(scale == FREQ_LOW){//frequency 2% f0 = max 500Khz
		s0_flg = 0;
		s1_flg = 1;
	}
	else if(scale == FREQ_MID){//frequency 20% f0 = max 500Khz
		s0_flg = 1;
		s1_flg = 0;
	}
	else if(scale == FREQ_HIGH){//frequency 100% f0 = max 500Khz
		s0_flg = 1;
		s1_flg = 1;
	}
	else{
		//POWER_DOWM
		s0_flg = 0;
		s1_flg = 0;
	}
	#endif // 0
	//FREQ_HIGH
	//s0_flg = 1;
	//s1_flg = 1;

	//FREQ_MID
	//frequency 20% f0 = max 500Khz
	//s0_flg = 1;
	//s1_flg = 0;

	//FREQ_LOW
	//s0_flg = 0;
	//s1_flg = 1;
}

//set fiter
void set_fiter(unsigned char fiter)
{
	if(fiter == RED_SELECT){
		s2_flg = 0;
		s3_flg = 0;
	}
	else if(fiter == GREEN_SELECT){
		s2_flg = 1;
		s3_flg = 1;
	}
	else if(fiter == BLUE_SELECT){
		s2_flg = 0;
		s3_flg = 1;
	}
	else{//NOFITER_SELECT
		s2_flg = 1;
		s3_flg = 0;
	}
	rgb_ch[RGB_CH1].set_filter_change_delay = scan_rgb_tm_set - 3;
	rgb_ch[RGB_CH2].set_filter_change_delay = scan_rgb_tm_set - 3;
	rgb_ch[RGB_CH3].set_filter_change_delay = scan_rgb_tm_set - 3;
	rgb_ch[RGB_CH4].set_filter_change_delay = scan_rgb_tm_set - 3;
	rgb_ch[RGB_CH5].set_filter_change_delay = scan_rgb_tm_set - 3;
}

//TIMER 16BIT
#define END_COUTER 0xFFFF

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
	
	//unsigned long *pos;
	//unsigned long dmy;
	unsigned long tmp;
	unsigned long tmp1;
	//unsigned long tmp5;
	//unsigned long tmp5_ct;
	//unsigned char i;
	//unsigned char flg;
	unsigned char flg2;
	unsigned char channel;


	flg2 = 0;
	if(htim->Instance == TIM2){
		if(s0_flg || s1_flg){
			
			if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
				flg2 = 1;
				channel = RGB_CH1;
				tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_1);
				//tmp1 = __HAL_TIM_GetCounter(&htim2);
				
				#if 1
				if(tmp1 >= rgb_ch[channel].ccr_bk){
					tmp = tmp1 - rgb_ch[channel].ccr_bk;
				}
				else{
					
					tmp = END_COUTER - rgb_ch[channel].ccr_bk + tmp1 + 1;
				}
				#endif
				#if 0
				htim->Instance->CCR1 = 0;
				//__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);
				//htim2.Instance->CCR1 = 0;
				tmp = tmp1;
				#endif

				if(rgb_ch[channel].set_filter_change_delay){
					rgb_ch[channel].set_filter_change_delay--;
					//rgb_ch[channel].set_filter_change_delay = 0;
				}
				else{
					//red
					if(RED_CHK){
						//Red_ct = tmp;
						//freq_R = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_red_ct = tmp;

						#if 0
						dmy = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_red[rgb_ch[channel].red_pos] = dmy;
						rgb_ch[channel].red_pos = (rgb_ch[channel].red_pos + 1) % (sizeof(rgb_ch[channel].freq_red)/ sizeof(rgb_ch[channel].freq_red)[0]);
						if(rgb_ch[channel].red_pos == 0){
							rgb_ch[channel].red_flg = 1;
						}
						rgb_ch[channel].freq_red_ave = dmy;//rgb_ch[channel].freq_red[0];
						#endif

						#if 0
						if(rgb_ch[channel].red_flg){
							pos = &rgb_ch[channel].freq_red[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red[0]));i++) {
								dmy += rgb_ch[channel].freq_red[i];
							}
							rgb_ch[channel].freq_red_ave = dmy / (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red[0]));
						}
						#endif
					}
					//blue
					else if(BLUE_CHK){
						//Blue_ct = tmp;
						//freq_B = clockCouter / tmp;
						rgb_ch[channel].freq_blue_ct = tmp;

						#if 0
						dmy = clockCouter / tmp;
						rgb_ch[channel].freq_blue[rgb_ch[channel].blue_pos] = dmy;
						rgb_ch[channel].blue_pos = (rgb_ch[channel].blue_pos + 1) % (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue)[0]);
						if(rgb_ch[channel].blue_pos == 0){
							rgb_ch[channel].blue_flg = 1;
						}
						rgb_ch[channel].freq_blue_ave = dmy;//rgb_ch[channel].freq_blue[0];
						#endif // 0


						#if 0
						if(rgb_ch[channel].blue_flg){
							pos = &rgb_ch[channel].freq_blue[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue[0]));i++)
							{
								dmy += rgb_ch[channel].freq_blue[i];
							}
							rgb_ch[channel].freq_blue_ave = dmy / (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue[0]));
						}
						#endif
					}
					//no filter
					else if(NOFITER_CHK){
						//nofiter_ct = tmp;
						//freq_nofiter = clockCouter / tmp;
						rgb_ch[channel].freq_nofiter_ct = tmp;

						#if 0
						dmy = clockCouter / tmp;
						rgb_ch[channel].freq_nofiter[rgb_ch[channel].nofiter_pos] = dmy;//clockCouter / tmp;
						rgb_ch[channel].nofiter_pos = (rgb_ch[channel].nofiter_pos + 1) % (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter)[0]);
						if(rgb_ch[channel].nofiter_pos == 0){
							rgb_ch[channel].nofiter_flg = 1;
						}
						//rgb_ch[channel].freq_nofiter_ave = rgb_ch[channel].freq_nofiter[0];
						rgb_ch[channel].freq_nofiter_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].nofiter_flg){
							pos = &rgb_ch[channel].freq_nofiter[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter[0]));i++)
							{
								dmy += rgb_ch[channel].freq_nofiter[i];
							}
							rgb_ch[channel].freq_nofiter_ave = dmy / (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter[0]));
						}
						#endif
					}
					//green 
					else if(GREEN_CHK){
						//Green_ct = tmp;
						//freq_G = clockCouter / tmp;
						rgb_ch[channel].freq_green_ct = tmp;

						#if 0
						dmy = clockCouter / tmp;
						rgb_ch[channel].freq_green[rgb_ch[channel].green_pos] = dmy;
						rgb_ch[channel].green_pos = (rgb_ch[channel].green_pos + 1) % (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green)[0]);
						if(rgb_ch[channel].green_pos == 0){
							rgb_ch[channel].green_flg = 1;
						}
						//rgb_ch[channel].freq_green_ave = rgb_ch[channel].freq_green[0];
						rgb_ch[channel].freq_green_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].green_flg){
							pos = &rgb_ch[channel].freq_green[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green[0]));i++)
							{
								dmy += rgb_ch[channel].freq_green[i];
							}
							rgb_ch[channel].freq_green_ave = dmy / (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green[0]));
						}
						#endif
					}				
				}


				rgb_ch[channel].ccr_bk = tmp1;
			}
			else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
			    flg2 = 1;
			    channel = RGB_CH2;
				tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_2);
				//tmp1 = __HAL_TIM_GetCounter(&htim2);
				
				#if 1
				if(tmp1 >= rgb_ch[channel].ccr_bk){
					tmp = tmp1 - rgb_ch[channel].ccr_bk;
				}
				else{
					//TIMER
					tmp = END_COUTER - rgb_ch[channel].ccr_bk + tmp1 + 1;
				}
				#endif
				#if 0
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,0);
				tmp = tmp1;
				#endif // 1

				if(rgb_ch[channel].set_filter_change_delay){
					rgb_ch[channel].set_filter_change_delay--;
					//rgb_ch[channel].set_filter_change_delay = 0;
				}
				else{
					//red
					if(RED_CHK){
						//Red_ct = tmp;
						//freq_R = clockCouter / tmp;
						rgb_ch[channel].freq_red_ct = tmp;

						#if 0
						dmy = clockCouter / tmp;
						rgb_ch[channel].freq_red[rgb_ch[channel].red_pos] = dmy;//clockCouter / tmp;
						rgb_ch[channel].red_pos = (rgb_ch[channel].red_pos + 1) % (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red)[0]);
						if(rgb_ch[channel].red_pos == 0){
							rgb_ch[channel].red_flg = 1;
						}
						//rgb_ch[channel].freq_red_ave = rgb_ch[channel].freq_red[0];
						rgb_ch[channel].freq_red_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].red_flg){
							pos = &rgb_ch[channel].freq_red[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red[0]));i++){
								dmy += rgb_ch[channel].freq_red[i];
							}
							rgb_ch[channel].freq_red_ave = dmy / (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red[0]));
						}
						#endif
					}
					//blue
					else if(BLUE_CHK){
						//Blue_ct = tmp;
						//freq_B = clockCouter / tmp;
						rgb_ch[channel].freq_blue_ct = tmp;

						#if 0
						dmy = clockCouter / tmp;
						rgb_ch[channel].freq_blue[rgb_ch[channel].blue_pos] = dmy;//clockCouter / tmp;
						rgb_ch[channel].blue_pos = (rgb_ch[channel].blue_pos + 1) % (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue)[0]);
						if(rgb_ch[channel].blue_pos == 0){
							rgb_ch[channel].blue_flg = 1;
						}
						//rgb_ch[channel].freq_blue_ave = rgb_ch[channel].freq_blue[0];
						rgb_ch[channel].freq_blue_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].blue_flg){
							pos = &rgb_ch[channel].freq_blue[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue[0]));i++){
								dmy += rgb_ch[channel].freq_blue[i];
							}
							rgb_ch[channel].freq_blue_ave = dmy / (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue[0]));
						}
						#endif
					}
					//no filter
					else if(NOFITER_CHK){
						//nofiter_ct = tmp;
						//freq_nofiter = clockCouter / tmp;
						rgb_ch[channel].freq_nofiter_ct = tmp;

						#if 0
						dmy = clockCouter / tmp;
						rgb_ch[channel].freq_nofiter[rgb_ch[channel].nofiter_pos] = dmy;//clockCouter / tmp;
						rgb_ch[channel].nofiter_pos = (rgb_ch[channel].nofiter_pos + 1) % (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter)[0]);
						if(rgb_ch[channel].nofiter_pos == 0){
							rgb_ch[channel].nofiter_flg = 1;
						}
						//rgb_ch[channel].freq_nofiter_ave = rgb_ch[channel].freq_nofiter[0];
						rgb_ch[channel].freq_nofiter_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].nofiter_flg){
							pos = &rgb_ch[channel].freq_nofiter[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter[0]));i++){
								dmy += rgb_ch[channel].freq_nofiter[i];
							}
							rgb_ch[channel].freq_nofiter_ave = dmy / (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter[0]));
						}
						#endif
					}
					//green 
					else if(GREEN_CHK){
						//Green_ct = tmp;
						//freq_G = clockCouter / tmp;
						rgb_ch[channel].freq_green_ct = tmp;

						#if 0
						dmy = clockCouter / tmp;
						rgb_ch[channel].freq_green[rgb_ch[channel].green_pos] = dmy;//clockCouter / tmp;
						rgb_ch[channel].green_pos = (rgb_ch[channel].green_pos + 1) % (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green)[0]);
						if(rgb_ch[channel].green_pos == 0){
							rgb_ch[channel].green_flg = 1;
						}
						//rgb_ch[channel].freq_green_ave = rgb_ch[channel].freq_green[0];
						rgb_ch[channel].freq_green_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].green_flg){
							pos = &rgb_ch[channel].freq_green[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green[0]));i++){
								dmy += rgb_ch[channel].freq_green[i];
							}
							rgb_ch[channel].freq_green_ave = dmy / (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green[0]));
						}
						#endif
					}
				}


				rgb_ch[channel].ccr_bk = tmp1;
			}
			else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
			    flg2 = 1;
				channel = RGB_CH3;
				tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_3);
				//tmp1 = __HAL_TIM_GetCounter(&htim2);
				#if 1
				if(tmp1 >= rgb_ch[channel].ccr_bk){
					tmp = tmp1 - rgb_ch[channel].ccr_bk;
				}
				else{
					tmp = END_COUTER - rgb_ch[channel].ccr_bk + tmp1 + 1;
				}
				#endif
				#if 0
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,0);
				tmp = tmp1;
				#endif // 1

				if(rgb_ch[channel].set_filter_change_delay){
					rgb_ch[channel].set_filter_change_delay--;
					//rgb_ch[channel].set_filter_change_delay = 0;
				}
				else{
					//red
					if(RED_CHK){
						//Red_ct = tmp;
						//freq_R = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_red_ct = tmp;

						#if 0
						dmy = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_red[rgb_ch[channel].red_pos] = dmy;//SYSTEM_CLOCK / tmp;
						rgb_ch[channel].red_pos = (rgb_ch[channel].red_pos + 1) % (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red)[0]);
						if(rgb_ch[channel].red_pos == 0){
							rgb_ch[channel].red_flg = 1;
						}
						//rgb_ch[channel].freq_red_ave = rgb_ch[channel].freq_red[0];
						rgb_ch[channel].freq_red_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].red_flg){
							pos = &rgb_ch[channel].freq_red[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red[0]));i++){
								dmy += rgb_ch[channel].freq_red[i];
							}
							rgb_ch[channel].freq_red_ave = dmy / (sizeof(rgb_ch[channel].freq_red) / sizeof(rgb_ch[channel].freq_red[0]));
						}
						#endif
					}
					//blue
					else if(BLUE_CHK){
						//Blue_ct = tmp;
						//freq_B = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_blue_ct = tmp;
						#if 0

						dmy = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_blue[rgb_ch[channel].blue_pos] = dmy;//clockCouter / tmp;
						rgb_ch[channel].blue_pos = (rgb_ch[channel].blue_pos + 1) % (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue)[0]);
						if(rgb_ch[channel].blue_pos == 0){
							rgb_ch[channel].blue_flg = 1;
						}
						//rgb_ch[channel].freq_blue_ave = rgb_ch[channel].freq_blue[0];
						rgb_ch[channel].freq_blue_ave = dmy;
						#endif // 0


						#if 0
						if(rgb_ch[channel].blue_flg){
							pos = &rgb_ch[channel].freq_blue[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue[0]));i++){
								dmy += rgb_ch[channel].freq_blue[i];
							}
							rgb_ch[channel].freq_blue_ave = dmy / (sizeof(rgb_ch[channel].freq_blue) / sizeof(rgb_ch[channel].freq_blue[0]));
						}
						#endif
					}
					//no filter
					else if(NOFITER_CHK){
						//nofiter_ct = tmp;
						//freq_nofiter = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_nofiter_ct = tmp;

						#if 0
						dmy = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_nofiter[rgb_ch[channel].nofiter_pos] = dmy;//SYSTEM_CLOCK / tmp;
						rgb_ch[channel].nofiter_pos = (rgb_ch[channel].nofiter_pos + 1) % (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter)[0]);
						if(rgb_ch[channel].nofiter_pos == 0){
							rgb_ch[channel].nofiter_flg = 1;
						}
						//rgb_ch[channel].freq_nofiter_ave = rgb_ch[channel].freq_nofiter[0];
						rgb_ch[channel].freq_nofiter_ave = dmy;
						#endif // 0

						#if 0
						if(rgb_ch[channel].nofiter_flg){
							pos = &rgb_ch[channel].freq_nofiter[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter[0]));i++){
								dmy += rgb_ch[channel].freq_nofiter[i];
							}
							rgb_ch[channel].freq_nofiter_ave = dmy / (sizeof(rgb_ch[channel].freq_nofiter) / sizeof(rgb_ch[channel].freq_nofiter[0]));
						}
						#endif
					}
					//green 
					else if(GREEN_CHK){
						//Green_ct = tmp;
						//freq_G = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_green_ct = tmp;
						#if 0
						dmy = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_green[rgb_ch[channel].green_pos] = dmy;//SYSTEM_CLOCK / tmp;
						rgb_ch[channel].green_pos = (rgb_ch[channel].green_pos + 1) % (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green)[0]);
						if(rgb_ch[channel].green_pos == 0){
							rgb_ch[channel].green_flg = 1;
						}
						//rgb_ch[channel].freq_green_ave = rgb_ch[channel].freq_green[0];
						rgb_ch[channel].freq_green_ave = dmy;
						#endif // 0


						#if 0
						if(rgb_ch[channel].green_flg){
							pos = &rgb_ch[channel].freq_green[0];
							dmy = 0;
							for(i = 0;i < (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green[0]));i++){
								dmy += rgb_ch[channel].freq_green[i];
							}
							rgb_ch[channel].freq_green_ave = dmy / (sizeof(rgb_ch[channel].freq_green) / sizeof(rgb_ch[channel].freq_green[0]));
						}						
						#endif

					}
				}


				rgb_ch[channel].ccr_bk = tmp1;
			}
			else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){
			    flg2 = 1;
				channel = RGB_CH4;
				tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_4);
				//tmp1 = __HAL_TIM_GetCounter(&htim2);
				#if 1
				if(tmp1 >= rgb_ch[channel].ccr_bk){
					tmp = tmp1 - rgb_ch[channel].ccr_bk;
				}
				else{
					tmp = END_COUTER - rgb_ch[channel].ccr_bk + tmp1 + 1;
				}
				#endif
				#if 0
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,0);
				tmp = tmp1;
				#endif // 1

				if(rgb_ch[channel].set_filter_change_delay){
					rgb_ch[channel].set_filter_change_delay--;
					//rgb_ch[channel].set_filter_change_delay = 0;
				}
				else{
					
					//red
					if(RED_CHK){
						rgb_ch[channel].freq_red_ct = tmp;
					}
					//blue
					else if(BLUE_CHK){
						rgb_ch[channel].freq_blue_ct = tmp;
					}
					//no filter
					else if(NOFITER_CHK){
						rgb_ch[channel].freq_nofiter_ct = tmp;
					}
					//green 
					else if(GREEN_CHK){
						rgb_ch[channel].freq_green_ct = tmp;
					}
				}


				rgb_ch[channel].ccr_bk = tmp1;
			}
		}
	}
#if 0
	else if(htim->Instance == TIM5){
		if(s0_flg || s1_flg){
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
			flg2 = 1;
			channel = RGB_CH5;
			tmp5 = __HAL_TIM_GetCompare(&htim5,TIM_CHANNEL_1);
			//tmp1 = __HAL_TIM_GetCounter(&htim5);
			#if 1
			if(tmp5 >= rgb_ch[channel].ccr_bk){
				tmp5_ct = tmp5 - rgb_ch[channel].ccr_bk;
			}
			else{
				//TIMER 32BIT
				tmp5_ct = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp5 + 1;
			}
			#endif
			#if 0
			__HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_1,0);
			tmp = tmp1;
			#endif // 1

			if(rgb_ch[channel].set_filter_change_delay){
				rgb_ch[channel].set_filter_change_delay--;
				//rgb_ch[channel].set_filter_change_delay = 0;
			}
			else{
				//red
				if(RED_CHK){
					rgb_ch[channel].freq_red_ct = tmp5_ct;
				}
				//blue
				else if(BLUE_CHK){
					rgb_ch[channel].freq_blue_ct = tmp5_ct;
				}
				//no filter
				else if(NOFITER_CHK){
					rgb_ch[channel].freq_nofiter_ct = tmp5_ct;
				}
				//green 
				else if(GREEN_CHK){
					rgb_ch[channel].freq_green_ct = tmp5_ct;
				}				
			}
			rgb_ch[channel].ccr_bk = tmp5;
		}
	}
#endif
	if(flg2){
		rgb_pwm_flg = 1;
		rgb_pwm_tot = 10;
	}

	//------------------------------------------------------------
}

//For Capture compare 1 event channel 01
void HAL_TIM_IRQHandler_Chn1(TIM_HandleTypeDef *htim)
{
	//unsigned long *pos;
    //unsigned long dmy;
	unsigned long tmp;
	unsigned long tmp1;
	//unsigned char i;
	//unsigned char flg;
	//unsigned char flg2;
	unsigned char channel;

	/* Capture compare 1 event */
	if(__HAL_TIM_GET_FLAG(htim,TIM_FLAG_CC1) != RESET){
		if(__HAL_TIM_GET_IT_SOURCE(htim,TIM_IT_CC1) != RESET){
			{
				__HAL_TIM_CLEAR_IT(htim,TIM_IT_CC1);
				htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

				/* Input capture event */
				if((htim->Instance->CCMR1 & TIM_CCMR1_CC1S) != 0x00U){
					#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
					htim->IC_CaptureCallback(htim);
					#else
					//HAL_TIM_IC_CaptureCallback(htim);

					//flg2 = 1;
					channel = RGB_CH1;
					tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_1);
					//tmp1 = __HAL_TIM_GetCounter(&htim2);

					#if 1
					if(tmp1 >= rgb_ch[channel].ccr_bk){
						tmp = tmp1 - rgb_ch[channel].ccr_bk;
					}
					else{
						//TIMER 32BIT
						tmp = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp1 + 1;
					}
					#endif
					#if 0
					htim->Instance->CCR1 = 0;
					//__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_1,0);
					//htim2.Instance->CCR1 = 0;
					tmp = tmp1;
					#endif

					if(rgb_ch[channel].set_filter_change_delay){
						rgb_ch[channel].set_filter_change_delay--;
						//rgb_ch[channel].set_filter_change_delay = 0;
					}
					else{
						//red
						if(RED_CHK){
							rgb_ch[channel].freq_red_ct = tmp;
						}
						//blue
						else if(BLUE_CHK){
							rgb_ch[channel].freq_blue_ct = tmp;
						}
						//no filter
						else if(NOFITER_CHK){
							rgb_ch[channel].freq_nofiter_ct = tmp;
						}
						//green 
						else if(GREEN_CHK){
							rgb_ch[channel].freq_green_ct = tmp;

						}
					}


					rgb_ch[channel].ccr_bk = tmp1;


					#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
				}
				/* Output compare event */
				else{
					#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
					htim->OC_DelayElapsedCallback(htim);
					htim->PWM_PulseFinishedCallback(htim);
					#else
					HAL_TIM_OC_DelayElapsedCallback(htim);
					HAL_TIM_PWM_PulseFinishedCallback(htim);
					#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
				}
				htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
			}
		}
	}
}

//For Capture compare 1 event channel 02
void HAL_TIM_IRQHandler_Chn2(TIM_HandleTypeDef *htim)
{
	//unsigned long *pos;
    //unsigned long dmy;
	unsigned long tmp;
	unsigned long tmp1;
	//unsigned char i;
	//unsigned char flg;
	//unsigned char flg2;
	unsigned char channel;


	/* Capture compare 2 event */
	if(__HAL_TIM_GET_FLAG(htim,TIM_FLAG_CC2) != RESET){
		if(__HAL_TIM_GET_IT_SOURCE(htim,TIM_IT_CC2) != RESET){
			__HAL_TIM_CLEAR_IT(htim,TIM_IT_CC2);
			htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
			/* Input capture event */
			if((htim->Instance->CCMR1 & TIM_CCMR1_CC2S) != 0x00U){
				#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
				htim->IC_CaptureCallback(htim);
				#else
				//HAL_TIM_IC_CaptureCallback(htim);
				#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

				//flg2 = 1;
				channel = RGB_CH2;
				tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_2);
				//tmp1 = __HAL_TIM_GetCounter(&htim2);

				#if 1
				if(tmp1 >= rgb_ch[channel].ccr_bk){
					tmp = tmp1 - rgb_ch[channel].ccr_bk;
				}
				else{
					//TIMER 32BIT
					tmp = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp1 + 1;
				}
				#endif
				#if 0
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,0);
				tmp = tmp1;
				#endif // 1

				if(rgb_ch[channel].set_filter_change_delay){
					rgb_ch[channel].set_filter_change_delay--;
					//rgb_ch[channel].set_filter_change_delay = 0;
				}
				else{
					//red
					if(RED_CHK){
						//Red_ct = tmp;
						//freq_R = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_red_ct = tmp;
					}
					//blue
					else if(BLUE_CHK){
						//Blue_ct = tmp;
						//freq_B = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_blue_ct = tmp;
						}
					//no filter
					else if(NOFITER_CHK){
						//nofiter_ct = tmp;
						//freq_nofiter = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_nofiter_ct = tmp;
					}
					//green 
					else if(GREEN_CHK){
						//Green_ct = tmp;
						//freq_G = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_green_ct = tmp;
					}
				}


				rgb_ch[channel].ccr_bk = tmp1;

			}
			/* Output compare event */
			else{
				#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
				htim->OC_DelayElapsedCallback(htim);
				htim->PWM_PulseFinishedCallback(htim);
				#else
				HAL_TIM_OC_DelayElapsedCallback(htim);
				HAL_TIM_PWM_PulseFinishedCallback(htim);
				#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
			}
			htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
		}
	}
}

//For Capture compare 1 event channel 03
void HAL_TIM_IRQHandler_Chn3(TIM_HandleTypeDef *htim)
{
	//unsigned long *pos;
    //unsigned long dmy;
	unsigned long tmp;
	unsigned long tmp1;
	//unsigned char i;
	//unsigned char flg;
	//unsigned char flg2;
	unsigned char channel;


	/* Capture compare 3 event */
	if(__HAL_TIM_GET_FLAG(htim,TIM_FLAG_CC3) != RESET){
		if(__HAL_TIM_GET_IT_SOURCE(htim,TIM_IT_CC3) != RESET){
			__HAL_TIM_CLEAR_IT(htim,TIM_IT_CC3);
			htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
			/* Input capture event */
			if((htim->Instance->CCMR2 & TIM_CCMR2_CC3S) != 0x00U){
				#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
				htim->IC_CaptureCallback(htim);
				#else
				//HAL_TIM_IC_CaptureCallback(htim);
				#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

				//flg2 = 1;
				channel = RGB_CH3;
				tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_3);
				//tmp1 = __HAL_TIM_GetCounter(&htim2);
				#if 1
				if(tmp1 >= rgb_ch[channel].ccr_bk){
					tmp = tmp1 - rgb_ch[channel].ccr_bk;
				}
				else{
					//TIMER 32BIT
					tmp = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp1 + 1;
			}
				#endif
				#if 0
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,0);
				tmp = tmp1;
				#endif // 1

				if(rgb_ch[channel].set_filter_change_delay){
					rgb_ch[channel].set_filter_change_delay--;
					//rgb_ch[channel].set_filter_change_delay = 0;
				}
				else{
					//red
					if(RED_CHK){
						//Red_ct = tmp;
						//freq_R = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_red_ct = tmp;
					}
					//blue
					else if(BLUE_CHK){
						//Blue_ct = tmp;
						//freq_B = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_blue_ct = tmp;
				}
					//no filter
					else if(NOFITER_CHK){
						//nofiter_ct = tmp;
						//freq_nofiter = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_nofiter_ct = tmp;
					}
					//green 
					else if(GREEN_CHK){
						//Green_ct = tmp;
						//freq_G = SYSTEM_CLOCK / tmp;
						rgb_ch[channel].freq_green_ct = tmp;
					}
			     }


				rgb_ch[channel].ccr_bk = tmp1;
			}
			/* Output compare event */
			else{
				#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
				htim->OC_DelayElapsedCallback(htim);
				htim->PWM_PulseFinishedCallback(htim);
				#else
				HAL_TIM_OC_DelayElapsedCallback(htim);
				HAL_TIM_PWM_PulseFinishedCallback(htim);
				#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
			}
			htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
		}
	}
}

//For Capture compare 1 event channel 04
void HAL_TIM_IRQHandler_Chn4(TIM_HandleTypeDef *htim)
{
	//unsigned long *pos;
    //unsigned long dmy;
	unsigned long tmp;
	unsigned long tmp1;
	//unsigned char i;
	//unsigned char flg;
	//unsigned char flg2;
	unsigned char channel;

	/* Capture compare 4 event */
	if(__HAL_TIM_GET_FLAG(htim,TIM_FLAG_CC4) != RESET){
		if(__HAL_TIM_GET_IT_SOURCE(htim,TIM_IT_CC4) != RESET){
			__HAL_TIM_CLEAR_IT(htim,TIM_IT_CC4);
			htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
			/* Input capture event */
			if((htim->Instance->CCMR2 & TIM_CCMR2_CC4S) != 0x00U){
				#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
				htim->IC_CaptureCallback(htim);
				#else
				//HAL_TIM_IC_CaptureCallback(htim);
				#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

				//flg2 = 1;
				channel = RGB_CH4;
				tmp1 = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_4);
				//tmp1 = __HAL_TIM_GetCounter(&htim2);
				#if 1
				if(tmp1 >= rgb_ch[channel].ccr_bk){
					tmp = tmp1 - rgb_ch[channel].ccr_bk;
				}
				else{
					//TIMER 32BIT
					tmp = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp1 + 1;
				}
				#endif
				#if 0
				__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_4,0);
				tmp = tmp1;
				#endif // 1

				if(rgb_ch[channel].set_filter_change_delay){
					rgb_ch[channel].set_filter_change_delay--;
					//rgb_ch[channel].set_filter_change_delay = 0;
				}
				else{

					//red
					if(RED_CHK){
						rgb_ch[channel].freq_red_ct = tmp;
					}
					//blue
					else if(BLUE_CHK){
						rgb_ch[channel].freq_blue_ct = tmp;
					}
					//no filter
					else if(NOFITER_CHK){
						rgb_ch[channel].freq_nofiter_ct = tmp;
					}
					//green 
					else if(GREEN_CHK){
						rgb_ch[channel].freq_green_ct = tmp;
					}
					}


				rgb_ch[channel].ccr_bk = tmp1;
			}
			/* Output compare event */
			else{
				#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
				htim->OC_DelayElapsedCallback(htim);
				htim->PWM_PulseFinishedCallback(htim);
				#else
				HAL_TIM_OC_DelayElapsedCallback(htim);
				HAL_TIM_PWM_PulseFinishedCallback(htim);
				#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
			}
			htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
		}
	}
}

//For Capture compare 1 event channel 05
void HAL_TIM_IRQHandler_Chn5(TIM_HandleTypeDef *htim)
{
	//unsigned long *pos;
    //unsigned long dmy;
	unsigned long tmp;
	unsigned long tmp1;
	//unsigned char i;
	//unsigned char flg;
	//unsigned char flg2;
	unsigned char channel;

	/* Capture compare 1 event */
	if(__HAL_TIM_GET_FLAG(htim,TIM_FLAG_CC1) != RESET){
		if(__HAL_TIM_GET_IT_SOURCE(htim,TIM_IT_CC1) != RESET){
			{
				__HAL_TIM_CLEAR_IT(htim,TIM_IT_CC1);
				//htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

				/* Input capture event */
				if((htim->Instance->CCMR1 & TIM_CCMR1_CC1S) != 0x00U){
					#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
					htim->IC_CaptureCallback(htim);
					#else
					//HAL_TIM_IC_CaptureCallback(htim);
					#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */


					//flg2 = 1;
					channel = RGB_CH5;
					tmp1 = __HAL_TIM_GetCompare(&htim5,TIM_CHANNEL_1);
					//tmp1 = __HAL_TIM_GetCounter(&htim5);
					#if 1
					if(tmp1 >= rgb_ch[channel].ccr_bk){
						tmp = tmp1 - rgb_ch[channel].ccr_bk;
					}
					else{
						//TIMER 32BIT
						tmp = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp1 + 1;
					}
					#endif
					#if 0
					__HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_1,0);
					tmp = tmp1;
					#endif // 1

					if(rgb_ch[channel].set_filter_change_delay){
						rgb_ch[channel].set_filter_change_delay--;
						//rgb_ch[channel].set_filter_change_delay = 0;
					}
					else{
						//red
						if(RED_CHK){
							rgb_ch[channel].freq_red_ct = tmp;
						}
						//blue
						else if(BLUE_CHK){
							rgb_ch[channel].freq_blue_ct = tmp;
						}
						//no filter
						else if(NOFITER_CHK){
							rgb_ch[channel].freq_nofiter_ct = tmp;
						}
						//green 
						else if(GREEN_CHK){
							rgb_ch[channel].freq_green_ct = tmp;
						}
					}
					rgb_ch[channel].ccr_bk = tmp1;
				}
				/* Output compare event */
				else{
					#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
					htim->OC_DelayElapsedCallback(htim);
					htim->PWM_PulseFinishedCallback(htim);
					#else
					HAL_TIM_OC_DelayElapsedCallback(htim);
					HAL_TIM_PWM_PulseFinishedCallback(htim);
					#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
				}
				htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
			}
		}
	}
}


// For Capture GRB via interrupt external
#if 0
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	//static unsigned long tmp1;
	//static unsigned long tmp1_ct;
	static unsigned long tmp2;
	static unsigned long tmp2_ct;
	static unsigned long tmp3;
	static unsigned long tmp3_ct;
	static unsigned long tmp4;
	static unsigned long tmp4_ct;
	//static unsigned long tmp5;
	//static unsigned long tmp5_ct;
	unsigned long max_couter = 0xFFFF;
	unsigned char flg2;
	unsigned char channel;
	
	
	flg2 = 0;
	
#if 1
	if(GPIO_Pin == GPIO_PIN_5){
		flg2 = 1;
		channel = RGB_CH2;
		tmp2 = __HAL_TIM_GetCounter(&htim2);

		if(tmp2 >= rgb_ch[channel].ccr_bk){
			tmp2_ct = tmp2 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 
			tmp2_ct = max_couter - rgb_ch[channel].ccr_bk + tmp2;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp2_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp2_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp2_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp2_ct;
			}
		}
		rgb_ch[channel].ccr_bk = tmp2;
	}
	else if(GPIO_Pin == GPIO_PIN_6){
		flg2 = 1;
		channel = RGB_CH3;
		tmp3 = __HAL_TIM_GetCounter(&htim2);

		if(tmp3 >= rgb_ch[channel].ccr_bk){
			tmp3_ct = tmp3 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 
			tmp3_ct = max_couter - rgb_ch[channel].ccr_bk + tmp3;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp3_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp3_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp3_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp3_ct;
			}
		}
		rgb_ch[channel].ccr_bk = tmp3;
	}
	else if(GPIO_Pin == GPIO_PIN_7){
		flg2 = 1;
		channel = RGB_CH4;
		tmp4 = __HAL_TIM_GetCounter(&htim2);

		if(tmp4 >= rgb_ch[channel].ccr_bk){
			tmp4_ct = tmp4 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 
			tmp4_ct = max_couter - rgb_ch[channel].ccr_bk + tmp4;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp4_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp4_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp4_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp4_ct;
			}
		}
		rgb_ch[channel].ccr_bk = tmp4;
	}
#endif	
	
#if 0	
	if(GPIO_Pin == GPIO_PIN_0){
		flg2 = 1;
		channel = RGB_CH1;
		tmp1 = __HAL_TIM_GetCounter(&htim2);

		if(tmp1 >= rgb_ch[channel].ccr_bk){
			tmp1_ct = tmp1 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 32BIT
			tmp1_ct = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp1;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp1_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp1_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp1_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp1_ct;

			}
		}
		rgb_ch[channel].ccr_bk = tmp1;
	}
	else if(GPIO_Pin == GPIO_PIN_1){
		flg2 = 1;
		channel = RGB_CH2;
		tmp2 = __HAL_TIM_GetCounter(&htim2);

		if(tmp2 >= rgb_ch[channel].ccr_bk){
			tmp2_ct = tmp2 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 32BIT
			tmp2_ct = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp2;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp2_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp2_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp2_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp2_ct;
			}
		}
		rgb_ch[channel].ccr_bk = tmp2;
	}
	else if(GPIO_Pin == GPIO_PIN_2){
		flg2 = 1;
		channel = RGB_CH3;
		tmp3 = __HAL_TIM_GetCounter(&htim2);

		if(tmp3 >= rgb_ch[channel].ccr_bk){
			tmp3_ct = tmp3 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 32BIT
			tmp3_ct = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp3;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp3_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp3_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp3_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp3_ct;
			}
		}
		rgb_ch[channel].ccr_bk = tmp3;
	}
	else if(GPIO_Pin == GPIO_PIN_3){
		flg2 = 1;
		channel = RGB_CH4;
		tmp4 = __HAL_TIM_GetCounter(&htim2);

		if(tmp4 >= rgb_ch[channel].ccr_bk){
			tmp4_ct = tmp4 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 32BIT
			tmp4_ct = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp4;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp4_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp4_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp4_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp4_ct;
			}
		}
		rgb_ch[channel].ccr_bk = tmp4;
	}
	else if(GPIO_Pin == GPIO_PIN_10){
		flg2 = 1;
		channel = RGB_CH5;
		tmp5 = __HAL_TIM_GetCounter(&htim2);

		if(tmp5 >= rgb_ch[channel].ccr_bk){
			tmp5_ct = tmp5 - rgb_ch[channel].ccr_bk;
		}
		else{
			//TIMER 32BIT
			tmp5_ct = 0XFFFFFFFF - rgb_ch[channel].ccr_bk + tmp5;
		}

		if(rgb_ch[channel].set_filter_change_delay){
			rgb_ch[channel].set_filter_change_delay--;
		}
		else{
			//red
			if(RED_CHK){
				rgb_ch[channel].freq_red_ct = tmp5_ct;
			}
			//blue
			else if(BLUE_CHK){
				rgb_ch[channel].freq_blue_ct = tmp5_ct;
			}
			//no filter
			else if(NOFITER_CHK){
				rgb_ch[channel].freq_nofiter_ct = tmp5_ct;
			}
			//green 
			else if(GREEN_CHK){
				rgb_ch[channel].freq_green_ct = tmp5_ct;
			}
		}
		rgb_ch[channel].ccr_bk = tmp5;
	}
#endif


	if(flg2){
		rgb_pwm_flg = 1;
		rgb_pwm_tot = 10;
	}
}
#endif

#if 1
// For Capture GRB via interrupt external
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	unsigned char flg2;

	flg2 = 0;
	if(GPIO_Pin == GPIO_PIN_5){
		get_ct_channel_irq(&htim2,RGB_CH2);
		flg2 = 1;
	}

	if(GPIO_Pin == GPIO_PIN_6){
		get_ct_channel_irq(&htim2,RGB_CH3);
		flg2 = 1;
	}

	if(GPIO_Pin == GPIO_PIN_7){
		get_ct_channel_irq(&htim2,RGB_CH4);
		flg2 = 1;
	}

	if(flg2){
		rgb_pwm_flg = 1;
		rgb_pwm_tot = 10;
	}
}
#endif


// Get Couter RGB
void get_ct_channel_comp(TIM_HandleTypeDef *htim,unsigned char tim_channel,unsigned char rgb_channel)
{
	//chn_ct[rgb_channel] = __HAL_TIM_GetCompare(&htim2,TIM_CHANNEL_1);
	rgb_chn_ct[rgb_channel] = __HAL_TIM_GetCompare(htim,tim_channel);

	if(rgb_chn_ct[rgb_channel] >= rgb_ch[rgb_channel].ccr_bk){
		rgb_chn_ct_cal[rgb_channel] = rgb_chn_ct[rgb_channel] - rgb_ch[rgb_channel].ccr_bk;
	}
	else{
		//TIMER 32BIT
		rgb_chn_ct_cal[rgb_channel] = END_COUTER - rgb_ch[rgb_channel].ccr_bk + rgb_chn_ct[rgb_channel] + 1;
	}

	if(rgb_ch[rgb_channel].set_filter_change_delay){
		rgb_ch[rgb_channel].set_filter_change_delay--;
		//rgb_ch[rgb_channel].set_filter_change_delay = 0;
	}
	else{
		//red
		if(RED_CHK){
			rgb_ch[rgb_channel].freq_red_ct = rgb_chn_ct_cal[rgb_channel];

#if 0
			dmy = SYSTEM_CLOCK / tmp;
			rgb_ch[rgb_channel].freq_red[rgb_ch[rgb_channel].red_pos] = dmy;
			rgb_ch[rgb_channel].red_pos = (rgb_ch[rgb_channel].red_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_red) / sizeof(rgb_ch[rgb_channel].freq_red)[0]);
			if(rgb_ch[rgb_channel].red_pos == 0){
				rgb_ch[rgb_channel].red_flg = 1;
			}
			rgb_ch[rgb_channel].freq_red_ave = dmy;//rgb_ch[rgb_channel].freq_red[0];
#endif

#if 0
			if(rgb_ch[rgb_channel].red_flg){
				pos = &rgb_ch[rgb_channel].freq_red[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_red) / sizeof(rgb_ch[rgb_channel].freq_red[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_red[i];
				}
				rgb_ch[rgb_channel].freq_red_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_red) / sizeof(rgb_ch[rgb_channel].freq_red[0]));
			}
#endif
		}
		//blue
		else if(BLUE_CHK){
			rgb_ch[rgb_channel].freq_blue_ct = rgb_chn_ct_cal[rgb_channel];

#if 0
			dmy = SYSTEM_CLOCK / tmp;
			rgb_ch[rgb_channel].freq_blue[rgb_ch[rgb_channel].blue_pos] = dmy;
			rgb_ch[rgb_channel].blue_pos = (rgb_ch[rgb_channel].blue_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_blue) / sizeof(rgb_ch[rgb_channel].freq_blue)[0]);
			if(rgb_ch[rgb_channel].blue_pos == 0){
				rgb_ch[rgb_channel].blue_flg = 1;
			}
			rgb_ch[rgb_channel].freq_blue_ave = dmy;//rgb_ch[rgb_channel].freq_blue[0];
#endif // 0


#if 0
			if(rgb_ch[rgb_channel].blue_flg){
				pos = &rgb_ch[rgb_channel].freq_blue[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_blue) / sizeof(rgb_ch[rgb_channel].freq_blue[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_blue[i];
				}
				rgb_ch[rgb_channel].freq_blue_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_blue) / sizeof(rgb_ch[rgb_channel].freq_blue[0]));
			}
#endif
		}
		//no filter
		else if(NOFITER_CHK){
			rgb_ch[rgb_channel].freq_nofiter_ct = rgb_chn_ct_cal[rgb_channel];

#if 0
			dmy = SYSTEM_CLOCK / tmp;
			rgb_ch[rgb_channel].freq_nofiter[rgb_ch[rgb_channel].nofiter_pos] = dmy;//SYSTEM_CLOCK / tmp;
			rgb_ch[rgb_channel].nofiter_pos = (rgb_ch[rgb_channel].nofiter_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_nofiter) / sizeof(rgb_ch[rgb_channel].freq_nofiter)[0]);
			if(rgb_ch[rgb_channel].nofiter_pos == 0){
				rgb_ch[rgb_channel].nofiter_flg = 1;
			}
			//rgb_ch[rgb_channel].freq_nofiter_ave = rgb_ch[rgb_channel].freq_nofiter[0];
			rgb_ch[rgb_channel].freq_nofiter_ave = dmy;
#endif // 0

#if 0
			if(rgb_ch[rgb_channel].nofiter_flg){
				pos = &rgb_ch[rgb_channel].freq_nofiter[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_nofiter) / sizeof(rgb_ch[rgb_channel].freq_nofiter[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_nofiter[i];
				}
				rgb_ch[rgb_channel].freq_nofiter_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_nofiter) / sizeof(rgb_ch[rgb_channel].freq_nofiter[0]));
			}
#endif
		}
		//green 
		else if(GREEN_CHK){
			rgb_ch[rgb_channel].freq_green_ct = rgb_chn_ct_cal[rgb_channel];

#if 0
			dmy = SYSTEM_CLOCK / tmp;
			rgb_ch[rgb_channel].freq_green[rgb_ch[rgb_channel].green_pos] = dmy;
			rgb_ch[rgb_channel].green_pos = (rgb_ch[rgb_channel].green_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_green) / sizeof(rgb_ch[rgb_channel].freq_green)[0]);
			if(rgb_ch[rgb_channel].green_pos == 0){
				rgb_ch[rgb_channel].green_flg = 1;
			}
			//rgb_ch[rgb_channel].freq_green_ave = rgb_ch[rgb_channel].freq_green[0];
			rgb_ch[rgb_channel].freq_green_ave = dmy;
#endif // 0

#if 0
			if(rgb_ch[rgb_channel].green_flg){
				pos = &rgb_ch[rgb_channel].freq_green[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_green) / sizeof(rgb_ch[rgb_channel].freq_green[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_green[i];
				}
				rgb_ch[rgb_channel].freq_green_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_green) / sizeof(rgb_ch[rgb_channel].freq_green[0]));
			}
#endif
		}
	}

	rgb_ch[rgb_channel].ccr_bk = rgb_chn_ct[rgb_channel];
	rgb_pwm_flg = 1;
	rgb_pwm_tot = 10;
}

// Get Couter RGB
void get_ct_channel_irq(TIM_HandleTypeDef *htim,unsigned char rgb_channel)
{
	unsigned long tmp;
	unsigned long dmy;

	//chn_ct[rgb_channel] = __HAL_TIM_GetCounter(&htim2);
	rgb_chn_ct[rgb_channel] = __HAL_TIM_GetCounter(htim);
	
	if(rgb_chn_ct[rgb_channel] >= rgb_ch[rgb_channel].ccr_bk){
		rgb_chn_ct_cal[rgb_channel] = rgb_chn_ct[rgb_channel] - rgb_ch[rgb_channel].ccr_bk;
	}
	else{
		//TIMER 32BIT
		rgb_chn_ct_cal[rgb_channel] = END_COUTER - rgb_ch[rgb_channel].ccr_bk + rgb_chn_ct[rgb_channel] + 1;
	}

	if(rgb_ch[rgb_channel].set_filter_change_delay){
		rgb_ch[rgb_channel].set_filter_change_delay--;
		//rgb_ch[rgb_channel].set_filter_change_delay = 0;
	}
	else{
		//red
		if(RED_CHK){
			rgb_ch[rgb_channel].freq_red_value_tot = 50;
			rgb_ch[rgb_channel].freq_red_ct = rgb_chn_ct_cal[rgb_channel];

			tmp = ((fact.clk_Prescaler_scan[0] << 8) | fact.clk_Prescaler_scan[1]) + 1;
			dmy = SYSTEM_CLOCK / ((rgb_chn_ct_cal[rgb_channel] + 1) * tmp);
			rgb_ch[rgb_channel].freq_red[rgb_ch[rgb_channel].red_pos] = dmy;
			rgb_ch[rgb_channel].red_pos = (rgb_ch[rgb_channel].red_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_red) / sizeof(rgb_ch[rgb_channel].freq_red)[0]);
			if(rgb_ch[rgb_channel].red_pos == 0){
				rgb_ch[rgb_channel].red_flg = 1;
			}
			rgb_ch[rgb_channel].freq_red_ave = dmy;//rgb_ch[rgb_channel].freq_red[0];

#if 0
			if(rgb_ch[rgb_channel].red_flg){
				pos = &rgb_ch[rgb_channel].freq_red[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_red) / sizeof(rgb_ch[rgb_channel].freq_red[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_red[i];
				}
				rgb_ch[rgb_channel].freq_red_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_red) / sizeof(rgb_ch[rgb_channel].freq_red[0]));
			}
#endif
		}
		//blue
		else if(BLUE_CHK){
			rgb_ch[rgb_channel].freq_blue_value_tot = 50;
			rgb_ch[rgb_channel].freq_blue_ct = rgb_chn_ct_cal[rgb_channel];

#if 1
			tmp = ((fact.clk_Prescaler_scan[0] << 8) | fact.clk_Prescaler_scan[1]) + 1;
			dmy = SYSTEM_CLOCK / ((rgb_chn_ct_cal[rgb_channel] + 1) * tmp);
			rgb_ch[rgb_channel].freq_blue[rgb_ch[rgb_channel].blue_pos] = dmy;
			rgb_ch[rgb_channel].blue_pos = (rgb_ch[rgb_channel].blue_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_blue) / sizeof(rgb_ch[rgb_channel].freq_blue)[0]);
			if(rgb_ch[rgb_channel].blue_pos == 0){
				rgb_ch[rgb_channel].blue_flg = 1;
			}
			rgb_ch[rgb_channel].freq_blue_ave = dmy;//rgb_ch[rgb_channel].freq_blue[0];
#endif // 0


#if 0
			if(rgb_ch[rgb_channel].blue_flg){
				pos = &rgb_ch[rgb_channel].freq_blue[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_blue) / sizeof(rgb_ch[rgb_channel].freq_blue[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_blue[i];
				}
				rgb_ch[rgb_channel].freq_blue_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_blue) / sizeof(rgb_ch[rgb_channel].freq_blue[0]));
			}
#endif
		}
		//no filter
		else if(NOFITER_CHK){
			rgb_ch[rgb_channel].freq_nofiter_value_tot = 50;
			rgb_ch[rgb_channel].freq_nofiter_ct = rgb_chn_ct_cal[rgb_channel];

#if 1
			tmp = ((fact.clk_Prescaler_scan[0] << 8) | fact.clk_Prescaler_scan[1]) + 1;
			dmy = SYSTEM_CLOCK / ((rgb_chn_ct_cal[rgb_channel] + 1) * tmp);
			rgb_ch[rgb_channel].freq_nofiter[rgb_ch[rgb_channel].nofiter_pos] = dmy;//SYSTEM_CLOCK / tmp;
			rgb_ch[rgb_channel].nofiter_pos = (rgb_ch[rgb_channel].nofiter_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_nofiter) / sizeof(rgb_ch[rgb_channel].freq_nofiter)[0]);
			if(rgb_ch[rgb_channel].nofiter_pos == 0){
				rgb_ch[rgb_channel].nofiter_flg = 1;
			}
			//rgb_ch[rgb_channel].freq_nofiter_ave = rgb_ch[rgb_channel].freq_nofiter[0];
			rgb_ch[rgb_channel].freq_nofiter_ave = dmy;
#endif // 0

#if 0
			if(rgb_ch[rgb_channel].nofiter_flg){
				pos = &rgb_ch[rgb_channel].freq_nofiter[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_nofiter) / sizeof(rgb_ch[rgb_channel].freq_nofiter[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_nofiter[i];
				}
				rgb_ch[rgb_channel].freq_nofiter_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_nofiter) / sizeof(rgb_ch[rgb_channel].freq_nofiter[0]));
			}
#endif
		}
		//green 
		else if(GREEN_CHK){
			rgb_ch[rgb_channel].freq_green_value_tot = 50;
			rgb_ch[rgb_channel].freq_green_ct = rgb_chn_ct_cal[rgb_channel];

#if 1
			tmp = ((fact.clk_Prescaler_scan[0] << 8) | fact.clk_Prescaler_scan[1]) + 1;
			dmy = SYSTEM_CLOCK / ((rgb_chn_ct_cal[rgb_channel] + 1) * tmp);
			rgb_ch[rgb_channel].freq_green[rgb_ch[rgb_channel].green_pos] = dmy;
			rgb_ch[rgb_channel].green_pos = (rgb_ch[rgb_channel].green_pos + 1) % (sizeof(rgb_ch[rgb_channel].freq_green) / sizeof(rgb_ch[rgb_channel].freq_green)[0]);
			if(rgb_ch[rgb_channel].green_pos == 0){
				rgb_ch[rgb_channel].green_flg = 1;
			}
			//rgb_ch[rgb_channel].freq_green_ave = rgb_ch[rgb_channel].freq_green[0];
			rgb_ch[rgb_channel].freq_green_ave = dmy;
#endif // 0

#if 0
			if(rgb_ch[rgb_channel].green_flg){
				pos = &rgb_ch[rgb_channel].freq_green[0];
				dmy = 0;
				for(i = 0;i < (sizeof(rgb_ch[rgb_channel].freq_green) / sizeof(rgb_ch[rgb_channel].freq_green[0]));i++){
					dmy += rgb_ch[rgb_channel].freq_green[i];
				}
				rgb_ch[rgb_channel].freq_green_ave = dmy / (sizeof(rgb_ch[rgb_channel].freq_green) / sizeof(rgb_ch[rgb_channel].freq_green[0]));
			}
#endif
		}
	}

	rgb_ch[rgb_channel].ccr_bk = rgb_chn_ct[rgb_channel];
	rgb_pwm_flg = 1;
	rgb_pwm_tot = 10;
}

// check value timeout
void check_rgb_value_tot()
{
	unsigned char channel;
	for(channel = 0;channel < sizeof(rgb_ch) / sizeof(rgb_ch[0]);channel++){
		// Check red
		if(RED_CHK){
			if(rgb_ch[channel].freq_red_value_tot == 0){
				rgb_ch[channel].freq_red_ct = 0;
				rgb_ch[channel].freq_red_ave = 0;
				rgb_ch[channel].red_flg = 0;
				rgb_ch[channel].error_flg = 1;
			}
			else{
				rgb_ch[channel].error_flg = 0;
			}
		}
		else if(GREEN_CHK){
			// check green
			if(rgb_ch[channel].freq_green_value_tot == 0){
				rgb_ch[channel].freq_green_ct = 0;
				rgb_ch[channel].freq_green_ave = 0;
				rgb_ch[channel].green_flg = 0;
				rgb_ch[channel].error_flg = 1;
			}
			else{
				rgb_ch[channel].error_flg = 0;
			}
		}
		else if(BLUE_CHK){
			// check blue
			if(rgb_ch[channel].freq_blue_value_tot == 0){
				rgb_ch[channel].freq_blue_ct = 0;
				rgb_ch[channel].freq_blue_ave = 0;
				rgb_ch[channel].blue_flg = 0;
				rgb_ch[channel].error_flg = 1;
			}
			else{
				rgb_ch[channel].error_flg = 0;
			}
		}
		else if(NOFITER_CHK){
			// check nofiter
			if(rgb_ch[channel].freq_nofiter_value_tot == 0){
				rgb_ch[channel].freq_nofiter_ct = 0;
				rgb_ch[channel].freq_nofiter_ave = 0;
				rgb_ch[channel].nofiter_flg = 0;
				rgb_ch[channel].error_flg = 1;
			}
			else{
				rgb_ch[channel].error_flg = 0;
			}
		}
	}
}
#endif // 1

/*----------------------------------------------------------------*/







/*----------------------------------------------------------------*/
//Other
/*----------------------------------------------------------------*/
#if 1
// comp string
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

//byte 2 ascii
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
								chr = utf8_vi_3byte_tab[j + 1];
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
							chr = utf8_vi_2byte_tab[j + 1];
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

//Compare string dynamic
//ex: str_compare(pos,"SRTING COMPARE")
//Input: pos: địa chỉ chuổi muốn kiểm tra ex: pos = &bluetooth_buff[0];
//Output: 1 nếu có chuổi ký tự trong pos/ 0: nếu không có string trong pos
unsigned char str_compare(unsigned char *str1,const char *str2,unsigned short len)
{
    unsigned char flg;
    unsigned short lenstr1;
    unsigned short lenstr2;
    unsigned short i,j;
    unsigned char same_flg;
    //unsigned char *pos;
    //unsigned long tmp;
    //unsigned char buff[128];

    //Serial.println("--------------------------");
    //Serial.println(sizeof(flg));
    //Serial.println(sizeof(tmp));
    //Serial.println("--------------------------");
    lenstr2 = strlen(str2);

    //lenstr1 = strlenghtV2(str1);

    //lenstr2 = strlenght(str2);


    lenstr1 = len;
    flg = 0;
    //Serial.print("lenstr1: ");
    //Serial.println(lenstr1);
    //Serial.print("lenstr2: ");
    //Serial.println(lenstr2); 

    if(lenstr1 < lenstr2){
        //Serial.println("ERROR");
        goto END_COMPARE;
    }

    //s0_send_buff(str1,lenstr1);
    //s1_send_buff(str2,lenstr2);


    for(i = 0;i < lenstr1 - lenstr2 + 1;i++){
        same_flg = 1;
        for(j = 0;j < lenstr2;j++){
            //Serial.println(*(str1 + i + j));
            //Serial.println(*(str2 + j));
            if((*(str1 + i + j)) ^ *(str2 + j)){
                same_flg = 0;
                break;
            }
        }
        if(same_flg){
            break;
        }
    }

    if(same_flg){
        flg = 1;
    }
    else{
        flg = 0;
    }

END_COMPARE:
    return flg;
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

//CheckSum16
unsigned char check_checksume16(unsigned char *pos,unsigned short len)
{
	unsigned short i;
	unsigned short checksum;
	unsigned short data;

	checksum = 0;
	//*(pos + len - 1) = 0;
	//*(pos + len - 2) = 0;
	data = (*(pos + len - 2) << 8) | *(pos + len - 1);

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
void memcopy(unsigned char *d_pos,unsigned char *s_pos,unsigned short len)
{
	unsigned short i;
	for(i=0;i<len;i++){
		*(d_pos + i) = *(s_pos + i);
	}
}

// check other
unsigned char chk_other(unsigned char* pos,unsigned char* pos2,unsigned short len)
{
	unsigned short i;
	unsigned char flg;
	flg = 0;
	for(i=0;i<len;i++){
		if(*(pos + i) ^ *(pos2 + i)){
			flg = 1;
			break;
		}
	}
	return flg;
}
#endif // 1

/*----------------------------------------------------------------*/
