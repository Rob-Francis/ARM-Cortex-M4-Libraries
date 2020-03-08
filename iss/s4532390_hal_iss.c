#include "board.h"
#include "processor_hal.h"
#include "s4532390_hal_iss.h"
#include "debug_printf.h"

uint32_t iss_eventcounter_val[3];
uint32_t iss_lasttimer_val[3]; //Globals automatically initialised to 0
uint32_t iss_delay_val[3];


void s4532390_hal_iss_init() {
    iss_delay_val[S4532390_HAL_ISS_SOURCE1] = 10;
    iss_delay_val[S4532390_HAL_ISS_SOURCE2] = 10;
    iss_delay_val[S4532390_HAL_ISS_SOURCE3] = 10;

}

void s4532390_hal_iss_synchroniser(unsigned char signal_source_index) {
    int time = HAL_GetTick();

    
    if ((time - s4532390_hal_iss_lasttimer_read(signal_source_index)) >= iss_delay_val[signal_source_index]) {
        debug_printf("Times : %d  %d \r\n", s4532390_hal_iss_lasttimer_read(signal_source_index), time);
        if (++iss_eventcounter_val[signal_source_index] == 10) {
            s4532390_hal_iss_eventcounter_reset(signal_source_index);
        }
        iss_lasttimer_val[signal_source_index] = time;
    }
    
    
}

uint32_t s4532390_hal_iss_eventcounter_read(unsigned char signal_source_index) {
    return iss_eventcounter_val[signal_source_index];
}

uint32_t s4532390_hal_iss_lasttimer_read(unsigned char signal_source_index) {
    return iss_lasttimer_val[signal_source_index];
}

void s4532390_hal_iss_eventcounter_reset(unsigned char signal_source_index) {
    iss_eventcounter_val[signal_source_index] = 0;
}

void s4532390_hal_iss_lasttimer_reset(unsigned char signal_source_index) {
    iss_lasttimer_val[signal_source_index] = 0;
}

void s4532390_hal_iss_delaytimer_ctrl(unsigned char signal_source_index, int delayvalue) {
    iss_delay_val[signal_source_index] = delayvalue;
}