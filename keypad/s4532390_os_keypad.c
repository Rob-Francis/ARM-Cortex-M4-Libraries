#include "s4532390_hal_keypad.h"
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "debug_printf.h"

#include "s4532390_os_keypad.h"

#define KEYPAD_PRIORITY ( tskIDLE_PRIORITY + 6 )
#define KEYPAD_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE * 5 )

EventGroupHandle_t s4532390_keypadEventGroup;

int KeypadStatus;

void s4532390_OS_Keyad_Init() {

    xTaskCreate( (void *) &s4532390_Keypad_Task, (const signed char *) "KEYPAD", KEYPAD_TASK_STACK_SIZE, NULL, KEYPAD_PRIORITY, NULL );

    s4532390_keypadEventGroup = xEventGroupCreate();
}

void s4532390_Keypad_Task() {

    for (;;) {

        s4532390_hal_keypad_fsmprocessing();

        if (KeypadStatus) {

            xEventGroupSetBits(s4532390_keypadEventGroup, 1 << s4532390_hal_keypad_read_key());
            BRD_LEDGreenOn();
        } else {

            BRD_LEDGreenOff();
        }

        

        vTaskDelay(10);

    }
}