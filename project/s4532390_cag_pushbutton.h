/**
*********************
*********************
*@file mylib/project/s4532390_cag_pushbutton.h
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Pushbutton library Header
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_pushbutton_init();
*********************
*********************
**/

#ifndef S4532390_PUSH_BUTTON_H
#define S4532390_PUSH_BUTTON_H

extern unsigned char s4532390_keypadToggle;
extern SemaphoreHandle_t s4532390_pbSemaphore;

void s4532390_cag_pushbutton_init();

#endif
