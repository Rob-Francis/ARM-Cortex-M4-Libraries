#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


#define GRID_WIDTH 15
#define GRID_HEIGHT 15

#define ALIVE 1
#define DEAD 0

unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];

// Cell AliveCells[];

// void addLiveCell(int i, int j) {

// }

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

void s4532390_CAG_Simulator_Task() {

    for (;;) {


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
        
        vTaskDelay(100);
    }
}