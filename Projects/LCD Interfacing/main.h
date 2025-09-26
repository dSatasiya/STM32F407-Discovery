/*
 * Created on:	September 22, 2025
 * 	   Author:	Dhruv Satasiya
 * */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/*
 * Some Basic Macros
 * */
#define TRUE						1
#define FALSE						0
#define ON							TRUE
#define OFF							FALSE

/*
 * LCD Pin Macros
 * */
#define LCD_PIN_RS					GPIO_PIN_0
#define LCD_PIN_E					GPIO_PIN_1

#define LCD_PIN_D4					GPIO_PIN_4
#define LCD_PIN_D5					GPIO_PIN_5
#define LCD_PIN_D6					GPIO_PIN_6
#define LCD_PIN_D7					GPIO_PIN_7

#endif /* __MAIN_H */
