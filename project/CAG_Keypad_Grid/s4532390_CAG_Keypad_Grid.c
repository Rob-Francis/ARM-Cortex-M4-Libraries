#include "s4532390_CAG_Keypad_Grid.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "debug_printf.h"
#include "board.h"

SemaphoreHandle_t KeypadSemaphore;	/* Semaphore for keypad states */

#define CAG_KEYPAD_GRID_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_KEYPAD_GRID_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

void s4532390_CAG_Keypad_Grid_Task() {

        for (;;) {

            if (xSemaphoreTake( KeypadSemaphore, 10 ) == pdTRUE) {
                // debug_printf("Grid got semaphore\r\n");
                BRD_LEDToggle(BRD_RED_LEDMASK);

            }

            vTaskDelay(1000);
        }
}

void s4532390_CAG_Keypad_Grid_Init() {

    KeypadSemaphore = xSemaphoreCreateBinary();

    xTaskCreate( (void *) &s4532390_CAG_Keypad_Grid_Task, (const signed char *) "GRID", CAG_KEYPAD_GRID_TASK_STACK_SIZE, NULL, CAG_KEYPAD_GRID_PRIORITY, NULL );

}