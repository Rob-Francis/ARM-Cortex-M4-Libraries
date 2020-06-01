/**
*********************
*********************
*@file mylib/keypad/s4532390_os_keypad.h
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief Keypad OS Library Header
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
*void s4532390_os_keyad_init()
*void s4532390_keypad_task()
*********************
*********************
**/

#ifndef S4532390_OS_KEYPAD_H
#define S4532390_OS_KEYPAD_H

void s4532390_keypad_task();

void s4532390_os_keyad_init();

extern EventGroupHandle_t s4532390_keypadEventGroup;

#endif