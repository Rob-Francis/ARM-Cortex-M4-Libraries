#ifndef S4532390_CAG_KEYPAD_GRID
#define S4532390_CAG_KEYPAD_GRID

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t KeypadSemaphore;	/* Semaphore for keypad states */

void s4532390_CAG_Keypad_Grid_Task();

void s4532390_CAG_Keypad_Grid_Init();

#endif
