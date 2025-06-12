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
extern TIM_HandleTypeDef htim6;

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();

} //eo SysTick_Handler::

/*
 * Interrupt IRQ Handlers functions declarations
 * */
void TIM6_DAC_IRQHandler(void)
{
	/*Remember, we can turn ON and OFF the clock only while UART is communicating, so to save power again*/
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
//	__HAL_RCC_USART2_CLK_ENABLE();
	HAL_TIM_IRQHandler(&htim6);
//	__HAL_RCC_USART2_CLK_DISABLE();
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

} //eo TIM7_IRQHandler::

