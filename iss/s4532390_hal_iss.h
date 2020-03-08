#ifndef HAL_ISS
#define HAL_ISS

#define S4532390_HAL_ISS_SOURCE1 0
#define S4532390_HAL_ISS_SOURCE2 1
#define S4532390_HAL_ISS_SOURCE3 2

void s4532390_hal_iss_init();

void s4532390_hal_iss_synchroniser(unsigned char signal_source_index);

uint32_t s4532390_hal_iss_eventcounter_read(unsigned char signal_source_index);

uint32_t s4532390_hal_iss_lasttimer_read(unsigned char signal_source_index);

void s4532390_hal_iss_eventcounter_reset(unsigned char signal_source_index);

void s4532390_hal_iss_lasttimer_reset(unsigned char signal_source_index);

void s4532390_hal_iss_delaytimer_ctrl(unsigned char signal_source_index, int delayvalue);

#endif