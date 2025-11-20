/*
 * lcd.c
 *
 *  Created on: 31-Oct-2025
 *      Author: Dhruv Satasiya
 */

#include "main.h"

/*
 * Global variables
 * */
GPIO_InitTypeDef hgpiod;

/*
 * @brief	The function initializes the GPIO Port D for LCD Interfacing.
 * @param	void
 * @retval	void
 * */
void GPIO_LCD_Init(void)
{
	/* Enable the clock for the whole GPIOD port */
	__HAL_RCC_GPIOD_CLK_ENABLE();

	hgpiod.Mode = GPIO_MODE_OUTPUT_PP;
	hgpiod.Speed = GPIO_SPEED_FREQ_LOW;
	hgpiod.Pin = LCD_PIN_RS | LCD_PIN_E | LCD_PIN_D4 | LCD_PIN_D5 | LCD_PIN_D6 | LCD_PIN_D7 | \
				 INTERRUPT_LED;

	HAL_GPIO_Init(GPIOD, &hgpiod);

}

/*
 * @brief 	Enables the LCD by trigerring the falling edge
 * @param	void
 * @retval	void
 * */
void LCD_Enable(void)
{
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_E, GPIO_PIN_SET);		/* Setting the pin */
	HAL_Delay(1);		/* wait for 1 milisecond */
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_E, GPIO_PIN_RESET);	/* releasing the pin to latch the data */
	HAL_Delay(1);		/* wait for 1 milisecond */
}

/*
 * @brief	Sends the nibble (4bits value) to the data pins of the LCD (D4 to D7)
 * @param	void
 * @retval	void
 * */
void LCD_SendNibble(uint8_t nibble)
{
	/* Logic to toggle D4 to D7 LCD pins to write exact data sent by MCU */
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_D4, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_D5, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_D6, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_D7, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

	/* transit LCD E pin from HIGH to LOW to latch the data received on LCD data pins*/
	LCD_Enable();
}

/*
 * @brief	Sends the corresponding commands to control the LCD
 * @param	cmd - 8bit hex value of a specific command
 * @retval	void
 * */
void LCD_SendCommand(uint8_t cmd)
{
	/* RS -> 0, RW -> already 0, Write to Instruction Register (MCU sends a command like clear,
	 *  home, cursor move, display on/off, etc.)
	 *  */
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_RS, GPIO_PIN_RESET); /* LCD set in Command Mode */
	LCD_SendNibble(cmd >> 4);			/* Upper nibble(4 to 7) sent */
	LCD_SendNibble(cmd & 0x0F);			/* Lower nibble(0 to 3) sent */
	HAL_Delay(2);		/* 2ms delay */
}

void LCD_SendData(uint8_t data_char)
{
	/* RS -> 1, RW -> already 0, Write to Data Register (MCU sends character data
	 * -> goes to DDRAM -> appears on LCD)
	 * */
	HAL_GPIO_WritePin(GPIOD, LCD_PIN_RS, GPIO_PIN_SET); /* LCD set into Data Mode */
	LCD_SendNibble(data_char >> 4);		/* Upper nibble(4 to 7) sent */
	LCD_SendNibble(data_char & 0x0F);	/* Lower nibble(0 to 3) sent */
	HAL_Delay(2);		/* 2ms delay */
}

/*
 * @brief 	Sends an entire string using LCD_SendData function
 * @param	Address to a string literal
 * @retval	void
 * */
void LCD_SendString(char* str)
{
	while(*str)
	{
		LCD_SendData(*str);
		str++;
	}
}

/*
 * @brief	It sets the DDRAM Address to set the exact cursor position.
 * @param	row -> first line or second line?
 * 			column -> horizontal location from 0 to 15
 * @retval	void
 * */
void LCD_SetCursor(uint8_t row, uint8_t column)
{
	uint8_t position;
	position = (row == 0) ? 0x80 + column : 0xC0 + column;
	LCD_SendCommand(position);
}

/*
 * @brief	It clears the display and sets the DDRAM address to 0 in address counter.
 * @param	void.
 * @retval	void.
 * */
void LCD_Clear(void)
{
	/* Clear Display
	 * Intruction register: Clear Display (0x01 is default value to clear the display)
	 * */
	LCD_SendCommand(0x01);
}

/*
 * @brief	It initializes the LCD by calling all helping functions.
 * @param	void
 * @retval 	void
 * */
void LCD_Init(void)
{
	HAL_Delay(50);			/* wait some time (50ms) to let the LCD powered up */

	/*********Refer to the HD44780 16x2 LCD data sheet for all the below commands***********/

	/* 4-bits, 2 lines, 5x7
	 * Intruction register: Function Set to configure the given settings
	 * */
	LCD_SendCommand(0x28);

	/* Display ON, Cursor OFF
	 * Intruction register: Display on/off Control to configure the given settings
	 * */
	LCD_SendCommand(0x0C);

	/* Entry Mode
	 * Intruction register: Entry Mode(I/D ->1(Increment), S -> 0(No display shift))
	 * 						to configure the given settings
	 * */
	LCD_SendCommand(0x06);

	/* Clear Display
	 * Intruction register: Clear Display (0x01 is default value to clear the display)
	 * */
	LCD_SendCommand(0x01);

	HAL_Delay(2);
}
