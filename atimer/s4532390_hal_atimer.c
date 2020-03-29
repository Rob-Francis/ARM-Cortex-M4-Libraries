#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4532390_hal_atimer.h"


TIM_HandleTypeDef ATIM_Init;
int atimerClkSpeedVal = S4532390_HAL_ATIMER_CLKSPEED;
int atimerCounterValue = 0, atimerMsPassed = 0;
int atimerPeriod = S4532390_HAL_ATIMER_PERIOD;
int pinOut = 1;
int countInterrupt = 0;

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

int s4532390_hal_atimer_timer_getms() {
	return atimerMsPassed;
}

void s4532390_hal_atimer_timer_reset() {
	atimerCounterValue = 0;
	countInterrupt = 0;
	atimerMsPassed = 0;
}

void s4532390_hal_atimer_timer_pause() {
	__TIM3_CLK_DISABLE();
}

void s4532390_hal_atimer_timer_resume() {
	__TIM3_CLK_ENABLE();
}

void s4532390_hal_atimer_clkspeed_set(int frequency) {
	
	

	if (frequency > 0) {
		atimerClkSpeedVal = frequency;	
	}

	unsigned short PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / atimerClkSpeedVal) - 1;

	ATIM_Init.Init.Prescaler = PrescalerValue;	//Set prescaler value
  	ATIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000;	
	HAL_TIM_Base_Init(&ATIM_Init);	

}

void s4532390_hal_atimer_period_set(int period) {
	
	if (period > 0) {
		atimerPeriod = period;
	}
	
	ATIM_Init.Instance = TIM3;				//Enable Timer 2
  	ATIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000;	
  	ATIM_Init.Init.ClockDivision = 0;			//Set clock division
	ATIM_Init.Init.RepetitionCounter = 0;	// Set reload Value
  	ATIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.
	HAL_TIM_Base_Init(&ATIM_Init);	
}



void s4532390_hal_atimer_init_pin() {
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	S4532390_HAL_ATIMER_PINCLK();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
    GPIO_InitStructure.Pin = S4532390_HAL_ATIMER_PIN;				//Pin
    HAL_GPIO_Init(S4532390_HAL_ATIMER_PINPORT, &GPIO_InitStructure);	//Initialise Pin
}

void atimer_isr_callback() {
	HAL_GPIO_WritePin(S4532390_HAL_ATIMER_PINPORT, S4532390_HAL_ATIMER_PIN, 0x01 & pinOut);
	pinOut = ~pinOut;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {

        if (countInterrupt >= 2) {
			
			++atimerCounterValue;
			atimer_isr_callback();
			countInterrupt = 0;

		}
		++countInterrupt;
		atimerMsPassed += atimerPeriod/2;
    }
}

//Override default mapping of this handler to Default_Handler
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&ATIM_Init);
}
