#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "s4532390_CAG_Keypad_Mnemonic.h"
#include "s4532390_CAG_PushButton.h"

unsigned char keypadToggle;

#define CAG_KEYPAD_MNEMONIC_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_KEYPAD_MNEMONIC_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

void s4532390_CAG_Keypad_Mnemonic_Task() {

        for (;;) {

            if (keypadToggle) {
                // debug_printf("Grid got semaphore\r\n");
                BRD_LEDRedOff();
                BRD_LEDBlueOn();

            }

            vTaskDelay(100);
        }
}

void s4532390_CAG_Keypad_Mnemonic_Init() {

    xTaskCreate( (void *) &s4532390_CAG_Keypad_Mnemonic_Task, (const signed char *) "MNEMONIC", CAG_KEYPAD_MNEMONIC_TASK_STACK_SIZE, NULL, CAG_KEYPAD_MNEMONIC_PRIORITY, NULL );

}
