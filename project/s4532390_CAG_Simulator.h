#ifndef S4532390_CAG_SIMULATOR_H
#define S4532390_CAG_SIMULATOR_H

#include "FreeRTOS.h"
#include "event_groups.h"
#include "s4532390_CAG_Display.h"

extern unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];

void s4532390_CAG_Simulator_Init();

void s4532390_CAG_Simulator_Deinit();

void s4532390_CAG_Simulator_Task();

extern EventGroupHandle_t s4532390_CAG_EventGroup;
extern QueueHandle_t s4532390_CAG_Queue;

typedef struct caMessage{ int  type;//Type - Cell , or  Lifeform2
                           int  cell_x;//Cell x position3
                           //or  Lifeform x position4
                           int  cell_y;//Cell y position5
                           //or  Lifeform y position6
                           } caMessage_t;

#define CLEAR_GRID_BIT (1 << 0)
#define START_BIT (1 << 1)
#define STOP_BIT (1 << 2)

#define LIVE_CELL (1 << 4 | 1 << 0)
#define DEAD_CELL (1 << 4)
#define STILL_BLOCK (2 << 4)
#define STILL_BEEHIVE (2 << 4 | 1 << 0)
#define STILL_LOAF (2 << 4 | 2 << 0)
#define OSCILLATOR_BLINKER (3 << 4)
#define OSCILLATOR_TOAD (3 << 4 | 1 << 0)
#define OSCILLATOR_BEACON (3 << 4 | 2 << 0)
#define SPACESHIP (4 << 4)

#define SINGLE_CELL (1 << 4)
#define STILL_LIFE (2 << 4)
#define OSCILLATOR (3 << 4)
#define SPACE_SHIP (4 << 4)


#endif