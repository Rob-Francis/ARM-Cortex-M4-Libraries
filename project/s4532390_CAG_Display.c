#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "s4532390_CAG_Display.h"
#include "s4532390_CAG_Simulator.h"



#define CAG_BOARD_PRIORITY ( tskIDLE_PRIORITY + 4 )
#define CAG_BOARD_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 8 )


#define SCREEN_CLEAR()  debug_printf("\e[2J")
#define SCREEN_HOME()  debug_printf("\e[H")

unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];

char cellColour[15] = CELL_BLACK;

void s4532390_CAG_Display_Init() {
    xTaskCreate( (void *) &s4532390_CAG_Display_Task, (const signed char *) "DISPLAY", CAG_BOARD_TASK_STACK_SIZE, NULL, CAG_BOARD_PRIORITY, NULL ); 
    // PAUSED FOR TESTING
}


void s4532390_CAG_Display_Task() {


    for (;;) {

            for (int j = 0; j < GRID_HEIGHT; ++j) {
                
                for (int i = 0; i < GRID_WIDTH; ++i) {
                    
                    if (Grid[i][j] == 0) {
                        portENTER_CRITICAL();
                        debug_printf("%s %s ", cellColour, cellColour);
                        portEXIT_CRITICAL();
                    } else {
                        portENTER_CRITICAL();
                        debug_printf("%s %s ", CELL_WHITE, CELL_WHITE);
                        portEXIT_CRITICAL();
                    }
                    
                }

                debug_printf("\n\r");
            }

        debug_printf("\n\r");
        vTaskDelay(100);
    }
}