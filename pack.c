/**************************************************************
 *
 *                     pack.c
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (ndrese01)
 *     Date:     3/8/23
 *
 * Purpose:  This file contains the implementation of the functions responsible
 *           for packing a quantized value and unpacking an unsigned integer
 *
 **************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "assert.h"
#include "bitpack.h"
#include "uarray2b.h"
#include "structs.h"
#include "pack.h"

/********** pack ********
 *
 * Purpose: 
 *      To bitpack the quantized values of pb, pr, a, b,c , and d.
 * Inputs:
 *       quantized *quan: the pointer to the struct that holds the quantized
 *                        values of a particular 2x2 block
 * Return/Effects: 
 *      The packed code word for the specific 2x2 block
 * Notes
 *     
 ************************/
uint32_t pack(quantized *quan) {
        assert(quan != NULL);
        uint32_t word = 0;
        /* Updates the code word to store the quantized values */
        word = Bitpack_newu(word, 4, 0, quan->q_pr);
        word = Bitpack_newu(word, 4, 4, quan->q_pb);
        word = Bitpack_news(word, 6, 8, quan->q_d);
        word = Bitpack_news(word, 6, 14, quan->q_c);
        word = Bitpack_news(word, 6, 20, quan->q_b);
        word = Bitpack_newu(word, 6, 26, quan->q_a);
        return word;
}

/********** unpack ********
 *
 * Purpose: 
 *      To unpack the quantized values of pb, pr, a, b,c , and d.
 * Inputs:
 *       quantized *quan: the pointer to the struct that holds the quantized
 *                        values of a particular 2x2 block
 * Return/Effects: 
 *     Extracts all the values from the code word and inserts them into the 
 *     quantized struct
 * Notes
 *     
 ************************/
void unpack(uint32_t word, quantized *quan) {
        assert(quan != NULL);
        /* Extracts and holds the bit packed values */
        uint32_t pr = Bitpack_getu(word, 4, 0);
        uint32_t pb = Bitpack_getu(word, 4, 4);
        int32_t d = Bitpack_gets(word, 6, 8);
        int32_t c = Bitpack_gets(word, 6, 14);
        int32_t b = Bitpack_gets(word, 6, 20);
        uint32_t a = Bitpack_getu(word, 6, 26);
        /* Stores the packed values in the quantized struct */
        quan->q_pb = pb;
        quan->q_pr = pr;
        quan->q_d = d;
        quan->q_c = c;
        quan->q_b = b;
        quan->q_a = a;
}