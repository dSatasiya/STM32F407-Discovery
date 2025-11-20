/*
 * it.c
 *
 *  Created on: 06-April-2025
 *      Author: Dhruv Satasiya
 *
 */

#include "main.h"

//extern TIM_HandleTypeDef htimer2;

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

} //eo SysTick_Handler::

//void TIM2_IRQHandler()
//{
//	HAL_TIM_IRQHandler(&htimer2);
//}
