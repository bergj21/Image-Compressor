/**************************************************************
 *
 *                     RgbCompVidConversions.h
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (sshiva02)
 *     Date:     3/8/23
 *
 * Purpose:  This file contains function declaration for the functions that are
 *          responsible for converting a pixel from RGB to component video and
 *          vice versa.
 *
 **************************************************************/
#ifndef RGBCOMPVIDCONVERSIONS_H_INCLUDED
#define RGBCOMPVIDCONVERSIONS_H_INCLUDED

#include <stdio.h>
#include "uarray2b.h"
#include "structs.h"

void toComponentVideo(int col, int row, UArray2b_T array, void *elem, 
                                                                void *cl);

void toRGB(int col, int row, UArray2b_T array, void *elem, void *cl);

float scaleRGB(float num);


#endif
