/*************************************
 * @Author Dhruv Satasiya
 * @Date   August 09, 2025
 * @File   stm32f4xx_hal_msp.c
 * ***********************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern GPIO_InitTypeDef hgpio;

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

} //eo HAL_MspInit::

/**
  * @brief  UART MSP Init.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	/* Enabling the required peripherals clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_UART4_CLK_ENABLE();

	/* Configuring the Low level GPIO setting */
	/* Configuration of PA0 as AF8 to use it as USART4_TX pin */
	hgpio.Mode = GPIO_MODE_AF_PP;
	hgpio.Alternate = GPIO_AF8_UART4;
	hgpio.Pull = GPIO_PULLUP;
	hgpio.Speed = GPIO_SPEED_FREQ_HIGH;
	hgpio.Pin = UART4_TX_Pin;

	HAL_GPIO_Init(UART4_TX_GPIO_Port, &hgpio);

	/* Configuration of PA1 as AF8 to use it as USART4_RX pin */
	hgpio.Pin = UART4_RX_Pin;

	HAL_GPIO_Init(UART4_RX_GPIO_Port, &hgpio);

	/* Configuring the NVIC side setting for interrupts */
	HAL_NVIC_EnableIRQ(UART4_IRQn);
	HAL_NVIC_SetPriority(UART4_IRQn, 15, 0);

} //eo HAL_UART_MspInit::
