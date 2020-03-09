 /** 
 **************************************************************
 * @file mylib/iss/s4532390_hal_iss.c
 * @author Robert Francis - 45323906
 * @date 8/03/20
 * @brief Hal Input Signal Sychroniser Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4532390_hal_iss_init() - initialises the iss variables
 * s4532390_hal_iss_synchroniser(unsigned char signal_source_index) - checks the time passed and 
    increments the event counter if it is greater than the delay value
 * s4532390_hal_iss_eventcounter_read(unsigned char signal_source_index) - returns the event counter value
 * s4532390_hal_iss_lasttimer_read(unsigned char signal_source_index) - returns the last timer value
 * s4532390_hal_iss_eventcounter_reset(unsigned char signal_source_index) - resets the event counter to 0
 * s4532390_hal_iss_lasttimer_reset(unsigned char signal_source_index) - resets the last timer value to 0
 * s4532390_hal_iss_delaytimer_ctrl(unsigned char signal_source_index, int delayvalue) - sets the delay value
**************************************************************** 
 */

#include "board.h"
#include "processor_hal.h"
#include "s4532390_hal_iss.h"
#include "debug_printf.h"

//GLOBAL VARIABLES
//Arrays used for storing information about the 3 sources
//Globals automatically initialised to 0
uint32_t iss_eventcounter_val[3]; //Number of times interrupt runs
uint32_t iss_lasttimer_val[3]; //Last time interrupt ran
uint32_t iss_delay_val[3]; //How long until interrupt should run again

//COMPILER DEFINITIONS
#define DEFAULT_SOURCE_VALUE 10

//Initialises the delay values to 10
void s4532390_hal_iss_init() {

    iss_delay_val[S4532390_HAL_ISS_SOURCE1] = DEFAULT_SOURCE_VALUE;
    iss_delay_val[S4532390_HAL_ISS_SOURCE2] = DEFAULT_SOURCE_VALUE;
    iss_delay_val[S4532390_HAL_ISS_SOURCE3] = DEFAULT_SOURCE_VALUE;
}

//Checks whether or not the interrupt should take place
void s4532390_hal_iss_synchroniser(unsigned char signal_source_index) {

    //Gets current time
    int time = HAL_GetTick();
  
    //If the time passed since last event is greater than the delay
    if ((time - s4532390_hal_iss_lasttimer_read(signal_source_index)) >= iss_delay_val[signal_source_index]) {

        if (++iss_eventcounter_val[signal_source_index] == 10) { //Increase the event counter (mod 10)

            s4532390_hal_iss_eventcounter_reset(signal_source_index);
        }

        iss_lasttimer_val[signal_source_index] = time; //Records time if event occurs
    }  
}

//Returns the event counter for a single source
uint32_t s4532390_hal_iss_eventcounter_read(unsigned char signal_source_index) {

    return iss_eventcounter_val[signal_source_index];
}

//Returns the last timer value for a single source
uint32_t s4532390_hal_iss_lasttimer_read(unsigned char signal_source_index) {

    return iss_lasttimer_val[signal_source_index];
}

//Resets the event counter
void s4532390_hal_iss_eventcounter_reset(unsigned char signal_source_index) {

    iss_eventcounter_val[signal_source_index] = 0;
}

void s4532390_hal_iss_lasttimer_reset(unsigned char signal_source_index) {

    iss_lasttimer_val[signal_source_index] = 0;
}

void s4532390_hal_iss_delaytimer_ctrl(unsigned char signal_source_index, int delayvalue) {
    
    iss_delay_val[signal_source_index] = delayvalue;
}