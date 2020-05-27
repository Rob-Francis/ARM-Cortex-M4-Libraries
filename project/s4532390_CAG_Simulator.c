#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "s4532390_CAG_Display.h"
#include "s4532390_CAG_Simulator.h"

#define CAG_SIMULATOR_PRIORITY ( tskIDLE_PRIORITY + 3 )
#define CAG_SIMULATOR_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 8 )

int updateTime = 1000;

unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];

EventGroupHandle_t CAG_EventGroup;

unsigned char simulationOn = 1;

// Cell AliveCells[];

// void addLiveCell(int i, int j) {

// }

void s4532390_CAG_Simulator_Init() {

    xTaskCreate( (void *) &s4532390_CAG_Simulator_Task, (const signed char *) "SIMULATOR", CAG_SIMULATOR_TASK_STACK_SIZE, NULL, CAG_SIMULATOR_PRIORITY, NULL );
    CAG_EventGroup = xEventGroupCreate();

}

int countNeighbours(int cellX, int cellY) {

    int neighbours = 0;

    for (int i = cellX - 1; i < cellX + 1; ++i) {

        for (int j = cellY - 1; j < cellY + 1; ++j) {

            if (i >= 0 && i < GRID_WIDTH && j >= 0 && j < GRID_HEIGHT) {
                if (Grid[i][j] == ALIVE) {
                    ++neighbours;
                }
            }
        }
    }
        
    return neighbours;

}

void clearGrid() {

    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            Grid[i][j] = DEAD;
        }
    }
}


void CAG_Step() {

    for (int j = 0; j < GRID_HEIGHT; ++j) {
            
        for (int i = 0; i < GRID_WIDTH; ++i) {
            
            int neighbours = countNeighbours(i, j);
            
            if (Grid[i][j] == ALIVE && (neighbours < 2 || neighbours > 3)) {

                Grid[i][j] = DEAD;
                    
            } else if (Grid[i][j] == DEAD && neighbours == 3) {

                Grid[i][j] = ALIVE;
            } 
        }         
    }
}


void handleEventBits() {

    switch (xEventGroupWaitBits(CAG_EventGroup, 0xFF, pdTRUE, pdFALSE, 10)) {
            case (CLEAR_GRID_BIT):
                clearGrid();
            break;
            case (START_BIT):
                simulationOn = 1;
            break;
            case (STOP_BIT):
                simulationOn = 0;
            break;
            case (1 << 3):
                updateTime = 500;
            break;
            case (1 << 4):
                updateTime = 1000;
            break;
            case (1 << 5):
                updateTime = 2000;
            break;
            case (1 << 6):
                updateTime = 5000;
            break;
            case (1 << 7):
                updateTime = 10000;
            break;
        }



}


void s4532390_CAG_Simulator_Task() {

    for (;;) {

        handleEventBits();
        
        if (simulationOn) {
            CAG_Step();
        }
        
        
        vTaskDelay(updateTime);
    }
}