/*
 * main.c
 *
 * Created on: April 22, 2025
 * Author: Dhruv Satasiya
 * */


/*
 * This is Output Compare Mode of the General Purpose Timers
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
char msg[100] = "\033[H===============================\r\n";
uint32_t pulse1_value = 25000;    //to produce 500Hz
uint32_t pulse2_value = 12500;    //to produce 1000Hz
uint32_t pulse3_value = 6250;	  //to produce 2000Hz
uint32_t pulse4_value = 3125;     //to produce 4000Hz
uint32_t ccr_content = 0;
/*
 * Function Prototypes
 * */
void SystemClock_Config(uint32_t);
void TIM2_Inits(void);
void GPIOs_Inits(void);
void UART_Inits(void);
void UART_Send_Data(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config(SYSCLK_FREQ_50MHz);

  /*Configure the GPIOs*/
  GPIOs_Inits();

  /*Configuring the General Purpose timer - TIM2*/
  TIM2_Inits();

  /*Configure the UART2 peripheral*/
  UART_Inits();

  /*Finally, start the timer for OC with interrupt method*/
#if TIM_GP_OC_START
  if( HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
  {
	  Error_Handler();
  }
  if( HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2) != HAL_OK)
  {
	  Error_Handler();
  }
  if( HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3) != HAL_OK)
  {
	  Error_Handler();
  }
  if( HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4) != HAL_OK)
  {
	  Error_Handler();
  }
#endif

  while (1)
  {
		  /*Sending the data over UART Line by invoking the send data function*/
		  UART_Send_Data();

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

void TIM2_Inits(void)
{
	/*
	 * This function includes the high-level GP timer Inits fof OC mode of the timer
	 * 1. HAL_TIM_OC_Init
	 * 2. HAL_TIM_OC_ConfigChannel
	 * */

	TIM_OC_InitTypeDef timerOC_Init;

	//1. Set TIMx init structure members
	htim2.Instance = TIM2;

	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0xFFFFFFFF;				/*this is maximum number of 32-bits value as TIM2 is of 32bits*/
	htim2.Init.Prescaler = 1;					/*That means timer clock will be divided by 2*/

	if( HAL_TIM_OC_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}

	timerOC_Init.OCMode = TIM_OCMODE_TOGGLE;
	timerOC_Init.OCPolarity = TIM_OCPOLARITY_HIGH;
	//value to be loaded into the CCR1 register.
	//25000 in this case to generate the 500Hz freq. pulse
	timerOC_Init.Pulse = pulse1_value;
	if( HAL_TIM_OC_ConfigChannel(&htim2, &timerOC_Init, TIM_CHANNEL_1) != HAL_OK )
	{
		Error_Handler();
	}

	timerOC_Init.Pulse = pulse2_value;						//for channel 2
	if( HAL_TIM_OC_ConfigChannel(&htim2, &timerOC_Init, TIM_CHANNEL_2) != HAL_OK )
	{
		Error_Handler();
	}

	timerOC_Init.Pulse = pulse3_value;						//for channel 3
	if( HAL_TIM_OC_ConfigChannel(&htim2, &timerOC_Init, TIM_CHANNEL_3) != HAL_OK )
	{
		Error_Handler();
	}

	timerOC_Init.Pulse = pulse4_value;						//for channel 4
	if( HAL_TIM_OC_ConfigChannel(&htim2, &timerOC_Init, TIM_CHANNEL_4) != HAL_OK )
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
 * HAL_TIM_OC_DelayElapsedCallback function declaration
 * */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	/*TIM2_CH1 toggling with frequency = 500Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (ccr_content + pulse1_value));
	}

	/*TIM2_CH2 toggling with frequency = 1000Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (ccr_content + pulse2_value));
	}

	/*TIM2_CH3 toggling with frequency = 2000Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (ccr_content + pulse3_value));
	}

	/*TIM2_CH4 toggling with frequency = 4000Hz*/
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, (ccr_content + pulse4_value));
	}

} //eo HAL_TIM_OC_DelayElapsedCallback::

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
	sprintf(msg, "SYSCLK Frequency: %"PRIu32"Hz\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

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

