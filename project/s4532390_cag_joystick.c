/**
*********************
*********************
*@file mylib/project/s4532390_cag_joystick.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Joystick library
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_joystick_init();
void s4532390_cag_joystick_deinit();
*********************
*********************
**/

#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4532390_os_joystick.h"
#include "s4532390_hal_joystick.h"
#include "s4532390_cag_display.h"
#include "s4532390_cag_simulator.h"


//DEFINITIONS

#define CAG_JOYSTICK_TASK_PRIORITY					( tskIDLE_PRIORITY + 1 ) //Task priority
#define CAG_JOYSTICK_STACK_SIZE	( configMINIMAL_STACK_SIZE * 4 ) //Task stack size

//Cut offs for cell colours
#define BLACK_CUT 300
#define RED_CUT 900
#define GREEN_CUT 1400
#define YELLOW_CUT 1900
#define BLUE_CUT 2100
#define MAGENTA_CUT 2800
#define CYAN_CUT 3600

//Cut offs for update speeds
#define S10_CUT 800
#define S5_CUT 1800
#define S2_CUT 2400
#define S1_CUT 3400

#define JOYSTICK_TASK_WAIT 100

//EXTERNAL VARIABLES
EventGroupHandle_t s4532390_cagEventGroup; //Event group for general cag functions
QueueHandle_t s4532390_joystickQueue; //Queue for joystick values
SemaphoreHandle_t s4532390_zSemaphore;	/* Semaphore for pushbutton interrupt */
char s4532390_cellColour[15]; // Alive cell colour

//GLOBALS
TaskHandle_t cagJoystickTaskHandle; // Task handler
int currentBitSpeed = S2_BIT; //Current update speed

/**
*@brief Selects a cell colour according to cut offs
*@param  int - x value of joystick
*@retval char* - colour for cells
*/
char* select_colour(int x) {

    if (x < BLACK_CUT) {

        return CELL_BLACK;
    } else if (x < RED_CUT) {

        return CELL_RED;
    } else if (x < GREEN_CUT) {

        return CELL_GREEN;
    } else if (x < YELLOW_CUT) {

        return CELL_YELLOW;
    } else if (x < BLUE_CUT) {

        return CELL_BLUE;
    } else if (x < MAGENTA_CUT) {

        return CELL_MAGENTA;
    } else if (x < CYAN_CUT) {

        return CELL_CYAN;
    } else {

        return CELL_WHITE;
    }
    
}

/**
*@brief Gets new update time for simulator
*@param  int y value of joystick
*@retval int - bit value refers to speed of simulator updates
*/
int get_update_time(int y) {

    if (y < S10_CUT) {

        return S10_BIT;
    } else if (y < S5_CUT) {

        return S5_BIT;
    } else if (y < S2_CUT) {

        return S2_BIT;
    } else if (y < S1_CUT) {

        return S1_BIT;
    } else {

        return MS500_BIT;
    }
}   

/**
*@brief CAG Joystick task - handles joystick x, y and z values
*@param  None
*@retval None
*/
void s4532390_cag_joystick_task() {

    JoystickMessage message;

    for (;;) {

        //Waits for joystick values
        if (xQueueReceive(s4532390_joystickQueue, &message, 10)) {
            
            //Gets the new update time
            int tempTimeBit = get_update_time(message.joystick_y);

            //If the update time is different than current, send it in event group
            if (tempTimeBit != currentBitSpeed) {

                xEventGroupSetBits(s4532390_cagEventGroup, tempTimeBit);
                currentBitSpeed = tempTimeBit;
            }

            //Sets the new cell colour according to joystick x
            strcpy(s4532390_cellColour, select_colour(message.joystick_x));

            if (s4532390_zSemaphore != NULL) {	/* Check if semaphore exists */

            /* See if we can obtain the semaphore. If the semaphore is not available
                wait 10 ticks to see if it becomes free. */
                
                if( xSemaphoreTake(s4532390_zSemaphore, 10) == pdTRUE) {

                    if (s4532390_debounce_joystick_z()) {
                        
                        //Sends clear grid bit if semaphore and debounce
                        xEventGroupSetBits(s4532390_cagEventGroup, CLEAR_GRID_BIT);
                    }
                } 
            }
        }

        vTaskDelay(JOYSTICK_TASK_WAIT);
    }
}

/**
*@brief Initialises the CAG Joystick task
*@param  None
*@retval None
*/
void s4532390_cag_joystick_init() {

    //Initialises all joystick functions
    s4532390_hal_joystick_init();
    s4532390_os_joystick_init();
    xTaskCreate( (void *) &s4532390_cag_joystick_task, (const signed char *) "CAG_JOYSTICK", CAG_JOYSTICK_STACK_SIZE, NULL, CAG_JOYSTICK_TASK_PRIORITY, &cagJoystickTaskHandle);
}

/**
*@brief Deinitialises the CAG Joystick task
*@param  None
*@retval None
*/
void s4532390_cag_joystick_deinit() {

    //Deinitialises and deletes task
    s4532390_os_joystick_deinit();
    vTaskDelete(cagJoystickTaskHandle);
}


