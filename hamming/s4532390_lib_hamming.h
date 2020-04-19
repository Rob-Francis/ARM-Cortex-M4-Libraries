/**
*********************
*********************
*@file mylib/hamming/s4532390_lib_hamming.h
*@author Robert Francis - 45323906
*@date 18/04/2020
*@brief Hamming library
*REFERENCE:
csse3010_mylib_lib_hamming.pdf 
*********************
*********************
*EXTERNAL FUNCTIONs
*********************
*********************
*uint16_t s4532390_lib_hamming_byte_encode(uint8_t input);
*uint8_t s4532390_lib_hamming_byte_decode(uint8_t in);
*uint8_t hamming_hbyte_encode(uint8_t input);
*int s4532390_lib_hamming_parity_error(uint8_t in);
*********************
*********************
**/

#ifndef S4532390_LIB_HAMMING_H
#define S4532390_LIB_HAMMING_H

#include <stdint.h>

uint16_t s4532390_lib_hamming_byte_encode(uint8_t input);

uint8_t s4532390_lib_hamming_byte_decode(uint8_t in);

uint8_t hamming_hbyte_encode(uint8_t input);

int s4532390_lib_hamming_parity_error(uint8_t in);







#endif