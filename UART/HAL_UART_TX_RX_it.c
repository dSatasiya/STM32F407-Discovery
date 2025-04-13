/*
 * it.c
 *
 *  Created on: 09-Mar-2025
 *      Author: Dhruv Satasiya
 *
 */


void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

} //eo SysTick_Handler::

