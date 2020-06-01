/**
*********************
*********************
*@file mylib/project/s4532390_cag_keypad_grid.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Keypad Grid library
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_keypad_grid_task();
void s4532390_cag_keypad_grid_init();
*********************
*********************
**/

#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "s4532390_cag_keypad_grid.h"
#include "s4532390_os_keypad.h"
#include "s4532390_cag_pushbutton.h"
#include "s4532390_cag_simulator.h"
#include "s4532390_cag_display.h"
#include "s4532390_cag_joystick.h"

//DEFINITIONS
#define CAG_KEYPAD_GRID_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_KEYPAD_GRID_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define NUM_KEYPAD_BITS 16
#define SUBX_POSITION 0
#define SUBY_POSITION 15
#define START_POSITION 10
#define STOP_POSITION 11
#define CLEAR_POSITION 12
#define CAG_KEYPAD_DELAY 100

//EXTERNAL VARIABLES
EventGroupHandle_t s4532390_keypadEventGroup; //Event group for keypad
EventGroupHandle_t s4532390_cagEventGroup; //General event group
QueueHandle_t s4532390_cagSimulatorQueue; //Queue for simulator
unsigned char s4532390_keypadToggle; //Toggle for keypad
unsigned char s4532390_grid[GRID_WIDTH][GRID_HEIGHT]; //Cell grid
unsigned short s4532390_lightbarValue; //Lightbar

//GLOBALS
int subX, subY; //Current x and y values for subgrids


/**
*@brief Clears the keypad values and lightbar values
*@param  None
*@retval None
*/
void clear_keypad() {

    subX = 0;
    subY = 0;
    s4532390_lightbarValue = s4532390_lightbarValue & 0b1111000000;
}


/**
*@brief Writes subgrid values to lightbar
*@param  None
*@retval None
*/
void display_subgrid_positions() {

    s4532390_lightbarValue = (s4532390_lightbarValue & 0b1111000000) | (subX << 3) | subY;
}


/**
*@brief Handles values received from the keypad in grid mode
*@param  keypadBits - bits from the keypad
*@retval None
*/
void handle_keypad_grid_bits(EventBits_t keypadBits) {

    int bitSet = 0;
    caMessage_t message;

    //Finds which bit was set by the keypad
    for (int i = 0; i < NUM_KEYPAD_BITS; ++i) {
        
        if (keypadBits & 0x01) {

            bitSet = i;
            break;
        }
        keypadBits = keypadBits >> 1;
    }

    //If the bit is a number
    if (bitSet > 0 && bitSet < 10) {

        //Convert subgrid to actual
        int xOffset = (bitSet - 1) % 3;
        int yOffset = (bitSet - 1) / 3;

        //Check whether the cell is dead or alive for toggling it
        if (s4532390_grid[(subX * 3) + xOffset][(subY * 3) + yOffset] == ALIVE) {

            message.type = DEAD_CELL;
        } else {

            message.type = LIVE_CELL;
        }

        //Convert subgrid to actual
        message.cell_x = (subX * 3) + xOffset;
        message.cell_y = (subY * 3) + yOffset;

        //Send message to toggle single cell
        xQueueSendToBack(s4532390_cagSimulatorQueue, &message, 20);


    } else if (bitSet == START_POSITION) { //Checks for start bit

        xEventGroupSetBits(s4532390_cagEventGroup, START_BIT);
    } else if (bitSet == STOP_POSITION) { // Checks for stop bit

        xEventGroupSetBits(s4532390_cagEventGroup, STOP_BIT);
    } else if (bitSet == CLEAR_POSITION) { //Checks for clear bit
        
        clear_keypad();
        xEventGroupSetBits(s4532390_cagEventGroup, CLEAR_GRID_BIT);
    } else if (bitSet == SUBX_POSITION) {

        if (subX < 4) { //Prevents overflow
            ++subX;
        }
    } else if (bitSet == SUBY_POSITION) {
        
        if (subY < 4) { //Prevents overflow
            ++subY;
        }
    }

}

/**
*@brief Grid keypad task
*@param  None
*@retval None
*/
void s4532390_cag_keypad_grid_task() {

        //Stores previous bits and current bits from keypad
        EventBits_t previousKeypadBits = 0;
        EventBits_t keypadBits;

        for (;;) {

            //Ensures grid keypad is on
            if (!s4532390_keypadToggle) {
                
                BRD_LEDRedOn();
                BRD_LEDBlueOff();

                //Gets bits from keypad
                keypadBits = xEventGroupWaitBits(s4532390_keypadEventGroup, 0xFFFF, pdTRUE, pdFALSE, 30);

                //Prevents multiple presses from a single press
                if (keypadBits != previousKeypadBits && keypadBits != 0) {

                    handle_keypad_grid_bits(keypadBits); //Handles bits
                } 
                
                previousKeypadBits = keypadBits;

                display_subgrid_positions();
            } else {

                clear_keypad(); // Clears keypad when changing off grid
            }

            
            vTaskDelay(CAG_KEYPAD_DELAY);
        } 
}

/**
*@brief Grid keypad task initialiser
*@param  None
*@retval None
*/
void s4532390_cag_keypad_grid_init() {

    xTaskCreate( (void *) &s4532390_cag_keypad_grid_task, (const signed char *) "GRID", CAG_KEYPAD_GRID_TASK_STACK_SIZE, NULL, CAG_KEYPAD_GRID_PRIORITY, NULL );

}

