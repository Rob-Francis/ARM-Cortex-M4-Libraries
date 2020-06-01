/**
*********************
*********************
*@file mylib/project/s4532390_cag_pushbutton.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Pushbutton library
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_pushbutton_init();
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

#include "s4532390_cag_pushbutton.h"

//DEFINTIONS
#define CAG_PUSH_BUTTON_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_PUSH_BUTTON_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define PUSHBUTTON_DELAY 40

//EXTERNAL VARIABLES
SemaphoreHandle_t s4532390_pbSemaphore;	/* Semaphore for keypad states */
unsigned char s4532390_keypadToggle;


/**
*@brief Pushbutton task
*@param  None
*@retval None
*/
void s4532390_cag_pushbutton_task() {

    for (;;) {

        if (s4532390_pbSemaphore != NULL) {	/* Check if semaphore exists */

			/* See if we can obtain the  semaphore. If the semaphore is not available
          	 wait 10 ticks to see if it becomes free. */
			if (xSemaphoreTake( s4532390_pbSemaphore, 10 ) == pdTRUE ) {

                s4532390_keypadToggle = ~s4532390_keypadToggle; // Toggles keypad
            }
        }
        vTaskDelay(PUSHBUTTON_DELAY);
    }
}

/**
*@brief Pushbutton task initialiser
*@param  None
*@retval None
*/
void s4532390_cag_pushbutton_init() {

    GPIO_InitTypeDef  GPIO_InitStructure;
    //USER BUTTON INITIALISE
    BRD_USER_BUTTON_GPIO_CLK_ENABLE();

    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin  = BRD_USER_BUTTON_PIN;
    HAL_GPIO_Init(BRD_USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

    HAL_NVIC_SetPriority(BRD_USER_BUTTON_EXTI_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(BRD_USER_BUTTON_EXTI_IRQn);

    s4532390_pbSemaphore = xSemaphoreCreateBinary(); // Creates semaphore and then task

    xTaskCreate( (void *) &s4532390_cag_pushbutton_task, (const signed char *) "PUSH", CAG_PUSH_BUTTON_TASK_STACK_SIZE, NULL, CAG_PUSH_BUTTON_PRIORITY, NULL );
}

