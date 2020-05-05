/**
*********************
*********************
*@file mylib/lta1000g/s4532390_os_lta1000g.c
*@author Robert Francis - 45323906
*@date 5/05/2020
*@brief RTOS LTA100G library
*REFERENCE:
csse3010_stage4.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
void s4532390_Tasklta1000g(void);
void s4532390_os_lta1000g_init(void);
void s4532390_os_lta1000g_deinit(void);
*********************
*********************
**/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "s4532390_hal_lta1000g.h"
#include "s4532390_os_lta1000g.h"

/* Private define ------------------------------------------------------------*/
#define LTA1000GTASK_PRIORITY					( tskIDLE_PRIORITY + 6 ) //Task priority
#define LTA1000GTASK_STACK_SIZE	( configMINIMAL_STACK_SIZE * 2 ) //Task stack size

/* Private variables ---------------------------------------------------------*/
unsigned short lightbarValue; // Stores current value written into lightbar
TaskHandle_t lta1000gHandle;


/**
*@brief Initialises the lightbar task
*@param  None
*@retval None
*/
void s4532390_os_lta1000g_init(void) {
    xTaskCreate( (void *) &s4532390_Tasklta1000g, (const signed char *) "LTAG", LTA1000GTASK_STACK_SIZE, NULL, LTA1000GTASK_PRIORITY, &lta1000gHandle);

}

/**
*@brief Deiitialises the lightbar task
*@param  None
*@retval None
*/
void s4532390_os_lta1000g_deinit(void) {
    vTaskDelete(NULL);
}

/**
  * @brief  Lightbar task - writes the current lightbarvalue into the bar
  * @param  None
  * @retval None
  */
void s4532390_Tasklta1000g(void) {

  	for (;;) {        
	  	  s4532390_hal_lta1000g_write(lightbarValue);
        vTaskDelay(10); // Delay of 10ms
	  }
}
