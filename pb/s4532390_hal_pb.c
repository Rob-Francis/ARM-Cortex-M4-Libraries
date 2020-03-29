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

//COMPILER DEFINITIONS
//Defines for the states of the LTA1000G
#define LTA1000G_START (0x03 << 8)
#define LTA1000G_END 0x03
#define LTA1000G_INTERMEDIATE ((0x01 << 9) ^ (0x01))


//GLOBAL VARIABLES
int direction = 0; //Controls direction of bar - 0 default, other reverse
unsigned short lightBarValue = LTA1000G_START; //Value of the lightbar

//Initialises the hardware for onboard pushbutton
void s4532390_hal_pb_on_init() {

    GPIO_InitTypeDef GPIO_InitStructure;

	BRD_USER_BUTTON_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin  = BRD_USER_BUTTON_PIN;
	HAL_GPIO_Init(BRD_USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

	//Sets priority to 10
	HAL_NVIC_SetPriority(BRD_USER_BUTTON_EXTI_IRQn, 14, 0);
	HAL_NVIC_EnableIRQ(BRD_USER_BUTTON_EXTI_IRQn);
}

//Interrupt for pushbutton just reverses direction
void s4532390_hal_pb_on_isr() {

    direction = ~direction;
}

//Deinitialiser disables onboard button
void s4532390_hal_pb_on_deinit() {

	BRD_USER_BUTTON_GPIO_CLK_DISABLE();
	HAL_GPIO_DeInit(BRD_USER_BUTTON_GPIO_PORT, BRD_USER_BUTTON_PIN);
}

//Initialiser for ISS reads the AO Pin rising edges
void s4532390_hal_pb_iss_init() {
	
	__BRD_A0_GPIO_CLK();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//Reads rising edges
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin  = BRD_A0_PIN;
	HAL_GPIO_Init(BRD_A0_GPIO_PORT, &GPIO_InitStructure);

	//Sets priority to 12
	HAL_NVIC_SetPriority(BRD_A0_EXTI_IRQ, 12, 0);
	HAL_NVIC_EnableIRQ(BRD_A0_EXTI_IRQ);

}

//ISS interrupt writes the bar and then steps the bar value
void s4532390_hal_pb_iss_isr() {
	
	//Writing the bar value out
	s4532390_hal_lta1000g_write(lightBarValue);
	debug_printf("Lighbar Value: %x\r\n", lightBarValue);

	//Checks for the edges/intermediate case
	if ((lightBarValue >= LTA1000G_START && direction) || (lightBarValue <= LTA1000G_END && !direction)) {

          lightBarValue = LTA1000G_INTERMEDIATE;
      } else if (lightBarValue == LTA1000G_INTERMEDIATE) {

		  //Ensures correct direction after the intermediate case
          if (direction) {

            lightBarValue = LTA1000G_END;
           } else {

			lightBarValue = LTA1000G_START;
           }
          
	  //Bit shifts for normal movement
      } else if (direction) {

          lightBarValue = lightBarValue << 1;
      } else {

          lightBarValue = lightBarValue >> 1;
      }
}

//ISS deninitialiser stops reading the AOPin
void s4532390_hal_pb_iss_deinit() {
	
	HAL_GPIO_DeInit(BRD_A0_GPIO_PORT, BRD_A0_PIN);
}

// void EXTI3_IRQHandler(void) {

//   HAL_GPIO_EXTI_IRQHandler(BRD_A0_PIN);
// }
