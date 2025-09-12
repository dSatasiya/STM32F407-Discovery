/*
 *Basic_Timer_LED_Blink.c
 *
 *  Created on: Aug 31, 2025
 *      Author: Dhruv Satasiya
 * Description:	This application blinks an LED at the interval of every 100ms using Basic Timer
 * 				and GPIos peripheral.
 */

#include "stm32f407xx.h"

/*
 * Global Variables
 * */
TIM_Handle_t htim7;
GPIO_Handle_t hgpiod;

/*
 * Functions Definitions
 * */
void TIM_Init(void);
void Error_Handler(void);
void GPIOs_Init(void);

int main()
{

	/* Configure the GPIOs */
	GPIOs_Init();

	/* Configure the Base Timer - TIM7*/
	TIM_Init();

	/* Enbale the timer by settng the Counter Enable(CEN) bit field*/
	TIM_Base_Start(&htim7);

	while(1)
	{
      /*  Do Nothing  */
	}// indefinite while loop::

	return 0;
} //eo main::

/*
 * @brief	Configures the GPIOs
 * @param	Void
 * @retval	Void
 * */
void GPIOs_Init(void)
{
	hgpiod.pGPIOx = GPIOD;

	hgpiod.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	hgpiod.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	hgpiod.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	hgpiod.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	hgpiod.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&hgpiod);

}

/*
 * @brief	Configures the Base Timer
 * @param	void
 * @retval	void
 * */
void TIM_Init(void)
{
	htim7.pTIMx = TIM7;

	/* 100ms Timer */
	htim7.Init.Period = 64000 - 1;
	htim7.Init.Prescaler = 24;

	if( TIM_Base_Init(&htim7) != STATUS_OK)
	{
		Error_Handler();
	}
}

/*
 * @brief	Performs the callback task after when timer has overflown
 * @param	Address to TIM Handle
 * @retval	None
 * */
void TIM_Base_PeriodElapsedCallback(TIM_Handle_t* htim)
{
	if(htim->pTIMx == TIM7)
	{
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
	}
}

void TIM7_IRQHandler()
{
	TIM_IRQHandler(&htim7);
}

void Error_Handler(void)
{
	while(1)
	{

	}
}
