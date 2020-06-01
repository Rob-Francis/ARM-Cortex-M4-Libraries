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

    GPIO_InitTypeDef  GPIO_InitStructure;
    //USER BUTTON INITIALISE
    BRD_USER_BUTTON_GPIO_CLK_ENABLE();

    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin  = BRD_USER_BUTTON_PIN;
    HAL_GPIO_Init(BRD_USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

    HAL_NVIC_SetPriority(BRD_USER_BUTTON_EXTI_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(BRD_USER_BUTTON_EXTI_IRQn);

    PBSemaphore = xSemaphoreCreateBinary();

    xTaskCreate( (void *) &s4532390_CAG_PushButton_Task, (const signed char *) "PUSH", CAG_PUSH_BUTTON_TASK_STACK_SIZE, NULL, CAG_PUSH_BUTTON_PRIORITY, NULL );
}

