/**************************************************************
 *
 *                     a2plain.c
 *
 *     Assignment: HW4 - arith
 *     Authors:  John Berg (jberg02), Dhanvi Shivadevuni (sshiva02) 
 *     Date:     3/6/23
 *
 *  Purpose: This file contains the a list of functions that manipulate and 
 *           provide information about a uarray2, which is a unboxed 2D array. 
 *           These functions are then exported to the a2methods suite of 
 *           functions which is essentially an interface that contains methods 
 *           for the Uarray2 and Uarray2b structures.
 **************************************************************/
#include <string.h>
#include <string.h>

#include <a2plain.h>
#include "uarray2.h"

/************************************************/
/* Defining a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/

typedef A2Methods_UArray2 A2;

/********** new ********
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
 * Notes
 *      The UArray2 that is returned back to the client is actually a single
 *      UArray that we will allow the client to access as if it is a 2D array
 *      It is the client's job to free the UArray2_T which is returned from this
 *      function by calling the UArray2_free function
 ************************/
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}


static A2 new_with_blocksize(int width, int height, int size, int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}

/********** free ********
 *
 * Purpose: 
 *      This function deallocates all memory which was allocated by the UArray2
 * Inputs:
 *      A2 *array2: A pointer to the A2 to deallocate the memory 
 * Return/Effects: 
 *      The parameterized pointer-to-UArray2 has the memory associated with
 *      the UArray2 deallocated
 * Notes
 *       This function does NOT deallocate the memory associated with any
 *       elements which have been placed into the UArray2 - that is the client's
 *       job
 ************************/
static void a2free(A2 * array2)
{
        UArray2_free((UArray2_T *) array2);
}


/********** width ********
 *
 * Purpose: 
 *      To return the width of a given UArray2
 * Inputs:
 *      A2 array2: The UArray2 to find and return the width of
 * Return:
 *      An integer which represents the width of the UArray2
 ************************/
static int width(A2 array2)
{
        return UArray2_width(array2);
}

/********** height ********
 *
 * Purpose: 
 *      To return the height of a given UArray2
 * Inputs:
 *      A2 array2: The UArray2 to find and return the height of
 * Return: 
 *      An integer representing the height of the UArray2
 ************************/
static int height(A2 array2) {
        return UArray2_height(array2);
}

/********** size ********
 *
 * Purpose: 
 *      To return the size of each element in the UArray2
 * Inputs:
 *      A2 array2: The UArray2 to find and return the size of
 * Return: 
 *      An integer which represents the size of each element in the UArray2
 *      in bytes
 * Notes
 *      Each element in the UArray2 is stored in an unboxed representation, so
 *      the size represents how much memory each element in the UArray2 requires
 *      in the underlying contiguous array which stores the elements
 ************************/
static int size(A2 array2)
{
        return UArray2_size(array2);
}

/********** blocksize ********
 *
 * Purpose: 
 *      To return the blocksize of each element in the UArray2b
 * Inputs:
 *      A2 array2: The UArray2 to find and return the size of blocksize
 * Return: 
 *      An integer which represents the blocksize of the blocks within the 
 *      Uarray2b, in this case blocksize is one.
 * Notes
 *      This function returns the blocksize of each block which is essential
 *      in computing the size of the Uarray or how many cells a particular 
 *      block can have.
 ************************/
static int blocksize(A2 array2)
{
        (void) array2;
        return 1;
}

/********** at ********
 *
 * Purpose: 
 *      To return a pointer to the element at index (i, j) in a given UArray2
 * Inputs:
 *      A2 array2: The Uarray2 to find the element at index (col, row) of
 *      int i: The column of the desired element in the Uarray2b
 *      int j: The row of the desired element in the UArray2b
 * Return: 
 *      A pointer to the element that is located at index (col, row) in the 
 *      UArray2
 * Expects:
 *      T uarray2b must be nonnull 
 *      index out of range is a checked run-time error
 * Notes
 *
 ************************/ 
static A2Methods_Object *at(A2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}

typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);


/********** map_row_major ********
 *
 * Purpose: 
 *      To iterate over the contents of the UArray2 in a row major order.
 * Inputs:
 *      A2Methods_Uarray2 uarray2: The UArray2 to iterate over in row major 
 *                                 order
 *      A2Methods_applyfun apply: the function that is to be applied to each 
 *                                 element in the Uarray2
 *      void *cl: A void pointer to whatever the client decides to pass into
 *                this map function as a closure
 * Return/Effects: 
 *      The apply function is called on every element in the UArray2 in column
 *      major order. The apply function has the ability to alter the entries
 *      within the UArray2, and thus alter the UArray2 itself.
 * Expects:
 *      T uarray2 must be nonnull
 ************************/
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(uarray2, (applyfun *) apply, cl);
}

/********** map_col_major ********
 *
 * Purpose: 
 *      To iterate over the contents of the UArray2 in a column major order.
 * Inputs:
 *      A2Methods_Uarray2 uarray2: The UArray2 to iterate over in column major 
 *                                 order
 *      A2Methods_applyfun apply: the function that is to be applied to each 
 *                                 element in the Uarray2
 *      void *cl: A void pointer to whatever the client decides to pass into
 *                this map function as a closure
 * Return/Effects: 
 *      The apply function is called on every element in the UArray2 in column
 *      major order. The apply function has the ability to alter the entries
 *      within the UArray2, and thus alter the UArray2 itself.
 * Expects:
 *      T uarray2 must be nonnull
 ************************/
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(uarray2, (applyfun *) apply, cl);
}

/* The following functions were already implemented */
struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void *cl;
};

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2Methods_UArray2 a2, 
                                A2Methods_smallapplyfun  apply, void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}


static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,                            /* new_with_blocksize */
        a2free,
        width,
        height,
        size,
        blocksize,                            /* blocksize */
        at,
        map_row_major,                   /* map_row_major */
        map_col_major,                   /* map_col_major */
        NULL,                            /*  map_block_major */
        map_row_major,                            /* map_default */
        small_map_row_major,             /* small_map_row_major */
        small_map_col_major,             /* small_map_col_major */
        NULL,                            /* small_map_block major */
        small_map_row_major                             /* small_map_default */
        
};

/* Here is the exported pointer to the struct */

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;