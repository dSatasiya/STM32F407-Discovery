/*
 * it.c
 *
 *  Created on: 08-Apr-2025
 *      Author: Dhruv Satasiya
 */


#include "main.h"

/*
 * Global variables
 * */
extern TIM_HandleTypeDef htim2;


void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

} //eo SysTick_Handler::


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);

} //eo TIM2_IRQHandler::
