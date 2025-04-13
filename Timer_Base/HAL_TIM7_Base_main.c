/*
 * main.c
 *
 * Created on: April 07, 2025
 * Author: Dhruv Satasiya
 * */

#include "main.h"

/*
 * Global Variables
 * */
GPIO_InitTypeDef hgpiod;

TIM_HandleTypeDef htim7;

/*
 * Function Prototypes
 * */
void SystemClock_Config(void);
void TIM7_Inits(void);
void GPIOs_Inits(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /*Configure the GPIOs*/
  GPIOs_Inits();

  /*Configuring the Base timer - TIM7*/
  TIM7_Inits();

  /*Start the timer with Interrupt service*/
  if( HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)					/*TIM7 starts counting here*/
  {
	  Error_Handler();
  } //eo HAL_TIM_Base_Start_IT::

  while (1)
  {
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  } //eo indefinite While::

} //eo main::

void SystemClock_Config(void)
{

} //eo SystemClock_Config::

void GPIOs_Inits(void)
{
	/*=================GPIOD Configurations====================*/
	//1. Enable the clock
	__HAL_RCC_GPIOD_CLK_ENABLE();

	//2. Set GPIOx init structure members
	hgpiod.Mode = GPIO_MODE_OUTPUT_PP;
	hgpiod.Pull = GPIO_NOPULL;
	hgpiod.Speed = GPIO_SPEED_FAST;
	hgpiod.Pin = GPIO_PIN_13;		     /*Orange on board LED*/

	HAL_GPIO_Init(GPIOD, &hgpiod);

	hgpiod.Pin = GPIO_PIN_15;			 /*Red on board LED*/
	HAL_GPIO_Init(GPIOD, &hgpiod);

} //eo GPIOs_Inits::

void TIM7_Inits(void)
{
	//1. Set TIMx init structure members
	htim7.Instance = TIM7;

	/*Preparing the TIM7 for 100ms delay*/
	htim7.Init.Prescaler = 24;
	htim7.Init.Period = 64000-1;

	if( HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}

} //eo TIM7_Inits::

/*this was weak attribution of a function, and can be rewritten by the user as per the requirements*/
/*
 * HAL_TIM_PeriodElapsedCallback function declaration
 * */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*Toggeling LED using base Timer's interrupt*/
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);

} //eo HAL_TIM_PeriodElapsedCallback::

void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
} //eo Error_Handler::


/*
 * Extra/Practice Code Below:
 *
 *
 * */
