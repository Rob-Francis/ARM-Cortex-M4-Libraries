/**
*********************
*********************
*@file mylib/project/s4532390_cag_display.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Display library
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_display_init();
void s4532390_cag_display_task();
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

#include "s4532390_cag_display.h"
#include "s4532390_cag_simulator.h"

//DEFINITIONS
#define CAG_BOARD_PRIORITY ( tskIDLE_PRIORITY + 4 )
#define CAG_BOARD_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 8 )
#define SCREEN_CLEAR()  debug_printf("\e[2J")
#define SCREEN_HOME()  debug_printf("\e[H")
#define DISPLAY_REFRESH_TIME 100

//EXTERNAL VARIABLES
unsigned char s4532390_grid[GRID_WIDTH][GRID_HEIGHT]; // Grid of cells
char s4532390_cellColour[15] = CELL_BLUE; //Colour of alive cells


/**
*@brief Initialises the display task
*@param  None
*@retval None
*/
void s4532390_cag_display_init() {
    xTaskCreate( (void *) &s4532390_cag_display_task, (const signed char *) "DISPLAY", CAG_BOARD_TASK_STACK_SIZE, NULL, CAG_BOARD_PRIORITY, NULL ); 
}


/**
*@brief The display task
*@param  None
*@retval None
*/
void s4532390_cag_display_task() {

    for (;;) {

            portENTER_CRITICAL();
            // SCREEN_CLEAR();
		    SCREEN_HOME();
            portEXIT_CRITICAL();
            //Goes through each cell, prints black if dead, alive colour otherwise
            for (int j = 0; j < GRID_HEIGHT; ++j) {
                
                for (int i = 0; i < GRID_WIDTH; ++i) {
                    
                    if (s4532390_grid[i][j] == 0) {
                        portENTER_CRITICAL();
                        debug_printf("%s %s ", CELL_BLACK, CELL_BLACK);
                        portEXIT_CRITICAL();
                    } else {
                        portENTER_CRITICAL();
                        debug_printf("%s %s ", s4532390_cellColour, s4532390_cellColour);
                        portEXIT_CRITICAL();
                    }
                    
                }

                debug_printf("\n\r");
            }

        debug_printf("\n\r");

        vTaskDelay(DISPLAY_REFRESH_TIME);
    }
}