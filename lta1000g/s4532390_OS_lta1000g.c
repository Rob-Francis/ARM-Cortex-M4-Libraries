/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "s4532390_hal_lta1000g.h"


unsigned short lightbarValue;

/**
  * @brief  Receiver Task. Used to receive messages.
  * @param  None
  * @retval None
  */
void s4532390_Tasklta1000g(void) {


	for (;;) {

        
		s4532390_hal_lta1000g_write(lightbarValue);
        vTaskDelay(100);
	}
}
