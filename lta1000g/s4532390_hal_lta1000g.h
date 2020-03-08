 /** 
 **************************************************************
 * @file mylib/s4532390_hal_lta1000g.h
 * @author Robert Francis - 45323906
 * @date 8/03/20
 * @brief Hal Lightbar Driver Header file
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s4532390_hal_lta1000g_init() - initialises the lightbar
 * lta1000g_seg_set(int segment, unsigned char segment_value) - sets a single segment of the bar
 * s4532390_hal_lta1000g_write(unsigned short value) - sets the entire lightbar according to value
 *************************************************************** 
 */

#ifndef HAL_LTA1000G_H
#define HAL_LTA1000G_H


void s4532390_hal_lta1000g_init();

void lta1000g_seg_set(int segment, unsigned char segment_value);

void s4532390_hal_lta1000g_write(unsigned short value);


#endif