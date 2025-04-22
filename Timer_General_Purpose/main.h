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
#include <inttypes.h>

/*
 * Macros Definitions
 * */

#define TRUE					1
#define FALSE					0

#define TIM_BASE_INITS			1
#define TIM_BASE_START			1
#define TIM_GP_INITS			1
#define TIM_GP_START			1
#define TIM_GP_OC_START			1
#define UART_INITS				1
#define UART_START				1

/*System Clock Frequency Macors*/
#define SYSCLK_FREQ_50MHz		  	50
#define SYSCLK_FREQ_84MHz			84
#define SYSCLK_FREQ_120MHz			120
#define SYSCLK_FREQ_HSI_DEFAULT		16

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


#endif /* __MAIN_H */
