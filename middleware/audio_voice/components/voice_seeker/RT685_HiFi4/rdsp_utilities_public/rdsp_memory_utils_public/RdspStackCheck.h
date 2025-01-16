/*
 * Copyright 2020-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef PL_MEASURE_STACK_H
#define PL_MEASURE_STACK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */




/****************************************************************************************/
/*                                                                                      */
/*  Standard Enumerated types                                                           */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Standard Function Definitions                                                        */
/*                                                                                      */
/****************************************************************************************/

#define STACK_CHECK     (1024*9)

static void TestApp_CorruptStack(char *stack_ptr)
{
    int i;
    for (i = 0; i<STACK_CHECK; i++)
    {
        *stack_ptr = 55;

        //printf("Corrupting stack at %p with %d\n", stack_ptr, *stack_ptr--);

        stack_ptr--;
    }

    return;
}

static int TestApp_StackUsage(char *stack_ptr)
{
    int stack_size = 0;
    int i;

    for (i = 0; i<STACK_CHECK; i++)
    {
        
        if (*stack_ptr != 55)
            stack_size++;

        stack_ptr--;
    }

    return stack_size;
}

static void TestApp_GetStackPtr(char **stack_ptr)
{
    char i;

    *stack_ptr = &i;

    return;
}

static char   *pStackAddress = NULL;
static int    StackSize = 0;
static int    MAX_StackSize = 0;


/****************************************************************************************/
/*                                                                                      */
/*  MACRO to be used for Stack measurement of a specific code  snippet                  */
/*                                                                                      */
/****************************************************************************************/

/*  usage    */
/*
    #include PL_Measure_Stack.h
    void main(void)
    {
        CORRUPT_STACK
        function()
        STACK_USAGE
    }
    
    PRINT_STACK_USAGE can be used in place of STACK_USAGE
    
*/


#define CORRUPT_STACK     TestApp_GetStackPtr(&pStackAddress); \
                          TestApp_CorruptStack(pStackAddress);


#define STACK_USAGE                                                           \
    do {                                                                      \
        TestApp_GetStackPtr(&pStackAddress);                                  \
        StackSize = TestApp_StackUsage(pStackAddress);                        \
        if (MAX_StackSize>=StackSize)                                         \
            (void)0;                                                          \
        else{                                                                 \
            MAX_StackSize = StackSize;                                        \
        }                                                                     \
    } while (0);


#define PRINT_STACK_USAGE                                                     \
    do {                                                                      \
        TestApp_GetStackPtr(&pStackAddress);                                  \
        StackSize = TestApp_StackUsage(pStackAddress);                        \
        if (MAX_StackSize>=StackSize)                                         \
            (void)0;                                                          \
        else{                                                                 \
            MAX_StackSize = StackSize;                                        \
            PRINTF("Stack usage = %d\n", MAX_StackSize);                      \
        }                                                                     \
    } while (0);

#else  //#define MEASURE_STACK_USAGE
    
#define CORRUPT_STACK
#define STACK_USAGE
#define PRINT_STACK_USAGE

#endif



/****************************************************************************************/
/*                                                                                      */
/*  End of file                                                                         */
/*                                                                                      */
/****************************************************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */


#pragma once
