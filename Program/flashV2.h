/**
 * @file    flash.h
 * @author  Ferenc Nemeth
 * @date    21 Dec 2018
 * @brief   This module handles the memory related functions.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f1xx_hal.h"

/* Start and end addresses of the user application. */
#define START_ADDR												((uint32_t)0x08000000)	
#define FACT_ADDRESS         							((uint32_t)0x0801FC00)	//Page 127
//#define FLASH_APP_START_ADDRESS 				((uint32_t)0x08008000u)
#define FLASH_APP_START_ADDRESS 					((uint32_t)0x08004000u)
#define FLASH_BOOTLOADER_START_ADDRESS 		((uint32_t)0x08000000u)
#define FLASH_APP_END_ADDRESS   					((uint32_t)FLASH_BANK1_END - 0x10u) /**< Leave a little extra space at the end. */

/* Status report for the functions. */
typedef enum {
  FLASH_OK              = 0x00u, /**< The action was successful. */
  FLASH_ERROR_SIZE      = 0x01u, /**< The binary is too big. */
  FLASH_ERROR_WRITE     = 0x02u, /**< Writing failed. */
  FLASH_ERROR_READBACK  = 0x04u, /**< Writing was successful, but the content of the memory is wrong. */
  FLASH_ERROR           = 0xFFu  /**< Generic error. */
} flash_status;

flash_status flash_erase(uint32_t address);
// Erase bootloader
flash_status flash_erase_bootloader(uint32_t start_addr,uint32_t num_page);

flash_status flash_write(uint32_t address,uint32_t *data,uint32_t length);
// Write flash
unsigned char flash_writeV2(unsigned long address,unsigned char *pos,unsigned short length);
// Read data flash
void flash_read(unsigned long address,unsigned char *pos,unsigned short length);
// Erase block
flash_status flash_eraseV2(unsigned long address);

void flash_jump_to_app(void);
//Jump to bootloader
void flash_jump_to_bootloader(void);
#endif /* FLASH_H_ */

