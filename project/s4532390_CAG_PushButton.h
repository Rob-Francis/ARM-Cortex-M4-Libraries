#ifndef S4532390_PUSH_BUTTON_H
#define S4532390_PUSH_BUTTON_H

extern unsigned char s4532390_keypadToggle;
extern SemaphoreHandle_t PBSemaphore;

void s4532390_CAG_PushButton_Init();

#endif
