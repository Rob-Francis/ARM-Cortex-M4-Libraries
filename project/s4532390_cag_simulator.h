/**
*********************
*********************
*@file mylib/project/s4532390_cag_simulator.h
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Simulator library Header
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_simulator_init();
void s4532390_cag_simulator_deinit();
void s4532390_cag_simulator_task();
void s4532390_cag_queue_task();
*********************
*********************
**/

#ifndef s4532390_CAG_SIMULATOR_H
#define s4532390_CAG_SIMULATOR_H

#include "FreeRTOS.h"
#include "event_groups.h"
#include "s4532390_cag_display.h"

extern unsigned char s4532390_grid[GRID_WIDTH][GRID_HEIGHT];

void s4532390_cag_simulator_init();

void s4532390_cag_simulator_deinit();

void s4532390_cag_simulator_task();

void s4532390_cag_queue_task();

extern EventGroupHandle_t s4532390_cagEventGroup;
extern QueueHandle_t s4532390_cagSimulatorQueue;

typedef struct caMessage{ int  type;//Type - Cell , or  Lifeform2
                           int  cell_x;//Cell x position3
                           //or  Lifeform x position4
                           int  cell_y;//Cell y position5
                           //or  Lifeform y position6
                           } caMessage_t;


//DEFINTIONS
#define CLEAR_GRID_BIT (1 << 0)
#define START_BIT (1 << 1)
#define STOP_BIT (1 << 2)
#define MS500_BIT (1 << 3)
#define S1_BIT (1 << 4)
#define S2_BIT (1 << 5)
#define S5_BIT (1 << 6)
#define S10_BIT (1 << 7)

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