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
#include "s4532390_CAG_Display.h"
#include "s4532390_CAG_Simulator.h"

#define CAG_JOYSTICK_TASK_PRIORITY					( tskIDLE_PRIORITY + 1 ) //Task priority
#define CAG_JOYSTICK_STACK_SIZE	( configMINIMAL_STACK_SIZE * 4 ) //Task stack size

TaskHandle_t CAG_Joystick_Task_Handle;
EventGroupHandle_t s4532390_CAG_EventGroup;
QueueHandle_t s4532390_JoystickQueue;
SemaphoreHandle_t ZSemaphore;	/* Semaphore for pushbutton interrupt */

char cellColour[15];
char *cell_colour_palatte[] = {CELL_BLACK, CELL_RED, CELL_GREEN, CELL_YELLOW, CELL_BLUE, CELL_MAGENTA, CELL_CYAN, CELL_WHITE};

char* selectColour(int x) {
    
    return cell_colour_palatte[(x + 200)/500];
}

void setUpdateTimer(int y) {
    int check = (y + 200) / 800;

    switch (check) {
        case 0:
            xEventGroupSetBits(s4532390_CAG_EventGroup, S10_BIT);
        break;
        case 1:
            xEventGroupSetBits(s4532390_CAG_EventGroup, S5_BIT);
        break;
        case 2:
            xEventGroupSetBits(s4532390_CAG_EventGroup, S2_BIT);
        break;
        case 3:
            xEventGroupSetBits(s4532390_CAG_EventGroup, S1_BIT);
        break;
        case 4:
            xEventGroupSetBits(s4532390_CAG_EventGroup, MS500_BIT);
        break;
    }
    
}

void s4532390_CAG_Joystick_Task() {

    JoystickMessage message;

    for (;;) {
        if (xQueueReceive(s4532390_JoystickQueue, &message, 10)) {

            debug_printf("X: %d, Y: %d\r\n", message.joystick_x, message.joystick_y);

            strcpy(cellColour, selectColour(message.joystick_x));
            setUpdateTimer(message.joystick_y);

            if (ZSemaphore != NULL) {	/* Check if semaphore exists */

            /* See if we can obtain the semaphore. If the semaphore is not available
                wait 10 ticks to see if it becomes free. */
                
                if( xSemaphoreTake(ZSemaphore, 10) == pdTRUE) {

                    if (s4532390_debounceJoystickZ()) {
                        //Toggles variable if semaphore is obtained
                        xEventGroupSetBits(s4532390_CAG_EventGroup, CLEAR_GRID_BIT);
                    }
                } 
            }
        }

        vTaskDelay(100);
    }
}


void s4532390_CAG_Joystick_Init() {

    s4532390_hal_joystick_init();
    s4532390_os_joystick_init();
    xTaskCreate( (void *) &s4532390_CAG_Joystick_Task, (const signed char *) "CAG_JOYSTICK", CAG_JOYSTICK_STACK_SIZE, NULL, CAG_JOYSTICK_TASK_PRIORITY, &CAG_Joystick_Task_Handle);
}


void s4532390_CAG_Joystick_Deinit() {

    s4532390_os_joystick_deinit();
    vTaskDelete(CAG_Joystick_Task_Handle);
}

