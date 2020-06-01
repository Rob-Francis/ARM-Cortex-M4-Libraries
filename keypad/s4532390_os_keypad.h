#ifndef S4532390_OS_KEYPAD
#define S4532390_OS_KEYPAD

void s4532390_Keypad_Task();

void s4532390_OS_Keyad_Init();

extern EventGroupHandle_t s4532390_keypadEventGroup;

#endif