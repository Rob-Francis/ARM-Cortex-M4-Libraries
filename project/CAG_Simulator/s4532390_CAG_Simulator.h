#ifndef S4532390_CAG_SIMULATOR_H
#define S4532390_CAG_SIMULATOR_H

#include "FreeRTOS.h"
#include "event_groups.h"

extern unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];

void s4532390_CAG_Simulator_Init();

void s4532390_CAG_Simulator_Task();

extern EventGroupHandle_t CAG_EventGroup;

#endif