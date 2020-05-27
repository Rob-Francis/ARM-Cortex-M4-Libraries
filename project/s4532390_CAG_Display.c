#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "s4532390_CAG_Display.h"

#define CELL_BLACK "\e[7;30;40m"
#define CELL_RED	"\e[7;31;41m"
#define CELL_GREEN	"\e[7;32;42m"
#define CELL_YELLOW		"\e[7;33;43m"
#define CELL_BLUE		"\e[7;34;44m"
#define CELL_MAGENTA	"\e[7;35;45m"
#define CELL_CYAN		"\e[7;36;46m"
#define CELL_WHITE 		"\e[7;37;47m"

#define CAG_BOARD_PRIORITY ( tskIDLE_PRIORITY + 4 )
#define CAG_BOARD_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 8 )


#define SCREEN_CLEAR()  debug_printf("\e[2J")
#define SCREEN_HOME()  debug_printf("\e[H")

unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];


void s4532390_CAG_Display_Init() {
    xTaskCreate( (void *) &s4532390_CAG_Display_Task, (const signed char *) "DISPLAY", CAG_BOARD_TASK_STACK_SIZE, NULL, CAG_BOARD_PRIORITY, NULL ); 
    // PAUSED FOR TESTING
}


void s4532390_CAG_Display_Task() {


    for (;;) {

            for (int j = 0; j < GRID_HEIGHT; ++j) {
                
                for (int i = 0; i < GRID_WIDTH; ++i) {
                    
                    if (Grid[i][j] == 0) {
                        debug_printf("%s %s ", CELL_BLACK, CELL_BLACK);
                    } else {
                        debug_printf("%s %s ", CELL_WHITE, CELL_WHITE);
                    }
                    
                }

                debug_printf("\n\r");
            }

        debug_printf("\n\r");
        vTaskDelay(100);
    }
}