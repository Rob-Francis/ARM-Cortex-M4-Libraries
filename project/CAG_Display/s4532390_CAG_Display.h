#ifndef S4532390_CAG_DISPLAY_H
#define S4532390_CAG_DISPLAY_H

#define GRID_WIDTH 15
#define GRID_HEIGHT 15

#define ALIVE 1
#define DEAD 0

extern unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];
extern EventGroupHandle_t CAG_EventGroup;

extern int updateTime;

void s4532390_CAG_Display_Task();



#endif