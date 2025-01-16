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
 * xf-ipc.h
 *
 * IPC absraction layer (minimalistic)
 ******************************************************************************/

/*******************************************************************************
 * Includes
 ******************************************************************************/
#ifndef _XF_IPC_H
#define _XF_IPC_H

#include "osal-thread.h"

#include <xtensa/system/xmp_subsystem.h>
#if XCHAL_HAVE_S32C1I
#include <xtensa/tie/xt_sync.h>
#endif

/*******************************************************************************
 * Core compatibility check macros
 ******************************************************************************/
#if (!XCHAL_HAVE_S32C1I) && (!XCHAL_HAVE_EXCLUSIVE)
#error "core configuration: S32C1I/L32EX instructions are absent"
#endif

#if (!XCHAL_HAVE_PIF_WR_RESP)
#error "core configuration: PIFWriteResponse is absent"
#endif

#if (XCHAL_NUM_INTERRUPTS<1)
#error "core configuration: need at least 1 edge or level triggered interrupt"
#endif

/*******************************************************************************
 * IPC data type definitions
 ******************************************************************************/
typedef struct xf_ipc_lock_s {
    uint8_t buf[2*XCHAL_DCACHE_LINESIZE];
} xf_ipc_lock_t;

/*******************************************************************************
 * Helper Macros
 ******************************************************************************/
#define LOCK_ADDR(lock) (((uint32_t)&(lock->buf[0])+XCHAL_DCACHE_LINESIZE-1) & ~(XCHAL_DCACHE_LINESIZE-1))

/*******************************************************************************
 * External interrupt(BINTR) to Processor Interrupt mapping (PROCINTR)
 ******************************************************************************/
#define __i_bint_to_procint(a)                  XCHAL_EXTINT## a ## _NUM
#define __bint_to_procint(a)                    __i_bint_to_procint(a)

/* ...Processor interrupt number is mapped from external interrupt number for use in the library */
#define XF_PROC_INTERRUPT_NUMBER                __bint_to_procint(XF_EXTERNAL_INTERRUPT_NUMBER)

/*******************************************************************************
 * Derive Interrupt Mask and Address
 ******************************************************************************/
#define __i_interrupt_mask(core, interrupt)   	XMP_core## core ## _BINTR ## interrupt ## _MASK
#define __interrupt_mask(core, interrupt)       __i_interrupt_mask(core, interrupt)

#define __i_interrupt_addr(core, interrupt)   	XMP_core## core ## _BINTR ## interrupt ## _ADDR
#define __interrupt_addr(core, interrupt)       __i_interrupt_addr(core, interrupt)

/*******************************************************************************
 * Helper functions
 ******************************************************************************/

static inline void __delay(int32_t delay_count)
{
    int32_t i;
#pragma loop_count min=1
    for (i = 0; i < delay_count; i++)
        asm volatile ("_nop");
}

/* If successful, returns 'from' else returns value that is not 'from' */
static inline uint32_t __atomic_conditional_set(volatile uint32_t *addr, uint32_t from, uint32_t to)
{

#if XCHAL_HAVE_S32C1I

#if XCHAL_DCACHE_SIZE>0 && !XCHAL_DCACHE_IS_COHERENT
    xthal_dcache_line_invalidate((void *)addr);
#endif

    uint32_t val = *addr;

    if (val == from) {
        XT_WSR_SCOMPARE1(from);
        val = to;
        XT_S32C1I(val, (uint32_t *)addr, 0);
    }

#if XCHAL_DCACHE_SIZE>0 && !XCHAL_DCACHE_IS_COHERENT
    //xthal_dcache_line_writeback((void *)addr);
#endif

#elif XCHAL_HAVE_EXCLUSIVE

#if XCHAL_DCACHE_SIZE>0 && !XCHAL_DCACHE_IS_COHERENT && (XF_LOCAL_IPC_NON_COHERENT)
    xthal_dcache_line_invalidate((void *)addr);
#endif

    //int32_t val = xthal_compare_and_set((int32_t *)addr, from, to);
    int32_t val = XT_L32EX((int32_t *)addr);
    int32_t tmp = to;
    if (val == from) {
      XT_S32EX(tmp, (volatile int32_t *)addr);
      XT_GETEX(tmp);
      if (!tmp) val++;
#if XCHAL_DCACHE_SIZE>0 && !XCHAL_DCACHE_IS_COHERENT && (XF_LOCAL_IPC_NON_COHERENT)
      if(tmp)
      {
        xthal_dcache_line_writeback((void *)addr);
      }
#endif

    }

#else //XCHAL_HAVE_S32C1I
#error "S32C1I/L32EX support is absent"
#endif //XCHAL_HAVE_S32C1I

    return (uint32_t)val;
}

/*******************************************************************************
 * Lock operation
 ******************************************************************************/

/* ...lock definition */
//typedef uint32_t xf_ipc_lock_t __attribute__ ((aligned(XCHAL_DCACHE_LINESIZE)));

/* ...lock initialization */
static inline uint32_t __xf_ipc_lock_init(xf_ipc_lock_t *lock)
{
	return 0;
}

/* ...lock destroy */
static inline uint32_t __xf_ipc_lock_destroy(xf_ipc_lock_t *lock)
{
	return 0;
}

/* ...lock acquisition */
static inline uint32_t __xf_ipc_lock(xf_ipc_lock_t *lock)
{
	uint32_t *lock_ptr = (uint32_t *) LOCK_ADDR(lock);

    __xf_disable_preemption();
    uint32_t pid = XT_RSR_PRID();

    while (__atomic_conditional_set((volatile uint32_t *)lock_ptr, 0, pid + 1) != 0)
    {
        __delay(16);
    }

	return 0;
}

/* ...lock release */
static inline uint32_t __xf_ipc_unlock(xf_ipc_lock_t *lock)
{
	uint32_t *lock_ptr = (uint32_t *) LOCK_ADDR(lock);

    uint32_t pid = XT_RSR_PRID();
#pragma flush_memory
    while (__atomic_conditional_set((volatile uint32_t *)lock_ptr, pid + 1, 0) != pid + 1)
        __delay(16);

    __xf_enable_preemption();

    return 0;
}

static inline void __xf_ipc_interrupt_clear(uint32_t core)
{
    switch(core)
    {
        case 0:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(0, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;

        case 1:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(1, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;

#if XF_CFG_CORES_NUM>2
        case 2:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(2, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>3
        case 3:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(3, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>4
        case 4:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(4, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>5
        case 5:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(5, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>6
        case 6:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(6, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>7
        case 7:
        if(Xthal_inttype[XF_PROC_INTERRUPT_NUMBER] == XTHAL_INTTYPE_EXTERN_LEVEL)
        {
            *((volatile unsigned *) __interrupt_addr(7, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

        default:
            /* ... log/notify error */
        break;
    }
}

static inline void __xf_ipc_interrupt_notify(uint32_t core, uint32_t *pinterrupt)
{
    switch(core)
    {
        case 0:
        *((volatile unsigned *) __interrupt_addr(0, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(0, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(0, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;

        case 1:
        *((volatile unsigned *) __interrupt_addr(1, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(1, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(1, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;

#if XF_CFG_CORES_NUM>2
        case 2:
        *((volatile unsigned *) __interrupt_addr(2, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(2, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(2, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>3
        case 3:
        *((volatile unsigned *) __interrupt_addr(3, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(3, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(3, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>4
        case 4:
        *((volatile unsigned *) __interrupt_addr(4, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(4, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(4, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>5
        case 5:
        *((volatile unsigned *) __interrupt_addr(5, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(5, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(5, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>6
        case 6:
        *((volatile unsigned *) __interrupt_addr(6, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(6, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(6, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

#if XF_CFG_CORES_NUM>7
        case 7:
        *((volatile unsigned *) __interrupt_addr(7, XF_EXTERNAL_INTERRUPT_NUMBER)) = __interrupt_mask(7, XF_EXTERNAL_INTERRUPT_NUMBER);
        if(pinterrupt[0] == XTHAL_INTTYPE_EXTERN_EDGE)
        {
            /* ...clear immediately for edge-triggered interrupt */
            *((volatile unsigned *) __interrupt_addr(7, XF_EXTERNAL_INTERRUPT_NUMBER)) = 0;
        }
        break;
#endif

        default:
            /* ... log/notify error */
        break;
    }
}

#include "xf-shared.h"

__attribute__((unused)) static inline uint32_t
ipc_load(volatile uint32_t *address)
{
#if (XCHAL_DCACHE_SIZE>0 && !XCHAL_DCACHE_IS_COHERENT)
  xthal_dcache_line_invalidate((void *)address);
#endif
  return *address;
}

__attribute__((unused)) static inline void
ipc_store(uint32_t value, volatile uint32_t *address)
{
  *address = value;
#if (XCHAL_DCACHE_SIZE>0 && !XCHAL_DCACHE_IS_COHERENT)
  xthal_dcache_line_writeback((void *)address);
#endif
}

/* synchronization for all procs in the subsystem. Does not
 * require initialization. Typically used during bootup synchronization.
 * Synchronization is performed with proc with id XF_CORE_ID_MASTER as the master processor.
 *
 * sync : Array of ipc_reset_sync_t structures, one per processor for
 *        initial synchronization. Note, each ipc_reset_sync_t element in
 *        the array is assumed to be max dache line size across all processors.
 *
 */

#define XF_IPC_SYNC_TIMEOUT_MSEC    1000

static inline int __xf_ipc_reset_sync(void)
{
  uint32_t pid;
  uint32_t my_pid = XT_RSR_PRID();
  volatile ipc_reset_sync_t *p_sync;
  volatile ipc_reset_sync_t *p_sync_sentinel;
  const uint8_t IPC_RESET_SYNC_POST = 128;
  /* Each element in the reset sync array is max dcache line size across
   * all processors. Convert to units of uint32s */
  //uint32_t reset_sync_elem_inc = XMP_MAX_DCACHE_LINESIZE/4;
  uint32_t reset_sync_elem_inc = sizeof(ipc_reset_sync_t);
  volatile ipc_reset_sync_t *sync = (ipc_reset_sync_t *)shared_mem_ipc_reset_sync;
  uint32_t timeout_counter;

  p_sync = sync;
  p_sync_sentinel = p_sync + reset_sync_elem_inc*XF_CORE_ID_MASTER;

  if (my_pid == XF_CORE_ID_MASTER)
  {
    /* ...master core */
    /* Initialize each proc's reset_sync location with their respective
     * proc ids */
    for (pid = 0; pid < XMP_NUM_PROCS; pid++) {
      /* Initialize my reset_sync location to a pattern */
      ipc_store(0, (uint32_t*)&p_sync->_);

      ipc_store(pid, (uint32_t *)&p_sync->_);
      p_sync+=reset_sync_elem_inc;
    }
    /* Wait for all the procs to set their respective reset_sync location
     * to 0 */
    p_sync = sync;
    timeout_counter = XF_IPC_SYNC_TIMEOUT_MSEC;
    for (pid = 0; pid < XMP_NUM_PROCS; pid++) {
      if(pid != my_pid)
      {
        while (ipc_load((uint32_t *)&p_sync->_) != 0)
        {
            if(timeout_counter--)
            {
                __xf_thread_sleep_msec(1);
                continue;
            }
            return -1;
        }
        /* Re-initialize each proc's reset_sync location with its proc id */
        ipc_store(pid, (uint32_t*)&p_sync->_);
      }
      p_sync+=reset_sync_elem_inc;
    }
#pragma flush_memory
    /* All procs have reached the barrier. Write a sentinal to proc0's
     * reset_sync location to let all other procs know the same. */
    ipc_store(IPC_RESET_SYNC_POST, (uint32_t*)&p_sync_sentinel->_);

    /* Final step: Wait for all the procs to set their respective reset_sync location
     * to 1. Worker cores write and proceed immediately.
     * Master core waits until all worker cores have written the pattern to complete the sync */
    p_sync = sync;
    timeout_counter = XF_IPC_SYNC_TIMEOUT_MSEC;
    for (pid = 0; pid < XMP_NUM_PROCS; pid++) {
      if(pid != my_pid)
      {
        while (ipc_load((uint32_t *)&p_sync->_) != 1)
        {
            if(timeout_counter--)
            {
                __xf_thread_sleep_msec(1);
                continue;
            }
            return -2;
        }
      }
      p_sync+=reset_sync_elem_inc;
    }
  }
  else
  {
    /* ...worker core */
    p_sync += (my_pid*reset_sync_elem_inc);

    /* Wait for my reset_sync location to be set to my proc id by proc 0 */
    timeout_counter = XF_IPC_SYNC_TIMEOUT_MSEC;
    while (ipc_load((uint32_t*)&p_sync->_) != my_pid)
    {
        if(timeout_counter--)
        {
            __xf_thread_sleep_msec(1);
            continue;
        }
        return -1;
    }

    /* Re-initialize my reset_sync location to 0 */
    ipc_store(0, (uint32_t*)&p_sync->_);
#pragma flush_memory
    /* Wait until all other procs have also reached the barrier by waiting
     * on proc 0 to write the sentinal. Proc 0 sets the sentinal in its
     * reset_sync location when all procs have reached the barrier */
    timeout_counter = XF_IPC_SYNC_TIMEOUT_MSEC;
    while (ipc_load((uint32_t*)&p_sync_sentinel->_) != IPC_RESET_SYNC_POST)
    {
        if(timeout_counter--)
        {
            __xf_thread_sleep_msec(1);
            continue;
        }
        return -2;
    }

    /* Final step: Re-initialize my reset_sync location to 1 from the worker cores
     * Worker cores write and proceed immediately.
     * Master core waits until all worker cores have written the pattern to complete the sync */
    ipc_store(1, (uint32_t*)&p_sync->_);
  }
  return 0;
}

#endif //_XF_IPC_H
