/**************************************************************
 *
 *                     quantize.c
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (sshiva02)
 *     Date:     3/8/23
 *
 * Purpose:  This file contains the implementation of the quantizaion process.
 *
 **************************************************************/
#include <stdio.h>
#include <math.h>
#include "assert.h"
#include "arith40.h"
#include "uarray2b.h"
#include "structs.h"
#include "quantize.h"

/********** dequantize ********
 *
 * Purpose: 
 *      To generate corresponding floating point numbers when  given the 
 *                              quantized chroma values and cosine coefficients
 * Inputs:
 *       quantized *quan: the pointer to the struct that holds the quantized
 *                        values of a particular 2x2 block
 *      compressed *conv: the ponter to the struct that holds the pb, pr, a, b,
 *                        c, and d values.
 * Return/Effects: 
 *     none, this is a void function 
 * Notes
 *    
 ************************/
void dequantize(quantized *quan, compressed *conv) {
        assert(conv != NULL);
        assert(quan != NULL);
        /* Stores the dequantized values in a struct */
        conv->pb = Arith40_chroma_of_index(quan->q_pb);
        conv->pr = Arith40_chroma_of_index(quan->q_pr);
        conv->a = ((float) quan->q_a) / 63;
        conv->b = ((float) quan->q_b) / (31.0 / 0.3);
        conv->c = ((float) quan->q_c) / (31.0 / 0.3);
        conv->d = ((float) quan->q_d) / (31.0 / 0.3);
}

/********** quantize ********
 *
 * Purpose: 
 *      To quantize the values given the pb, pr, a, b, c, and d values
 * Inputs:
 *       quantized *quan: the pointer to the struct that holds the quantized
 *                        values of a particular 2x2 block
 *       compressed *conv: the pointer to the struct that holds the pb, pr, a,
 *                         b, c, and d values.
 * Return/Effects: 
 *     none, this is a void function 
 * Notes
 *     
 ************************/
void quantize(compressed *conv, quantized *quan) {
        assert(conv != NULL);
        assert(quan != NULL);
        /* Stores the quantized values in a struct */
        quan->q_pb = Arith40_index_of_chroma(conv->pb);
        quan->q_pr = Arith40_index_of_chroma(conv->pr);
        quan->q_a = quantizeA(conv->a);
        quan->q_b = quantizeBCD(conv->b);
        quan->q_c = quantizeBCD(conv->c);
        quan->q_d = quantizeBCD(conv->d);
} 

/********** quantizeA ********
 *
 * Purpose: 
 *      To quantize the value of A 
 * Inputs:
 *       quantized *quan: the pointer to the struct that holds the quantized
 *                        values of a particular 2x2 block
 *       compressed *conv: the pointer to the struct that holds the pb, pr, a,
 *                         b, c, and d values.
 * Return/Effects: 
 *     none, this is a void function 
 * Notes
 *     
 ************************/
unsigned quantizeA(float val) {
        /* Make sure that a is within the correct bounds */
        if (val < 0) {
                val = 0;
        } else if (val > 1) {
                val = 1;
        }
        unsigned q = round(val * 63);
        return q;

}

/********** quantizeBCD ********
 *
 * Purpose: 
 *      To quantize the values of B, C, and D
 * Inputs:
 *       float val: the floating point values of B, C, and D that need to be 
 *                  quantized
 * Return/Effects: 
 *     return the quantized value of B, C, or D
 * Notes
 *     
 ************************/
int quantizeBCD(float val) {
        /* Make sure that b, c, and d are within the correct bounds */
        if (val > 0.3) {
                val = 0.3;
        } else if (val < -0.3) {
                val = -0.3;
        }
        int q = round(val * (31.0 / 0.3));
        return q;
}