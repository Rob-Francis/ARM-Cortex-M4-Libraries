 /** 
 **************************************************************
 * @file mylib/pb/s4532390_hal_pb.c
 * @author Robert Francis - 45323906
 * @date 8/03/20
 * @brief Hal Pushbutton Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4532390_hal_pb_on_init() - initialises the onboard button
 * s4532390_hal_pb_on_isr() - interrupt routine for the button
 * s4532390_hal_pb_on_deinit() - deinitialiser for the button
 * s4532390_hal_pb_iss_init() - initialises the iss
 * s4532390_hal_pb_iss_isr() - iss interrupt routine (moves the light)
 * s4532390_hal_pb_iss_deinit() - deinitialiser for the iss
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

