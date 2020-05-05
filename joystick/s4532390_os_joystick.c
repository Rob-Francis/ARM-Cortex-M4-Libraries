/**
*********************
*********************
*@file mylib/joystick/s4532390_os_joystick.c
*@author Robert Francis - 45323906
*@date 5/05/2020
*@brief RTOS Joystick library
*REFERENCE:
csse3010_stage4.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_os_joystick_init(void);
void s4532390_os_joystick_deinit(void); 
void s4532390_TaskJoystickZ(void);
*********************
*********************
**/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4532390_os_joystick.h"

/* Private define ------------------------------------------------------------*/
#define DEBOUNCE_TICKS 500 //ms waited for debouncing
#define JOYSTICKTASK_STACK_SIZE	( configMINIMAL_STACK_SIZE * 2 ) //Size of stack for task
#define JOYSTICKTASK_PRIORITY					( tskIDLE_PRIORITY + 7 ) //Priority of task


/* Private variables ---------------------------------------------------------*/
SemaphoreHandle_t ZSemaphore;	/* Semaphore for pushbutton interrupt */
int previousTickTime; //Stores time when last pressed
unsigned char buttonToggle;
TaskHandle_t joystickHandle;


/**
*@brief Initialises the joystick task
*@param  None
*@retval None
*/
void s4532390_os_joystick_init(void) {
 
    /* Create Semaphore */
    ZSemaphore = xSemaphoreCreateBinary();
    xTaskCreate( (void *) &s4532390_TaskJoystickZ, (const signed char *) "JOYS", JOYSTICKTASK_STACK_SIZE, NULL, JOYSTICKTASK_PRIORITY, &joystickHandle);
}

/**
*@brief Deinitialises the joystick task
*@param  None
*@retval None
*/
void s4532390_os_joystick_deinit(void) {
    vSemaphoreDelete(ZSemaphore);
    vTaskSuspend(joystickHandle);
}

/**
*@brief Internal function for debouncing lightbar
*@param  None
*@retval Returns 1 if enough time has passed 0 otherwise
*/
int debounceJoystickZ(void) {

    int tick = xTaskGetTickCountFromISR(); // Gets current tick count

    if (tick > previousTickTime + DEBOUNCE_TICKS) { // Makes sure 500ms has passed

        previousTickTime = tick;
        return 1;
    } else {
        return 0;
    }

}

/**
  * @brief  Task for toggling variable using joystick z semaphore
  * @param  None
  * @retval None
  */
void s4532390_TaskJoystickZ(void) {


	  for (;;) {

        if (ZSemaphore != NULL) {	/* Check if semaphore exists */

            /* See if we can obtain the semaphore. If the semaphore is not available
                wait 10 ticks to see if it becomes free. */
            if( xSemaphoreTake(ZSemaphore, 10) == pdTRUE) {

                if (debounceJoystickZ()) {
                    //Toggles variable if semaphore is obtained
                    buttonToggle = ~buttonToggle & 0x01;
                }
            } 
        }

		    vTaskDelay(10);
	  }
}
