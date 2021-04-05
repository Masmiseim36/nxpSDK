/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "isr.h"
#include "vectors_iar.h"

/*******************************************************************************
* Definitions
******************************************************************************/
typedef void (*vector_entry)(void);

#pragma location = ".intvec"
const vector_entry  __vector_table[] =
{
    VECTOR_000,           /* Initial SP           */
    VECTOR_001,           /* Initial PC           */
    VECTOR_002,
    VECTOR_003,
    VECTOR_004,
    VECTOR_005,
    VECTOR_006,
    VECTOR_007,
    VECTOR_008,
    VECTOR_009,
    VECTOR_010,
    VECTOR_011,
    VECTOR_012,
    VECTOR_013,
    VECTOR_014,
    VECTOR_015,  
    VECTOR_016,
    VECTOR_017,
    VECTOR_018,
    VECTOR_019,
    VECTOR_020,
    VECTOR_021,
    VECTOR_022,
    VECTOR_023,
    VECTOR_024,
    VECTOR_025,
    VECTOR_026,
    VECTOR_027,
    VECTOR_028,
    VECTOR_029,
    VECTOR_030,
    VECTOR_031,
    VECTOR_032,
    VECTOR_033,
    VECTOR_034,
    VECTOR_035,
    VECTOR_036,
    VECTOR_037,
    VECTOR_038,
    VECTOR_039,
    VECTOR_040,
    VECTOR_041,
    VECTOR_042,
    VECTOR_043,
    VECTOR_044,
    VECTOR_045,
    VECTOR_046,
    VECTOR_047,
    VECTOR_048,
    VECTOR_049,
    VECTOR_050,
    VECTOR_051,
    VECTOR_052,
    VECTOR_053,
    VECTOR_054,
    VECTOR_055,
    VECTOR_056,
    VECTOR_057,
    VECTOR_058,
    VECTOR_059,
    VECTOR_060,
    VECTOR_061,
    VECTOR_062,
    VECTOR_063,
    VECTOR_064,
    VECTOR_065,
    VECTOR_066,
    VECTOR_067,
    VECTOR_068,
    VECTOR_069,
    VECTOR_070,
    VECTOR_071,
    VECTOR_072,
    VECTOR_073,
    VECTOR_074,
    VECTOR_075,
    VECTOR_076,
    VECTOR_077,
    VECTOR_078,
    VECTOR_079,
    VECTOR_080,
    VECTOR_081,
    VECTOR_082,
    VECTOR_083,
    VECTOR_084,
    VECTOR_085,
    VECTOR_086,
    VECTOR_087,
    VECTOR_088,
    VECTOR_089
};

#if (!defined(_LPC845_H_) && !defined(_LPC824_H_) && !defined(_LPC804_H_) && !defined(_LPC802_H_))
#pragma location = ".memcfg"
__root const uint32_t __memcfg_table[] = 
{    
   CONFIG_1,
   CONFIG_2,
   CONFIG_3,
   CONFIG_4,
};
#endif

/*******************************************************************************
* Code
******************************************************************************/

/*!
* default_isr(void)
*
* Default ISR definition.
*
* In:  n/a
* Out: n/a
*/
void default_isr(void)
{
#define VECTORNUM                     (*(volatile uint8_t *)(0xE000ED04))
    return;
}

/*
* NMI_isr(void)
*
* Non-maskable Interrupt (NMI)ISR definition.
*
* In:  n/a
* Out: n/a
*/
void NMI_isr(void)
{
}

/*
* Exception frame without floating-point storage
* hard fault handler in C,
* with stack frame location as input parameter
*/
void hard_fault_handler_c(uint32_t *hardfault_args)
{
}
