/**
*********************
*********************
*@file mylib/project/s4532390_cag_simulator.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Simulator library
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

#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "s4532390_cag_display.h"
#include "s4532390_cag_simulator.h"
#include "s4532390_os_lta1000g.h"


//DEFINITIONS
#define CAG_SIMULATOR_PRIORITY ( tskIDLE_PRIORITY + 5 )
#define CAG_SIMULATOR_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 8 )
#define CAG_QUEUE_TASK_DELAY 100

//Times for queue handling
#define MS500_TIME 500
#define S1_TIME 1000
#define S2_TIME 2000
#define S5_TIME 5000
#define S10_TIME 10000


//EXTERNAL VARIABLES
unsigned char s4532390_grid[GRID_WIDTH][GRID_HEIGHT]; // Cell grid
unsigned short s4532390_lightbarValue; //Lightbar
EventGroupHandle_t s4532390_cagEventGroup; //General event group
QueueHandle_t s4532390_cagSimulatorQueue;	/* Queue used */


//GLOBAL VARIABLES
TaskHandle_t cagSimulatorTaskHandle; //Handle
TaskHandle_t cagSimulatorQueueTaskHandle; //Handle for queue task
unsigned char simulationOn = 1; //Simulator toggle
int updateTime = 1000; // delay


/**
*@brief Clears grid of life
*@param  None
*@retval None
*/
void clear_grid() {
    
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {

            s4532390_grid[i][j] = DEAD;
        }
    }
}


/**
*@brief Initialises the simulator task and queue task
*@param  None
*@retval None
*/
void s4532390_cag_simulator_init() {

    xTaskCreate( (void *) &s4532390_cag_queue_task, (const signed char *) "SIM_QUEUE", CAG_SIMULATOR_TASK_STACK_SIZE, NULL, CAG_SIMULATOR_PRIORITY, &cagSimulatorQueueTaskHandle);
    xTaskCreate( (void *) &s4532390_cag_simulator_task, (const signed char *) "SIMULATOR", CAG_SIMULATOR_TASK_STACK_SIZE, NULL, CAG_SIMULATOR_PRIORITY, &cagSimulatorTaskHandle);

    //Creates event group and queue and clears the grid
    s4532390_cagEventGroup = xEventGroupCreate();
    s4532390_cagSimulatorQueue = xQueueCreate(10, sizeof(caMessage_t));
    clear_grid();

}


/**
*@brief Deinitialises the simulator task and queue task
*@param  None
*@retval None
*/
void s4532390_cag_simulator_deinit() {

    //Deletes group and queue and tasks
    vEventGroupDelete(s4532390_cagEventGroup);
    vQueueDelete(s4532390_cagSimulatorQueue);
    vTaskDelete(cagSimulatorQueueTaskHandle);
    vTaskDelete(cagSimulatorTaskHandle);
}


/**
*@brief Counts the neighbours of a single cell
*@param  tempGrid - copy of grid, x and y value of a cell
*@retval number of neighbours
*/
int count_neighbours(unsigned char tempGrid[GRID_WIDTH][GRID_HEIGHT], int cellX, int cellY) {

    int neighbours = 0;

    for (int i = cellX - 1; i <= cellX + 1; ++i) {

        for (int j = cellY - 1; j <= cellY + 1; ++j) {

            if (i >= 0 && i < GRID_WIDTH && j >= 0 && j < GRID_HEIGHT) { //Dont go out of bounds

                if (i != cellX || j != cellY) { //Dont count self

                    neighbours += tempGrid[i][j];
                }
            }
        }
    }
    
    return neighbours;
}



/**
*@brief Does a single step of the simulation
*@param  None
*@retval None
*/
void cag_step() {

    //Creates a copy of the grid
    unsigned char tempGrid[GRID_WIDTH][GRID_HEIGHT];

    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            tempGrid[i][j] = s4532390_grid[i][j];
        }
    }


    //Goes through each cell and performs operations
    for (int i = 0; i < GRID_WIDTH; ++i) {
            
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            
            int neighbours = count_neighbours(tempGrid, i, j); // Counts neighbours

            if (tempGrid[i][j] == ALIVE && (neighbours < 2 || neighbours > 3)) {
                
                s4532390_grid[i][j] = DEAD;
            } else if (tempGrid[i][j] == DEAD && neighbours == 3) {
                
                s4532390_grid[i][j] = ALIVE;
            } 
        }         
    }
}


/**
*@brief Draws a single block
*@param  xy values of top left cell
*@retval None
*/
void draw_block(int x, int y) {
    s4532390_grid[x][y] = ALIVE;
    s4532390_grid[x+1][y] = ALIVE;
    s4532390_grid[x][y+1] = ALIVE;
    s4532390_grid[x+1][y+1] = ALIVE;
}


/**
*@brief Draws a single beehive
*@param  xy values of top left cell
*@retval None
*/
void draw_beehive(int x, int y) {
    s4532390_grid[x + 1][y] = ALIVE;
    s4532390_grid[x + 2][y] = ALIVE;
    s4532390_grid[x][y + 1] = ALIVE;
    s4532390_grid[x + 3][y + 1] = ALIVE;
    s4532390_grid[x + 1][y + 2] = ALIVE;
    s4532390_grid[x + 2][y + 2] = ALIVE;
}


/**
*@brief Draws a single loaf
*@param  xy values of top left cell
*@retval None
*/
void draw_loaf(int x, int y) {
    s4532390_grid[x + 1][y] = ALIVE;
    s4532390_grid[x + 2][y] = ALIVE;
    s4532390_grid[x][y + 1] = ALIVE;
    s4532390_grid[x + 3][y + 1] = ALIVE;
    s4532390_grid[x + 1][y + 2] = ALIVE;
    s4532390_grid[x + 3][y + 2] = ALIVE;
    s4532390_grid[x + 2][y + 3] = ALIVE;
}


/**
*@brief Draws a single blinker
*@param  xy values of top left cell
*@retval None
*/
void draw_blinker(int x, int y) {
    s4532390_grid[x][y + 1] = ALIVE;
    s4532390_grid[x + 1][y + 1] = ALIVE;
    s4532390_grid[x + 2][y + 1] = ALIVE;
}


/**
*@brief Draws a single toad
*@param  xy values of top left cell
*@retval None
*/
void draw_toad(int x, int y) {
    s4532390_grid[x][y] = ALIVE;
    s4532390_grid[x][y + 1] = ALIVE;
    s4532390_grid[x][y + 2] = ALIVE;
    s4532390_grid[x + 1][y + 1] = ALIVE;
    s4532390_grid[x + 1][y + 2] = ALIVE;
    s4532390_grid[x + 1][y + 3] = ALIVE;
}


/**
*@brief Draws a single beacon
*@param  xy values of top left cell
*@retval None
*/
void draw_beacon(int x, int y) {
    draw_block(x, y);
    draw_block(x + 2, y + 2);
}


/**
*@brief Draws a single glider
*@param  xy values of top left cell
*@retval None
*/
void draw_glider(int x, int y) {
    s4532390_grid[x + 2][y] = ALIVE;
    s4532390_grid[x][y + 1] = ALIVE;
    s4532390_grid[x + 2][y + 1] = ALIVE;
    s4532390_grid[x + 1][y + 2] = ALIVE;
    s4532390_grid[x + 2][y + 2] = ALIVE;
}


/**
*@brief Handles a message from the simualtor queue
*@param  message - from queue
*@retval None
*/
void handle_message(caMessage_t message) {

    //Gets top left cell
    int x = message.cell_x;
    int y = message.cell_y;

    switch(message.type) {
        case LIVE_CELL:
            s4532390_grid[x][y] = ALIVE;
        break;
        case DEAD_CELL:
            s4532390_grid[x][y] = DEAD;
        break;
        case STILL_BLOCK:
            draw_block(x, y);
        break;
        case STILL_BEEHIVE:
            draw_beehive(x, y);
        break;
        case STILL_LOAF:
            draw_loaf(x, y);
        break;
        case OSCILLATOR_BLINKER:
            draw_blinker(x, y);
        break;
        case OSCILLATOR_TOAD:
            draw_toad(x, y);
        break;
        case OSCILLATOR_BEACON:
            draw_beacon(x, y);
        break;
        case SPACESHIP:
            draw_glider(x, y);
        break;
    }
}


/**
*@brief Handles a single event from the event queue
*@param  None
*@retval None
*/
void handle_event_bits() {

    //Waits for an event and handles it
    switch (xEventGroupWaitBits(s4532390_cagEventGroup, 0xFF, pdTRUE, pdFALSE, 20)) {
            case (CLEAR_GRID_BIT):
                clear_grid();
            break;
            case (START_BIT):
                simulationOn = 1;
            break;
            case (STOP_BIT):
                simulationOn = 0;
            break;
            case (MS500_BIT):
                updateTime = MS500_TIME;
            break;
            case (S1_BIT):
                updateTime = S1_TIME;
            break;
            case (S2_BIT):
                updateTime = S2_TIME;
            break;
            case (S5_BIT):
                updateTime = S5_TIME;
            break;
            case (S10_BIT):
                updateTime = S10_TIME;
            break;
            default:
            break;
        }
}


/**
*@brief Simulator queue task
*@param  None
*@retval None
*/
void s4532390_cag_queue_task() {

    for (;;) {

        caMessage_t message;

        handle_event_bits();

        //Waits for a message from the queue and then handles it
        if (xQueueReceive(s4532390_cagSimulatorQueue, &message, 20) == pdTRUE) {
            
            
            handle_message(message);
        }

        vTaskDelay(CAG_QUEUE_TASK_DELAY);
    }
}


/**
*@brief Simulator task - runs game logic
*@param  None
*@retval None
*/
void s4532390_cag_simulator_task() {

    for (;;) {

        if (simulationOn) { 
            
            //Does a single step if simulation is on
            cag_step();

            //Toggles the lighbar value
            s4532390_lightbarValue = s4532390_lightbarValue ^ (1 << 9);
        }
               
        vTaskDelay(updateTime);
    }
}