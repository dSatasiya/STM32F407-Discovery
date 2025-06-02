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


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/*
 * Some Basic Macros
 * */
#define TRUE						1
#define FALSE						0
#define ON							TRUE
#define OFF							FALSE

#endif /* __MAIN_H */
