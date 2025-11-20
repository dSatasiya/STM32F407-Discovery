/*
 * main.h
 *
 *  Created on: 31-Oct-2025
 *      Author: Dhruv Satasiya
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
#define ULTRA_SENSOR_TRIG   GPIO_PIN_1
#define ULTRA_SENSOR_ECHO   GPIO_PIN_2

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

#endif
