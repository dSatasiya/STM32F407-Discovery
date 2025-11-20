/*
 * lcd.h
 *
 *  Created on: 31-Oct-2025
 *      Author: Dhruv Satasiya
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

/*
 * LCD Pin Macros
 * */
#define LCD_PIN_RS					GPIO_PIN_0
#define LCD_PIN_E					GPIO_PIN_1

#define LCD_PIN_D4					GPIO_PIN_4
#define LCD_PIN_D5					GPIO_PIN_5
#define LCD_PIN_D6					GPIO_PIN_6
#define LCD_PIN_D7					GPIO_PIN_7

#define INTERRUPT_LED				GPIO_PIN_13

/*
 * Function Prototypes (Function Declarations) for LCD Module.
 * */
void LCD_Enable(void);
void LCD_SendNibble(uint8_t);
void LCD_SendCommand(uint8_t);
void LCD_SendData(uint8_t);
void LCD_SendString(char*);
void LCD_SetCursor(uint8_t, uint8_t);
void LCD_Clear(void);
void LCD_Init(void);
void GPIO_LCD_Init(void);

#endif /* INC_LCD_H_ */
