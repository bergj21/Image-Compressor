/**************************************************************
 *
 *                     CompVidDCTConversions.c
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (ndrese01)
 *     Date:     3/8/23
 *
 *     The implementation for component video color space to discrete cosine 
 *     transformation and the inverse
 *
 **************************************************************/
#include <stdio.h>
#include "assert.h"
#include "uarray2b.h"
#include "structs.h"
#include "CompVidDCTConversions.h"

/********** compressConversion ********
 *
 * Purpose: Transform component video pixel space to DCT space
 *      
 * Inputs:
 *        UArray2b_T array: the array which is traversed so that each pixel 
 *        unsigned col, unsigned row: the specific index that is being 
 *              evaluated                           
 *        compressed *conv: a pointer to struct compresss which contains the 
 *                         pb, pr, a, b, c, and d values
 * Return/Effects: 
 *     Uses the discrete cosine transformation to determine the value of
 *          a,b,c, d, and take the average of the chroma values.
 * Notes
 *     This function is part of the compression process.
 ************************/
void compressConversion(UArray2b_T array, unsigned col, unsigned row, 
                                                        compressed *conv) {
        assert(array != NULL);
        /* Get each pixel in a current 2x2 block */
        compVid p1 = UArray2b_at(array, col, row);
        compVid p2 = UArray2b_at(array, col + 1, row);
        compVid p3 = UArray2b_at(array, col, row + 1);
        compVid p4 = UArray2b_at(array, col + 1, row + 1);
        /* Calculate the floating point cosine coefficient and chroma values */ 
        conv->pb = (p1->pb + p2->pb + p3->pb + p4->pb) / 4.0;
        conv->pr = (p1->pr + p2->pr + p3->pr + p4->pr) / 4.0;
        conv->a = (p4->y + p3->y + p2->y + p1->y) / 4.0;
        conv->b = (p4->y + p3->y - p2->y - p1->y) / 4.0;
        conv->c = (p4->y - p3->y + p2->y - p1->y) / 4.0;
        conv->d = (p4->y - p3->y - p2->y + p1->y) / 4.0;
}

/********** decompressConversion ********
 *
 * Purpose: Transform DCT space to component video color space
 *      
 * Inputs:
 *        UArray2b_T array: the array which is traversed so that each pixel 
 *        unsigned col, unsigned row: the specific index that is being 
 *              evaluated
 *        compressed *conv:
 * Return/Effects: 
 *     Using the inverse DCT function to calculate the component video pixels
 * Notes
 *     
 ************************/
void decompressConversion(UArray2b_T array, unsigned col, unsigned row, 
                                                        compressed *conv) {
        assert(array != NULL);
        /* Store the cosine coefficient and chroma values */
        float pb = conv->pb;
        float pr = conv->pr;
        float a = conv->a;
        float b = conv->b;
        float c = conv->c;
        float d = conv->d;
        /* Calculate each pixels Y value in a 2x2 block using the cosine 
                                                        coefficients */
        float y1 = a - b - c + d;
        float y2 = a - b + c - d;
        float y3 = a + b - c - d;
        float y4 = a + b + c + d;

        /* Insert the component video color space values into each pixel */
        compVidPixel(array, pb, pr, y1, col, row);
        compVidPixel(array, pb, pr, y2, col + 1, row);
        compVidPixel(array, pb, pr, y3, col, row + 1);
        compVidPixel(array, pb, pr, y4, col + 1, row + 1);
}

/********** compVidPixel ********
 *
 * Purpose: To populate a 2D array with a component video pixel
 *      
 * Inputs:
 *       UArray2b_T array: A blocked 2D array
 *       float pb: The Pb chroma value of the pixel
 *       float pr: The Pr chroma value of the pixel
 *       float y: The Y value of the pixel
 *       unsigned col, unsigned row: the specific index that is being 
 *              evaluated
 * Return/Effects: 
 *      Inserts the values of a component video color space pixel into 
 *      a 2D array
 * Notes
 *     
 ************************/
void compVidPixel(UArray2b_T array, float pb, float pr, float y, unsigned col, 
                                                                unsigned row) {
        assert(array != NULL);
        compVid src = UArray2b_at(array, col, row);
        src->y = y;
        src->pb = pb;
        src->pr = pr;
}