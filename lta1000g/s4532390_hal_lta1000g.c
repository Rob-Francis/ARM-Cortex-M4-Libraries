 /** 
 **************************************************************
 * @file mylib/s4532390_hal_lta1000g.c
 * @author Robert Francis - 45323906
 * @date 8/03/20
 * @brief Hal Lightbar Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4532390_hal_lta1000g_init() - initialises the lightbar
 * lta1000g_seg_set(int segment, unsigned char segment_value) - sets a single segment of the bar
 * s4532390_hal_lta1000g_write(unsigned short value) - sets the entire lightbar according to value
 *************************************************************** 
 */


#include "board.h"
#include "processor_hal.h"

//COMPILER DEFINITIONS
#define NUM_PINS 10 //Numbers of pins used by lightbar

//GLOBAL VARIABLES
//Array of pins
uint16_t IOPins[] = {
        BRD_D16_PIN,
        BRD_D17_PIN,
        BRD_D18_PIN,
        BRD_D19_PIN,
        BRD_D20_PIN,
        BRD_D21_PIN,
        BRD_D22_PIN,
        BRD_D23_PIN,
        BRD_D24_PIN,
        BRD_D25_PIN
    };

//Array of ports
GPIO_TypeDef* IOPorts[] = {
        BRD_D16_GPIO_PORT,
        BRD_D17_GPIO_PORT,
        BRD_D18_GPIO_PORT,
        BRD_D19_GPIO_PORT,
        BRD_D20_GPIO_PORT,
        BRD_D21_GPIO_PORT,
        BRD_D22_GPIO_PORT,
        BRD_D23_GPIO_PORT,
        BRD_D24_GPIO_PORT,
        BRD_D25_GPIO_PORT
    };

//Initialiser for lighbar - sets all pins to output
void s4532390_hal_lta1000g_init() {

    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //Sets clocks for pins
    __BRD_D16_GPIO_CLK();
    __BRD_D17_GPIO_CLK();
    __BRD_D18_GPIO_CLK();
    __BRD_D19_GPIO_CLK();
    __BRD_D20_GPIO_CLK();
    __BRD_D21_GPIO_CLK();
    __BRD_D22_GPIO_CLK();
    __BRD_D23_GPIO_CLK();
    __BRD_D24_GPIO_CLK();
    __BRD_D25_GPIO_CLK();
    

    for (int i = 0; i < NUM_PINS; ++i) {

        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	    GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
        GPIO_InitStructure.Pin = IOPins[i];				//Pin
        HAL_GPIO_Init(IOPorts[i], &GPIO_InitStructure);	//Initialise Pin
    }

};

//Sets the lightbar for a single segment
void lta1000g_seg_set(int segment, unsigned char segment_value) {

    HAL_GPIO_WritePin(IOPorts[segment], IOPins[segment], segment_value & 0x01); //Writes 1 bit in
}

//Sets lighbar for the whole 10 segments
void s4532390_hal_lta1000g_write(unsigned short value) {
    
    //Loops through 10 times
    for (int i = 0; i < NUM_PINS; ++i) {
        lta1000g_seg_set(i, 0x01 & (value >> i)); //Shifts bits over for each pin
    }
}
