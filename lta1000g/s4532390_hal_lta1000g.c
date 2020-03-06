#include "s4532390_hal_lta1000g.h"
#include "debug_printf.h"

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

int IOPorts[] = {
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

    // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	//     GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	//     GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
    //     GPIO_InitStructure.Pin = IOPins[0];				//Pin
    //     HAL_GPIO_Init(IOPorts[0], &GPIO_InitStructure);	//Initialise Pin
  	

};

void lta1000g_seg_set(int segment, unsigned char segment_value) {
    HAL_GPIO_WritePin(IOPorts[segment], IOPins[segment], segment_value & 0x01);
}

void s4532390_hal_lta1000g_write(unsigned short value) {
    for (int i = 0; i < NUM_PINS; i++) {
        lta1000g_seg_set(i, 0x01 & (value >> i));
        debug_printf("Value: %d\n\r", 0x01 & (value >> i));	
    }
}
