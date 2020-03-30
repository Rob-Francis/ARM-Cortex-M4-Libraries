#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4532390_hal_atimer.h"

//GLOBAL VARIABLES
TIM_HandleTypeDef ATIM_Init;

//Atimer clockspeed
int atimerClkSpeedVal = S4532390_HAL_ATIMER_CLKSPEED;
//Atimer period
int atimerPeriod = S4532390_HAL_ATIMER_PERIOD;
//Counters for ticks and milliseconds
int atimerCounterValue = 0, atimerMsPassed = 0;
//For toggling output
int pinOut = 1;
int countInterrupt = 0;

/**
*@brief Initialiser for the atimer
*@param None
*@retval None
*/
void s4532390_hal_atimer_init() {

	unsigned short PrescalerValue;
    // Timer 3 clock enable
	__TIM3_CLK_ENABLE();

	// Compute the prescaler value
	// Set the clock prescaler to 25kHz
	// SystemCoreClock is the system clock frequency
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / atimerClkSpeedVal) - 1;

	/* TIM Base configuration */
	ATIM_Init.Instance = TIM3;				//Enable Timer 2
  	ATIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000;	
  	ATIM_Init.Init.Prescaler = PrescalerValue;	//Set prescaler value
  	ATIM_Init.Init.ClockDivision = 0;			//Set clock division
	ATIM_Init.Init.RepetitionCounter = 0;	// Set reload Value
  	ATIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* Initialise Timer */
	HAL_TIM_Base_Init(&ATIM_Init);

	/* Set priority of Timer 3 update Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	/* 	DO NOT SET INTERRUPT PRIORITY HIGHER THAN 3 */
	HAL_NVIC_SetPriority(TIM3_IRQn, 10, 0);		//Set Main priority to 10 and sub-priority to 0.

	// Enable the Timer 3 interrupt
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	// Start Timer 3 base unit in interrupt mode
	HAL_TIM_Base_Start_IT(&ATIM_Init);
}

/**
*@brief Returns how many milliseconds have passed since timer start/reset
*@param None
*@retval atimerMsPassed (int) - tracks how many milliseconds passed
*/
int s4532390_hal_atimer_timer_getms() {
	return atimerMsPassed;
}

/**
*@brief Resets the timer counts
*@param None
*@retval None
*/
void s4532390_hal_atimer_timer_reset() {
	atimerCounterValue = 0;
	countInterrupt = 0;
	atimerMsPassed = 0;
}

/**
*@brief Pauses the timer
*@param None
*@retval None
*/
void s4532390_hal_atimer_timer_pause() {
	__TIM3_CLK_DISABLE();
}

/**
*@brief Resumes the timer
*@param None
*@retval None
*/
void s4532390_hal_atimer_timer_resume() {
	__TIM3_CLK_ENABLE();
}

/**
*@brief Sets the clockspeed of the timer
*@param frequency - int of new clockspeed
*@retval None
*/
void s4532390_hal_atimer_clkspeed_set(int frequency) {

	if (frequency > 0) { // Ensures it isn't invalid
		atimerClkSpeedVal = frequency;	
	}
	
	//Recalculates prescaler
	unsigned short PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / atimerClkSpeedVal) - 1;

	ATIM_Init.Init.Prescaler = PrescalerValue;	//Set prescaler value
  	ATIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000; //Recalculates period
	HAL_TIM_Base_Init(&ATIM_Init);	

}

/**
*@brief Sets the period of the clock
*@param period - (int) new period in ms
*@retval None
*/
void s4532390_hal_atimer_period_set(int period) {
	
	if (period > 0) { //Ensures period is valid
		atimerPeriod = period;
	}
	
	//Recalculates period
  	ATIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000;	
	HAL_TIM_Base_Init(&ATIM_Init);	
}

/**
*@brief Initialiser for output pin
*@param None
*@retval None
*/
void s4532390_hal_atimer_init_pin() {
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	S4532390_HAL_ATIMER_PINCLK();

	//Output pin for atimer
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
    GPIO_InitStructure.Pin = S4532390_HAL_ATIMER_PIN;				//Pin
    HAL_GPIO_Init(S4532390_HAL_ATIMER_PINPORT, &GPIO_InitStructure);	//Initialise Pin
}

/**
*@brief Callback function for writing out from atimer pin
*@param None
*@retval None
*/
void atimer_isr_callback() {
	//Writes a value out then toggles
	HAL_GPIO_WritePin(S4532390_HAL_ATIMER_PINPORT, S4532390_HAL_ATIMER_PIN, 0x01 & pinOut);
	pinOut = ~pinOut;
}

/**
*@brief Callback function for timers
*@param htim - pointer to the timer handle
*@retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	//Checks for TIM3
    if (htim->Instance == TIM3) {

        if (countInterrupt >= 2) { //Ticks at half rate of atimer
			
			++atimerCounterValue;
			atimer_isr_callback();
			countInterrupt = 0;

		}
		//Counts ticks and milliseconds
		++countInterrupt;
		atimerMsPassed += atimerPeriod/2;
    }
}

/**
*@brief Timer Handler
*@param None
*@retval None
*/
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&ATIM_Init);
}
