#include "board.h"
#include "processor_hal.h"
#include "s4532390_hal_pwm.h"

//GLOBAL VARIABLES
//Handler for PWM Timer
TIM_HandleTypeDef PWMTIM_Init;

/**
*@brief Initialiser for PWM
*@param None
*@retval None
*/
void s4532390_hal_pwm_init() {

    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OC_InitTypeDef PWMConfig;

	uint16_t PrescalerValue = 0;

	//Clock for timer and for pin
	S4532390_HAL_PWM_TIMER_CLK();
	S4532390_HAL_PWM_PINCLK();

	//Sets up PMW output pin
	GPIO_InitStructure.Pin = S4532390_HAL_PWM_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = S4532390_HAL_PWM_PINCLKSPEED;
	GPIO_InitStructure.Alternate = S4532390_HAL_PWM_GPIOAF;
	HAL_GPIO_Init(S4532390_HAL_PWM_PINGPIOPORT, &GPIO_InitStructure);

	//Calculates prescaler
	PrescalerValue = (uint16_t) ((SystemCoreClock / S4532390_HAL_PWM_CLOCKFREQ) - 1);

	S4532390_HAL_PWM_HANDLER.Instance = S4532390_HAL_PWM_TIMER;
	S4532390_HAL_PWM_HANDLER.Init.Period = S4532390_HAL_PWM_PERIOD;
	S4532390_HAL_PWM_HANDLER.Init.Prescaler = PrescalerValue;
	S4532390_HAL_PWM_HANDLER.Init.ClockDivision = 0;
	S4532390_HAL_PWM_HANDLER.Init.RepetitionCounter = 0;
	S4532390_HAL_PWM_HANDLER.Init.CounterMode = TIM_COUNTERMODE_UP;

	//Configures prescaler
	PWMConfig.OCMode = TIM_OCMODE_PWM1;
	PWMConfig.Pulse = S4532390_HAL_PWM_PULSEPERIOD;
	PWMConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	PWMConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	PWMConfig.OCFastMode = TIM_OCFAST_DISABLE;
	PWMConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
	PWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	HAL_TIM_PWM_Init(&S4532390_HAL_PWM_HANDLER);
	HAL_TIM_PWM_ConfigChannel(&S4532390_HAL_PWM_HANDLER, &PWMConfig, S4532390_HAL_PWM_CHANNEL);

	HAL_TIM_PWM_Start(&S4532390_HAL_PWM_HANDLER, S4532390_HAL_PWM_CHANNEL);

}
