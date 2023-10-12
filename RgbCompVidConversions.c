/**************************************************************
 *
 *                     conversions.c
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (ndrese01)
 *     Date:     3/8/23
 *
 * Purpose: This file contains the implementation of the functions that contain 
 *         conversions from RGB to Compvid and vice versa.
 *
 **************************************************************/
#include <stdio.h>
#include "assert.h"
#include "pnm.h"
#include "RgbCompVidConversions.h"
#include "structs.h"

/********** toComponentVideo ********
 *
 * Purpose: 
 *      Convert the RGB color space pixels to component color space
 * Inputs:
 *       int col: The integer value of the column
 *       int row: The integer value of the row
 *       UArray2b_T array: The blocked 2D array containing the component video
 *              pixels
 *       void *elem: The current pixel 
 *       void *cl: A struct containing the copy array and the denominator
 * Return/Effects: 
 *      Converts and copys RGB pixels to component video pixels into a 
 *      new blocked 2D array.
 * Notes
 *     This function is part of the compression process
 ************************/
void toComponentVideo(int col, int row, UArray2b_T array, void *elem, 
                                                                void *cl) {
        assert(array != NULL);
        arrInfo *info = cl;
        UArray2b_T comp_vid_arr = info->array;
        Pnm_rgb curr_pix = elem;
        /* Convert the r, g, b integers to floating point values */
        float r = ((float) curr_pix->red) / info->denom;
        float g = ((float) curr_pix->green) / info->denom;
        float b = ((float) curr_pix->blue) / info->denom;
        /* Calculate the component video color space pixel representation */
        float y = (0.299 * r) + (0.587 * g) + (0.114 * b);
        float pb = (-0.168736 * r) - (0.331264 * g) + (0.5 * b);
        float pr = (0.5 * r) -(0.418688 * g) - (0.081312 * b);
        /* Update the pixel in the array with the new component video pixel */
        compVid src = UArray2b_at(comp_vid_arr, col, row);
        src->y = y;
        src->pb = pb;
        src->pr = pr;

        (void) array;
        (void) col;
        (void) row;
}

/********** toRGB ********
 *
 * Purpose: 
 *      Convert the component video color space pixels to RGB color space
 * Inputs:
 *       int col: The integer value of the column
 *       int row: The integer value of the row
 *       UArray2b_T array: The blocked 2D array containing the RGB pixels
 *       void *elem: The current pixel 
 *       void *cl: A struct containing the copy array and the denominator
 * Return/Effects: 
 *      Converts and copys component video pixels to RGB pixels into a 
 *      new blocked 2D array.
 * Notes
 *     This function is part of the decompression process
 ************************/
void toRGB(int col, int row, UArray2b_T array, void *elem, void *cl) {
        assert(array != NULL);
        arrInfo *info = cl;
        UArray2b_T rgb_arr = info->array;
        compVid curr_pix = elem;
        /* Store the component video floating points values */
        float y = curr_pix->y;
        float pb = curr_pix->pb;
        float pr = curr_pix->pr;
        /* Calculate the r, g, b color space representation */
        float r = (1.0 * y + 0.0 * pb + 1.402 * pr) * info->denom;
        float g = (1.0 * y - 0.344136 * pb -  0.714136 * pr) * info->denom;
        float b = (1.0 * y + 1.772 * pb + 0.0 * pr) * info->denom;
        /* Update the pixel in the array with the new r, g, b pixel */
        Pnm_rgb src = UArray2b_at(rgb_arr, col, row);
        src->red = (int) scaleRGB(r);
        src->green = (int) scaleRGB(g);
        src->blue = (int) scaleRGB(b);

        (void) array;
        (void) col;
        (void) row;
}

/********** scaleRGB ********
 *
 * Purpose: 
 *      Scale the RGB value 
 * Inputs:
 *       float num: The floating point pixel value
 * Return/Effects: 
 *      Returns a floating point pixel value in the range 0-255
 * Notes
 *     
 ************************/
float scaleRGB(float num) {
        if (num < 0) {
                return 0;
        } else if (num > 255) {
                return 255;
        } else {
                return num;
        }
}
