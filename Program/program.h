//#include "stm32h7xx_hal.h"
#include "stm32f1xx_hal.h"

/*----------------------------------------------------------------*/
//Main program
void program(void);
/*----------------------------------------------------------------*/






/*----------------------------INIT data-------------------------------*/
//Init Data
void init_data(void);

//Init fact
void init_fact(void);

//init drive
void init_drive(void);
/*----------------------------------------------------------------*/





/*----------------------------------------------------------------*/
//Couter tm
void timer_couter(void);

//
void opera(void);
/*----------------------------------------------------------------*/





/*----------------------------------------------------------------*/
// drive xe
void drive_xe(void);

// Car Run Down
void car_run_down_sp1(void);

// Car Run UP
void car_run_up_sp1(void);

// Right Up
void right_up(unsigned char duty);

// Right Down
void right_down(unsigned char duty);

// left Up
void left_up(unsigned char duty);

// Left down
void left_down(unsigned char duty);

// Send status to PC
void send_status_car(void);

// Carv stop
void car_stop(void);

// Left stop
void left_stop(void);

// Right stop
void right_stop(void);

// Car init
void car_init(void);

//Thiết lập chạy trường hợp CDVL - CD&PD - CDI&PDN&ADN
void setup_run_th(unsigned char mode);

//PWM drive
void pwm_drive_opera(void);

//Set duty
//duty(0% <-> 100%)
void set_duty(unsigned char duty,unsigned long channel);

//set freqency PWM
void set_freq(unsigned short value);


/*----------------------------------------------------------------*/



/*----------------------------------------------------------------*/
//RASP
//RASP opera
void rasp_opera(void);

//get status rasp
void get_status_rasp(void);
/*----------------------------------------------------------------*/



/*----------------------------------------------------------------*/
//mem flash
void mem_set(void);

// Read fact
unsigned char read_fact(void);

// Write fact
void write_fact(void);

// Read fact & write address
void read_fact_write_address(unsigned char address);

// read fact & read address
void read_fact_read_address(void);
/*----------------------------------------------------------------*/




/*----------------------------------------------------------------*/
//Process data UART2 IR
void serial1_comm(void);

//send data to module IR
void IR_Send(unsigned char addr,unsigned char data);
/*----------------------------------------------------------------*/





/*----------------------------------------------------------------*/
//Communication with due arduino
void uart2_with_due(void);

// Get data flash
unsigned char get_data_flash(unsigned char *pos);

//Vesion
void send_version(void);

//send couter rgb
void send_couter_rgb(unsigned char channel);

//send freq rgb
void send_freq_rgb(unsigned char channel);

//send buff hex convert to ascii
void uart2_send_buff(unsigned char* s_pos,unsigned short len);

//get fact data
unsigned char get_fact(void);

// Send fact
void send_fact(unsigned char addr);

//send tring and hex
void uart2_send_plus(unsigned char* str_pos,unsigned short str_len,unsigned char* hex_pos,unsigned short hex_len);

//uart2 send string
void uart2_send_string(unsigned char* s_pos,unsigned short len);

//send nak/ok
void uart2_send_nak(void);

//send ack/ng
void uart2_send_ack(void);

//uart2 send and address
void u2_send_addr_data_str(char* s_pos,unsigned short len);

//u2 send string
//uart2 send and address
void u2_send_str(char *s_pos,unsigned short len);
/*----------------------------------------------------------------*/








/*----------------------------------------------------------------*/
//with module Bluetooth
void serial3_communication(void);

// get run and get pass addr
unsigned char get_run_pass_addr(void);

// send to pc add module ir
void send_signal_bat_tay_ir(void);

// send to pc add module ir with stop signal
void send_signal_stop_ir(void);

// set send to pc mini
void set_send_pc_must_ok(unsigned char on, unsigned char type, unsigned short tm);

// set stop send to pc mini
void set_stop_send_pc_must_ok(void);

// send model
void u3_send_model(void);

//send buff hex convert to ascii
void u3_send_buff(unsigned char *s_pos, unsigned short len);

//u3 send string
void u3_send_string(unsigned char *s_pos, unsigned short len);

//send nak/ok
void u3_send_nak(void);

//send ack/ng
void u3_send_ack(void);

/*----------------------------------------------------------------*/






/*----------------------------------------------------------------*/
//irq uart1 rx it
unsigned char uart1_rx_it(unsigned char data);

//irq uart2 rx it
unsigned char uart2_rx_it(unsigned char data);

//uart for bluetooth
unsigned char uart3_rx_it(unsigned char data);
/*----------------------------------------------------------------*/





/*----------------------------------------------------------------*/

//scan_rgb_seq
void rgb_opera(void);

//init struct grb
void init_st_rgb_channel(unsigned char channel,unsigned char rgb);

//OUTPUT_FREQUENCY_SCALING
//set scale frequency
void set_scale_freq(unsigned char scale);

//set fiter
void set_fiter(unsigned char fiter);

//For Capture compare 1 event channel 01
void HAL_TIM_IRQHandler_Chn1(TIM_HandleTypeDef *htim);

//For Capture compare 1 event channel 02
void HAL_TIM_IRQHandler_Chn2(TIM_HandleTypeDef *htim);

//For Capture compare 1 event channel 03
void HAL_TIM_IRQHandler_Chn3(TIM_HandleTypeDef *htim);

//For Capture compare 1 event channel 04
void HAL_TIM_IRQHandler_Chn4(TIM_HandleTypeDef *htim);

//For Capture compare 1 event channel 05
void HAL_TIM_IRQHandler_Chn5(TIM_HandleTypeDef *htim);

// Get Couter RGB
void get_ct_channel_comp(TIM_HandleTypeDef *htim,unsigned char tim_channel,unsigned char rgb_channel);

// Get Couter RGB
void get_ct_channel_irq(TIM_HandleTypeDef *htim,unsigned char rgb_channel);

// check value timeout
void check_rgb_value_tot(void);
/*----------------------------------------------------------------*/




/*----------------------------------------------------------------*/
/// so sánh string
unsigned char string_compera(unsigned char* s_pos,char* d_pos);

//1 byte type Bin to 2 byte type ascii
void bin2ascii(unsigned char* s_pos,unsigned char* d_pos,unsigned short len_s_pos);

// bin 2 ascii vesion 2
void bin2ascii_v2(unsigned char* s_pos,unsigned char* d_pos,unsigned short len_s_pos,unsigned short *necxt_pos);

//2 byte ascii to 1byte bin
unsigned char ascii2bin(unsigned char* s_pos,unsigned char* d_pos,unsigned short len_d_pos);

//utf8_to_ascii()
//Input: Data type UFT-8
//Output: ascci không dấu 
/*-------------------------------------------*/
unsigned char utf8_to_ascii(unsigned char *s_pos,unsigned short s_pos_len,unsigned char *d_pos,unsigned short *d_pos_len);


//Support type ascii
void Upper(unsigned char *s_pos,unsigned short s_pos_len);

//Compare string dynamic
//ex: str_compare(pos,"SRTING COMPARE")
//Input: pos: địa chỉ chuổi muốn kiểm tra ex: pos = &bluetooth_buff[0];
//Output: 1 nếu có chuổi ký tự trong pos/ 0: nếu không có string trong pos
unsigned char str_compare(unsigned char *str1,const char *str2,unsigned short len);

//Add CRC16
void add_crc16(unsigned char *pos,unsigned short len);

//Check CRC16
unsigned char check_crc16(unsigned char *pos,unsigned short len);

//Add CheckSum16
void add_checksume16(unsigned char *pos,unsigned short len);

// test crc
unsigned char test_crc16(void);

//CheckSum16
unsigned char check_checksume16(unsigned char *pos,unsigned short len);

// Char to long
unsigned long char2long(unsigned char *pos);

//Mem copy
void memcopy(unsigned char* d_pos,unsigned char* s_pos,unsigned short len);

//Mem set
void mem_set_data(unsigned char* d_pos,unsigned char data,unsigned short len);

// check container
unsigned char chk_container(unsigned char* pos,unsigned char chk,unsigned short len);

// delete value on buff
void delete_value_on_buff(unsigned char* pos,unsigned char chk,unsigned short len);

// check other
unsigned char chk_other(unsigned char* pos,unsigned char* pos2,unsigned short len);
/*----------------------------------------------------------------*/

