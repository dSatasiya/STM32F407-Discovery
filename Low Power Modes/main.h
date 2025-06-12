/*
 * @Author Dhruv Satasiya
 * */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE				1
#define FLASE				0
#define UART_MSP			TRUE


/*System Clock Frequency Macors*/
#define SYSCLK_FREQ_50MHz		  	50
#define SYSCLK_FREQ_84MHz			84
#define SYSCLK_FREQ_120MHz			120
#define SYSCLK_FREQ_HSI_DEFAULT		16

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


#endif /* __MAIN_H */
