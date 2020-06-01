/**
*********************
*********************
*@file mylib/keypad/s4532390_hal_keypad.c
*@author Robert Francis - 45323906
*@date 18/04/2020
*@brief Keypad Hal Library
*REFERENCE:
csse3010_mylib_hal_keypad.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
*void s4532390_hal_keypad_init();
*void s4532390_hal_keypad_deinit();
*void s4532390_hal_keypad_fsmprocessing();
*void keypad_gpio_init();
*void keypad_writecol(int colval);
*int keypad_readrow();
*int s4532390_hal_keypad_read_status();
*int s4532390_hal_keypad_read_key();
*char s4532390_hal_keypad_read_ascii();
*********************
*********************
**/

#include "debug_printf.h"
#include "board.h"
#include "processor_hal.h"
#include "s4532390_hal_keypad.h"


/*********
*DEFINTIONS
**********/

//State machine states
#define INIT_STATE 0
#define COL1_STATE 1
#define COL2_STATE 2
#define COL3_STATE 3
#define COL4_STATE 4
#define ROWSCAN_STATE 5
#define DEINIT_STATE 6

//Row states
#define ROW_EMPTY_STATE 15
#define ROW1_STATE 14
#define ROW2_STATE 13
#define ROW3_STATE 11
#define ROW4_STATE 7

//Set pins to true
#define keypad_col1() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL1PINPORT, S4532390_HAL_KEYPAD_COL1PIN, 0x01)
#define keypad_col2() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL2PINPORT, S4532390_HAL_KEYPAD_COL2PIN, 0x01)
#define keypad_col3() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL3PINPORT, S4532390_HAL_KEYPAD_COL3PIN, 0x01)
#define keypad_col4() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL4PINPORT, S4532390_HAL_KEYPAD_COL4PIN, 0x01)

//Set pins false
#define keypad_col1_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL1PINPORT, S4532390_HAL_KEYPAD_COL1PIN, 0x00)
#define keypad_col2_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL2PINPORT, S4532390_HAL_KEYPAD_COL2PIN, 0x00)
#define keypad_col3_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL3PINPORT, S4532390_HAL_KEYPAD_COL3PIN, 0x00)
#define keypad_col4_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL4PINPORT, S4532390_HAL_KEYPAD_COL4PIN, 0x00)

/*********
*GLOBALS
**********/

//Stores current and previous keypad states
int KeypadFsmCurrentstate, KeypadFsmLastState;

//1 if key pressed, 0 otherwise
int KeypadStatus;

//Stores current keypad value
unsigned char KeypadValue;

//Toggle variable so 2 characters are printed per line
unsigned char KeypadToggle;

int statusCount;

/**
*@brief Set the state of the keypadFSM to INIT state
*@param None
*@retval None
*/
void s4532390_hal_keypad_init() {

    KeypadFsmCurrentstate = INIT_STATE;
}


/**
*@brief Set the state of the keypadFSM to DEINIT state
*@param None
*@retval None
*/
void s4532390_hal_keypad_deinit() {

    KeypadFsmCurrentstate = DEINIT_STATE;
}


// void keypad_writecol(uint8_t colval) {
//     for (int i = 0; c < 4; c++) {
//         HAL_GPIO_WritePin(COL_PIN_PORTS[i], COL_PINS[i], colval & (1 << i));
//     }
// }

/**
*@brief Converts column and row to hexadecimal value of keypad
*@param column and row - column of keypad and row of keypad
*@retval hexadecimal value of key pressed
*/
int get_keypad_num(int column, int row) {

    switch (column) {
        case COL1_STATE:

            switch(row) {
                case ROW1_STATE:

                    return 1;
                case ROW2_STATE:

                    return 4;
                case ROW3_STATE:

                    return 7;
                case ROW4_STATE:

                    return 0;
            }

        case COL2_STATE:

            switch(row) {
                case ROW1_STATE:

                    return 2;
                case ROW2_STATE:

                    return 5;
                case ROW3_STATE:

                    return 8;
                case ROW4_STATE:

                    return 15;
            }

        case COL3_STATE:

            switch(row) {
                case ROW1_STATE:

                    return 3;
                case ROW2_STATE:

                    return 6;
                case ROW3_STATE:

                    return 9;
                case ROW4_STATE:
                
                    return 14;
            }

        case COL4_STATE:

            switch(row) {
                case ROW1_STATE:

                    return 10;
                case ROW2_STATE:

                    return 11;
                case ROW3_STATE:

                    return 12;
                case ROW4_STATE:

                    return 13;
            }
    }
}


/**
*@brief Scans then prints the keys pressed on the keypad
*@param None
*@retval None
*/
void handle_rowscan() {

    //Scans row pins
    int tempRow = keypad_readrow();

    //Leaves if nothing pressed
    if (tempRow == ROW_EMPTY_STATE) {
        ++statusCount;

        if (statusCount % 4 == 0) {
            KeypadStatus = 0;
        }
        return;
    }



    //Converts into a hex value
    unsigned char temp = get_keypad_num(KeypadFsmLastState, tempRow);
    
    //Prints characters 2 at a time
    if (KeypadToggle) {

        KeypadToggle = 0;
    } else {

        KeypadToggle = 1;
    }
    
    //Stores the last value pressed
    KeypadValue = temp;
}


/**
*@brief Keypad FSM processing loop
*@param None
*@retval None
*/
void s4532390_hal_keypad_fsmprocessing() {

    //Switch statement for the state machine
    switch(KeypadFsmCurrentstate) {

        case INIT_STATE:

            keypad_gpio_init();
            KeypadFsmLastState = INIT_STATE;
            KeypadFsmCurrentstate = COL1_STATE;
            break;   

        case ROWSCAN_STATE:

            handle_rowscan();
            KeypadFsmCurrentstate = (KeypadFsmLastState % 4) + 1;
            break;

        case DEINIT_STATE:

            s4532390_hal_keypad_deinit();
            break;

        default:

            keypad_writecol(KeypadFsmCurrentstate);
            KeypadFsmLastState = KeypadFsmCurrentstate;
            KeypadFsmCurrentstate = ROWSCAN_STATE;
            break;
    }
}


/**
*@brief Initalise all GPIO pins
*@param None
*@retval None
*/
void keypad_gpio_init() {

    GPIO_InitTypeDef  GPIO_InitStructure;

    //Sets pins clocks
    S4532390_HAL_KEYPAD_COL1PINCLK();
    S4532390_HAL_KEYPAD_COL2PINCLK();
    S4532390_HAL_KEYPAD_COL3PINCLK();
    S4532390_HAL_KEYPAD_COL4PINCLK();
    S4532390_HAL_KEYPAD_ROW1PINCLK();
    S4532390_HAL_KEYPAD_ROW2PINCLK();
    S4532390_HAL_KEYPAD_ROW3PINCLK();
    S4532390_HAL_KEYPAD_ROW4PINCLK();
        
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
    GPIO_InitStructure.Pull = GPIO_PULLUP;			//Enable pull up
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency

    //Sets output pins and initialises them
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL1PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL1PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL2PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL2PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL3PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL3PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL4PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL4PINPORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable pull down

    //Sets input pins and initialises them
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_ROW1PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_ROW1PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_ROW2PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_ROW2PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_ROW3PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_ROW3PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_ROW4PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_ROW4PINPORT, &GPIO_InitStructure);
}


/**
*@brief Reads data from row pins
*@param None
*@retval 4 bits - low for row that is on
*/
int keypad_readrow() {

    int out = ROW_EMPTY_STATE;
    

    if (!HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW1PINPORT, S4532390_HAL_KEYPAD_ROW1PIN)) {
        statusCount = 0;
        KeypadStatus = 1;
        return out - (0x01);
    }
    if (!HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW2PINPORT, S4532390_HAL_KEYPAD_ROW2PIN)) {
        statusCount = 0;
        KeypadStatus = 1;
        return out - (0x01 << 1);
    } 
    if (!HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW3PINPORT, S4532390_HAL_KEYPAD_ROW3PIN)) {
        statusCount = 0;
        KeypadStatus = 1;
        return out - (0x01 << 2);
    } 
    if (!HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW4PINPORT, S4532390_HAL_KEYPAD_ROW4PIN)) {
        statusCount = 0;
        KeypadStatus = 1;
        return out - (0x01 << 3);
    } 

    

    return out;
}


/**
*@brief Turns a row pin on
*@param colval - 1 for row 1, 2 for row 2, etc (done as it allows state variable to be used)
*@retval None
*/
void keypad_writecol(int colval) {

    //Turns columns off first
    keypad_col1();
    keypad_col2();
    keypad_col3();
    keypad_col4();
    
    switch (colval) {
        
        case COL1_STATE:

            keypad_col1_off();
            break;

        case COL2_STATE:

            keypad_col2_off();
            break;

        case COL3_STATE:

            keypad_col3_off();
            break;

        case COL4_STATE:

            keypad_col4_off();
            break;
    }

}


/**
*@brief Return the KeypadStatus variable
*@param None
*@retval 1 if keypad on, 0 if off
*/
int s4532390_hal_keypad_read_status() {

    return KeypadStatus;
}


/**
*@brief Return the current hexadecimal value of the keypad
*@param None
*@retval Hexadecimal from 0 - 15
*/
int s4532390_hal_keypad_read_key() {

    return KeypadValue;
}


/**
*@brief Return the current ASCII value of the keypad
*@param None
*@retval Ascii from 0 - 9 and A - F
*/
char s4532390_hal_keypad_read_ascii() {

    return (KeypadValue <= 9) ? (KeypadValue + '0') : (KeypadValue + 'A' + 10);
}