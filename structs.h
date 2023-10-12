/**************************************************************
 *
 *                     structs.h
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (sshiva02)
 *     Date:     3/8/23
 *
 * Purpose: This file holds all the structs necessary for the compression
 *          and decompression process. 
 *
 **************************************************************/

#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <stdio.h>
#include "uarray2b.h"

/* This struct contains the quantized values of the cosine coefficients
                                                        and chroma values */
struct quantized {
        unsigned q_pb; 
        unsigned q_pr; 
        unsigned q_a;
        int q_b;
        int q_c;
        int q_d;
};

typedef struct quantized quantized;

/* This struct contains the floating point values of the cosine coefficients
                                                        and chroma values */
struct compressed {
        float pb; 
        float pr; 
        float a;
        float b;
        float c;
        float d;
};

typedef struct compressed compressed;

/* This struct contains the values of a component video pixel  */
typedef struct compVid {
        float y;
        float pb; 
        float pr; 
} *compVid; 

/* This struct contains a 2D array as well as the denominator of a ppm image */
struct arrInfo {
        UArray2b_T array;
        unsigned denom;
};

typedef struct arrInfo arrInfo;

#endif
