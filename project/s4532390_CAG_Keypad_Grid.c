#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "s4532390_CAG_Keypad_Grid.h"
#include "s4532390_os_keypad.h"
#include "s4532390_CAG_PushButton.h"
#include "s4532390_CAG_Simulator.h"

EventGroupHandle_t keypadEventGroup;
EventGroupHandle_t CAG_EventGroup;

unsigned char keypadToggle;

#define CAG_KEYPAD_GRID_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_KEYPAD_GRID_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

int subX, subY;


void clearKeypad() {
    subX = 0;
    subY = 0;

}

void handleKeypadBits(EventBits_t keypadBits) {

    int bitSet = 0;

    for (int i = 0; i < 16; ++i) {
        
        if (keypadBits & 0x01) {
            bitSet = i;
            break;
        }

        keypadBits = keypadBits >> 1;
    }


    debug_printf("Bit set %d\r\n", bitSet);
    
    if (bitSet > 0 && bitSet < 10) {


    } else if (bitSet == 10) {

        xEventGroupSetBits(CAG_EventGroup, START_BIT);
    } else if (bitSet == 11) {

        xEventGroupSetBits(CAG_EventGroup, STOP_BIT);
    } else if (bitSet == 12) {
        
        clearKeypad();
        xEventGroupSetBits(CAG_EventGroup, CLEAR_GRID_BIT);
    } else if (bitSet == 0) {

        ++subX;
    } else if (bitSet == 15) {
        
        ++subY;
    }
}

void s4532390_CAG_Keypad_Grid_Task() {

        EventBits_t previousKeypadBits = 0;

        EventBits_t keypadBits;

        for (;;) {

            if (!keypadToggle) {
                // debug_printf("Grid got semaphore\r\n");
                BRD_LEDRedOn();
                BRD_LEDBlueOff();

                keypadBits = xEventGroupWaitBits(keypadEventGroup, 0xFFFF, pdTRUE, pdFALSE, 10);

                if (keypadBits != previousKeypadBits && keypadBits != 0) {
                    handleKeypadBits(keypadBits);
                } 
                
                previousKeypadBits = keypadBits;

                
                


            }

            vTaskDelay(100);
        }
}

void s4532390_CAG_Keypad_Grid_Init() {

    xTaskCreate( (void *) &s4532390_CAG_Keypad_Grid_Task, (const signed char *) "GRID", CAG_KEYPAD_GRID_TASK_STACK_SIZE, NULL, CAG_KEYPAD_GRID_PRIORITY, NULL );

}

