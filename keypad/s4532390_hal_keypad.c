/**
*********************
*********************
*@file mylib/keypad/s4532390_hal_keypad.h
*@author Robert Francis - 45323906
*@date 18/04/2020
*@brief Keypad library
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

#define INIT_STATE 0
#define COL1_STATE 1
#define COL2_STATE 2
#define COL3_STATE 3
#define COL4_STATE 4
#define ROWSCAN_STATE 5
#define DEINIT_STATE 6

#define ROW_EMPTY_STATE 15
#define ROW1_STATE 14
#define ROW2_STATE 13
#define ROW3_STATE 11
#define ROW4_STATE 7


#define keypad_col1() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL1PINPORT, S4532390_HAL_KEYPAD_COL1PIN, 0x01)
#define keypad_col2() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL2PINPORT, S4532390_HAL_KEYPAD_COL2PIN, 0x01)
#define keypad_col3() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL3PINPORT, S4532390_HAL_KEYPAD_COL3PIN, 0x01)
#define keypad_col4() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL4PINPORT, S4532390_HAL_KEYPAD_COL4PIN, 0x01)

#define keypad_col1_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL1PINPORT, S4532390_HAL_KEYPAD_COL1PIN, 0x00)
#define keypad_col2_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL2PINPORT, S4532390_HAL_KEYPAD_COL2PIN, 0x00)
#define keypad_col3_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL3PINPORT, S4532390_HAL_KEYPAD_COL3PIN, 0x00)
#define keypad_col4_off() HAL_GPIO_WritePin(S4532390_HAL_KEYPAD_COL4PINPORT, S4532390_HAL_KEYPAD_COL4PIN, 0x00)

int KeypadFsmCurrentstate, KeypadFsmLastState;

int KeypadStatus;
unsigned char KeypadValue;
unsigned char KeypadToggle;


void s4532390_hal_keypad_init() {
    KeypadFsmCurrentstate = INIT_STATE;
    
}

void s4532390_hal_keypad_deinit() {
    // Key
}

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

void handleRowscan() {

    int tempRow = keypad_readrow();

    if (tempRow == ROW_EMPTY_STATE) {
        return;
    }

    unsigned char temp = get_keypad_num(KeypadFsmLastState, tempRow);
    
    if (KeypadToggle) {
        debug_printf("%x%x\r\n", KeypadValue, temp);
        debug_flush();
        KeypadToggle = 0;
    } else {
        KeypadToggle = 1;
    }
    
    KeypadValue = temp;
}

void s4532390_hal_keypad_fsmprocessing() {

    
    switch(KeypadFsmCurrentstate) {
        case INIT_STATE:
            keypad_gpio_init();
            KeypadFsmLastState = INIT_STATE;
            KeypadFsmCurrentstate = COL1_STATE;
            break;        
        case ROWSCAN_STATE:
            handleRowscan();
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

void keypad_gpio_init() {
    GPIO_InitTypeDef  GPIO_InitStructure;

    S4532390_HAL_KEYPAD_COL1PINCLK();
    S4532390_HAL_KEYPAD_COL2PINCLK();
    S4532390_HAL_KEYPAD_COL3PINCLK();
    S4532390_HAL_KEYPAD_COL4PINCLK();
    S4532390_HAL_KEYPAD_ROW1PINCLK();
    S4532390_HAL_KEYPAD_ROW2PINCLK();
    S4532390_HAL_KEYPAD_ROW3PINCLK();
    S4532390_HAL_KEYPAD_ROW4PINCLK();
        
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency


    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL1PIN;			//Pin
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL1PINPORT, &GPIO_InitStructure);	//Initialise Pin
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL2PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL2PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL3PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL3PINPORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = S4532390_HAL_KEYPAD_COL4PIN;
    HAL_GPIO_Init(S4532390_HAL_KEYPAD_COL4PINPORT, &GPIO_InitStructure);

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


int keypad_readrow() {

    int out = ROW_EMPTY_STATE;
    KeypadStatus = 1;

    if (HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW1PINPORT, S4532390_HAL_KEYPAD_ROW1PIN)) {
        return out - (0x01);
    }
    if (HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW2PINPORT, S4532390_HAL_KEYPAD_ROW2PIN)) {
        return out - (0x01 << 1);
    } 
    if (HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW3PINPORT, S4532390_HAL_KEYPAD_ROW3PIN)) {
        return out - (0x01 << 2);
    } 
    if (HAL_GPIO_ReadPin(S4532390_HAL_KEYPAD_ROW4PINPORT, S4532390_HAL_KEYPAD_ROW4PIN)) {
        return out - (0x01 << 3);
    } 

    KeypadStatus = 0;

    return out;
}

void keypad_writecol(int colval) {
    keypad_col1_off();
    keypad_col2_off();
    keypad_col3_off();
    keypad_col4_off();

    switch (colval) {
        case COL1_STATE:
            keypad_col1();
            break;
        case COL2_STATE:
            keypad_col2();
            break;
        case COL3_STATE:
            keypad_col3();
            break;
        case COL4_STATE:
            keypad_col4();
            break;
    }

}


int s4532390_hal_keypad_read_status() {
    return KeypadStatus;
}

int s4532390_hal_keypad_read_key() {
    return KeypadValue;
}

char s4532390_hal_keypad_read_ascii() {
    return (KeypadValue <= 9) ? (KeypadValue + '0') : (KeypadValue + 'A' + 10);
}