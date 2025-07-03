/*
 * msp.c
 *
 *  Created on: 01-Jul-2025
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

/**
  * @brief  Initializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	/*
	 * @note This function is sepcially to provide appropriate clock source to the RTC Clock Mux of
	 * 		 RCC Clock Tree. Usually LSE is used as RTC clock source as it is more precise and accurate,
	 * 		 however, STM32F407 Discovery Board does not have X3(Crystal for LSE) soldered on board.
	 * 		 Hence, LSI is used here as a clock source of RTC.
	 * */
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef RCC_RTCPeriClkInit;

	//Step - 1: Turning ON the LSE
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

	if( HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK )
	{
		Error_Handler();
	}

	//Step - 2: Selecting the LSE as RTCCLK
	RCC_RTCPeriClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	RCC_RTCPeriClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;

	if( HAL_RCCEx_PeriphCLKConfig(&RCC_RTCPeriClkInit) != HAL_OK )
	{
		Error_Handler();
	}

	//step - 3: Enable the RTC Clock
	/*This enables the RTC Backup domain clock of RTC*/
	__HAL_RCC_RTC_ENABLE();

} //eo HAL_RTC_MspInit::
