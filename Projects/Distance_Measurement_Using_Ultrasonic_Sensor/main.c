/*
 * main.c
 *
 * Created on: 	November 06, 2025
 * Author: 		Dhruv Satasiya
 * Descreption: This DIY project is to interface the Ultrasonic Sensor(HC-SR04)
 * 				using STM32 MCU. It also measures the distance of object detected from the
 * 				sensor.
 * */

#include "main.h"

/*
 * Global Variables
 * */
GPIO_InitTypeDef hgpiod;	/* Specifically, for LCD Module */

GPIO_InitTypeDef hgpioc;	/* Specifically, for Ultrasonic Sensor */

TIM_HandleTypeDef htimer2;

char buffer[16];

/*
 * Function Prototypes (Function Declarations)
 * */
void SystemClock_Config(void);
void GPIO_Ultra_Sensor_Init(void);
void TIM_Init(void);
void TIM_us_Delay(uint32_t);			/* To get the microsecond level delay*/
uint32_t Read_Ultrasonic(void);

int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure GPIOD for LCD interfacing */
  GPIO_LCD_Init();

  /* Configuring the GPIOC foe Ultrasonic Sensor(HC-SR04) */
  GPIO_Ultra_Sensor_Init();

  /* Configure the LCD */
  LCD_Init();

  /* Sending data */
  LCD_SendString("HC-SR04 Sensor");

  LCD_SetCursor(1,0);

  LCD_SendString("With STM32");

  /* Timer Initialization for various purposes */
  TIM_Init();

  while (1)
  {
	  uint32_t distance = Read_Ultrasonic();
	  sprintf(buffer, "Distance: %02lucm", distance);


	  if(distance < 100)
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
		  LCD_SetCursor(1, 0);
		  LCD_SendString(buffer);
	  }
	  else
	  {
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		  sprintf(buffer, "Distance: %02lucm", (uint32_t)0);
		  LCD_SetCursor(1, 0);
		  LCD_SendString(buffer);
	  }

	  HAL_Delay(100);		/* 100ms delay */
  } //eo indefinite While::

} //eo main::

void SystemClock_Config(void)
{

} //eo SystemClock_Config::

/**
 * @brief	It initializes the GPIO module pins for Ultrasonic sensor pins
 * @param	None
 * @retval	None
 * */
void GPIO_Ultra_Sensor_Init()
{
	/* Enabling the clock for GPIOC */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* Configuring the TRIG pin */
	hgpioc.Mode = GPIO_MODE_OUTPUT_PP;
	hgpioc.Speed = GPIO_SPEED_FREQ_LOW;
	hgpioc.Pull = GPIO_NOPULL;
	hgpioc.Pin = ULTRA_SENSOR_TRIG;

	HAL_GPIO_Init(GPIOC, &hgpioc);

	/* Configuring the ECHO pin */
	hgpioc.Mode = GPIO_MODE_INPUT;
	hgpioc.Speed = GPIO_SPEED_FREQ_LOW;
	hgpioc.Pull = GPIO_NOPULL;
	hgpioc.Pin = ULTRA_SENSOR_ECHO;

	HAL_GPIO_Init(GPIOC, &hgpioc);

	/* Configuring the LED pin */
	hgpiod.Mode = GPIO_MODE_OUTPUT_PP;
	hgpiod.Speed = GPIO_SPEED_FREQ_LOW;
	hgpiod.Pull = GPIO_NOPULL;
	hgpiod.Pin = GPIO_PIN_14 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;

	HAL_GPIO_Init(GPIOD, &hgpiod);

}

/**
 * @brief	It reads the Ultrasonic sensor and measures the distance between object and
 * 			sensor upon detection.
 * @param	None
 * @retval	uint32_t - distance between sensor and object in CM.
 * */
uint32_t Read_Ultrasonic(void)
{
	/* Setting the Trig pin HIGH for 10us to send the ultrasonic brust */
	 HAL_GPIO_WritePin(GPIOC, ULTRA_SENSOR_TRIG, GPIO_PIN_SET);
	 TIM_us_Delay(10); // Use your microsecond delay function
	 HAL_GPIO_WritePin(GPIOC, ULTRA_SENSOR_TRIG, GPIO_PIN_RESET);

	 /* Frist wait until ECHO pin is low because that means no object is detected */
	 while(HAL_GPIO_ReadPin(GPIOC, ULTRA_SENSOR_ECHO) == GPIO_PIN_RESET);

	 __HAL_TIM_SET_COUNTER(&htimer2, 0); /* Initially setting the Timer count to 0 */

	 /* Start the timer to count */
	 HAL_TIM_Base_Start(&htimer2);

	 /* Wait until ECHO pin goes low, because ECHO pin HIGH means object is being detected */
	 /* Here, simultaneouly TIM_COUNTER increments */
	 while(HAL_GPIO_ReadPin(GPIOC, ULTRA_SENSOR_ECHO) == GPIO_PIN_SET);

	 /* When ECHO goes low, it exits the above while loop and gives the current Timer counter */
	 uint32_t end_time = __HAL_TIM_GET_COUNTER(&htimer2);

	 HAL_TIM_Base_Stop(&htimer2);

	 /* As TIM_COUNTER is set to run at exactly 1 microsencond,
	  * Total counts = Total width of ECHO pulse in microseconds
	  * */
	 uint32_t ECHO_pulse_width = end_time;

	 /* it is equivalent to pulse_width * 0.01715,
	  * calculated using formula, Distance = Speed of Sound (0.034 cm/us) x ECHO_pulse_width */

	 return ECHO_pulse_width / 58;
}

/*
 * @brief  To initialize the timer for basic counter
 * @param  None
 * @retval None
 * */
void TIM_Init()
{
	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 15;		/* HSI is 16MHz hence using prescaler value of 15 to get 1us */
	htimer2.Init.ClockDivision  = TIM_CLOCKDIVISION_DIV1;   /* No Division, Clock Division is used for sampling purpose */

	if( HAL_TIM_Base_Init(&htimer2) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
 * @brief	Generates the desired delay in terms of microseconds.
 * @param	Delay time
 * @retval  None
 * */
void TIM_us_Delay(uint32_t delay)
{
	 /* Set the initial Timer value using TIM Macros */
	 __HAL_TIM_SET_COUNTER(&htimer2, 0);

	 /* Start the timer to count */
	 HAL_TIM_Base_Start(&htimer2);

	 while(__HAL_TIM_GET_COUNTER(&htimer2) < delay );

	 HAL_TIM_Base_Stop(&htimer2);
}

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
 * */
