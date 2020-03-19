#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4532390_hal_atimer.h"


TIM_HandleTypeDef TIM_Init;
int atimerClockSpeed = 25000;
int atimerPeriod = 2;

void s4532390_hal_atimer_init() {

	unsigned short PrescalerValue;
    // Timer 3 clock enable
	__TIM3_CLK_ENABLE();


	BRD_LEDInit(); // Initialise LEDS 
	
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	// Compute the prescaler value
	// Set the clock prescaler to 25kHz
	// SystemCoreClock is the system clock frequency
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / atimerClockSpeed) - 1;

	/* TIM Base configuration */
	TIM_Init.Instance = TIM3;				//Enable Timer 2
  	TIM_Init.Init.Period = atimerClockSpeed * atimerPeriod / 1000;			//Set period count to be 1ms, so timer interrupt occurs every 1ms.
  	TIM_Init.Init.Prescaler = PrescalerValue;	//Set prescaler value
  	TIM_Init.Init.ClockDivision = 0;			//Set clock division
	TIM_Init.Init.RepetitionCounter = 0;	// Set reload Value
  	TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* Initialise Timer */
	HAL_TIM_Base_Init(&TIM_Init);

	/* Set priority of Timer 3 update Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	/* 	DO NOT SET INTERRUPT PRIORITY HIGHER THAN 3 */
	HAL_NVIC_SetPriority(TIM3_IRQn, 10, 0);		//Set Main priority to 10 and sub-priority to 0.

	// Enable the Timer 3 interrupt
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	// Start Timer 3 base unit in interrupt mode
	HAL_TIM_Base_Start_IT(&TIM_Init);
}

int s4532390_hal_atimer_timer_getms() {}

void s4532390_hal_atimer_timer_reset() {}

void s4532390_hal_atimer_timer_pause() {}

void s4532390_hal_atimer_timer_resume() {}

void s4532390_hal_atimer_clkspeed_set(int frequency) {}

void s4532390_hal_atimer_period(int period) {}

void s4532390_hal_atimer_init_pin() {}

void atimer_isr_callback() {}