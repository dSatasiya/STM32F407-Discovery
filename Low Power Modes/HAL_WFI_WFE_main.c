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

//GPIO_InitTypeDef hgpioa;

GPIO_InitTypeDef hgpioc, hgpioa;

UART_HandleTypeDef huart2;

//EXTI_ConfigTypeDef hexti2;

extern char some_data[];

char msg[25] = {0};

/*
 * Function Prototypes
 * */
void SystemClock_Config(void);
void GPIOs_LED_Config(void);
void GPIO_Init(void);
void UART_Init(void);
void GPIO_AnalogConfig(void);

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
//  GPIOs_LED_Config();

  /*Configuring the GPIO Button as an interrupt*/
  GPIO_Init();

  /*Analog setting for various pins to reduce the power consumption*/
  GPIO_AnalogConfig();

  /*Configuring the UART*/
  UART_Init();

  while (1)
  {

	/*sending some data over UART in while loop in case of WFE instruction*/
	if( HAL_UART_Transmit(&huart2, (uint8_t*)some_data, strlen((char*)some_data), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	sprintf(msg, "Going to Sleep !\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);

	/* Systick is not required so disabled it before going to sleep*/
	HAL_SuspendTick();

	  //going to sleep instead of keep rotating in the infinite while loop
//	  __WFI();
	__WFE();
	  //When interrupt is there(button press in our case), MCU finishes the task and again goes back to sleep.

	  /*Continues from here when MCU wakes up*/
	HAL_ResumeTick();

	sprintf(msg, "Woke Up !\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);

  } //eo indefinite While::

  return 0;

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

void GPIO_Init(void)
{
	/*=================GPIOA Configurations====================*/
	//1. Enable the clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();

	//2. Set GPIOx init structure members
//	hgpioc.Mode = GPIO_MODE_IT_FALLING;         //for interrupt generation on PC13
	hgpioc.Mode = GPIO_MODE_EVT_FALLING;		//for event generation on PC13
	hgpioc.Pull = GPIO_NOPULL;
	hgpioc.Speed = GPIO_SPEED_FAST;
	hgpioc.Pin = GPIO_PIN_13;

	HAL_GPIO_Init(GPIOC, &hgpioc);

	/*
	 * NVIC IRQ setting is required when interrupt is used
	 * NVIC IRQ setting is not required when event is used
	 * */
#if NVIC_IRQ_SETTING
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif
} //eo GPIO_Button_Config::


#if 1
void GPIO_AnalogConfig(void)
{
		/*for GPIOA port*/
		uint32_t gpioa_pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | \
							 GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
							 GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
							 GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
							 GPIO_PIN_14 | GPIO_PIN_15;

		hgpioa.Pin = gpioa_pins;
		hgpioa.Mode = GPIO_MODE_ANALOG;

		HAL_GPIO_Init(GPIOA, &hgpioa);

		/*for GPIOC port*/
		uint32_t gpioc_pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3| GPIO_PIN_4 | \
							 GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
							 GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
							 GPIO_PIN_11 | GPIO_PIN_12 | \
							 GPIO_PIN_14 | GPIO_PIN_15;

		hgpioc.Pin = gpioc_pins;
		hgpioc.Mode = GPIO_MODE_ANALOG;

		HAL_GPIO_Init(GPIOC, &hgpioc);

} //eo GPIO_AnalogConfig::
#endif
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	if(GPIO_Pin == GPIO_PIN_13)
//	{
//		/*sending some data over UART*/
//		if( HAL_UART_Transmit(&huart2, (uint8_t*)some_data, strlen((char*)some_data), HAL_MAX_DELAY) != HAL_OK)
//		{
//			Error_Handler();
//		}
//	}

} //eo HAL_GPIO_EXTI_Callback::

void UART_Init(void)
{
	huart2.Instance = USART2;

	huart2.Init.BaudRate = 115200;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	if( HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

} //eo UART_Init::

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
