/**************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  John Berg (jberg02)
 *     Date:     3/6/23
 *
 *     Purpose: This file contains the implementation for a uarray2, which is a
 *              unboxed 2D array. This implementation will ultimately utilize a
 *              singular uarray (Hanson structure) which we will treat like a 
 *              2D array to accomplish most of its functionality
 *     
 **************************************************************/

#include <stdlib.h>
#include "assert.h"
#include "uarray2.h"
#include "uarray.h"

#define T UArray2_T

/* This is the struct definition for a UArray2_T, each of which contains      *
 * an int height: the height of the UArray2_T                                 *
 * an int width: the width of the UArray2_T                                   *
 * a Uarray_T uarray: The underlying 1D UArray used to store the elements the *
 *                      client puts in the UArray2_T and the size of the      *
 *                      elements                                              */
struct T {
        int height;
        int width;
        UArray_T uarray;
};


/********** UArray2_new ********
 *
 * Purpose: 
 *      This function creates a UArray2 with the specified width, height, and
 *      size and returns it to the client.
 * Inputs:
 *      int width: width of the UArray2
 *      int height: height of the UArray2
 *      int size: the size of each element in the UArray2 in bytes
 * Return: 
 *      An initialized UArray2 with the width, height, and size
 * Expects:
 *      All of the width, height, and size must be > 0
 * Notes
 *      The UArray2 that is returned back to the client is actually a single
 *      UArray that we will allow the client to access as if it is a 2D array
 *      It is the client's job to free the UArray2_T which is returned from this
 *      function by calling the UArray2_free function
 ************************/
T UArray2_new(int width, int height, int size) 
{
        assert(width > 0);
        assert(height > 0);
        assert(size > 0);
        /* Allocate space for a new UArray2 */
        T uarray2 = malloc(sizeof(*uarray2));
        assert(uarray2 != NULL);
        uarray2->width = width;
        uarray2->height = height;

        uarray2->uarray = UArray_new(width * height, size);
        return uarray2;
        
}

/********** UArray2_free ********
 *
 * Purpose: 
 *      This function deallocates all memory which was allocated by the UArray2
 * Inputs:
 *      T *uarray2: A pointer to the UArray2_T to deallocate the memory of and
 * Return/Effects: 
 *      The parameterized pointer-to-UArray2 has the memory associated with
 *      the UArray2 deallocated
 * Expects:
 *      uarray2 and *urray2 must both be nonnull
 * Notes
 *       This function does NOT deallocate the memory associated with any
 *       elements which have been placed into the UArray2 - that is the client's
 *       job
 ************************/
void UArray2_free(T *uarray2) 
{
        assert(uarray2 != NULL);
        assert((*uarray2) != NULL);
        UArray_free(&((*uarray2)->uarray));
        free(*uarray2);
        *uarray2 = NULL;
}

/********** UArray2_height ********
 *
 * Purpose: 
 *      To return the height of a given UArray2
 * Inputs:
 *      T uarray2: The UArray2 to find and return the height of
 * Return: 
 *      An integer representing the height of the UArray2
 * Expects:
 *      T uarray2 must be nonnull 
 ************************/
int UArray2_height(T uarray2) 
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/********** UArray2_width ********
 *
 * Purpose: 
 *      To return the width of a given UArray2
 * Inputs:
 *      T uarray2: The UArray2 to find and return the width of
 * Return:
 *      An integer which represents the width of the UArray2
 * Expects:
 *      T uarray2 must be nonnull
 ************************/
int UArray2_width(T uarray2) 
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/********** UArray2_size ********
 *
 * Purpose: 
 *      To return the size of each element in the UArray2
 * Inputs:
 *      T *uarray2: The UArray2 to find and return the size of
 * Return: 
 *      An integer which represents the size of each element in the UArray2
 *      in bytes
 * Expects:
 *      T uarray2 must be nonnull
 * Notes
 *      Each element in the UArray2 is stored in an unboxed representation, so
 *      the size represents how much memory each element in the UArray2 requires
 *      in the underlying contiguous array which stores the elements
 ************************/
int UArray2_size(T uarray2) 
{
        assert(uarray2 != NULL);
        return UArray_size(uarray2->uarray);
}

/********** UArray2_at ********
 *
 * Purpose: 
 *      To return a pointer to the element at index (col, row) in a given
 *      UArray2
 * Inputs:
 *      T uarray2: The Uarray2 to find the element at index (col, row) of
 *      int col: The column of the desired element in the Uarray2
 *      int row: The row of the desired element in the UArray2
 * Return: 
 *      A pointer to the element that is located at index (col, row) in the 
 *      UArray2
 * Expects:
 *      T uarray2 must be nonnull and
 *      That column and row are > 0 and less than width and height,
 *      respectively.
 * Notes
 *       
 ************************/
void *UArray2_at(T uarray2, int col, int row) 
{
        assert(uarray2 != NULL);
        assert(col >= 0);
        assert(row >= 0);
        assert(col < uarray2->width);
        assert(row < uarray2->height);
        /* Since we're using a singular UArray to represent the UArray2,      *
         * the index of an element in the 1D UArray is calculated by          *
         * multiplying the row by the width of the uarray2 and adding the col */
        int index = uarray2->width * row + col;
        return UArray_at(uarray2->uarray, index);
}

/********** UArray2_map_col_major ********
 *
 * Purpose: 
 *      To iterate over the contents of the UArray2 in a column major order.
 * Inputs:
 *      T uarray2: The UArray2 to iterate over in column major order
 *      apply(int col, int row, T uarray2, void *entry, void *cl): 
 *              The function which will be called on each (col, row)
 *              combination in the UArray2 in column major order. Its arguments
 *              are:
 *              int col - The column of the element which this apply function
 *                        is called on
 *              int row - The row of the element which this apply function
 *                        is called on
 *              T uarray2 - The UArray2 which this apply function is called on
 *              void *entry - A pointer to the entry at (col, row) in the
 *                            T uarray2 
 *              void *cl - A pointer to a closure which the client can use
 *                         however they desire
 *      void *cl: A void pointer to whatever the client decides to pass into
 *                this map function as a closure
 * Return/Effects: 
 *      The apply function is called on every element in the UArray2 in column
 *      major order. The apply function has the ability to alter the entries
 *      within the UArray2, and thus alter the UArray2 itself.
 * Expects:
 *      T uarray2 must be nonnull
 ************************/
void UArray2_map_col_major(T uarray2, 
void apply(int col, int row, T uarray2, void *entry, void *cl), void *cl) 
{
        assert(uarray2 != NULL);
        /* In a column major map, row indices vary more rapidly than column */
        for (int col = 0; col < uarray2->width; col++) {
                for (int row = 0; row < uarray2->height; row++) {
                        apply(col, row, uarray2, 
                              UArray2_at(uarray2, col, row), cl);
                }
        }
}

/********** UArray2_map_row_major ********
 *
 * Purpose: 
 *      To iterate over the contents of the UArray2 in a row major order.
 * Inputs:
 *      T uarray2: The UArray2 to iterate over in row major order
 *      apply(int col, int row, T uarray2, void *entry, void *cl): 
 *              The function which will be called on each (col, row)
 *              combination in the UArray2 in column major order. Its arguments
 *              are:
 *              int col - The column of the element which this apply function
 *                        is called on
 *              int row - The row of the element which this apply function
 *                        is called on
 *              T uarray2 - The UArray2 which this apply function is called on
 *              void *entry - A pointer to the entry at (col, row) in the
 *                            T uarray2 
 *              void *cl - A pointer to a closure which the client can use
 *                         however they desire
 *      void *cl: A void pointer to whatever the client decides to pass into
 *                this map function as a closure
 * Return/Effects: 
 *      The apply function is called on every element in the UArray2 in row
 *      major order. The apply function has the ability to alter the entries
 *      within the UArray2, and thus alter the UArray2 itself.
 * Expects:
 *      T uarray2 must be nonnull
 ************************/
void UArray2_map_row_major(T uarray2, 
void apply(int col, int row, T uarray2, void *entry, void *cl), void *cl) 
{
        assert(uarray2 != NULL);
        /* In a row major map, column indices vary more rapidly than row */
        for (int row = 0; row < uarray2->height; row++) {
                for (int col = 0; col < uarray2->width; col++) {
                        apply(col, row, uarray2, 
                              UArray2_at(uarray2, col, row), cl);
                }
        }
}

#undef T
