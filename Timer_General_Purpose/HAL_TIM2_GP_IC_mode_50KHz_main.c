/*
 * main.c
 *
 * Created on: April 08, 2025
 * Author: Dhruv Satasiya
 * */

#include "main.h"

/*
 * Global Variables
 * */

/******All Handle Variables Below*************/
GPIO_InitTypeDef hgpiod;

TIM_HandleTypeDef htim7;				/*This is the Basic Timer-7*/

TIM_HandleTypeDef htim2;				/*This is the General Purpose Timer-2*/

UART_HandleTypeDef huart2;

RCC_OscInitTypeDef osc_init;

RCC_ClkInitTypeDef clk_init;

/******All Conventional Variables Below*************/
char msg[100] = "\033[H=========STM32F407-Disc Frequencies=========\r\n";
uint32_t timer_count = 0;
uint32_t input_captures[2] = {0};
uint8_t count = 1;
uint8_t is_capture_done = FALSE;
double user_signal_frq = 0;
/*
 * Function Prototypes
 * */
void SystemClock_Config(uint32_t);
void TIM7_Inits(void);
void TIM2_Inits(void);
void GPIOs_Inits(void);
void UART_Inits(void);
void UART_Send_Data(void);
void MCOPin_Configuration(void);

int main(void)
{
	uint32_t capture_difference = 0;
	double user_signal_time_period = 0;
	double timer2_cnt_res = 0;
	double timer2_cnt_frq = 0;

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config(SYSCLK_FREQ_50MHz);

  /*LSE clock configuration*/
//  MCOPin_Configuration();

  /*Configure the GPIOs*/
  GPIOs_Inits();

  /*Configuring the Base timer - TIM7*/
  TIM7_Inits();

  /*Configuring the General Purpose timer - TIM2*/
  TIM2_Inits();

  /*Configure the UART2 peripheral*/
  UART_Inits();


#if TIM_BASE_START
  /*Start the timer with Interrupt service*/
  if( HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)					/*TIM7 starts counting here*/
  {
	  Error_Handler();
  } //eo HAL_TIM_Base_Start_IT::

#endif

#if TIM_GP_START
  /*Start the general purpose timer for capture event with Interrupt method*/
  if( HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
  {
	  Error_Handler();
  }
#endif


  while (1)
  {
	 if(is_capture_done == TRUE)
	 {
		 if(input_captures[1] > input_captures[0])
		 {
			capture_difference = input_captures[1] - input_captures[0];
		 }
		 else
		 {
			//this is the case when timer overflows between 2 captures.
			capture_difference = ((0xFFFFFFFF) - input_captures[0]) +  input_captures[1];
		 }

		 timer2_cnt_frq = (HAL_RCC_GetPCLK1Freq() * 2) / (htim2.Init.Prescaler + 1);
		 timer2_cnt_res = 1/timer2_cnt_frq;
		 user_signal_time_period = capture_difference * timer2_cnt_res;
		 user_signal_frq = 1 / user_signal_time_period;

		  /*Sending the data over UART Line by invoking the send data function*/
		  UART_Send_Data();

		 is_capture_done = FALSE; 		//resetting the capture done flag
	 }

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

void MCOPin_Configuration(void)
{
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCOSOURCE_HSI, RCC_MCODIV_4); //produces the 4MHz properly but
	//cannot be used as TIM2_IC1 input as it is 0.25us and it is too low to provide sufficient time to IRQHandler of Timer
	//to be executes as it needs at least 5.5us when 50MHz SYSTEM CLOCK is used.

} //eo LSE_Configuration::

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

	hgpiod.Pin = GPIO_PIN_15;			 /*Blue on board LED*/
	HAL_GPIO_Init(GPIOD, &hgpiod);

} //eo GPIOs_Inits::

void TIM7_Inits(void)
{
	//1. Set TIMx init structure members
	htim7.Instance = TIM7;

	/*Preparing the TIM7 for 20us delay*/
	htim7.Init.Prescaler = 9;
	htim7.Init.Period = 50-1;

	if( HAL_TIM_Base_Init(&htim7) != HAL_OK)
	{
		Error_Handler();
	}

} //eo TIM7_Inits::

void TIM2_Inits(void)
{
	/*
	 * This function includes the high-level GP timer Inits for IC mode using 2 structures
	 * 1. TIM_HandleTypeDef &
	 * 2. TIM_IC_InitTypeDef
	 * */

	/*Structure variable for IC channel config.*/
	TIM_IC_InitTypeDef timer2IC_Config;

	//1. Set TIMx init structure members
	htim2.Instance = TIM2;

	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0xFFFFFFFF;				/*this is maximum number of 32-bits value as TIM2 is of 32bits*/
	htim2.Init.Prescaler = 1;					/*That means timer clock will be divided by 2*/

	if( HAL_TIM_IC_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	/*Do the configuration for input channel as input capture mode*/
	timer2IC_Config.ICFilter = 0;
	timer2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;

	if( HAL_TIM_IC_ConfigChannel(&htim2, &timer2IC_Config, TIM_CHANNEL_1) != HAL_OK)
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


/*this was weak attribution of a function, and can be rewritten by the user as per the requirements*/
/*
 * HAL_TIM_PeriodElapsedCallback function declaration
 * */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*Toggeling LED using base Timer's interrupt to get the 50KHz(20us) signal on PD13 pin 
 to provide it to TIM2_IC1(PA0)*/
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13); //Orange on board LED
	/*HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15); //Blue on board LED*/

	/*memset(msg, 0, sizeof(msg));
	sprintf(msg, "\033[HTimer Counts: %d\r\n", timer_count);

    if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK)
    {
	    Error_Handler();
    } //eo UART Transmission

    timer_count++;*/

} //eo HAL_TIM_PeriodElapsedCallback::


/*this was weak attribution of a function, and can be rewritten by the user as per the requirements*/
/*
 * HAL_TIM_IC_CaptureCallback function declaration
 * */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(! is_capture_done)
	{
		if(count == 1)
		{
			input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		}
		else if(count == 2)
		{
			input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			is_capture_done = TRUE;
			count = 1;						//resetting the count

		}
	}

} //eo HAL_TIM_IC_CaptureCallback::


void UART_Send_Data()
{
	/*if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	} //eo UART Transmission*/

	/*
	 * ========================Printing all the frequencies on the Tera Term=======================
	 * */
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "\033[HFrequency of the signal applied = %.2fHz\r\n", user_signal_frq);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "SYSCLK Frequency: %"PRIu32"Hz\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	/*memset(msg, 0, sizeof(msg));
	sprintf(msg, "HCLK Frequency:   %"PRIu32"Hz\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK1 Frequency:  %"PRIu32"Hz\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK2 Frequency:  %"PRIu32"Hz\r\n", HAL_RCC_GetPCLK2Freq());
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);*/

} //eo UART_Send_Data::

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

