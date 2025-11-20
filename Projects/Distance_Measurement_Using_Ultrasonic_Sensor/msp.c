/*
 * msp.c
 *
 *  Created on: 31-Oct-2025
 *      Author: Dhruv Satasiya
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
}


/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM2)
  {

    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

//    HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
//    HAL_NVIC_EnableIRQ(TIM2_IRQn);

  }

}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM2)
  {

    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

  }

}
