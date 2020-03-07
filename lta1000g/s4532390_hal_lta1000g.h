#ifndef HAL_LTA1000G_H
#define HAL_LTA1000G_H

#include "board.h"
#include "processor_hal.h"

void s4532390_hal_lta1000g_init();

void lta1000g_seg_set(int segment, unsigned char segment_value);

void s4532390_hal_lta1000g_write(unsigned short value);


#endif