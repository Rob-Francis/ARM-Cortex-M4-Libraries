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
#include "s4532390_CAG_Display.h"

EventGroupHandle_t s4532390_keypadEventGroup;
EventGroupHandle_t s4532390_CAG_EventGroup;
QueueHandle_t s4532390_CAG_Queue;

unsigned short lightbarValue;
unsigned char s4532390_keypadToggle;
unsigned char Grid[GRID_WIDTH][GRID_HEIGHT];

#define CAG_KEYPAD_GRID_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_KEYPAD_GRID_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

int subX, subY;


void clearKeypad() {
    subX = 0;
    subY = 0;
    lightbarValue = lightbarValue & 0b1111000000;

}

void displaySubgridPositions() {
    lightbarValue = (lightbarValue & 0b1111000000) | (subX << 3) | subY;
}

void handleKeypadGridBits(EventBits_t keypadBits) {

    int bitSet = 0;
    caMessage_t message;

    for (int i = 0; i < 16; ++i) {
        
        if (keypadBits & 0x01) {
            bitSet = i;
            break;
        }

        keypadBits = keypadBits >> 1;
    }

    if (bitSet > 0 && bitSet < 10) {

        int xOffset = (bitSet - 1) % 3;
        int yOffset = (bitSet - 1) / 3;


        if (Grid[(subX * 3) + xOffset][(subY * 3) + yOffset] == ALIVE) {
            message.type = DEAD_CELL;
            
        } else {
            message.type = LIVE_CELL;
            
        }

        message.cell_x = (subX * 3) + xOffset;
        message.cell_y = (subY * 3) + yOffset;

        // portENTER_CRITICAL();
        // debug_printf("here4\r\n");
        // portEXIT_CRITICAL();
        // BRD_LEDBlueToggle();
        xQueueSendToBack(s4532390_CAG_Queue, &message, 20);


    } else if (bitSet == 10) {

        xEventGroupSetBits(s4532390_CAG_EventGroup, START_BIT);
    } else if (bitSet == 11) {

        xEventGroupSetBits(s4532390_CAG_EventGroup, STOP_BIT);
    } else if (bitSet == 12) {
        
        clearKeypad();
        xEventGroupSetBits(s4532390_CAG_EventGroup, CLEAR_GRID_BIT);
    } else if (bitSet == 0) {

        if (subX < 5) {
            ++subX;
        }
    } else if (bitSet == 15) {
        
        if (subY < 5) {
            ++subY;
        }
    }

}

void s4532390_CAG_Keypad_Grid_Task() {

        EventBits_t previousKeypadBits = 0;

        EventBits_t keypadBits;

        for (;;) {

            if (!s4532390_keypadToggle) {
                // debug_printf("Grid got semaphore\r\n");
                BRD_LEDRedOn();
                BRD_LEDBlueOff();

                keypadBits = xEventGroupWaitBits(s4532390_keypadEventGroup, 0xFFFF, pdTRUE, pdFALSE, 30);

//                 portENTER_CRITICAL();
//                 debug_printf("Bits : %d\r\n", keypadBits);
// portEXIT_CRITICAL();
                if (keypadBits != previousKeypadBits && keypadBits != 0) {
                    handleKeypadGridBits(keypadBits);
                } 
                
                previousKeypadBits = keypadBits;

                displaySubgridPositions();
            } else {
                clearKeypad();
            }

            
            vTaskDelay(100);
        } 
}

void s4532390_CAG_Keypad_Grid_Init() {

    xTaskCreate( (void *) &s4532390_CAG_Keypad_Grid_Task, (const signed char *) "GRID", CAG_KEYPAD_GRID_TASK_STACK_SIZE, NULL, CAG_KEYPAD_GRID_PRIORITY, NULL );

}

