/**************************************************************
 *
 *                     CompVidDCTConversions.h
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (sshiva02)
 *     Date:     3/8/23
 *
 * The interface for the component video color space and discrete cosine 
 *      transformation methods.
 *
 **************************************************************/
#ifndef COMPVIDDCTCONVERSIONS_H_INCLUDED
#define COMPVIDDCTCONVERSIONS_H_INCLUDED

#include <stdio.h>
#include "uarray2b.h"
#include "structs.h"

void compressConversion(UArray2b_T array, unsigned col, unsigned row, 
                                                        compressed *conv);

void decompressConversion(UArray2b_T array, unsigned col, unsigned row, 
                                                        compressed *conv);

void compVidPixel(UArray2b_T array, float pb, float pr, float y, unsigned col, 
                                                                unsigned row);

#endif
