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
extern TIM_HandleTypeDef htim7;

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

} //eo SysTick_Handler::

/*
 * Interrupt IRQ Handlers functions declarations
 * */
void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim7);

} //eo TIM7_IRQHandler::

