 /** 
 **************************************************************
 * @file mylib/s4532390_hal_pb.c
 * @author Robert Francis - 45323906
 * @date 8/03/20
 * @brief Hal Pushbutton Driver
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
#include "s4532390_hal_pb.h"
#include "s4532390_hal_lta1000g.h"
#include "debug_printf.h"

#define LTA1000G_START (0x03 << 8)
#define LTA1000G_END 0x03
#define LTA1000G_INTERMEDIATE ((0x01 << 9) ^ (0x01))


int direction = 0;
unsigned short lightBarValue = LTA1000G_START;

void s4532390_hal_pb_on_init() {

    GPIO_InitTypeDef GPIO_InitStructure;

	BRD_USER_BUTTON_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin  = BRD_USER_BUTTON_PIN;
	HAL_GPIO_Init(BRD_USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(BRD_USER_BUTTON_EXTI_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(BRD_USER_BUTTON_EXTI_IRQn);
}

void s4532390_hal_pb_on_isr() {
    BRD_LEDToggle(BRD_GREEN_LEDMASK|BRD_BLUE_LEDMASK|BRD_RED_LEDMASK);
    direction = ~direction;
}

void s4532390_hal_pb_on_deinit() {
	BRD_USER_BUTTON_GPIO_CLK_DISABLE();
	HAL_GPIO_DeInit(BRD_USER_BUTTON_GPIO_PORT, BRD_USER_BUTTON_PIN);
}

void s4532390_hal_pb_iss_init() {
	
	__BRD_A0_GPIO_CLK();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin  = BRD_A0_PIN;
	HAL_GPIO_Init(BRD_A0_GPIO_PORT, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(BRD_A0_EXTI_IRQ, 12, 0);
	HAL_NVIC_EnableIRQ(BRD_A0_EXTI_IRQ);

}

void s4532390_hal_pb_iss_isr() {
	
	s4532390_hal_lta1000g_write(lightBarValue);
	debug_printf("Lighbar Value: %x", lightBarValue);

	if ((lightBarValue >= LTA1000G_START && direction) || (lightBarValue <= LTA1000G_END && !direction)) {
          lightBarValue = LTA1000G_INTERMEDIATE;
      } else if (lightBarValue == LTA1000G_INTERMEDIATE) {

          if (!direction) {
            lightBarValue = LTA1000G_START;
           } else {
            lightBarValue = LTA1000G_END;
           }
          
      } else if (direction) {
          lightBarValue = lightBarValue << 1;
      } else {
          lightBarValue = lightBarValue >> 1;
      }
}

void s4532390_hal_pb_iss_deinit() {
	HAL_GPIO_DeInit(BRD_A0_GPIO_PORT, BRD_A0_PIN);
}

