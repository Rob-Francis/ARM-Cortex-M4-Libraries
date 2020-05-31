#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "s4532390_CAG_PushButton.h"

#define CAG_PUSH_BUTTON_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_PUSH_BUTTON_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

SemaphoreHandle_t PBSemaphore;	/* Semaphore for keypad states */
unsigned char s4532390_keypadToggle;
int lastPressTime;

void s4532390_CAG_PushButton_Task() {

    for (;;) {

        if (PBSemaphore != NULL) {	/* Check if semaphore exists */

			/* See if we can obtain the  semaphore. If the semaphore is not available
          	 wait 10 ticks to see if it becomes free. */
			if ( xSemaphoreTake( PBSemaphore, 10 ) == pdTRUE ) {
                s4532390_keypadToggle = ~s4532390_keypadToggle;
            }
        }
        vTaskDelay(40);
    }
}

void s4532390_CAG_PushButton_Init() {


    PBSemaphore = xSemaphoreCreateBinary();

    xTaskCreate( (void *) &s4532390_CAG_PushButton_Task, (const signed char *) "PUSH", CAG_PUSH_BUTTON_TASK_STACK_SIZE, NULL, CAG_PUSH_BUTTON_PRIORITY, NULL );
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    BaseType_t xHigherPriorityTaskWoken;

    int tempTime = HAL_GetTick();
    if (GPIO_Pin == GPIO_PIN_13 && lastPressTime + 500 < tempTime) {

        lastPressTime = tempTime;
        
        xSemaphoreGiveFromISR(PBSemaphore, &xHigherPriorityTaskWoken);
    } 
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}


void EXTI15_10_IRQHandler(void)
{
	  HAL_GPIO_EXTI_IRQHandler(BRD_USER_BUTTON_PIN);
}