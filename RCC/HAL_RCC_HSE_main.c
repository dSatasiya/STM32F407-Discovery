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

TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart2;

RCC_OscInitTypeDef osc_init;

RCC_ClkInitTypeDef clk_init;

/******All Conventional Variables Below*************/
char msg[100] = "\033[H=========STM32F407-Disc Frequencies=========\r\n";
uint32_t timer_count = 0;

/*
 * Function Prototypes
 * */
void SystemClock_Config(void);
void UART_Inits(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  
  /*Configure the UART2 peripheral*/
  UART_Inits();


#if TIM_BASE_START
  /*Start the timer with Interrupt service*/
  if( HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)					/*TIM7 starts counting here*/
  {
	  Error_Handler();
  } //eo HAL_TIM_Base_Start_IT::
#endif


  /*Sending the data over UART Line*/
  if( HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK)
  {
	  Error_Handler();
  } //eo UART Transmission

  memset(msg, 0, sizeof(msg));
  sprintf(msg, "\nSYSCLK Frequency: %"PRIu32"Hz\r\n", HAL_RCC_GetSysClockFreq());
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  memset(msg, 0, sizeof(msg));
  sprintf(msg, "HCLK Frequency:   %"PRIu32"Hz\r\n", HAL_RCC_GetHCLKFreq());
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  memset(msg, 0, sizeof(msg));
  sprintf(msg, "PCLK1 Frequency:  %"PRIu32"Hz\r\n", HAL_RCC_GetPCLK1Freq());
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  memset(msg, 0, sizeof(msg));
  sprintf(msg, "PCLK2 Frequency:  %"PRIu32"Hz\r\n", HAL_RCC_GetPCLK2Freq());
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  while (1)
  {
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  } //eo indefinite While::

} //eo main::

void SystemClock_Config(void)
{
	/***************Configuring the System Oscillators*********************/
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
//	osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

	HAL_RCC_OscConfig(&osc_init);

	/***************Configuring the Peripheral Clocks*********************/
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS);

} //eo SystemClock_Config::

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

