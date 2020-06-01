/**
*********************
*********************
*@file mylib/project/s4532390_cag_keypad_mnemonic.c
*@author Robert Francis - 45323906
*@date 1/06/2020
*@brief CAG Keypad Mnemonic library
*REFERENCE:
csse3010_project.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_cag_keypad_mnemonic_task();
void s4532390_cag_keypad_mnemonic_init();
*********************
*********************
**/

#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "s4532390_cag_keypad_mnemonic.h"
#include "s4532390_cag_pushbutton.h"
#include "s4532390_cag_simulator.h"
#include "s4532390_cag_joystick.h"

//DEFINITIONS
#define CAG_KEYPAD_MNEMONIC_PRIORITY (tskIDLE_PRIORITY + 5)
#define CAG_KEYPAD_MNEMONIC_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2)
#define NUM_KEYPAD_BITS 16
#define MNEMONIC_KEYPAD_DELAY 50

//Assigns values to bit positions
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

//For state machine
#define IDLE_STATE 0
#define ENTER_CHAR_STATE 1
#define STL_STATE 2
#define OSC_STATE 3
#define GLD_STATE 4
#define DLT_STATE 5
#define CRE_STATE 6

//EXTERNAL VARIABLES
EventGroupHandle_t s4532390_cagEventGroup; //General event group
EventGroupHandle_t s4532390_keypadEventGroup; //Keypad group
QueueHandle_t s4532390_cagSimulatorQueue; //Simulator queue

//GLOBALS
caMessage_t currentMessage; // Message to be sent
int currentState = IDLE_STATE; //State machine state
int cyclesInState; //How many times cycled through current state


/**
*@brief Increases character by 1 according to telephone keypad
*@param  in - character being increased
*@retval character 1 incremented
*/
char increment_character(char in) {

    //Checks for those with 3 in row
    if (in == 'C' || in == 'F' || in == 'I' || in == 'L' || in == 'O' || in == 'V') {

        return in - 2;
    } else if (in == 'S' || in == 'Z') { //4 in row

        return in - 3;
    } else {

        return in + 1;
    }
}


/**
*@brief Checks if characters are on the same button
*@param  old - character being stored, new - character just pressed
*@retval 1 for same button, 0 not same
*/
int in_range(char old, char new) {

    int difference = old - new;

    //Checks buttons with 3 in range
    if (new == 'A' || new == 'D' || new == 'G' || new == 'J' || new == 'M' || new == 'T') {

        return (difference <= 2 && difference >= 0) ? 1 : 0;
    } else if (new == 'P' || new == 'W') { // Checks those with 4
        
        return (difference <= 3 && difference >= 0) ? 1 : 0;
    } else {

        return 0;
    }
}


/**
*@brief Finds how many along the bit is for the event bits
*@param  bits - Event bits from keypad
*@retval number of bits along
*/
char get_bit_value(EventBits_t bits) {

    for (int i = 0; i < NUM_KEYPAD_BITS; ++i) {
        
        if (bits & 0x01) {
            
            return i;
        }

        bits = bits >> 1;
    }
}


/**
*@brief Converts bits to the character they represent
*@param  bits - Event bits from keypad
*@retval character on keypad, 0 if invalid
*/
char convert_to_char_value(EventBits_t bits) {

    //Finds how many along the bit is
    char test = get_bit_value(bits);

    //Returns value
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
        case MNE_HASH:
            return '#';
        case MNE_ASTERIX:
            return '*';
        default:
            return 0;        
    }
}


/**
*@brief Checks if the character is a valid type
*@param  in - character just pressed
*@retval 1 if valid, 0 otherwise
*/
int is_valid_type(char in) {

    //Types must be 0 - 2 ( A = 2 )
    if (in == '0' || in == '1' || in == 'A') {

        return 1;
    }
}


/**
*@brief Checks if character is a valid subgrid value
*@param  in - character just pressed
*@retval 1 if valid, 0 otherwise
*/
int is_valid_sub_grid(char in) {

    //Subgrid must be from 0 - 4
    if (in == '0' || in == '1' || in == 'A' || in == 'D' || in == 'G') {

        return 1;
    } else {

        return 0;
    }
}


/**
*@brief Converts the character to a numeric value on keypad
*@param  in - character just pressed
*@retval value represented on keypad
*/
int convert_to_numeric(char in) {

    if (in == '0' || in == '1') {

        return in - '0'; // 0 and 1 don't represent letters
    } else if (in == 'A') {

        return 2;
    } else if(in == 'D') {

        return 3;
    } else if (in == 'G') {

        return 4;
    } else { // Converts up to 4

        return 0;
    }
}


/**
*@brief Handles deleting a driver
*@param  in - character just pressed
*@retval None
*/
void handle_delete_state(char in) {

    ++cyclesInState;

    if (((cyclesInState == 1) && (in == 'L')) ||
        ((cyclesInState == 2) && (in == 'T'))) {

        //Continues
    } else if (cyclesInState == 3) {

        if (in == '0') {
            
            //Simulator if 0
            s4532390_cag_simulator_deinit();
        } else if (in == '1') {

            //Joystick if 1
            s4532390_cag_joystick_deinit();
        }

        //Returns to idle
        cyclesInState = 0;
        currentState = IDLE_STATE;  
        
    } else {

        //Returns to idle if wrong character entered
        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}


/**
*@brief Handles creating a driver
*@param  in - character just pressed
*@retval None
*/
void handle_create_state(char in) {

    ++cyclesInState;

    if (((cyclesInState == 1) && (in == 'R')) ||
        ((cyclesInState == 2) && (in == 'E'))) {
        
        //Continue
    } else if (cyclesInState == 3) {

        if (in == '0') {

            s4532390_cag_simulator_init();
        } else if (in == '1') {

            s4532390_cag_joystick_init();
        }
          
        cyclesInState = 0;
        currentState = IDLE_STATE;  
        

    } else {

        //Returns to idle if wrong character entered
        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}

/**
*@brief Handles creating a glider lifeform
*@param  in - character just pressed
*@retval None
*/
void handle_glider_state(char in) {

    ++cyclesInState;

    if (((cyclesInState == 1) && (in == 'L')) ||
        ((cyclesInState == 2) && (in == 'D'))) {
        
        //Continue
    } else if (cyclesInState == 3 && is_valid_sub_grid(in)) {
        
        currentMessage.cell_x = 3 * convert_to_numeric(in); //Gets x value
    } else if (cyclesInState == 4 && is_valid_sub_grid(in)) {

        currentMessage.cell_y = 3 * convert_to_numeric(in); //Gets y value
        currentMessage.type = SPACE_SHIP; // Type will always be space ship

        //Sends to simulator then resets to idle
        xQueueSendToBack(s4532390_cagSimulatorQueue, &currentMessage, 20);

        cyclesInState = 0;
        currentState = IDLE_STATE;
    } else {
        
        //Returns to idle if wrong character entered
        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
}


/**
*@brief Handles creating an oscillator lifeform
*@param  in - character just pressed
*@retval None
*/
void handle_oscillator_state(char in) {

    ++cyclesInState;

    if (((cyclesInState == 1) && (in == 'S')) ||
        ((cyclesInState == 2) && (in == 'C'))) {
        
        //Continue
    } else if (cyclesInState == 3 && is_valid_type(in)) {

        //Gets the type of oscillator
        currentMessage.type = OSCILLATOR | convert_to_numeric(in); 
    } else if (cyclesInState == 4 && is_valid_sub_grid(in)) {

        currentMessage.cell_x = 3 * convert_to_numeric(in); //Gets x value
    } else if (cyclesInState == 5 && is_valid_sub_grid(in)) {

        currentMessage.cell_y = 3 * convert_to_numeric(in); //Gets y value

        //Sends to simulator and resets
        xQueueSendToBack(s4532390_cagSimulatorQueue, &currentMessage, 20);

        cyclesInState = 0;
        currentState = IDLE_STATE;
    } else {

        //Returns to idle if wrong character entered
        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}


/**
*@brief Handles creating a still lifeform
*@param  in - character just pressed
*@retval None
*/
void handle_still_state(char in) {

    ++cyclesInState;

    if (((cyclesInState == 1) && (in == 'T')) ||
        ((cyclesInState == 2) && (in == 'L'))) {

        //Continue
    } else if (cyclesInState == 3 && is_valid_type(in)) {
        
        //Gets type of still life
        currentMessage.type = STILL_LIFE | convert_to_numeric(in);
    } else if (cyclesInState == 4 && is_valid_sub_grid(in)) {
        
        currentMessage.cell_x = 3 * convert_to_numeric(in);
    } else if (cyclesInState == 5 && is_valid_sub_grid(in)) {

        currentMessage.cell_y = 3 * convert_to_numeric(in);
        
        //Sends to simulator queue and resets
        xQueueSendToBack(s4532390_cagSimulatorQueue, &currentMessage, 20);

        cyclesInState = 0;
        currentState = IDLE_STATE;
    } else {
        
        //Returns to idle if wrong character entered
        cyclesInState = 0;
        currentState = IDLE_STATE;
    }
    
}


/**
*@brief Handles the idle state of the machine - waiting for a valid character
*@param  in - character just pressed
*@retval None
*/
void handle_idle_state(char in) {

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
            xEventGroupSetBits(s4532390_cagEventGroup, START_BIT);
        break;
        case '#':
            xEventGroupSetBits(s4532390_cagEventGroup, STOP_BIT);
        break;
        case '0':
            xEventGroupSetBits(s4532390_cagEventGroup, CLEAR_GRID_BIT);
        break;
        case 'D':
            currentState = DLT_STATE;
        break;
        case 'C':
            currentState = CRE_STATE;
        break;
        default:
            currentState = IDLE_STATE; // Not a valid char - back to idle
        break;
    }
}


/**
*@brief The mnemonic state machine
*@param  in - character just pressed
*@retval None
*/
void mnemoic_state_machine(char in) {

    switch (currentState) {
        case IDLE_STATE:
            handle_idle_state(in); // Idle resets the cycles to 0
            cyclesInState = 0;
        break;
        case STL_STATE:
            handle_still_state(in);
        break;
        case OSC_STATE:
            handle_oscillator_state(in);
        break;
        case GLD_STATE:
            handle_glider_state(in);
        break;
        case DLT_STATE:
            handle_delete_state(in);
        break;
        case CRE_STATE:
            handle_create_state(in);
        break;
    }           
}


/**
*@brief Mnemonic keypad task
*@param  None
*@retval None
*/
void s4532390_cag_keypad_mnemonic_task() {

    //Character last pressed and its bit position
    char currentChar = 0;
    char currentBitPosition = 0;
    int previousBits = 0;

    //Last time key was pressed
    unsigned int previousKeyPressTime = 0;
    

    for (;;) {

        if (s4532390_keypadToggle) { // Checks for mnemonic to be on

            BRD_LEDRedOff();
            BRD_LEDBlueOn();

            if (previousKeyPressTime + 1000 < HAL_GetTick() && currentChar != 0) { //If waited more than 1 seconds to press

                mnemoic_state_machine(currentChar); //Puts current character in if waiting for over a second
                currentChar = 0;
            }

            //Gets bits from keypad group
            int keypadBits = xEventGroupWaitBits(s4532390_keypadEventGroup, 0xFFFF, pdTRUE, pdFALSE, 10);

            if (keypadBits != previousBits && keypadBits != 0) { // Prevents a single press doing multiple actions

                char tempChar = convert_to_char_value(keypadBits);

                if (tempChar == '0' || tempChar == '*' || tempChar == '#' || tempChar == '1') { //CHECKS 1 char commands

                    if (currentChar != 0) { //Puts any current char into the state machine

                        mnemoic_state_machine(currentChar);
                    }

                    mnemoic_state_machine(tempChar);
                    currentChar = 0;
                } else if (currentChar == 0) { // if First character pressed

                    currentChar = tempChar;
                } else if (in_range(currentChar, tempChar)) { //If on same button as previous character (and is a letter)

                    currentChar = increment_character(currentChar);
                } else { // If different character from last pressed

                    mnemoic_state_machine(currentChar);
                    currentChar = tempChar;
                }
                
                previousKeyPressTime = HAL_GetTick(); //Records last time
                
            }

            previousBits = keypadBits;
        }

        vTaskDelay(MNEMONIC_KEYPAD_DELAY);
    }
}


/**
*@brief Mnemonic keypad task initialiser
*@param  None
*@retval None
*/
void s4532390_cag_keypad_mnemonic_init() {

    xTaskCreate( (void *) &s4532390_cag_keypad_mnemonic_task, (const signed char *) "MNEMONIC", CAG_KEYPAD_MNEMONIC_TASK_STACK_SIZE, NULL, CAG_KEYPAD_MNEMONIC_PRIORITY, NULL );

}
