#ifndef HAL_ATIMER
#define HAL_ATIMER

#include "board.h"

extern TIM_HandleTypeDef TIM_Init;

#define s4532390_hal_atimer_timer_read()

void s4532390_hal_atimer_init();

int s4532390_hal_atimer_timer_getms();

void s4532390_hal_atimer_timer_reset();

void s4532390_hal_atimer_timer_pause();

void s4532390_hal_atimer_timer_resume();

void s4532390_hal_atimer_clkspeed_set(int frequency);

void s4532390_hal_atimer_period(int period);

void s4532390_hal_atimer_init_pin();

void atimer_isr_callback();

#ifdef S4532390_MYLIB_CONFIGURE_INC 
#define S4532390_HAL_ATIMER_PIN BRD_D0_PIN
#define S4532390_HAL_ATIMER_PINPORT BRD_D0_GPIO_PORT
#define S4532390_HAL_ATIMER_PINCLK 
#define S4532390_HAL_ATIMER_PERIOD 1000
#define S4532390_HAL_ATIMER_CLKSPEED 50000
#endif





#endif
