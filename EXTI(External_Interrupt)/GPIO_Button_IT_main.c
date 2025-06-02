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

GPIO_InitTypeDef hgpioa;

//EXTI_ConfigTypeDef hexti2;

/*
 * Function Prototypes
 * */
void SystemClock_Config(void);
void GPIOs_LED_Config(void);
void GPIO_Button_Config(void);
//void EXTI_Config(void);
void delay(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /*Configuration of EXTI Line for button interrupt*/
//  EXTI_Config();

  /*Configure the GPIOs*/
  GPIOs_LED_Config();

  /*Configuring the GPIO Button as an interrupt*/
  GPIO_Button_Config();

  while (1)
  {
	  //Do Nothing....

  } //eo indefinite While::

} //eo main::

void SystemClock_Config(void)
{

} //eo SystemClock_Config::

/*Not required for current application
 *************************************
void EXTI_Config()
{

	EXTI_HandleTypeDef hexti_handle;

	hexti_handle.Line = EXTI_LINE_2;

	hexti2.Line = EXTI_LINE_2;
	hexti2.Mode = EXTI_MODE_INTERRUPT;
	hexti2.Trigger = EXTI_TRIGGER_FALLING;
	hexti2.GPIOSel = EXTI_GPIOA;

	if( HAL_EXTI_SetConfigLine(&hexti_handle, &hexti2) != HAL_OK)
	{
		Error_Handler();
	}

} //eo EXTI_Config::
*/
void GPIOs_LED_Config(void)
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

	hgpiod.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &hgpiod);

	hgpiod.Pin = GPIO_PIN_15;			 /*Red on board LED*/
	HAL_GPIO_Init(GPIOD, &hgpiod);

} //eo GPIOs_LED_Config::

void GPIO_Button_Config(void)
{
	/*=================GPIOA Configurations====================*/
	//1. Enable the clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();

	//2. Set GPIOx init structure members
	hgpioa.Mode = GPIO_MODE_IT_FALLING;
	hgpioa.Pull = GPIO_NOPULL;
	hgpioa.Speed = GPIO_SPEED_FAST;
	hgpioa.Pin = GPIO_PIN_2;

	HAL_GPIO_Init(GPIOA, &hgpioa);

	HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

} //eo GPIO_Button_Config::

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_2)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		delay();
	}

} //eo HAL_GPIO_EXTI_Callback::

void delay()
{
	for(uint32_t index = 0; index < 20000; index++);
} //eo delay::

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


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
