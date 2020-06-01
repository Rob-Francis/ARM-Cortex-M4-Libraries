 /** 
 **************************************************************
 * @file mylib/pb/s4532390_hal_pb.h
 * @author Robert Francis - 45323906
 * @date 8/03/20
 * @brief Hal Pushbutton Driver Header file
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4532390_hal_pb_on_init() - initialises the onboard button
 * s4532390_hal_pb_on_isr() - interrupt routine for the button
 * s4532390_hal_pb_on_deinit() - deinitialiser for the button
 * s4532390_hal_pb_iss_init() - initialises the iss
 * s4532390_hal_pb_iss_isr() - iss interrupt routine (moves the light)
 * s4532390_hal_pb_iss_deinit() - deinitialiser for the iss
 *************************************************************** 
 */

#ifndef HAL_PB
#define HAL_PB

//GLOBAL VARIABLES
extern int direction; //Controls direction lightbar is moving
extern unsigned short s4532390_lightbarValue; //Controls what is wiritten to the lightbar

void s4532390_hal_pb_on_init();

void s4532390_hal_pb_on_isr();

void s4532390_hal_pb_on_deinit();

void s4532390_hal_pb_iss_init();

void s4532390_hal_pb_iss_isr();

void s4532390_hal_pb_iss_deinit();

#endif