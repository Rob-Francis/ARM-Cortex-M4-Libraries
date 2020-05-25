#include "s4532390_hal_keypad.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define KEYPAD_PRIORITY ( tskIDLE_PRIORITY + 5 )
#define KEYPAD_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 2 )

EventGroupHandle_t keypad_EventGroup;

void s4532390_OS_Keyad_Init() {
    xTaskCreate( (void *) &s4532390_Keypad_Task, (const signed char *) "KEYPAD", KEYPAD_TASK_STACK_SIZE, NULL, KEYPAD_PRIORITY, NULL );

    keypad_EventGroup = xEventGroupCreate();
}

void s4532390_Keypad_Task() {

    for (;;) {

        s4532390_hal_keypad_fsmprocessing();

        if (s4532390_hal_keypad_read_status()) {
            BRD_LEDGreenToggle();
        }

        vTaskDelay(50);

    }
}