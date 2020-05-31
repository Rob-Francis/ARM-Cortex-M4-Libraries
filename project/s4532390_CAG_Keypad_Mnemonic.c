#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "s4532390_CAG_Keypad_Mnemonic.h"
#include "s4532390_CAG_PushButton.h"
#include "s4532390_CAG_Simulator.h"

#define CAG_KEYPAD_MNEMONIC_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_KEYPAD_MNEMONIC_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)

#define MNE_ZERO 0
#define MNE_A 2
#define MNE_D 3
#define MNE_G 4
#define MNE_J 5
#define MNE_M 6
#define MNE_P 7
#define MNE_T 8
#define MNE_W 9

#define MNE_HASH 13
#define MNE_ASTERIX 15

#define IDLE_STATE 0
#define ENTER_CHAR_STATE 1
#define STL_STATE 2
#define OSC_STATE 3
#define GLD_STATE 4
#define DLT_STATE 5
#define CRE_STATE 6

EventGroupHandle_t s4532390_CAG_EventGroup;
EventGroupHandle_t s4532390_keypadEventGroup;
QueueHandle_t s4532390_CAG_Queue;

caMessage_t currentMessage;

int currentState = IDLE_STATE;
int cyclesInState;


void runCommand() {


    // for (int i = 0; i < 6; ++i) {
    //     debug_printf("%c", command[i]);
    // }

    // debug_printf("/r/n");
}

// void addToCommand(char currentChar) {

//     for (int i = 0; i < 6; ++i) {
//         if (command[i] == 0) {
//             command[i] = currentChar;
//             return;
//         }
//     }



// }

char incrementCharacter(char in) {

    debug_printf("Increment\r\n");

    if (in == 'C' || in == 'F' || in == 'I' || in == 'L' || in == 'O' || in == 'V') {

        return in - 2;
    } else if (in == 'S' || in == 'Z') { 

        return in - 3;
    } else {

        return in + 1;
    }
}


char getBitValue(EventBits_t bits) {

    for (int i = 0; i < 16; ++i) {
        
        if (bits & 0x01) {
            return i;
        }

        bits = bits >> 1;
    }
}


char convertToCharValue(EventBits_t bits) {
    char test = getBitValue(bits);

    switch (test) {
        case MNE_ZERO:
            return '0';
        case 1:
            return '1';
        case MNE_A:
            return 'A';
        case MNE_D:
            return 'D';
        case MNE_G:
            return 'G';
        case MNE_J:
            return 'J';
        case MNE_M:
            return 'M';
        case MNE_P:
            return 'P';
        case MNE_T:
            return 'T';
        case MNE_W:
            return 'W';
        default:
            return 0;        
    }
}

int isNumeric(char in) {
    return (in >= '0' && in <= '9') ? 1 : 0;
}


void handleDeleteState(char in) {

    if (((cyclesInState == 1) && (in == 'L')) ||
        ((cyclesInState == 2) && (in == 'T'))) {
        
        ++cyclesInState;
    } else if (cyclesInState == 3) {

        if (in == '0') {

            s4532390_CAG_Simulator_Deinit();

        } else if (in == '1') {

            
        }

        cyclesInState = 0;
        currentState = IDLE_STATE;  
        

    } else {

        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}

void handleCreateState(char in) {

    if (((cyclesInState == 1) && (in == 'R')) ||
        ((cyclesInState == 2) && (in == 'E'))) {
        
        ++cyclesInState;
    } else if (cyclesInState == 3) {

        if (in == '0') {

            s4532390_CAG_Simulator_Init();

        } else if (in == '1') {

            
        }
          
        cyclesInState = 0;
        currentState = IDLE_STATE;  
        

    } else {

        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}


void handleGliderState(char in) {

    if (((cyclesInState == 1) && (in == 'L')) ||
        ((cyclesInState == 2) && (in == 'D'))) {
        
        ++cyclesInState;
    } else if (cyclesInState == 3 && isNumeric(in)) {
        
        currentMessage.cell_x = in - '0';
    } else if (cyclesInState == 4 && isNumeric(in)) {

        currentMessage.cell_y = in - '0';
        currentMessage.type = SPACE_SHIP;
        xQueueSendToBack(s4532390_CAG_Queue, &currentMessage, 20);

        cyclesInState = 0;
        currentState = IDLE_STATE;
    } else {
        
        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
}


void handleOscillatorState(char in) {

    if (((cyclesInState == 1) && (in == 'S')) ||
        ((cyclesInState == 2) && (in == 'C'))) {
        
        ++cyclesInState;
    } else if (cyclesInState == 3) {

        if (in == '0' || in == '1' || in == '2') {

            currentMessage.type = OSCILLATOR | (in - '0');
        } else {
            
            cyclesInState = 0;
            currentState = IDLE_STATE;  
        }

    } else if (cyclesInState == 4 && isNumeric(in)) {

        currentMessage.cell_x = in - '0';
    } else if (cyclesInState == 4 && isNumeric(in)) {

        currentMessage.cell_y = in - '0';
        
        xQueueSendToBack(s4532390_CAG_Queue, &currentMessage, 20);

        cyclesInState = 0;
        currentState = IDLE_STATE;
    } else {

        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}

void handleStillState(char in) {

    if (((cyclesInState == 1) && (in == 'T')) ||
        ((cyclesInState == 2) && (in == 'L'))) {
        
        ++cyclesInState;
    } else if (cyclesInState == 3) {
        
        if (in == '0' || in == '1' || in == '2') {

            currentMessage.type = STILL_LIFE | (in - '0');
        } else {
            
            cyclesInState = 0;
            currentState = IDLE_STATE;  
        }
    } else if (cyclesInState == 4 && isNumeric(in)) {
        
        currentMessage.cell_x = in - '0';
    } else if (cyclesInState == 5 && isNumeric(in)) {

        currentMessage.cell_y = in - '0';
        currentMessage.type = LIVE_CELL;
        xQueueSendToBack(s4532390_CAG_Queue, &currentMessage, 20);


        cyclesInState = 0;
        currentState = IDLE_STATE;
    } else {
        
        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}

void handleIdleState(char in) {

    ++cyclesInState;

    switch (in) {
        case 'S':
            currentState = STL_STATE;
        break;
        case 'O':
            currentState = OSC_STATE;
        break;
        case 'G':
            currentState = GLD_STATE;
        break;
        case '*':
            xEventGroupSetBits(s4532390_CAG_EventGroup, START_BIT);
        break;
        case '#':
            xEventGroupSetBits(s4532390_CAG_EventGroup, STOP_BIT);
        break;
        case '0':
            xEventGroupSetBits(s4532390_CAG_EventGroup, CLEAR_GRID_BIT);
        break;
        case 'D':
            currentState = DLT_STATE;
        break;
        case 'C':
            currentState = CRE_STATE;
        break;
        default:
            currentState = IDLE_STATE;
        break;
    }
}

void mnemoicStateMachine(char in) {

    // debug_printf("Character: %c\r\n", in);
    

    switch (currentState) {
        case IDLE_STATE:
            handleIdleState(in);
            cyclesInState = 0;
        break;
        case STL_STATE:
            handleStillState(in);
        break;
        case OSC_STATE:
            handleOscillatorState(in);
        break;
        case GLD_STATE:
            handleGliderState(in);
        break;
        case DLT_STATE:
            handleDeleteState(in);
        break;
        case CRE_STATE:
            handleCreateState(in);
        break;
    }
            
}

void s4532390_CAG_Keypad_Mnemonic_Task() {

    char currentChar = 0;
    char currentBitPosition = 0;

    unsigned int previousKeyPressTime = 0;
    int previousBits = 0;

    for (;;) {

        if (s4532390_keypadToggle) {

            BRD_LEDRedOff();
            BRD_LEDBlueOn();

            if (previousKeyPressTime + 1000 < HAL_GetTick() && currentChar != 0) { //If waited more than 0.5seconds to press

                // debug_printf("time\r\n");
                mnemoicStateMachine(currentChar);
                currentChar = 0;
            }

            int keypadBits = xEventGroupWaitBits(s4532390_keypadEventGroup, 0xFFFF, pdTRUE, pdFALSE, 10);

            if (keypadBits != previousBits && keypadBits != 0) {

                char tempChar = convertToCharValue(keypadBits);
                // debug_printf("Char: %c\r\n", tempChar);

                if (tempChar == '0' || tempChar == '*' || tempChar == '#') { //CHECKS 1 char commands

                    mnemoicStateMachine(tempChar);
                    currentChar = 0;
                } else if (tempChar == 0 || tempChar == '1') {
                    //DO NOTHING
                    
                } else if (currentChar == 0) { // if First character pressed

                    currentChar = tempChar;
                } else if (currentChar == tempChar) { //If same as previous character

                    currentChar = incrementCharacter(currentChar);
                } else { // If different character from last pressed

                    // debug_printf("here\r\n");
                    mnemoicStateMachine(currentChar);
                    currentChar = tempChar;
                }
                
                previousKeyPressTime = HAL_GetTick();
                
            }
            previousBits = keypadBits;
            
        }

        vTaskDelay(50);
    }
}

void s4532390_CAG_Keypad_Mnemonic_Init() {

    xTaskCreate( (void *) &s4532390_CAG_Keypad_Mnemonic_Task, (const signed char *) "MNEMONIC", CAG_KEYPAD_MNEMONIC_TASK_STACK_SIZE, NULL, CAG_KEYPAD_MNEMONIC_PRIORITY, NULL );

}
