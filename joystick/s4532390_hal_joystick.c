#include "s4532390_hal_joystick.h"

ADC_HandleTypeDef handleX, handleY;
int joystickZ = 0;

void s4532390_hal_joystick_init() {

	GPIO_InitTypeDef GPIO_InitStructure;

	//Set pin A1 for X
	__BRD_A1_GPIO_CLK();
	GPIO_InitStructure.Pin = BRD_A1_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(BRD_A1_GPIO_PORT, &GPIO_InitStructure);
 
	//Set pin A2 for Y
    __BRD_A2_GPIO_CLK();
    GPIO_InitStructure.Pin = BRD_A2_PIN;
    HAL_GPIO_Init(BRD_A2_GPIO_PORT, &GPIO_InitStructure);

	// //Set pin A3 for Z
    // __BRD_A3_GPIO_CLK();
    // GPIO_InitStructure.Pin = BRD_A3_PIN;
    // HAL_GPIO_Init(BRD_A3_GPIO_PORT, &GPIO_InitStructure);

	__ADC1_CLK_ENABLE();

	handleX.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1
	handleX.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	handleX.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	handleX.Init.ScanConvMode          = DISABLE;
	handleX.Init.ContinuousConvMode    = DISABLE;
	handleX.Init.DiscontinuousConvMode = DISABLE;
	handleX.Init.NbrOfDiscConversion   = 0;
	handleX.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	handleX.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	handleX.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	handleX.Init.NbrOfConversion       = 1;
	handleX.Init.DMAContinuousRequests = DISABLE;
	handleX.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&handleX);		//Initialise ADC

	ADC_ChannelConfTypeDef AdcChanConfigX;
	/* Configure ADC Channel */
	AdcChanConfigX.Channel = BRD_A1_ADC_CHAN;							//Use A1 pin
	AdcChanConfigX.Rank         = 1;
	AdcChanConfigX.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfigX.Offset       = 0;

	HAL_ADC_ConfigChannel(&handleX, &AdcChanConfigX);		//Initialise ADC channel


	__ADC2_CLK_ENABLE();

	handleY.Instance = (ADC_TypeDef *)(ADC2_BASE);						//Use ADC1
	handleY.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	handleY.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	handleY.Init.ScanConvMode          = DISABLE;
	handleY.Init.ContinuousConvMode    = DISABLE;
	handleY.Init.DiscontinuousConvMode = DISABLE;
	handleY.Init.NbrOfDiscConversion   = 0;
	handleY.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	handleY.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	handleY.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	handleY.Init.NbrOfConversion       = 1;
	handleY.Init.DMAContinuousRequests = DISABLE;
	handleY.Init.EOCSelection          = DISABLE;

	HAL_ADC_Init(&handleY);		//Initialise ADC

    ADC_ChannelConfTypeDef AdcChanConfigY;
	/* Configure ADC Channel */
	AdcChanConfigY.Channel = BRD_A2_ADC_CHAN;							//Use A1 pin
	AdcChanConfigY.Rank         = 1;
	AdcChanConfigY.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfigY.Offset       = 0;

	HAL_ADC_ConfigChannel(&handleY, &AdcChanConfigY);		//Initialise ADC channel


	GPIO_InitTypeDef GPIO_InitStructureZ;

	__BRD_A3_GPIO_CLK();

	GPIO_InitStructureZ.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructureZ.Pull = GPIO_PULLUP;
	GPIO_InitStructureZ.Pin  = BRD_A3_PIN;
	HAL_GPIO_Init(BRD_A3_GPIO_PORT, &GPIO_InitStructureZ);

	//Sets priority to 10
	HAL_NVIC_SetPriority(BRD_A3_EXTI_IRQ, 10, 0);
	HAL_NVIC_EnableIRQ(BRD_A3_EXTI_IRQ);
}



int joystick_readxy(ADC_HandleTypeDef Handler) {

	HAL_ADC_Start(&Handler); // Start ADC conversion

	// Wait for ADC conversion to finish
	while (HAL_ADC_PollForConversion(&Handler, 10) != HAL_OK);

	int adc_value = (uint16_t)(HAL_ADC_GetValue(&Handler));

	return adc_value;
}


