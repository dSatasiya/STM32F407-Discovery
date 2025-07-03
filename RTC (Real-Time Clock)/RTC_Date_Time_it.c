/*
 * it.c
 *
 *  Created on: 01-Jul-2025
 *      Author: Dhruv Satasiya
 */

#include "main.h"

/*
 * Global variables
 * */

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

} //eo SysTick_Handler::

void EXTI9_5_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
} //eo EXTI4_IRQHandler::
