/*
 * msp.c
 *
 *  Created on: September 22, 2025
 *      Author: Dhruv Satasiya
 */

#include "main.h"

void HAL_MspInit(void)
{
	/*Here we will do low-level processor specific inits.*/

	//1. set up the priority grouping of the ARM Cortex-Mx processor
	/*grouping 4 includes: 4 bits of preemption priority &
	 * 					   0 bits of sub-priority
	 */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exception of the ARM Cortex-Mx Processor
	/*
	 * There are various types of system exceptions
	 * --> MemManage
	 * --> BusFault
	 * --> UsageFault
	 * --> HardFault		(by default enabled)
	 * --> Interrupts(IRQ)
	 * --> NMI (Non Maskable Interrupts)
	 * --> SVCall (Supervisor Call)
	 * --> SysTick (System Timer)
	 * --> Reset
	 */
	SCB->SHCSR |= (0x7 << 16);			//in one short, Mem, Bus & Usage falut enabled



	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

} //eo HAL_MspInit::

