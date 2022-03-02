/**
 * @file    flash.c
 * @author  Ferenc Nemeth
 * @date    21 Dec 2018
 * @brief   This module handles the memory related functions.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth
 */

#include "flashV2.h"

/* Function pointer for jumping to user application. */
typedef void (*fnc_ptr)(void);

/**
 * @brief   This function erases the memory.
 * @param   address: First address to be erased (the last is the end of the flash).
 * @return  status: Report about the success of the erasing.
 */
flash_status flash_erase(uint32_t address)
{
  HAL_FLASH_Unlock();

  flash_status status = FLASH_ERROR;
  FLASH_EraseInitTypeDef erase_init;
  uint32_t error = 0u;

  erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
  erase_init.PageAddress = address;
  erase_init.Banks = FLASH_BANK_1;
  /* Calculate the number of pages from "address" and the end of flash. */
  erase_init.NbPages = (FLASH_BANK1_END - address) / FLASH_PAGE_SIZE;
  /* Do the actual erasing. */
  if (HAL_OK == HAL_FLASHEx_Erase(&erase_init, &error))
  {
    status = FLASH_OK;
  }

  HAL_FLASH_Lock();

  return status;
}

// Erase bootloader
flash_status flash_erase_bootloader(uint32_t start_addr,uint32_t num_page)
{
	HAL_FLASH_Unlock();

	flash_status status = FLASH_ERROR;
	FLASH_EraseInitTypeDef erase_init;
	uint32_t error = 0u;

	erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_init.PageAddress = start_addr;
	erase_init.Banks = FLASH_BANK_1;
	/* Calculate the number of pages from "address" and the end of flash. */
	erase_init.NbPages = num_page;
	/* Do the actual erasing. */
	if(HAL_OK == HAL_FLASHEx_Erase(&erase_init,&error)){
		status = FLASH_OK;
	}

	HAL_FLASH_Lock();

	return status;
}


/**
 * @brief   This function flashes the memory.
 * @param   address: First address to be written to.
 * @param   *data:   Array of the data that we want to write.
 * @param   *length: Size of the array.
 * @return  status: Report about the success of the writing.
 */
flash_status flash_write(uint32_t address, uint32_t *data, uint32_t length)
{
  flash_status status = FLASH_OK;

  HAL_FLASH_Unlock();

  /* Loop through the array. */
  for (uint32_t i = 0u; (i < length) && (FLASH_OK == status); i++)
  {
    /* If we reached the end of the memory, then report an error and don't do anything else.*/
    if (FLASH_APP_END_ADDRESS <= address)
    {
      status |= FLASH_ERROR_SIZE;
    }
    else
    {
      /* The actual flashing. If there is an error, then report it. */
      if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data[i]))
      {
        status |= FLASH_ERROR_WRITE;
      }
      /* Read back the content of the memory. If it is wrong, then report an error. */
      if (((data[i])) != (*(volatile uint32_t*)address))
      {
        status |= FLASH_ERROR_READBACK;
      }

      /* Shift the address by a word. */
      address += 4u;
    }
  }

  HAL_FLASH_Lock();

  return status;
}

// Write flash
unsigned char flash_writeV2(unsigned long address, unsigned char *pos, unsigned short length)
{
	flash_status status;
	unsigned long addr;
	unsigned short i;
	unsigned long data;
	uint32_t data1;
	unsigned short len_r;
	unsigned char flg;
	
	status = FLASH_OK;
	flg = 1;
	len_r = length / 4;
	addr = address;
	for(i=0;i<len_r;i++){
		data = (unsigned long)*(pos + i * 4 + 0);
		data |= (unsigned long)*(pos + i * 4 + 1) << 8;
		data |= (unsigned long)*(pos + i * 4 + 2) << 16;
		data |= (unsigned long)*(pos + i * 4 + 3) << 24;
		
		data1 = (uint32_t)data;
		status = flash_write(addr,&data1,1);
		if(status != FLASH_OK){
			flg = 0;
			goto END_ERROR;
		}
		addr += 4;
	}
	
END_ERROR:	
	return flg;
}

// Read data flash
void flash_read(unsigned long address, unsigned char *pos, unsigned short length)
{
	unsigned long data;
	unsigned short i;
	unsigned short len_r;
	
	len_r = length / 4;
	
	for(i=0;i<len_r;i++){
		data = *(unsigned long*)(address + i * 4);
		*(pos + i * 4 + 0) = (data & 0xFF);
		*(pos + i * 4 + 1) = (data & 0xFF00) >> 8;
		*(pos + i * 4 + 2) = (data & 0xFF0000) >> 16;
		*(pos + i * 4 + 3) = (data & 0xFF000000) >> 24;
	}	
}

// Erase 1 block
flash_status flash_eraseV2(unsigned long address)
{
  HAL_FLASH_Unlock();

  flash_status status = FLASH_ERROR;
  FLASH_EraseInitTypeDef erase_init;
  uint32_t error = 0u;

  erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
  //erase_init.PageAddress = address;
	erase_init.PageAddress = (uint32_t)address;
  erase_init.Banks = FLASH_BANK_1;
  /* Calculate the number of pages from "address" and the end of flash. */
  //erase_init.NbPages = (FLASH_BANK1_END - address) / FLASH_PAGE_SIZE;
	erase_init.NbPages = 1;
  /* Do the actual erasing. */
  if (HAL_OK == HAL_FLASHEx_Erase(&erase_init, &error))
  {
    status = FLASH_OK;
  }

  HAL_FLASH_Lock();

  return status;
}

/**
 * @brief   Actually jumps to the user application.
 * @param   void
 * @return  void
 */
void flash_jump_to_app(void)
{
  /* Function pointer to the address of the user application. */
  fnc_ptr jump_to_app;
  jump_to_app = (fnc_ptr)(*(volatile uint32_t*) (FLASH_APP_START_ADDRESS+4u));
  HAL_DeInit();
  /* Change the main stack pointer. */
  __set_MSP(*(volatile uint32_t*)FLASH_APP_START_ADDRESS);
  jump_to_app();
}

//Jump to bootloader
void flash_jump_to_bootloader(void)
{
  /* Function pointer to the address of the user application. */
  fnc_ptr jump_to_bootloader;
  jump_to_bootloader = (fnc_ptr)(*(volatile uint32_t*) (FLASH_BOOTLOADER_START_ADDRESS+4u));
  HAL_DeInit();
  /* Change the main stack pointer. */
  __set_MSP(*(volatile uint32_t*)FLASH_BOOTLOADER_START_ADDRESS);
  jump_to_bootloader();
}





