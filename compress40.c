/**************************************************************
 *
 *                     compress40.c
 *
 *     Assignment: HW4
 *     Authors:  John Berg (jberg02), Dhanvi Shivadevuni (sshiva02)
 *     Date:     3/7/23
 *
 *     This file contains the  functions that encapsulate 
 *     the compression and 
 *     decompression process.
 *     
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "assert.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "uarray2b.h"
#include "bitpack.h"
#include "structs.h"
#include "RgbCompVidConversions.h"
#include "CompVidDCTConversions.h"
#include "quantize.h"
#include "pack.h"

UArray2b_T trim(Pnm_ppm image);
void writeDecompressed(UArray2b_T array, A2Methods_T methods);
void writeCodeWord(uint32_t codeword);
uint32_t readCodeWord(FILE *input);

/********** compress40 ********
 *
 * Purpose: 
 *      To compress an image and print the compressed version.
 * Inputs:
 *       FILE *input: The input file containing a ppm image
 * Return/Effects: 
 *      Takes in a ppm file and writes a compressed version of the file. 
 * Notes
 *     
 ************************/
extern void compress40(FILE *input) {
        assert(input != NULL);

        /* default to UArray2b methods */
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods);

        /* Read Image */
        Pnm_ppm image = Pnm_ppmread(input, methods);

        /* Image Trimming */
        UArray2b_T trim_array = trim(image);
        unsigned width = UArray2b_width(trim_array);
        unsigned height = UArray2b_height(trim_array);
        
        /* Initialize array */
        UArray2b_T comp_vid_arr = UArray2b_new(width, height, 
                                                sizeof(struct compVid), 2);
        assert(comp_vid_arr != NULL);
        /* Store array + denominator */
        arrInfo info = {comp_vid_arr, image->denominator};
        /* RGB -> Component Video Color Space */
        UArray2b_map(trim_array, toComponentVideo, &info);

        /* Print header */
        printf("COMP40 Compressed image format 2\n%u %u\n", width, height);
        /* Loop through each block */
        for (unsigned row = 0; row < height; row += 2) {
                for (unsigned col = 0; col < width; col += 2) {
                        /* Initialize the conversion struct */
                        compressed *conv = malloc(sizeof(compressed));
                        assert(conv != NULL);
                        /* Convert the component video pixels 
                                (comp vid --> consine coefficient + chroma) */
                        compressConversion(comp_vid_arr, col, row, conv);

                        /* Initialize the quantization struct */
                        quantized *quan = malloc(sizeof(quantized));
                        assert(quan != NULL);
                        /* Quantize the converted values (floats --> ints)*/
                        quantize(conv, quan);
                        /* Pack the quantized values */
                        uint32_t codeword = pack(quan);
                        /* Write the current codeword */
                        writeCodeWord(codeword);

                        /* Free the current structs */
                        free(quan);
                        free(conv);
                }
        }

        /* Free memory */
        UArray2b_free(&trim_array);
        UArray2b_free(&comp_vid_arr);
        Pnm_ppmfree(&image);
}

/********** decompress40 ********
 *
 * Purpose: 
 *      Decompress an image.
 * Inputs:
 *       FILE *input: The input file containing a ppm image
 * Return/Effects: 
 *      Takes in a compressed image and writes the decompressed image to output
 * Notes
 *     
 ************************/
extern void decompress40(FILE *input) {
        assert(input != NULL);

        /* default to UArray2b methods */
        A2Methods_T methods = uarray2_methods_blocked;
        assert(methods != NULL);

        /* Read image header */
        unsigned width, height;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                                                        &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        /* Assert the image is at least 2x2 */
        assert(width >= 2);
        assert(height >= 2);

        /* Initalize the component video color space array */
        UArray2b_T comp_vid_arr = UArray2b_new(width, height, 
                                                sizeof(struct compVid), 2);
        assert(comp_vid_arr != NULL);

        unsigned numCodeWords = 0;
        /* Traverse each codeword in the compressed file */
        for (unsigned row = 0; row < height; row += 2) {
                for (unsigned col = 0; col < width; col += 2) {
                        /* Read the code word */
                        uint32_t word = readCodeWord(input);
                        /* Initialize the quantized struct */
                        quantized *quan = malloc(sizeof(quantized));
                        assert(quan != NULL);
                        /* Unpack the codeword into the quantized struct */
                        unpack(word, quan);
                        /* Initialize the conversion struct */
                        compressed *conv = malloc(sizeof(compressed));
                        assert(conv != NULL);
                        /* Dequantize the values into the conversion struct
                                (ints --> floats)                           */
                        dequantize(quan, conv);

                        /* Insert the component video pixels into array */
                        decompressConversion(comp_vid_arr, col, row, conv);

                        /* Free the current structs */
                        free(quan);
                        free(conv);
                        numCodeWords++;
                }
        }
        /* Make sure that there are the correct number of code words */
        assert(!(numCodeWords < width * height / 4));

        /* Initalize the RGB color space array */
        UArray2b_T RGB_arr = UArray2b_new(width, height, 
                                                sizeof(struct Pnm_rgb), 2);
        assert(RGB_arr != NULL);
        /* Initalize the array info struct */
        arrInfo info_test = {RGB_arr, 255};
        /* CompVid --> RGB */
        UArray2b_map(comp_vid_arr, toRGB, &info_test);
        /* Write the decompressed image to output */
        writeDecompressed(RGB_arr, methods);

        /* Free arrays */
        UArray2b_free(&comp_vid_arr);
        UArray2b_free(&RGB_arr);
}

/********** readCodeWord ********
 *
 * Purpose: 
 *      Reads in the current code word in big endian order
 * Inputs:
 *       FILE *input: The file containing the decompressed image
 * Return/Effects: 
 *      Reads in each byte and inserts them into the current code word
 ************************/
uint32_t readCodeWord(FILE *input) {
        assert(input != NULL);
        uint32_t codeword = 0;
        for (int i = 0; i < 32; i += 8) {
                int c = getc(input);
                /* Make sure there is no incomplete code words */
                assert(c != EOF);
                codeword = Bitpack_newu(codeword, 8, 24 - i, c);
        }
        return codeword;
}

/********** writeCodeWord ********
 *
 * Purpose: 
 *      Write a code word to output in big endian order
 * Inputs:
 *       uint32_t codeword: The code word at the current 2x2 block
 * Return/Effects: 
 *      Gets all 4 bytes of the current code word and writes them to output
 ************************/
void writeCodeWord(uint32_t codeword) {
        putchar(Bitpack_getu(codeword, 8, 24));
        putchar(Bitpack_getu(codeword, 8, 16));
        putchar(Bitpack_getu(codeword, 8, 8));
        putchar(Bitpack_getu(codeword, 8, 0));
}

/********** trim ********
 *
 * Purpose: 
 *      To trim the original image based on its dimensions.
 * Inputs:
 *       Pnm_ppm image: the pixel map whose dimensions we are evaluating and
 *                      trimming accordingly  
 * Return/Effects: 
 *      This function returns the UArray2b that contains same pixels but 
 *      trimmed dimensions of the original image.
 ************************/
UArray2b_T trim(Pnm_ppm image) {
        assert(image != NULL);
        unsigned width = image->width;
        unsigned height = image->height;
        /* Check if the width and height are even values */
        if (image->width % 2 != 0) {
                width = image->width - 1;
        }
        if (image->height % 2 != 0) {
                height = image->height - 1;
        }
        image->width = width;
        image->height = height;
        /* Initialize a new blocked 2D array to store the pixels */
        UArray2b_T array = UArray2b_new(width, height, 
                                        sizeof(struct Pnm_rgb), 2);
        assert(array != NULL);
        /* Copy the pixels from the Pnm_ppm pixels array into the new one */
        for (unsigned row = 0; row < height; row++) {
                for (unsigned col = 0; col < width; col++) {
                        Pnm_rgb src = (Pnm_rgb) image->methods->
                                                at(image->pixels, col, row);
                        Pnm_rgb dst = (Pnm_rgb) UArray2b_at(array, col, row);
                        *dst = *src;
                }
        }
        return array;
}

/********** writeDecompressed ********
 *
 * Purpose: 
 *      Writes an image to stdout.
 * Inputs:
 *       UArray2b_T array: A blocked 2D array of pixels
 *       A2Methods_T methods: The A2Methods struct of function pointers
 * Return/Effects: 
 *      Initialize a Pnm_ppm struct with the decompressed values and write 
 *                                                       the image to output
 * Notes
 *     
 ************************/
void writeDecompressed(UArray2b_T array, A2Methods_T methods) {
        assert(array != NULL);
        assert(methods != NULL);
        struct Pnm_ppm image;
        image.width = UArray2b_width(array);
        image.height = UArray2b_height(array);
        image.denominator = 255;
        image.pixels = array;
        image.methods = methods;
        Pnm_ppmwrite(stdout, &image);
}
