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



#define SCREEN_CLEAR()  debug_printf("\e[2J")
#define SCREEN_HOME()  debug_printf("\e[H")

unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];
EventGroupHandle_t CAG_EventGroup;

unsigned char pauseCheck = 1;

int updateTime = 100;

void clearGrid() {
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            Grid[i][j] = DEAD;
        }
    }
}

void s4532390_CAG_Display_Task() {


    for (;;) {

        switch (xEventGroupWaitBits(CAG_EventGroup, 0xFF, pdTRUE, pdFALSE, 10)) {
            case (1 << 8):
                clearGrid();
            break;
            case (1 << 7):
                pauseCheck = 1;
            break;
            case (1 << 6):
                pauseCheck = 0;
            break;
            case (1 << 5):
                updateTime = 500;
            break;
            case (1 << 4):
                updateTime = 1000;
            break;
            case (1 << 3):
                updateTime = 2000;
            break;
            case (1 << 2):
                updateTime = 5000;
            break;
            case (1 << 1):
                updateTime = 10000;
            break;
            case (1 << 0):
            break;
        }

        if (pauseCheck) {
            
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
        }

        vTaskDelay(updateTime);
    }
}