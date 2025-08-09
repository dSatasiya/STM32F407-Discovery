/*************************************
 * @Author Dhruv Satasiya
 * @Date   August 09, 2025
 * @File   stm32f4xx_it.c
 * ***********************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
extern UART_HandleTypeDef huart;
/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();

} //eo SysTick_Handler::

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

void UART4_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart);
}
