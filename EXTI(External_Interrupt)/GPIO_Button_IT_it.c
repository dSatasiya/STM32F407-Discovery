/*
 * it.c
 *
 *  Created on: 07-Apr-2025
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

void EXTI2_IRQHandler(void)
{
//	HAL_EXTI_IRQHandler();

	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);

} //EXTI02_IRQHandler::

