#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "string.h"

void deleteBuffer(char* data);
void 	Flash_Lock(void);
void 	Flash_Unlock(void);
void 	Flash_Erase(uint32_t addr);
void 	Flash_Write_Int(uint32_t addr, int data);
uint16_t Flash_Read_Int(uint32_t addr);
void 	Flash_Write_Char(uint32_t addr, char* data);
void 	Flash_ReadChar(char* dataOut, uint32_t addr1, uint32_t addr2);
void 	Flash_ProgramPage(char* dataIn, uint32_t addr1, uint32_t addr2);

// Write max len page = 1kbyte = 1024byte
void flash_write_pages_tm32f(unsigned long addr,unsigned char *pos,unsigned short len);

// Write 32bit
void flash_write(unsigned long addr,unsigned short data);

//Read page 
void Flash_Read_page_stm32f(uint32_t addr,unsigned char *pos,unsigned short len);


