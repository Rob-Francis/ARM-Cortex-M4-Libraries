#ifndef S4532390_CAG_KEYPAD_MNEMONIC
#define S4532390_CAG_KEYPAD_MNEMONIC

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t KeypadSemaphore;	/* Semaphore for keypad states */

void s4532390_CAG_Keypad_Mnemonic_Task();

void s4532390_CAG_Keypad_Mnemonic_Init();

#endif
