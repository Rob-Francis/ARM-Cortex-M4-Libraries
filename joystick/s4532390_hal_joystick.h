

#ifndef HAL_JOYSTICK
#define HAL_JOYSTICK

#define S4532390_HAL_X_READ() joystick_readxy(handleX)
#define S4532390_HAL_Y_READ() joystick_readxy(handleY)
#define S4532390_HAL_Z_READ() joystickZ

#include "board.h"
#include "processor_hal.h"

extern ADC_HandleTypeDef handleX, handleY;
extern int joystickZ;

void s4532390_hal_joystick_init();

int joystick_readxy(ADC_HandleTypeDef handler);


#endif