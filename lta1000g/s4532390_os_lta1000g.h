/**
*********************
*********************
*@file mylib/lta1000g/s4532390_os_lta1000g.h
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

#ifndef S4532390_OS_LTA1000G_H
#define S4532390_OS_LTA1000G_H


/**GLOBALS**/
extern unsigned short lightbarValue; // Stores current value written into lightbar

void s4532390_Tasklta1000g(void);

void s4532390_os_lta1000g_init(void);

void s4532390_os_lta1000g_deinit(void);



#endif