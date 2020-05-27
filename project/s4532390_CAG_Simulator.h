#ifndef S4532390_CAG_SIMULATOR_H
#define S4532390_CAG_SIMULATOR_H

#include "FreeRTOS.h"
#include "event_groups.h"

extern unsigned char Grid[15][15];

void s4532390_CAG_Simulator_Init();

void s4532390_CAG_Simulator_Task();

extern EventGroupHandle_t CAG_EventGroup;

#define CLEAR_GRID_BIT (1 << 0)
#define START_BIT (1 << 1)
#define STOP_BIT (1 << 2)

#endif