/**************************************************************
 *
 *                     pack.h
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Sridhanvi Shivadevuni (sshiva02)
 *     Date:     3/8/23
 *
 * Purpose:  This file contains declares the functions that are responsible 
 *              for packing quantized values and unpacking an unsigned 32-bit 
 *              integer.
 **************************************************************/
#ifndef PACK_H_INCLUDED
#define PACK_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include "uarray2b.h"
#include "structs.h"

uint32_t pack(quantized *quan);

void unpack(uint32_t word, quantized *quan);

#endif