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
void s4532390_task_joystick(void);
int s4532390_debounce_joystick_z(void);
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
#define JOYSTICKTASK_STACK_SIZE	( configMINIMAL_STACK_SIZE * 4 ) //Size of stack for task
#define JOYSTICKTASK_PRIORITY					( tskIDLE_PRIORITY + 1 ) //Priority of task
#define JOYSTICK_DELAY 100

/* Private variables ---------------------------------------------------------*/
int previousTickTime; //Stores time when last pressed
TaskHandle_t joystickHandle; //Handle to the joystick task
QueueHandle_t s4532390_joystickQueue;	/* Queue used */
SemaphoreHandle_t s4532390_zSemaphore;

/**
*@brief Initialises the joystick task
*@param  None
*@retval None
*/
void s4532390_os_joystick_init(void) {
 
    /* Create Semaphore */
    s4532390_zSemaphore = xSemaphoreCreateBinary();

    //Creates queue of size 1
    s4532390_joystickQueue = xQueueCreate(1, sizeof(JoystickMessage));

    //Initialises task
    xTaskCreate( (void *) &s4532390_task_joystick, (const signed char *) "JOYS", JOYSTICKTASK_STACK_SIZE, NULL, JOYSTICKTASK_PRIORITY, &joystickHandle);
}

/**
*@brief Deinitialises the joystick task
*@param  None
*@retval None
*/
void s4532390_os_joystick_deinit(void) {

    //Deletes semaphore and task
    vSemaphoreDelete(s4532390_zSemaphore);
    vTaskDelete(joystickHandle);
}

/**
*@brief Function for debouncing lightbar
*@param  None
*@retval Returns 1 if enough time has passed 0 otherwise
*/
int s4532390_debounce_joystick_z(void) {

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
void s4532390_task_joystick(void) {

    JoystickMessage message;  

	for (;;) {

        //Sets x and y values for message
        message.joystick_x = S4532390_HAL_X_READ();
        message.joystick_y = S4532390_HAL_Y_READ();

        //Sends to queue
        xQueueSendToFront(s4532390_joystickQueue, (void *) &message, (portTickType) 10);

	    vTaskDelay(JOYSTICK_DELAY);
	}
}
