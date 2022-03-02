/* 
 * File:   const_def.h
 * Author: FI
 *
 * Created on September 27, 2018, 10:46 PM
 */

#if 1
#define VER_H   0X00

#define VER_l   0X06		
// 2021/06.27 Change CMD Check
#endif

#if 1// Advance
#define OK  1
#define NG  0
#define ON  OK
#define OFF  NG
#define SYSTEM_CLOCK	72000000


//WATCH DOG TIMER SET
//#define REFREST_WWDR		HAL_IWDG_Refresh(&hiwdg);
#define REFREST_WWDR		__nop();


//#ifdef RESET_ENA
//MX_IWDG_Init();
//#endif

#endif


#if 1
//FLASH PROGRAM
#define JIMP_APP						0X55
#define JIMP_BOOTLOADER			0XAA
#endif


#if 1//IR CMD
#define IR_CMD_FREE  							'/'
#define IR_CMD_STOP  							'S'
#define IR_CMD_RUN   							'R'
#define IR_CMD_OK    							'O'
#define IR_CMD_TH1_CDVL							'1'
#define IR_CMD_TH2_CDBPD						'2'
#define IR_CMD_TH3_CDIPDNADN					'3'
#define IR_CMD_REFEST							'C'

#define IR_CMD_TH2GAPTHANH			    		'4'
#define IR_CMD_TH2KHONGGAPTHANH			    	'5'
#define IR_CMD_THAN03END			    	    '6'
#define IR_CMD_TH3_THANH01END			        '7'
#define IR_CMD_TH3_DONGTOCEND			        '8'

//FOR CHECK
#define IR_CMD_CHECK				    	    'C'

#define TIMER_RESEND_TO_MODULE_IR                   20
#endif // 1//IR CMD

#if 1//RGB
#define RED_SELECT    0
#define GREEN_SELECT  1
#define BLUE_SELECT   2
#define NOFITER_SELECT   3

//#define RED_CHK    0
//#define GREEN_CHK 1
//#define BLUE_CHK 2
//#define NOFITER_CHK 3
#define RED_CHK      ((s2_flg == 0) && (s3_flg == 0))
#define GREEN_CHK    ((s2_flg == 1) && (s3_flg == 1))
#define BLUE_CHK	 ((s2_flg == 0) && (s3_flg == 1))
#define NOFITER_CHK  ((s2_flg == 1) && (s3_flg == 0))

#define RGB_CH1 0
#define RGB_CH2 1
#define RGB_CH3 2
#define RGB_CH4 3
#define RGB_CH5 4

#define POWER_DOWM 0
#define FREQ_LOW 1
#define FREQ_MID 2
#define FREQ_HIGH 3

#define L  0
#define R  1

#define RBB_STATUS_SEE_HIGH_LEVEL  0
#define RGB_STATUS_FULL_CHIP       1

#define CAR_RUN_UP      0
#define CAR_RUN_DOWN    1
#define CAR_STOP	    2


#endif // 1//RGB

#if 1//Flash mem
//0x0810 0000
#define FLASH_FACT_BANK_SECTOR  7
#define FLASH_FACT_BANK1        1	
#define FLASH_FACT_BANK2        2	
#define FLASH_FACT_BANK         FLASH_FACT_BANK2		        
//#define FLASH_FACT_BANK_BASE    0x08000000		    
#define FLASH_FACT_BANK_BASE    0x08100000		    

#endif // 1//Flash mem

#if 1//Mode RUN LINE
#define CAR_RUN_MERGE_LINE         3

#define CAR_RUN_CDVL         2
#define CAR_RUN_CDBPD        1
#define CAR_RUN_CDIPDNADN    0

#define CAR_RUN_NEXT         0
#define CAR_RUN_BACK         1
#endif // 1//Mode RUN LINE













