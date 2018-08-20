/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
