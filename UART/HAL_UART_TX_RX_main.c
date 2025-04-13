/*
 * main.c
 *
 *  Created on: 09-Mar-2025
 *      Author: Dhruv Satasiya
 */

#include "main.h"

/*
 * Global variables
 * */
UART_HandleTypeDef huart2;

char *user_data = "\033[HThe application is running...\r\n";

/*
 * Function prototypes
 * */
void SystemClockConfig(void);
void UART2_Init(void);
void Error_Handler(void);
uint8_t convert_to_capital(uint8_t);

int main()
{
	/*To initialize first the HAL related configurations*/
	HAL_Init();

	/*System clock related configurations*/
	SystemClockConfig();

	/*Peripheral specific configuration, low as well as high level initializations*/
	UART2_Init();

	/*Finally, after all configs., sending data....*/
	uint16_t len_of_data = strlen(user_data);
	if( HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data , HAL_MAX_DELAY) != HAL_OK )
	{
		Error_Handler();
	}


	/*Recption of data*/
	uint8_t rcvd_data;							/*Becuase trying to read only 1 byte first*/
	uint8_t data_buffer[100];					/*buffer to store all the data*/
	uint32_t count = 0;

	while(1)
	{
		HAL_UART_Receive(&huart2, &rcvd_data, 1, HAL_MAX_DELAY);
		//first check for the carriage
		if(rcvd_data == '\r')
		{
			break;
		}
		else
		{
			data_buffer[count++] = convert_to_capital(rcvd_data);
		}

	}

	data_buffer[count++] = '\r';
	HAL_UART_Transmit(&huart2, data_buffer, count , HAL_MAX_DELAY);


	while(1);  //eo indefinite while::

	return 0;
}

/*
 * Functions declarations
 * */
void SystemClockConfig(void)
{

} // eo SystemClockConfig::

void UART2_Init(void)
{
	/*This all is known as high-level initialization of UART2 Peripheral*/
	huart2.Instance = USART2;

	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	//huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	//now pass these all updated conf. data through UART_Init function
	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		//There is problem
		Error_Handler();
	}

} //eo UART_Init::


void Error_Handler(void)
{
	//Trap here because of the error
	//do infinite while or
	// blink LEDs --> RED

	while(1);

} //eo Error_Handler::

uint8_t convert_to_capital(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		//means letter is lower case
		data = data - 32;
	}
	return data;
}// eo convert_to_capital::



/*
 * practice code below========================================================================================================
 */
 /*
  *

	user_data = "\nUpper case conversion of data...\r\n";
	len_of_data = strlen(user_data);

	if( HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data , HAL_MAX_DELAY) != HAL_OK )
	{
		Error_Handler();
	}

 	while(count != 0)
	{
//		char uppercase_letter;

		if(data_buffer[index] >= 97 && data_buffer[index] <= 122)
		{
			//means letter is lower case
			data_buffer[index] = data_buffer[index] - 32;
			index++;

		}
		else
		{
			index++;
		}
		//another method
		//else if(data_buffer[index] < 97 || data_buffer[index] > 122)
		//{
			//means, it is some other letter of it is already upper case
			index++;
		//}
		count--;
	}

	count = index;				//to reset the count to transmit all data

	HAL_UART_Transmit(&huart2, data_buffer, count , HAL_MAX_DELAY);

 * */
