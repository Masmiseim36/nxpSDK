/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************
 * xf-hal.h
 *
 * Platform-specific HAL definitions
 *******************************************************************************/

#ifndef __XF_H
#error "xf-hal.h mustn't be included directly"
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/

/* ...primitive types */
#include "xf-dp_types.h"
#include "osal-isr.h"

/*******************************************************************************
 * Auxilliary macros definitions
 ******************************************************************************/

#if (XF_CFG_CORES_NUM > 1)
/* ...use system-specific cache-line size */
#define XF_PROXY_ALIGNMENT              XCHAL_DCACHE_LINESIZE
#else //(XF_CFG_CORES_NUM > 1)
/* ...use fixed alignment independent of cores used, this matches the memory numbers quoted in PG */
#define XF_PROXY_ALIGNMENT              64
#endif //(XF_CFG_CORES_NUM > 1)

/* ...properly aligned shared memory structure */
#define __xf_shmem__        __attribute__((__aligned__(XF_PROXY_ALIGNMENT)))

/* ...use hifi core agnostic maximum cache-line size */
#define XF_PROXY_MAX_CACHE_ALIGNMENT    256

/* ...properly aligned shared memory structure */
#define __xf_shmem_max_cache__          __attribute__((__aligned__(XF_PROXY_MAX_CACHE_ALIGNMENT)))

/*******************************************************************************
 * Auxiliary system-specific functions
 ******************************************************************************/

#if XF_CFG_CORES_NUM > 1
/* ...current core identifier (from HW) */
static inline UWORD32 xf_core_id(void)
{
    /* ...retrieve core identifier from HAL */
    return (UWORD32) xthal_get_prid();
}
#else
#define xf_core_id()        0
#endif

/*******************************************************************************
 * Atomic operations (atomicity is assured on local core only)
 ******************************************************************************/

static inline int xf_atomic_test_and_set(volatile UWORD32 *bitmap, UWORD32 mask)
{
    UWORD32     status;
    UWORD32     v;

    /* ...atomicity is assured by interrupts masking */
    status = __xf_disable_interrupts();
    v = *bitmap, *bitmap = v | mask;
    __xf_restore_interrupts(status);

    return !(v & mask);
}

static inline int xf_atomic_test_and_clear(volatile UWORD32 *bitmap, UWORD32 mask)
{
    UWORD32     status;
    UWORD32     v;

    /* ...atomicity is assured by interrupts masking */
    status = __xf_disable_interrupts();
    v = *bitmap, *bitmap = v & ~mask;
    __xf_restore_interrupts(status);

    return (v & mask);
}

static inline UWORD32 xf_atomic_set(volatile UWORD32 *bitmap, UWORD32 mask)
{
    UWORD32     status;
    UWORD32     v;

    /* ...atomicity is assured by interrupts masking */
    status = __xf_disable_interrupts();
    v = *bitmap, *bitmap = (v |= mask);
    __xf_restore_interrupts(status);

    return v;
}

static inline UWORD32 xf_atomic_clear(volatile UWORD32 *bitmap, UWORD32 mask)
{
    UWORD32     status;
    UWORD32     v;

    /* ...atomicity is assured by interrupts masking */
    status = __xf_disable_interrupts();
    v = *bitmap, *bitmap = (v &= ~mask);
    __xf_restore_interrupts(status);

    return v;
}

/*******************************************************************************
 * Abortion macro (debugger should be configured)
 ******************************************************************************/

/* ...breakpoint function */
extern void breakpoint(void);

/* ...abort execution (enter into debugger) */
#define __xf_abort()            breakpoint()
