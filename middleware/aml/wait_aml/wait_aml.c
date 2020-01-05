/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file wait_aml.c
 *
 * This driver creates abstraction for WAIT functions for S32 SDK and MCUXpresso
 * SDK.
 */

/*******************************************************************************
* Includes
 ******************************************************************************/
#include "wait_aml.h"

/*******************************************************************************
 * Prototypes of internal functions
 ******************************************************************************/

/*!
 * @brief Waits for exact number of cycles which can be expressed as multiple of 4.
 *
 * MOV - 1 cycle
 * SUB - 1 cycle
 * BNE - 1 cycle or 2 cycles if jump is realized
 *
 * Output list (empty) - which registers are output and how to map them to C code.
 * Input list (Cycles) - which registers are input and how to map them to C code.
 * Clobber list (r0, r1, cc) - which registers might have changed during
 * execution of asm code (compiler will have to reload them).
 *
 * @param Cycles | Number of cycles to wait.
 */
inline static void WAIT_AML_WaitForMul4Cycles(uint32_t cycles);

/*******************************************************************************
 * Internal functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_GetSystemClockFreq
 * Description   : Waits for exact number of cycles which can be expressed as multiple of 4.
 *
 *END**************************************************************************/
inline static void WAIT_AML_WaitForMul4Cycles(uint32_t cycles)
{
#if defined(__CC_ARM)                            /* For ARM Compiler */
    __asm volatile{ nop
                loop:
                    subs cycles, cycles, #4
                    nop
                    bne loop};
#elif defined(__thumb__) && !defined(__thumb2__) /* Thumb instruction set only */
    __asm("mov r0, %r0 \n\t" 
          "loop: \n\t"              
          "sub r0, #4 \n\t"      
          "nop \n\t"             
          "bne loop \n\t"            
          : "+r" (cycles));		  
#else /* Thumb2 or A32 instruction set */
    __asm("movs r0, %[cycles] \n"
          "0: \n"
          "subs r0, r0, #4 \n"
          "nop \n\t"
          "bne 0b \n"
          : 
          : [cycles] "r" (cycles) 
          : "r0", "r1", "cc");
#endif
}

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_GetSystemClockFreq
 * Description   : Returns system clock frequency.
 *
 *END**************************************************************************/
uint32_t WAIT_AML_GetSystemClockFreq(void)
{
    uint32_t freq;

#if (SDK_VERSION == SDK_KSDK)
    freq = CLOCK_GetCoreSysClkFreq();
#elif (SDK_VERSION == SDK_S32)
    CLOCK_SYS_GetFreq(CORE_CLOCK, &freq);
#endif

    return freq;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitCycles
 * Description   : Waits for specified amount of cycles which is given by 32bit 
 *                 value range. Assumption for this function is that target 
 *                 architecture is using 32bit general purpose registers.
 *
 *END**************************************************************************/
void WAIT_AML_WaitCycles(uint32_t cycles)
{
    /* Advance to next multiple of 4. Value 0x04U ensures that the number
     * is not zero. */
    cycles = (cycles & 0xFFFFFFFCU) | 0x04U;

    WAIT_AML_WaitForMul4Cycles(cycles);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitSec
 * Description   : Waits for specified amount of seconds.
 *
 *END**************************************************************************/
void WAIT_AML_WaitSec(uint16_t delay)
{
    for (; delay > 0U; delay--) {
        WAIT_AML_WaitMs(1000U);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitMs
 * Description   : Waits for specified amount of milliseconds.
 *
 *END**************************************************************************/
void WAIT_AML_WaitMs(uint16_t delay)
{
    uint32_t cycles = (uint32_t) WAIT_AML_GET_CYCLES_FOR_MS(1U, WAIT_AML_SYSTEM_CLOCK_FREQ);

    /* Advance to multiple of 4. */
    cycles = cycles & 0xFFFFFFFCU;

    for (; delay > 0U; delay--) {
        WAIT_AML_WaitForMul4Cycles(cycles);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitUs
 * Description   : Waits for specified amount of microseconds.
 *
 *END**************************************************************************/
void WAIT_AML_WaitUs(uint16_t delay)
{
    uint32_t cycles = (uint32_t) WAIT_AML_GET_CYCLES_FOR_US(delay, WAIT_AML_SYSTEM_CLOCK_FREQ);

    /* Advance to next multiple of 4. Value 0x04U ensures that the number
     * is not zero. */
    cycles = (cycles & 0xFFFFFFFCU) | 0x04U;
    WAIT_AML_WaitForMul4Cycles(cycles);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
