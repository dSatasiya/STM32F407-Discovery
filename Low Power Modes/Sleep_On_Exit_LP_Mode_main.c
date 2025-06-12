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

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

RCC_OscInitTypeDef osc_init;

RCC_ClkInitTypeDef clk_init;


//char msg[50] = "Hello There!!\r\n";
extern uint8_t some_data[];

/*
 * Function Prototypes
 * */
void SystemClock_Config(uint32_t);
void TIM6_Inits(void);
void GPIOs_Inits(void);
void UART_Init(void);
void GPIO_AnalogConfig(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config(SYSCLK_FREQ_HSI_DEFAULT);

  /*Configure the GPIOs*/
  GPIOs_Inits();

  /*Configuring the Base timer - TIM7*/
  TIM6_Inits();

  /*Configuring the UART*/
  UART_Init();

  /*Configuration of unused GPIOx pins as analog pins to reduce the current consumption*/
  GPIO_AnalogConfig();

  //Method - 1
  /*The SLEEPONEXIT feature can be enabled thorugh setting 1st bit of ARM Cortex specific SCR (System Control Register)
   * - refer to the ARM generic reference manual for more information on that
   * */
//  SCB -> SCR |= (1 << 1);

  //Method - 2
  /*This method is to use directly HAL APIs given in hal_pwr.c source file
   * - This is easy method*/
  HAL_PWR_EnableSleepOnExit();


  /*let's start with fresh Status Register of Timer to avoid any conflicts*/
  TIM6->SR = 0;

  /*Start the timer with Interrupt service*/
  if( HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)					/*TIM7 starts counting here*/
  {
	  Error_Handler();
  } //eo HAL_TIM_Base_Start_IT::

  while (1)
  {
//	  if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK)
//	  {
//		  Error_Handler();
//	  }
  } //eo indefinite While::

} //eo main::

void SystemClock_Config(uint32_t SysClk)
{
	uint32_t FLatency = 0;
//	FLatency = FLASH_ACR_LATENCY_0WS;

	/***************Configuring the System Oscillators*********************/
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
//	osc_init.HSEState = RCC_HSE_BYPASS;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	osc_init.PLL.PLLState = RCC_PLL_ON;
//	osc_init.LSEState = RCC_LSE_ON;

	/***************Configuring the Peripheral Clocks*********************/
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;


	switch(SysClk)
	{
		case SYSCLK_FREQ_50MHz:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
//			osc_init.PLL.PLLQ = 2;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_1WS;
			break;

		case SYSCLK_FREQ_84MHz:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
//			osc_init.PLL.PLLQ = 4;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_2WS;
			break;

		case SYSCLK_FREQ_120MHz:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
//			osc_init.PLL.PLLQ = 4;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;
			break;

		case SYSCLK_FREQ_HSI_DEFAULT:
			osc_init.PLL.PLLState = RCC_PLL_OFF;
			osc_init.PLL.PLLSource = RCC_PLL_NONE;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

			FLatency = FLASH_ACR_LATENCY_0WS;
			break;

		default:
			break;
	} //eo switch::

	if( HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_Handler();
	}

	if( HAL_RCC_ClockConfig(&clk_init, FLatency) != HAL_OK)
	{
		Error_Handler();
	}

	/*====================== SYSTICK configuration below============================================*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	/*===========================================================================================*/

} //eo SystemClock_Config::

void GPIO_AnalogConfig(void)
{
		GPIO_InitTypeDef GpioA;

		uint32_t gpio_pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | \
							 GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
							 GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
							 GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
							 GPIO_PIN_14 | GPIO_PIN_15;

		GpioA.Pin = gpio_pins;
		GpioA.Mode = GPIO_MODE_ANALOG;

		HAL_GPIO_Init(GPIOA, &GpioA);

} //eo GPIO_AnalogConfig::


void GPIOs_Inits(void)
{
	/*=================GPIOD Configurations====================*/
	//1. Enable the clock
//	__HAL_RCC_GPIOD_CLK_ENABLE();
	/*
	 * As for the current application we don't need GPIOs, so to save the power we can turn off the clock.
	 * */

	//2. Set GPIOx init structure members
	hgpiod.Mode = GPIO_MODE_OUTPUT_PP;
	hgpiod.Pull = GPIO_NOPULL;
	hgpiod.Speed = GPIO_SPEED_FAST;
	hgpiod.Pin = GPIO_PIN_13;		     /*Orange on board LED*/

	HAL_GPIO_Init(GPIOD, &hgpiod);

	hgpiod.Pin = GPIO_PIN_15;			 /*Red on board LED*/
	HAL_GPIO_Init(GPIOD, &hgpiod);

} //eo GPIOs_Inits::

void TIM6_Inits(void)
{
	//1. Set TIMx init structure members
	htim6.Instance = TIM6;

	/*Preparing the TIM7 for 10ms delay*/
	htim6.Init.Prescaler = 4999;
	htim6.Init.Period = 32 - 1;

	if( HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}

} //eo TIM7_Inits::

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

/*this was weak attribution of a function, and can be rewritten by the user as per the requirements*/
/*
 * HAL_TIM_PeriodElapsedCallback function declaration
 * */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Blinking an LED for just testing purpose
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

	/*sending some data over UART*/
	if( HAL_UART_Transmit(&huart2, (uint8_t*)some_data, strlen((char*)some_data), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

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
