/**
*********************
*********************
*@file mylib/keypad/s4532390_os_keypad.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief Keypad OS Library
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
*void s4532390_os_keyad_init()
*void s4532390_keypad_task()
*void s4532390_keypad_task();
*********************
*********************
**/


#include "s4532390_hal_keypad.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "debug_printf.h"

#include "s4532390_os_keypad.h"

#define KEYPAD_PRIORITY ( tskIDLE_PRIORITY + 6 )
#define KEYPAD_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 5 )
#define KEYPAD_TASK_DELAY 10

EventGroupHandle_t s4532390_keypadEventGroup;

int KeypadStatus;

/**
*@brief Initialises the keypad task
*@param  None
*@retval None
*/
void s4532390_os_keyad_init() {

    //Creates event group and task
    s4532390_keypadEventGroup = xEventGroupCreate();
    xTaskCreate( (void *) &s4532390_keypad_task, (const signed char *) "KEYPAD", KEYPAD_TASK_STACK_SIZE, NULL, KEYPAD_PRIORITY, NULL );
}

/**
*@brief The keypad task
*@param  None
*@retval None
*/
void s4532390_keypad_task() {

    for (;;) {

        //Runs keypad state machine
        s4532390_hal_keypad_fsmprocessing();

        //Sends to event group if keypad is pressed
        if (KeypadStatus) {

            xEventGroupSetBits(s4532390_keypadEventGroup, 1 << s4532390_hal_keypad_read_key());
            BRD_LEDGreenOn();
        } else {

            BRD_LEDGreenOff();
        }

        vTaskDelay(KEYPAD_TASK_DELAY);

    }
}