#ifndef HAL_ATIMER
#define HAL_ATIMER

// #include "board.h"

// extern TIM_HandleTypeDef TIM_Init;

extern int atimerCounterValue;
extern int atimerPeriod;
extern int atimerClkSpeedVal;

#define s4532390_hal_atimer_timer_read() atimerCounterValue

void s4532390_hal_atimer_init();

int s4532390_hal_atimer_timer_getms();

void s4532390_hal_atimer_timer_reset();

void s4532390_hal_atimer_timer_pause();

void s4532390_hal_atimer_timer_resume();

void s4532390_hal_atimer_clkspeed_set(int frequency);

void s4532390_hal_atimer_period_set(int period);

void s4532390_hal_atimer_init_pin();

void atimer_isr_callback();


#ifdef S4532390_MYLIB_CONFIGURE_INC
#include "s4532390_hal_mylib_configure.h"
#endif

#ifndef S4532390_HAL_ATIMER_PIN
#define S4532390_HAL_ATIMER_PIN BRD_D0_PIN
#endif

#ifndef S4532390_HAL_ATIMER_PINPORT
#define S4532390_HAL_ATIMER_PINPORT BRD_D0_GPIO_PORT
#endif

#ifndef S4532390_HAL_ATIMER_PINCLK
#define S4532390_HAL_ATIMER_PINCLK __BRD_D0_GPIO_CLK
#endif

#ifndef S4532390_HAL_ATIMER_PERIOD
#define S4532390_HAL_ATIMER_PERIOD 1000
#endif

#ifndef S4532390_HAL_ATIMER_CLKSPEED
#define S4532390_HAL_ATIMER_CLKSPEED 50000
#endif


#endif
