/**
*********************
*********************
*@file mylib/joystick/s4532390_os_joystick.h
*@author Robert Francis - 45323906
*@date 5/05/2020
*@brief RTOS Joystick library
*REFERENCE:
csse3010_stage4.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_os_joystick_init(void);
void s4532390_os_joystick_deinit(void); 
void s4532390_TaskJoystickZ(void);
*********************
*********************
**/

#ifndef S4532390_OS_JOYSTICK_H
#define S4532390_OS_JOYSTICK_H

void s4532390_os_joystick_init(void);

void s4532390_os_joystick_deinit(void); 

void s4532390_TaskJoystick(void);

extern QueueHandle_t s4532390_JoystickQueue;
extern SemaphoreHandle_t ZSemaphore;
extern unsigned char buttonToggle;

typedef struct JoystickMessages {
    int joystick_x;
    int joystick_y;
} JoystickMessage;

#endif