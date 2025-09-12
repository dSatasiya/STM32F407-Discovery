/*
 * stm32f407xx_basic_timers_driver.h
 *
 *  Created on: Aug 31, 2025
 *      Author: Dhruv Satasiya
 */

#ifndef INC_STM32F407XX_BASIC_TIMERS_DRIVER_H_
#define INC_STM32F407XX_BASIC_TIMERS_DRIVER_H_

#include "stm32f407xx.h"

/*
 * @brief	Enable the Base TIM peripheral
 * @param	__HANDLE__	TIM Base Handle
 * @retval	None
 * */
#define __TIM_BASE_START(__HANDLE__)				((__HANDLE__)->pTIMx->CR1 |= TIM_CR1_CEN)

/*
 * @brief Base Timer configuration structure definition
 * */
typedef struct
{
	uint32_t Period;			/* Specifies the period value to be loaded into the
	 	 	 	 	 	 	 	   Auto-Reload Register (ARR). This parameter can be a number between
	 	 	 	 	 	 	 	   minimum value = 0x0000 and maximum value = 0xFFFF */
	uint32_t Prescaler;			/* Specifies the prescaler value used to divide the TIM_CLK.
	 	 	 	 	 	 	 	   This parameter can be a number between
	 	 	 	 	 	 	 	   minimum value = 0x0000 and maximum value = 0xFFFF */
}TIM_Base_Init_t;


/*
 * @brief Base Timer handle structure defintion
 * */
typedef struct
{
	TIM_Base_Init_t Init;		/* Holds the TIM base configuration structure object */
	TIM_RegDef_t* pTIMx;		/* Holds the address to specific base timer (TIM6 or TIM7)*/

}TIM_Handle_t;

/*******************************
 * Function Declarations (Blueprints)
 *******************************/
STATUS_Type TIM_Base_Init(TIM_Handle_t* htim);
void TIM_Base_Start(TIM_Handle_t* htim);
void TIM_Base_IRQ_Config(IRQn_Type IRQNumber, uint8_t EnorDi);
void TIM_Base_IRQ_Priority(IRQn_Type IRQNumber, uint32_t IRQPriority);
void TIM_IRQHandler(TIM_Handle_t* htim);
void TIM_Base_PeriodElapsedCallback(TIM_Handle_t* htim);
#endif /* INC_STM32F407XX_BASIC_TIMERS_DRIVER_H_ */
