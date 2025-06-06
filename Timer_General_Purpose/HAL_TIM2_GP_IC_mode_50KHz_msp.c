/*
 * msp.c
 *
 *  Created on: 08-Apr-2025
 *      Author: Dhruv Satasiya
 */


#include "main.h"

/*
 * Global Extern Variables
 * */
//extern TIM_HandleTypeDef htim7;
//extern UART_HandleTypeDef huart2;
//extern TIM_HandleTypeDef htim2;


void HAL_MspInit(void)
{
	/*Here we will do low-level processor specific inits.*/

	//1. set up the priority grouping of the ARM Cortex-Mx processor
	/*grouping 4 includes: 4 bits of preemption priority &
	 * 					   0 bits of sub-priority
	 */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exception of the ARM Cortex-Mx Processor
	/*
	 * There are various types of system exceptions
	 * --> MemManage
	 * --> BusFault
	 * --> UsageFault
	 * --> HardFault		(by default enabled)
	 * --> Interrupts(IRQ)
	 * --> NMI (Non Maskable Interrupts)
	 * --> SVCall (Supervisor Call)
	 * --> SysTick (System Timer)
	 * --> Reset
	 */
	SCB->SHCSR |= (0x7 << 16);			//in one short, Mem, Bus & Usage falut enabled



	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);


} //eo HAL_MspInit::

/*this was weak attribution of a function, and can be rewritten by the user as per the requirements*/
/*
 * HAL_TIM_Base_MspInit function declaration
 * */
#if TIM_BASE_INITS
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//1. Enable the clock for TIM7
	__HAL_RCC_TIM7_CLK_ENABLE();

	//2. Do the Cortex(NVIC) level IRQ config.
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_NVIC_SetPriority(TIM7_IRQn, 15, 0);

} //eo HAL_TIM_Base_MspInit::
#endif

/*this was weak attribution of a function, and can be rewritten by the user as per the requirements*/
/*
 * HAL_UART_MspInit function declaration
 * */
#if UART_INITS
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

	GPIO_InitTypeDef gpio_uart;

	/*Low-level initialization of the UART2 is done here*/

	//1. Enable the clock for the USART2 peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. Do the pin muxing configurations

	/*
	 * Configuration of PA2 as AF7 to use it as USART2_TX pin
	 * */
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_uart.Alternate = GPIO_AF7_USART2;						/*Alternate function mode 7 selected*/

	HAL_GPIO_Init(GPIOA, &gpio_uart);

	/*
	 * Configuration of PA3 as AF7 to use it as USART2_RX pin
	 * */
	gpio_uart.Pin = GPIO_PIN_3;

	HAL_GPIO_Init(GPIOA, &gpio_uart);

	//3. Enable the IRQ and set up the priority (NVIC settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

} //eo HAL_UART_MspInit::
#endif


/*this was weak attribution of a function, and can be rewritten by the user as per the requirements*/
/*
 * HAL_TIM_IC_MspInit function declaration
 * */
#if TIM_GP_INITS
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef hgpioa_timer;

	//1. Enable the clock for TIM2
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. set the alternate funtionality for GPIOs to behave as TIM2_CH1
	hgpioa_timer.Alternate	= GPIO_AF1_TIM2;
	hgpioa_timer.Mode = GPIO_MODE_AF_PP;
	hgpioa_timer.Pin = GPIO_PIN_0;

	HAL_GPIO_Init(GPIOA, &hgpioa_timer);

	//3. Do the Cortex(NVIC) level IRQ config.
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);

} //eo HAL_TIM_IC_MspInit::
#endif



