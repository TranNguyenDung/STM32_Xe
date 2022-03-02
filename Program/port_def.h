/* 
 * File:   port_def.h
 * Author: FI
 *
 * Created on September 26, 2018, 6:16 AM
 */
 //#include "stm32f10x.h"

//Memory
//#define SCK_LOW   	GPIO_WriteLow(GPIOB,GPIO_Pin_6)
#define SCK_LOW   	GPIOB->ODR &= ~GPIO_PIN_6;
//#define SCK_HIGH   	GPIO_WriteHigh(GPIOB,GPIO_Pin_6)
#define SCK_HIGH   	GPIOB->ODR |= GPIO_PIN_6;

//#define CS_HIGH	 		GPIO_WriteHigh(GPIOB,GPIO_Pin_9)
#define CS_HIGH	 		GPIOB->ODR |= GPIO_PIN_9;
//#define CS_LOW	 		GPIO_WriteLow(GPIOB,GPIO_Pin_9)
#define CS_LOW	 		GPIOB->ODR &= ~GPIO_PIN_9;

//#define SO_INPUT		GPIO_ReadInput(GPIOB,GPIO_Pin_5)
#define SO_INPUT		GPIOB->IDR & GPIO_PIN_5

//#define SI_HIGH 		GPIO_WriteHigh(GPIOB,GPIO_Pin_7)
#define SI_HIGH	 		GPIOB->ODR |= GPIO_PIN_7;
//#define SI_LOW	 		GPIO_WriteLow(GPIOB,GPIO_Pin_7)
#define SI_LOW	 		GPIOB->ODR &= ~GPIO_PIN_7;

//#define CE_HIGH 		GPIO_WriteHigh(GPIOB,GPIO_Pin_8)
#define CE_HIGH	 		GPIOB->ODR |= GPIO_PIN_8;
//#define CE_LOW	 		GPIO_WriteLow(GPIOB,GPIO_Pin_8)
#define CE_LOW	 		GPIOB->ODR &= ~GPIO_PIN_8;

#define CE_STATUS		(GPIOB->ODR & GPIO_PIN_8)

#define SET_HIGH(GPIOx,PIN)		GPIOx->ODR |= PIN
#define SET_LOW(GPIOx,PIN)		GPIOx->ODR &= ~PIN

#define LED_ON 		SET_LOW(GPIOC,GPIO_PIN_13)
#define LED_OFF 	SET_HIGH(GPIOC,GPIO_PIN_13)

#define BUTTON		GPIOB->IDR & GPIO_PIN_12
