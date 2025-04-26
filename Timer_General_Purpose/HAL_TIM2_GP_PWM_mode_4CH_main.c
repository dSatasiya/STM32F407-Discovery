/*
 * main.c
 *
 * Created on: April 26, 2025
 * Author: Dhruv Satasiya
 * */


/*
 * This is PWM mode of the General Purpose Timers
 *
 * */
#include "main.h"

/*
 * Global Variables
 * */

/******All Handle Variables Below*************/
GPIO_InitTypeDef hgpiod;

TIM_HandleTypeDef htim2;				/*This is the General Purpose Timer-2*/

UART_HandleTypeDef huart2;

RCC_OscInitTypeDef osc_init;

RCC_ClkInitTypeDef clk_init;

/******All Conventional Variables Below*************/
char msg[100] = {0};

/*
 * Function Prototypes
 * */
void SystemClock_Config(uint32_t);
void TIM2_Inits(void);
void UART_Inits(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config(SYSCLK_FREQ_50MHz);

  /*Configuring the General Purpose timer - TIM2*/
  TIM2_Inits();

  /*Configure the UART2 peripheral*/
  UART_Inits();

  /*Start the timer's different channels in PWM mode*/
#if TIM_GP_PWM_START
  if( HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
  {
	  Error_Handler();
  }
  if( HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2) != HAL_OK)
  {
 	  Error_Handler();
  }
  if( HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3) != HAL_OK)
  {
 	  Error_Handler();
  }
  if( HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_4) != HAL_OK)
  {
 	  Error_Handler();
  }
#endif

  while (1)
  {
	  //Do Nothing
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

void TIM2_Inits(void)
{
	/*
	 * This function includes the high-level GP timer Inits for PWM mode of the timer
	 * 1. TIM_HandleTypeDef
	 * 2. TIM_OC_InitTypeDef
	 * */

	TIM_OC_InitTypeDef timer2PWM_Config;

	//1. Set TIMx init structure members
	htim2.Instance = TIM2;

	/*The base unit of TIM2 is set to run at 1 second of time period*/
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 10000-1;
	htim2.Init.Prescaler = 4999;

	if( HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}

	//2. Configure the OC Channel for PWM settings
	timer2PWM_Config.OCMode = TIM_OCMODE_PWM1;
	timer2PWM_Config.OCPolarity =TIM_OCPOLARITY_HIGH;
	timer2PWM_Config.Pulse = (htim2.Init.Period * 25) / 100;	//25% Duty Cycle
	if( HAL_TIM_PWM_ConfigChannel(&htim2, &timer2PWM_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	timer2PWM_Config.Pulse = (htim2.Init.Period * 45) / 100;	//45% Duty Cycle
	if( HAL_TIM_PWM_ConfigChannel(&htim2, &timer2PWM_Config, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	timer2PWM_Config.Pulse = (htim2.Init.Period * 75) / 100;	//75% Duty Cycle
	if( HAL_TIM_PWM_ConfigChannel(&htim2, &timer2PWM_Config, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}

	timer2PWM_Config.Pulse = (htim2.Init.Period * 95) / 100;	//95% Duty Cycle
	if( HAL_TIM_PWM_ConfigChannel(&htim2, &timer2PWM_Config, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
} //eo TIM2_Inits::

void UART_Inits()
{
	/*High level configurations like setting up the Init structure is done here*/
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;

	if( HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

} //eo UART_Inits::

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

