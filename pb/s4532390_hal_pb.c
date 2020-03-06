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

}

void s4532390_hal_pb_on_deinit() {

}

void s4532390_hal_pb_iss_init() {

}

void s4532390_hal_pb_iss_isr() {

}

void s4532390_hal_pb_iss_deinit() {

}