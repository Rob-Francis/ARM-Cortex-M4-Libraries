#include "board.h"
#include "processor_hal.h"
#include "s4532390_hal_pb.h"
#include "debug_printf.h"

int direction = 0;

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
	BRD_LEDToggle(BRD_GREEN_LEDMASK|BRD_BLUE_LEDMASK|BRD_RED_LEDMASK);
	debug_printf("XENON WORK\r\n");
}

void s4532390_hal_pb_iss_deinit() {

}

