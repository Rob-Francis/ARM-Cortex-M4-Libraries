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

#ifndef KEYPAD_HEADER_H
#define KEYPAD_HEADER_H

#include "s4532390_hal_mylib_configure.h"

void s4532390_hal_keypad_init();

void s4532390_hal_keypad_deinit();

void s4532390_hal_keypad_fsmprocessing();

void keypad_gpio_init();

void keypad_writecol(int colval);

int keypad_readrow();

int s4532390_hal_keypad_read_status();

int s4532390_hal_keypad_read_key();

char s4532390_hal_keypad_read_ascii();

extern int KeypadStatus;

#ifndef S4532390_HAL_KEYPAD_COL1PIN
#define S4532390_HAL_KEYPAD_COL1PIN BRD_D0_PIN
#define S4532390_HAL_KEYPAD_COL2PIN BRD_D0_PIN
#define S4532390_HAL_KEYPAD_COL3PIN BRD_D0_PIN
#define S4532390_HAL_KEYPAD_COL4PIN BRD_D0_PIN
#endif

#ifndef S4532390_HAL_KEYPAD_COL1PINPORT
#define S4532390_HAL_KEYPAD_COL1PINPORT BRD_D0_GPIO_PORT
#define S4532390_HAL_KEYPAD_COL2PINPORT BRD_D0_GPIO_PORT
#define S4532390_HAL_KEYPAD_COL3PINPORT BRD_D0_GPIO_PORT
#define S4532390_HAL_KEYPAD_COL4PINPORT BRD_D0_GPIO_PORT
#endif

#ifndef S4532390_HAL_KEYPAD_COL1PINCLK
#define S4532390_HAL_KEYPAD_COL1PINCLK __BRD_D0_GPIO_CLK
#define S4532390_HAL_KEYPAD_COL2PINCLK __BRD_D0_GPIO_CLK
#define S4532390_HAL_KEYPAD_COL3PINCLK __BRD_D0_GPIO_CLK
#define S4532390_HAL_KEYPAD_COL4PINCLK __BRD_D0_GPIO_CLK
#endif

#ifndef S4532390_HAL_KEYPAD_ROW1PIN
#define S4532390_HAL_KEYPAD_ROW1PIN BRD_D0_PIN
#define S4532390_HAL_KEYPAD_ROW2PIN BRD_D0_PIN
#define S4532390_HAL_KEYPAD_ROW3PIN BRD_D0_PIN
#define S4532390_HAL_KEYPAD_ROW4PIN BRD_D0_PIN
#endif

#ifndef S4532390_HAL_KEYPAD_ROW1PINPORT
#define S4532390_HAL_KEYPAD_ROW1PINPORT BRD_D0_GPIO_PORT
#define S4532390_HAL_KEYPAD_ROW2PINPORT BRD_D0_GPIO_PORT
#define S4532390_HAL_KEYPAD_ROW3PINPORT BRD_D0_GPIO_PORT
#define S4532390_HAL_KEYPAD_ROW4PINPORT BRD_D0_GPIO_PORT
#endif

#ifndef S4532390_HAL_KEYPAD_ROW1PINCLK
#define S4532390_HAL_KEYPAD_ROW1PINCLK __BRD_D0_GPIO_CLK
#define S4532390_HAL_KEYPAD_ROW2PINCLK __BRD_D0_GPIO_CLK
#define S4532390_HAL_KEYPAD_ROW3PINCLK __BRD_D0_GPIO_CLK
#define S4532390_HAL_KEYPAD_ROW4PINCLK __BRD_D0_GPIO_CLK
#endif

#endif