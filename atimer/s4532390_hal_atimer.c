#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4532390_hal_atimer.h"


TIM_HandleTypeDef TIM_Init;
int atimerClkSpeedVal = 25000;
int atimerCounterValue = 0;
int atimerPeriod = 2;
int pinOut = 1;
int countInterrupt = 0;

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
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / atimerClkSpeedVal) - 1;

	/* TIM Base configuration */
	TIM_Init.Instance = TIM3;				//Enable Timer 2
  	TIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000;			//Set period count to be 1ms, so timer interrupt occurs every 1ms.
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

int s4532390_hal_atimer_timer_getms() {
	return atimerPeriod * atimerCounterValue;
}

void s4532390_hal_atimer_timer_reset() {
	atimerCounterValue = 0;
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

	// s4532390_hal_atimer_timer_pause();
	
	unsigned short PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / atimerClkSpeedVal) - 1;

	TIM_Init.Init.Prescaler = PrescalerValue;	//Set prescaler value
  	TIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000;	
	HAL_TIM_Base_Init(&TIM_Init);	

	// s4532390_hal_atimer_timer_resume();
}

void s4532390_hal_atimer_period_set(int period) {
	
	if (period > 0) {
		atimerPeriod = period;
	}
	
	
	TIM_Init.Init.Period = atimerClkSpeedVal * atimerPeriod / 1000;	
	HAL_TIM_Base_Init(&TIM_Init);
}



void s4532390_hal_atimer_init_pin() {
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	__BRD_D10_GPIO_CLK();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
    GPIO_InitStructure.Pin = BRD_D10_PIN;				//Pin
    HAL_GPIO_Init(BRD_D10_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin
}

void atimer_isr_callback() {
	++atimerCounterValue;
	++countInterrupt;
	
	HAL_GPIO_WritePin(BRD_D10_GPIO_PORT, BRD_D10_PIN, 0x01 & pinOut);
	pinOut = ~pinOut;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {
		

        if (countInterrupt > 1000) {
			/* Toggle all 3 board LEDs */
			BRD_LEDToggle(BRD_GREEN_LEDMASK|BRD_BLUE_LEDMASK|BRD_RED_LEDMASK);
			countInterrupt = 0;	
		}
		atimer_isr_callback();
    }
}

//Override default mapping of this handler to Default_Handler
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_Init);
}
