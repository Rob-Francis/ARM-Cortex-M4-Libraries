/**
*********************
*********************
*@file mylib/hamming/s4532390_lib_hamming.c
*@author Robert Francis - 45323906
*@date 18/04/2020
*@brief Hamming library
*REFERENCE:
csse3010_mylib_hal_hamming.pdf 
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


#include "s4532390_lib_hamming.h"
#include "debug_printf.h"
#include <stdint.h>

/**
*@brief Encodes a single byte using hamming 7/4
*@param input - the byte being encoded
*@retval out - 2 bytes of encoded data
*/
uint16_t s4532390_lib_hamming_byte_encode(uint8_t input) {

    uint16_t out;

	/* first encode D0..D3 (first 4 bits),
	 * then D4..D7 (second 4 bits).
	 */
	out = hamming_hbyte_encode(input & 0xF) |
		(hamming_hbyte_encode(input >> 4) << 8);

	return out;
}

/**
*@brief Decoder function for one byte
*@param in - an encoded byte
*@retval a byte - lower 4 bits data
*/
uint8_t s4532390_lib_hamming_byte_decode(uint8_t in) {

    uint8_t h0, h1, h2;
    uint8_t p0;
    uint8_t d0, d1, d2, d3;
    uint8_t s0, s1, s2;

    //Parity bit
    p0 = !!((in & 0x01));

    //Hamming bits
    h0 = (in >> 1) & 0x1;
    h1 = (in >> 2) & 0x1;
    h2 = (in >> 3) & 0x01;

    //data bits
    d3 = !!(in & 0x80);
    d2 = !!(in & 0x40);
    d1 = !!(in & 0x20);
    d0 = !!(in & 0x10);

    //sydrome calculation
    s0 = h0 ^ d1 ^ d2 ^ d3;
    s1 = h1 ^ d0 ^ d2 ^ d3;
    s2 = h2 ^ d0 ^ d1 ^ d3;

    //Syndrome
    int syndrome = (s0 << 0) + (s1 << 1) + (s2 << 2);

    //Parity
    int parity = h0 ^ h1 ^ h2 ^ d0 ^ d1 ^ d2 ^ d3;


    //Error correction
    if (syndrome != 0) {
        if (s4532390_lib_hamming_parity_error(in)) {
            // debug_printf("2 bit error\r\n");

        } else if (s0 && !s1 && !s2) {
            d0 = !d0;
        } else if (!s0 && s1 && !s2) {
            d1 = !d1;
        } else if (!s0 && !s1 && s2) {
            d2 = !d2;
        } else if (!s0 && !s1 && !s2) {
            d3 = !d3;
        }      
    }
    
    return  (d0 << 0) | (d1 << 1) | (d2 << 2) | (d3 << 3);
    
}

/**
*@brief Encoder function for one byte
*@param in - a byte - lower 4 bits data
*@retval an encoded byte
*/
uint8_t hamming_hbyte_encode(uint8_t input) {
    uint8_t d0, d1, d2, d3;
	uint8_t p0 = 0, h0, h1, h2;
	uint8_t z;
	uint8_t out;

    //Data bits
	d0 = !!(input & 0x01);
    d1 = !!(input & 0x02);
    d2 = !!(input & 0x04);
    d3 = !!(input & 0x08);

    //Hamming bits
    h0 = d1 ^ d2 ^ d3;
    h1 = d0 ^ d2 ^ d3;
    h2 = d0 ^ d1 ^ d3;

    /* generate  byte without parity bit P0 */
    out = (h0 << 1) | (h1 << 2) | (h2 << 3) |
    (d0 << 4) | (d1 << 5) | (d2 << 6) | (d3 << 7);

    //Calculate parity bit 
    for (z = 0; z < 7; z++) {
        p0 = p0 ^ !!(out & (1 << z));
    }
    out |= p0;
        
	return out;
}


/**
*@brief Checks for parity error
*@param in - an encoded byte
*@retval 1 if error has occurred otherwise 0
*/
int s4532390_lib_hamming_parity_error(uint8_t in) {
    
    uint8_t h0, h1, h2;
    uint8_t p0;
    uint8_t d0, d1, d2, d3;
    uint8_t s0, s1, s2;

    p0 = !!((in & 0x01));

    //parity bits

    h0 = (in >> 1) & 0x1;
    h1 = (in >> 2) & 0x1;
    h2 = (in >> 3) & 0x01;

    //data bits
    d3 = !!(in & 0x80);
    d2 = !!(in & 0x40);
    d1 = !!(in & 0x20);
    d0 = !!(in & 0x10);

    //Calculates parity
    int parity = h0 ^ h1 ^ h2 ^ d0 ^ d1 ^ d2 ^ d3;

    //Returns 1 if parity values do not match
    return (parity == p0 ? 0 : 1);
}