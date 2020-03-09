 /** 
 **************************************************************
 * @file mylib/iss/s4532390_hal_iss.h
 * @author Robert Francis - 45323906
 * @date 8/03/20
 * @brief Hal Input Signal Sychroniser Driver Header file
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

#ifndef HAL_ISS
#define HAL_ISS

//Used for referring to different interrupt sources
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