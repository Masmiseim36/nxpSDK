/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
/*!=============================================================================
\file       zcl_heap.c
\brief      Manages the heap used to allocate memory to components. 
            Simple design: doesn't allow the memory to be returned.
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>

#include "zcl.h"
#include "zcl_common.h"
#include "zcl_internal.h"
#include "zcl_heap.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Imported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME:        pvZCL_HeapAlloc
 *
 * DESCRIPTION:
 * Allocates a block of memory from the heap.
 *
 * RETURNS:
 * Pointer to block, or NULL if the heap didn't have enough space. If block
 * was already assigned, returns original value and doesn't take anything
 * from heap.
 *
 * NOTES:
 * If buffer has already been allocated, it is not cleared. If it is a fresh
 * allocation, it is cleared on request.
 *
 ****************************************************************************/

PUBLIC  void* pvZCL_HeapAlloc(void *pvPointer, uint32 u32BytesNeeded, bool_t bClear)
{
    uint32 u32HeapStartAligned;

    /* If value already assigned, this is an error in ZCL so return NULL*/
    if (pvPointer != NULL)
    {
        return NULL;
    }

    /* Ensure heap start is word aligned */
    u32HeapStartAligned = (psZCL_Common->u32HeapStart + 3) & 0xfffffffc;

    /* If not enough space, return null */
    if ((u32HeapStartAligned + u32BytesNeeded) > psZCL_Common->u32HeapEnd)
    {
        return NULL;
    }

    /* Clear assigned space */
    if (bClear)
    {
        FLib_MemSet((void *)u32HeapStartAligned, 0, u32BytesNeeded);
    }

    /* Move heap start along (not necessarily word aligned, doesn't matter) */
    psZCL_Common->u32HeapStart = u32HeapStartAligned + u32BytesNeeded;

    return (void *)u32HeapStartAligned;
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
