/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  John Berg (jberg02), Dhanvi Shivadevuni (sshiva02) 
 *     Date:     3/6/23
 *
 *     The bitpack implementation offering signed and unsigned methods for 
 *     fit, get, and new
 *          
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "bitpack.h"
#include "assert.h"
#include "except.h"

const uint64_t MAX_BITS = 64;
const uint64_t ZERO = 0;
const uint64_t ONE = 1;

Except_T Bitpack_Overflow = { "Overflow packing bits" };

uint64_t Bitpack_shiftl(uint64_t n, unsigned shift);

uint64_t Bitpack_shiftr(uint64_t n, unsigned shift);

int64_t Bitpack_shiftrs(int64_t n, unsigned shift);

/********** Bitpack_fitsu ********
 *
 * Purpose: 
 *      Determine whether an unsigned integer can be encoded given a specified
 *      number of bits
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned width: An unsigned number of bits
 * Return/Effects: 
 *      The boolean value corresponding to whether the integer can fit
 * Notes
 *     
 ************************/
bool Bitpack_fitsu(uint64_t n, unsigned width) {
        uint64_t c = Bitpack_shiftl((uint64_t) ONE, width);
        return !(n >= c);
}

/********** Bitpack_fitss ********
 *
 * Purpose: 
 *      Determine whether a signed integer can be encoded given a specified
 *      number of bits
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned width: An unsigned number of bits
 * Return/Effects: 
 *      The boolean value corresponding to whether the integer can fit
 * Notes
 *     
 ************************/
bool Bitpack_fitss(int64_t n, unsigned width) {
        int64_t low_bound = ~(Bitpack_shiftl(ONE, width - 1)) + 1;
        int64_t up_bound = Bitpack_shiftl(ONE, width - 1) - 1;
        return n >= low_bound && n <= up_bound;
}  

/********** Bitpack_getu ********
 *
 * Purpose: 
 *      Extracts the value in a unsigned 64-bit integer located at a specified 
 *      lsb and with given width
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned width: An unsigned number of bits
 *       unsigned lsb: The least significant bit
 * Return/Effects: 
 *      Shifts the word left and right to eliminate the surrounding bits 
 * Notes
 *     
 ************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);
        if (word == ZERO) {
                return word;
        }
        uint64_t get = Bitpack_shiftl(word, MAX_BITS - (width + lsb));
        return Bitpack_shiftr(get, MAX_BITS - width);
}

/********** Bitpack_getss ********
 *
 * Purpose: 
 *      Return the extracted value in a signed 64-bit integer located at a 
 *      specified lsb with a given width
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned width: An unsigned number of bits
 *       unsigned lsb: The least significant bit
 * Return/Effects: 
 *      Shifts the word left and right to eliminate the surrounding bits 
 * Notes
 *     
 ************************/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);
        if (word == ZERO) {
                return word;
        }
        int64_t get = Bitpack_shiftl(word, MAX_BITS - (width + lsb));
        return Bitpack_shiftrs(get, MAX_BITS - width);
}

/********** Bitpack_newu ********
 *
 * Purpose: 
 *      Insert a new value into an unsigned word at a specified lsb with a 
 *      given width
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned width: An unsigned number of bits
 *       unsigned lsb: The least significant bit
 *      uint64_t value: The value that is to be inserted
 * Return/Effects: 
 *      Return the updated word in a given 64-bit integer given a 
 *      specified lsb, width, and a value
 * Notes
 *     
 ************************/
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
                                        unsigned lsb, uint64_t value) {
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        } 
        uint64_t new_value = Bitpack_shiftl(value, lsb);
        uint64_t mask_h = Bitpack_shiftl(~ZERO, width + lsb);
        uint64_t mask_l = Bitpack_shiftr(~ZERO, MAX_BITS - lsb);
        uint64_t mask = mask_h | mask_l;
        return (word & mask) | new_value;
}

/********** Bitpack_news ********
 *
 * Purpose: 
 *      Insert a new value into a signed word at a specified lsb with a 
 *      given width
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned width: An unsigned number of bits
 *       unsigned lsb: The least significant bit
 *      uint64_t value: The value that is to be inserted
 * Return/Effects: 
 *      Return the updated word in a given 64-bit integer given a 
 *      specified lsb, width, and a value
 * Notes
 *     
 ************************/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                                                        int64_t value) {
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }
        uint64_t new_value = Bitpack_shiftr(Bitpack_shiftl(value, MAX_BITS - 
                                        width), MAX_BITS - width - lsb);
        uint64_t mask_h = Bitpack_shiftl(~ZERO, width + lsb);
        uint64_t mask_l = Bitpack_shiftr(~ZERO, MAX_BITS - lsb);
        uint64_t mask = mask_h | mask_l;
        return (word & mask) | new_value;
}

/********** Bitpack_shiftl ********
 *
 * Purpose: 
 *      Shift an unsigned 64-bit integer left by a given number of bits
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned shift: The number that we are shifting by
 * Return/Effects: 
 *      The left shifted unsigned 64-bit integer
 * Notes
 *     
 ************************/
uint64_t Bitpack_shiftl(uint64_t n, unsigned shift) {
        return n << shift;
}

/********** Bitpack_shiftr ********
 *
 * Purpose: 
 *      Shift an unsigned 64-bit integer right by a given number of bits
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned shift: The number that we are shifting by
 * Return/Effects: 
 *      The right shifted unsigned 64-bit integer
 * Notes
 *     
 ************************/
uint64_t Bitpack_shiftr(uint64_t n, unsigned shift) {
        return n >> shift;
}

/********** Bitpack_shiftrs ********
 *
 * Purpose: 
 *      Shift a signed 64-bit integer right by a given number of bits
 * Inputs:
 *       uint64_t n: An unsigned 64 bit integer
 *       unsigned shift: The number that we are shifting by
 * Return/Effects: 
 *      The right shifted signed 64-bit integer
 * Notes
 *     
 ************************/
int64_t Bitpack_shiftrs(int64_t n, unsigned shift) {
        if (shift == MAX_BITS) {
                return ~ZERO;
        }
        return n >> shift;
}