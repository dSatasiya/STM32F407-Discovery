/*
 * main.c
 *
 * Created on: July 01, 2025
 * Author: Dhruv Satasiya
 * */

#include "main.h"

/*
 * Global Variables
 * */

/******All Handle Variables Below*************/
GPIO_InitTypeDef hgpiod;

UART_HandleTypeDef huart2;

RCC_OscInitTypeDef osc_init;

RCC_ClkInitTypeDef clk_init;

RTC_HandleTypeDef hrtc;

/******All Conventional Variables Below*************/
char msg[100] = "\033[H\033[?25l*This is RTC Testing*\r\n";

/*
 * Function Prototypes
 * */
void SystemClock_Config(uint32_t);
void GPIOs_Inits(void);
void GPIO_Button_Init(void);
void UART_Inits(void);
void UART_Send_Data(void);
void RTC_Init(void);
void RTC_CalendarConfig(void);
char* getDaysofWeek(uint8_t);
void delay(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config(SYSCLK_FREQ_50MHz);

  /*Configure the GPIOs only for LEDs*/
  GPIOs_Inits();

  /*Configure the GPIOs only for buttons*/
  GPIO_Button_Init();

  /*Configure the UART2 peripheral*/
  UART_Inits();

  /*Sending Initial Message*/
  UART_Send_Data();

  /*Configure the RTC Pripheral Initializations*/
  RTC_Init();

  /*Configure the RTC Calendar*/
  RTC_CalendarConfig();

  while (1)
  {
      //do nothing....
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

void RTC_Init(void)
{
	hrtc.Instance = RTC;

	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 0x7F;  			//This is nothing but the first prescaler = 127
	hrtc.Init.SynchPrediv = 0xFF;				//This is nothing but the second prescaler = 256
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if( HAL_RTC_Init(&hrtc) != HAL_OK )
	{
		Error_Handler();
	}

} //eo RTC_Init::

void RTC_CalendarConfig(void)
{
	RTC_DateTypeDef hrtc_date;

	RTC_TimeTypeDef hrtc_time;

	//This function does the RTC calendar related configurations
	//Let's Configure the calendar for Time: 12:11:10 PM Date: 12 June, 2018, Tuesday

	hrtc_time.TimeFormat = RTC_HOURFORMAT12_PM;
	hrtc_time.Hours = 12;
	hrtc_time.Minutes = 11;
	hrtc_time.Seconds = 10;

	hrtc_date.Date = 12;
	hrtc_date.Month = RTC_MONTH_JUNE;
	hrtc_date.Year = 18;
	hrtc_date.WeekDay = RTC_WEEKDAY_TUESDAY;

	if( HAL_RTC_SetTime(&hrtc, &hrtc_time, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	if( HAL_RTC_SetDate(&hrtc, &hrtc_date, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

} //eo RTC_CalendarConfig::

void GPIOs_Inits(void)
{
	/*=================GPIOD Configurations====================*/
	//1. Enable the clock
	__HAL_RCC_GPIOD_CLK_ENABLE();

	//2. Set GPIOx init structure members
	hgpiod.Mode = GPIO_MODE_OUTPUT_PP;
	hgpiod.Pull = GPIO_NOPULL;
	hgpiod.Speed = GPIO_SPEED_FAST;
	hgpiod.Pin = GPIO_PIN_15;				/*Blue on board LED*/

	HAL_GPIO_Init(GPIOD, &hgpiod);

} //eo GPIOs_Inits::

void GPIO_Button_Init()
{
	/*=================GPIOD Configurations====================*/
	//1. Enable the clock
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SYSCFG_CLK_ENABLE();

	//2. Set GPIOx init structure members
	hgpiod.Mode = GPIO_MODE_IT_FALLING;
	hgpiod.Pull = GPIO_NOPULL;
	hgpiod.Speed = GPIO_SPEED_FAST;
	hgpiod.Pin = GPIO_PIN_6;

	HAL_GPIO_Init(GPIOB, &hgpiod);

	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);

} //eo GPIO_Button_Init::

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

void UART_Send_Data()
{
	if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint8_t)strlen(msg), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}
} //eo UART_Send_Data::

char* getDaysofWeek(uint8_t number)
{
	char* weekdays[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

	return weekdays[number - 1];

} //eo getDaysofWeek::

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/*Checking whether it's entering the EXTI_callback or not*/
//	memset(msg, 0, sizeof(msg));
//	sprintf(msg, "......Entered into Button Callback.....\r\n");
//	if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint8_t)strlen(msg), HAL_MAX_DELAY) != HAL_OK)
//	{
//		Error_Handler();
//	}

	RTC_TimeTypeDef RTC_TimeRead;
	RTC_DateTypeDef RTC_DateRead;

	HAL_RTC_GetTime(&hrtc, &RTC_TimeRead, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateRead, RTC_FORMAT_BIN);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "\033[H\033[?25lCurrent Time is: %02d:%02d:%02d\r\n", RTC_TimeRead.Hours, RTC_TimeRead.Minutes, RTC_TimeRead.Seconds);
	if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint8_t)strlen(msg), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "Current Date is: %2d-%2d-20%2d  <%s>\r\n", RTC_DateRead.Month, RTC_DateRead.Date, RTC_DateRead.Year,
			getDaysofWeek(RTC_DateRead.WeekDay));
	if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, (uint8_t)strlen(msg), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}

} //eo HAL_GPIO_EXTI_Callback::

void delay(void)
{
	for(uint32_t delay_index = 0; delay_index < 5000; delay_index++);
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

