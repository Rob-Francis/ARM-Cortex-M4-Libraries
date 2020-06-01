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
#include "s4532390_hal_joystick.h"

/* Private define ------------------------------------------------------------*/
#define DEBOUNCE_TICKS 500 //ms waited for debouncing
#define JOYSTICKTASK_STACK_SIZE	( configMINIMAL_STACK_SIZE * 2 ) //Size of stack for task
#define JOYSTICKTASK_PRIORITY					( tskIDLE_PRIORITY + 7 ) //Priority of task


/* Private variables ---------------------------------------------------------*/
SemaphoreHandle_t ZSemaphore;	/* Semaphore for pushbutton interrupt */
int previousTickTime; //Stores time when last pressed
unsigned char buttonToggle;
TaskHandle_t joystickHandle;



QueueHandle_t s4532390_JoystickQueue;	/* Queue used */



/**
*@brief Initialises the joystick task
*@param  None
*@retval None
*/
void s4532390_os_joystick_init(void) {
 
    /* Create Semaphore */
    ZSemaphore = xSemaphoreCreateBinary();
    s4532390_JoystickQueue = xQueueCreate(1, sizeof(JoystickMessage));
    xTaskCreate( (void *) &s4532390_TaskJoystick, (const signed char *) "JOYS", JOYSTICKTASK_STACK_SIZE, NULL, JOYSTICKTASK_PRIORITY, &joystickHandle);
}

/**
*@brief Deinitialises the joystick task
*@param  None
*@retval None
*/
void s4532390_os_joystick_deinit(void) {
    vSemaphoreDelete(ZSemaphore);
    vTaskDelete(joystickHandle);
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

// void s4532390_TaskJoystickXY(void) {

//         JoystickMessage message;    

//     for (;;) {
        
//         message.joystick_x = S4532390_HAL_X_READ();
//         message.joystick_y = S4532390_HAL_Y_READ();
//         xQueueSendToFront(s4532390_JoystickQueue, (void *) &message, (portTickType) 10);
//         vTaskDelay(20);
//     }
// }

/**
  * @brief  Task for toggling variable using joystick z semaphore
  * @param  None
  * @retval None
  */
void s4532390_TaskJoystick(void) {

    JoystickMessage message;  

	for (;;) {

        message.joystick_x = S4532390_HAL_X_READ();
        message.joystick_y = S4532390_HAL_Y_READ();
        xQueueSendToFront(s4532390_JoystickQueue, (void *) &message, (portTickType) 10);

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

	vTaskDelay(20);
	}
}
