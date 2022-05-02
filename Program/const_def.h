/* 
 * File:   const_def.h
 * Author: FI
 *
 * Created on September 27, 2018, 10:46 PM
 */

#if 1
#define VER_H   0X00
#define VER_l   0x06

#define VER_H1   (((VER_H & 0xF0) >> 4) + '0')
#define VER_H2   (((VER_H & 0xF)) + '0')
#define VER_l1   (((VER_l & 0xF0) >> 4) + '0')
#define VER_l2   (((VER_l & 0xF)) + '0')
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
#endif

#if 1//IR CMD
#define IR_CMD_STOP  							'S'
#define IR_CMD_BATTAY  							'B'
#define IR_CMD_RUN   							'R'
#define IR_CMD_OK    							'O'
#define IR_CMD_REFEST							'C'

#define TIMER_RESEND_TO_MODULE_IR               20
#endif

#if 1// stm with pc mini
#define SEND2PC_TM		20
#endif

#if 1//Flash mem
//0x0810 0000
#define FLASH_FACT_BANK_SECTOR  7
#define FLASH_FACT_BANK1        1	
#define FLASH_FACT_BANK2        2	
#define FLASH_FACT_BANK         FLASH_FACT_BANK2		        
//#define FLASH_FACT_BANK_BASE  0x08000000		    
#define FLASH_FACT_BANK_BASE    0x08100000		    

#endif // 1//Flash mem











