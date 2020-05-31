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

#define CAG_SIMULATOR_PRIORITY ( tskIDLE_PRIORITY + 5 )
#define CAG_SIMULATOR_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 8 )

int updateTime = 1000;

unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];


EventGroupHandle_t s4532390_CAG_EventGroup;
QueueHandle_t s4532390_CAG_Queue;	/* Queue used */

TaskHandle_t CAG_Simulator_Task_Handle;

unsigned char simulationOn = 1;


void clearGrid() {
    
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            Grid[i][j] = DEAD;
        }
    }
}

void s4532390_CAG_Simulator_Init() {

    xTaskCreate( (void *) &s4532390_CAG_Simulator_Task, (const signed char *) "SIMULATOR", CAG_SIMULATOR_TASK_STACK_SIZE, NULL, CAG_SIMULATOR_PRIORITY, &CAG_Simulator_Task_Handle);
    s4532390_CAG_EventGroup = xEventGroupCreate();
    s4532390_CAG_Queue = xQueueCreate(10, sizeof(caMessage_t));
    clearGrid();

}

void s4532390_CAG_Simulator_Deinit() {

    vEventGroupDelete(s4532390_CAG_EventGroup);
    vQueueDelete(s4532390_CAG_Queue);
    vTaskDelete(CAG_Simulator_Task_Handle);
}

int countNeighbours(unsigned char tempGrid[GRID_WIDTH][GRID_HEIGHT], int cellX, int cellY) {

    int neighbours = 0;

    for (int i = cellX - 1; i <= cellX + 1; ++i) {

        for (int j = cellY - 1; j <= cellY + 1; ++j) {

            if (i >= 0 && i < GRID_WIDTH && j >= 0 && j < GRID_HEIGHT) {

                if (i != cellX || j != cellY) {
                    
                    neighbours += tempGrid[i][j];
                }
            }
        }
    }
    
    return neighbours;

}




void CAG_Step() {

    unsigned char tempGrid[GRID_WIDTH][GRID_HEIGHT];

    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            tempGrid[i][j] = Grid[i][j];
        }
    }

    for (int i = 0; i < GRID_WIDTH; ++i) {
            
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            
            int neighbours = countNeighbours(tempGrid, i, j);

            if (tempGrid[i][j] == ALIVE && (neighbours < 2 || neighbours > 3)) {
                BRD_LEDBlueToggle();
                Grid[i][j] = DEAD;
                    
            } else if (tempGrid[i][j] == DEAD && neighbours == 3) {
                
                Grid[i][j] = ALIVE;
            } 
        }         
    }
}

void drawBlock(int x, int y) {
    Grid[x][y] = ALIVE;
    Grid[x+1][y] = ALIVE;
    Grid[x][y+1] = ALIVE;
    Grid[x+1][y+1] = ALIVE;
}



void drawBeehive(int x, int y) {
    Grid[x + 1][y] = ALIVE;
    Grid[x + 2][y] = ALIVE;
    Grid[x][y + 1] = ALIVE;
    Grid[x + 3][y + 1] = ALIVE;
    Grid[x + 1][y + 2] = ALIVE;
    Grid[x + 2][y + 2] = ALIVE;
}

void drawLoaf(int x, int y) {
    Grid[x + 1][y] = ALIVE;
    Grid[x + 2][y] = ALIVE;
    Grid[x][y + 1] = ALIVE;
    Grid[x + 3][y + 1] = ALIVE;
    Grid[x + 1][y + 2] = ALIVE;
    Grid[x + 3][y + 2] = ALIVE;
    Grid[x + 2][y + 3] = ALIVE;
}

void drawBlinker(int x, int y) {
    Grid[x][y] = ALIVE;
    Grid[x + 1][y] = ALIVE;
    Grid[x + 1][y] = ALIVE;
}

void drawToad(int x, int y) {
    Grid[x][y] = ALIVE;
    Grid[x][y + 1] = ALIVE;
    Grid[x][y + 2] = ALIVE;
    Grid[x + 1][y + 1] = ALIVE;
    Grid[x + 1][y + 2] = ALIVE;
    Grid[x + 1][y + 3] = ALIVE;
}

void drawBeacon(int x, int y) {
    drawBlock(x, y);
    drawBlock(x + 2, y + 2);
}

void drawGlider(int x, int y) {
    Grid[x + 2][y] = ALIVE;
    Grid[x][y + 1] = ALIVE;
    Grid[x + 2][y + 1] = ALIVE;
    Grid[x + 1][y + 2] = ALIVE;
    Grid[x + 2][y + 2] = ALIVE;
}

void handleMessage(caMessage_t message) {

    

    int x = message.cell_x;
    int y = message.cell_y;

    switch(message.type) {
        case LIVE_CELL:
            Grid[x][y] = ALIVE;
        break;
        case DEAD_CELL:
            Grid[x][y] = DEAD;
        break;
        case STILL_BLOCK:
            drawBlock(x, y);
        break;
        case STILL_BEEHIVE:
            drawBeehive(x, y);
        break;
        case STILL_LOAF:
            drawLoaf(x, y);
        break;
        case OSCILLATOR_BLINKER:
            drawBlinker(x, y);
        break;
        case OSCILLATOR_TOAD:
            drawToad(x, y);
        break;
        case OSCILLATOR_BEACON:
            drawBeacon(x, y);
        break;
        case SPACESHIP:
            drawGlider(x, y);
        break;
    }
    
}

void handleEventBits() {

    switch (xEventGroupWaitBits(s4532390_CAG_EventGroup, 0xFF, pdTRUE, pdFALSE, 10)) {
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

        caMessage_t message;
        
        

        
        while (xQueueReceive(s4532390_CAG_Queue, &message, 10) == pdTRUE) {
                handleMessage(message);
        }

        if (simulationOn) { 
            
            CAG_Step();

        }
        
        
        vTaskDelay(updateTime);
    }
}