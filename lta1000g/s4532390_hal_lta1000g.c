#include "board.h"
#include "processor_hal.h"

#define NUM_PINS 10

uint8_t IOPins[] = {
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

uint8_t IOPorts[] = {
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

void s4532390_hal_lta1000g_init() {

    GPIO_InitTypeDef  GPIO_InitStructure;
    

    for (int i = 0; i < NUM_PINS; ++i) {
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	    GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
        GPIO_InitStructure.Pin = IOPins[i];				//Pin
        HAL_GPIO_Init(IOPorts[i], &GPIO_InitStructure);	//Initialise Pin
    }
  	

};

void lta1000g_seg_set(int segment, unsigned char segment_value) {
    HAL_GPIO_WritePin(IOPorts[segment], IOPins[segment], 1 & segment);
    //IOPorts[segment] ^= IOPins[segment];
}

void s4532390_hal_lta1000g_write(unsigned short value) {
    for (int i = 0; i < NUM_PINS; i++) {
        lta1000g_seg_set(i, 1 & (value << i));
    }
}
