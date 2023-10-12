/**************************************************************
 *
 *                     quantize.h
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (sshiva02)
 *     Date:     3/8/23
 *
 * The interface for the quantization methods.
 *
 **************************************************************/
#ifndef QUANTIZE_H_INCLUDED
#define QUANTIZE_H_INCLUDED

#include <stdio.h>
#include "uarray2b.h"
#include "structs.h"

void dequantize(quantized *quan, compressed *conv);

void quantize(compressed *conv, quantized *quan);

unsigned quantizeA(float val);

int quantizeBCD(float val);

#endif
