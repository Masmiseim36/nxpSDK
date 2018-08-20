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
\file       ZQueus.c
\brief      Provides the api functions for ZPS Queue Module
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#ifndef CPU_MKW41Z512VHT4
#include "MicroSpecific.h"
#else
#include "Messaging.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#endif
#include "ZQueue.h"
#include "dbg.h"
#include "pwrm.h"
#include "FunctionLib.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifdef DEBUG_ZQUEUE
#define TRACE_ZQUEUE    TRUE
#else
#define TRACE_ZQUEUE    FALSE
#endif

#ifdef CPU_MKW41Z512VHT4
    /*! The MemManager Poll Id used by the Zigbee layer */
    #ifndef gZbPoolId_d
        #define gZbPoolId_d 0
    #endif
    /* Default memory allocator */
    #ifndef ZB_BufferAlloc
        #define ZB_BufferAlloc(numBytes)   MEM_BufferAllocWithId(numBytes, gZbPoolId_d, (void*)__get_LR())
    #endif
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

PUBLIC void ZQ_vQueueCreate ( tszQueue*       psQueueHandle, 
                              const uint32    u32QueueLength, 
                              const uint32    u32ItemSize, 
                              uint8*          pu8StartQueue )
{
#ifdef CPU_MKW41Z512VHT4
        ListInit(&psQueueHandle->list,u32QueueLength);
        psQueueHandle->u32ItemSize =  u32ItemSize;
#else    
        psQueueHandle->pvHead =  pu8StartQueue;
        psQueueHandle->u32ItemSize =  u32ItemSize;
        psQueueHandle->u32Length =  u32QueueLength;
        psQueueHandle->pvWriteTo =  psQueueHandle->pvHead;
        psQueueHandle->u32MessageWaiting =  0;
        psQueueHandle->pvReadFrom =  psQueueHandle->pvHead; 
#endif
        DBG_vPrintf(TRACE_ZQUEUE, "ZQ: Initialised a queue: Handle=%08x Length=%d ItemSize=%d\n", (uint32)psQueueHandle, u32QueueLength, u32ItemSize); 
}

PUBLIC bool_t ZQ_bQueueSend ( void*          pvQueueHandle, 
                              const void*    pvItemToQueue )
{
#ifdef CPU_MKW41Z512VHT4
    OSA_InterruptDisable();
    tszQueue *psQueueHandle = (tszQueue *)pvQueueHandle;
    /* Put a message in a queue. */
    if(ListGetAvailable(&psQueueHandle->list) || (0 == psQueueHandle->list.max))
    {
        void* pMsg = ZB_BufferAlloc(psQueueHandle->u32ItemSize);
        if(pMsg)
        {
            FLib_MemCpy(pMsg, (void*)pvItemToQueue, psQueueHandle->u32ItemSize);
            /* Put a message in a queue. */
            MSG_Queue(&psQueueHandle->list, pMsg);
            OSA_InterruptEnable();
            return TRUE;
        }   
    }
    OSA_InterruptEnable();
    return FALSE;
#else
    MICRO_INT_STORAGE;

    tszQueue *psQueueHandle = (tszQueue *)pvQueueHandle;
    if(psQueueHandle->u32MessageWaiting >= psQueueHandle->u32Length)
    {
        DBG_vPrintf(TRACE_ZQUEUE, "ZQ: Queue overflow: Handle=%08x\n", (uint32)pvQueueHandle);
        return FALSE;
    }
    else
    {

        MICRO_INT_ENABLE_ONLY(0);
#ifndef __ICCARM__        
        if( psQueueHandle->pvWriteTo >= (psQueueHandle->pvHead+(psQueueHandle->u32Length*psQueueHandle->u32ItemSize)))
        {
             psQueueHandle->pvWriteTo = psQueueHandle->pvHead;
        }
        ( void ) FLib_MemCpy( psQueueHandle->pvWriteTo, pvItemToQueue, psQueueHandle->u32ItemSize );
        psQueueHandle->u32MessageWaiting++;
        psQueueHandle->pvWriteTo += psQueueHandle->u32ItemSize;
#else
        uint32 u32Size = psQueueHandle->u32Length*psQueueHandle->u32ItemSize;
        uint8 *pu8Head = psQueueHandle->pvHead;
        if( psQueueHandle->pvWriteTo >= (pu8Head + u32Size))
        {
            psQueueHandle->pvWriteTo = psQueueHandle->pvHead;
        }
        ( void ) FLib_MemCpy( psQueueHandle->pvWriteTo, pvItemToQueue, psQueueHandle->u32ItemSize );    
        psQueueHandle->u32MessageWaiting++;
        
        uint8 *pu8WrtieTo = psQueueHandle->pvWriteTo;
        pu8WrtieTo += psQueueHandle->u32ItemSize;
        
        psQueueHandle->pvWriteTo = pu8WrtieTo;
#endif
        
        /* Increase power manager activity count */
        PWRM_eStartActivity();

        MICRO_INT_RESTORE_STATE();
    }
    return TRUE;
#endif
  
}

PUBLIC bool_t ZQ_bQueueReceive ( void*    pvQueueHandle, 
                                 void*    pvItemFromQueue )
{
#ifdef CPU_MKW41Z512VHT4
    OSA_InterruptDisable();
    tszQueue *psQueueHandle = (tszQueue *)pvQueueHandle;
    if( MSG_Pending(&psQueueHandle->list))
    {
        void* pMsg = MSG_DeQueue(&psQueueHandle->list);
        FLib_MemCpy( pvItemFromQueue, pMsg, psQueueHandle->u32ItemSize );
        MSG_Free(pMsg);
    }
    else
    {
        OSA_InterruptEnable(); 
        return FALSE;
    }
    OSA_InterruptEnable();
    return TRUE;
#else
    MICRO_INT_STORAGE;
    tszQueue *psQueueHandle = (tszQueue *)pvQueueHandle;
    if( psQueueHandle->u32MessageWaiting >  0)
    {

        MICRO_INT_ENABLE_ONLY(0);

#ifndef __ICCARM__        
        if( psQueueHandle->pvReadFrom >= (psQueueHandle->pvHead+(psQueueHandle->u32Length*psQueueHandle->u32ItemSize) ))
        {
            psQueueHandle->pvReadFrom = psQueueHandle->pvHead;
        }
        ( void ) FLib_MemCpy( pvItemFromQueue, psQueueHandle->pvReadFrom, psQueueHandle->u32ItemSize );
        psQueueHandle->pvReadFrom += psQueueHandle->u32ItemSize;
        psQueueHandle->u32MessageWaiting--;
#else
        uint32 u32Size = psQueueHandle->u32Length*psQueueHandle->u32ItemSize;
        uint8 *pu8Head = psQueueHandle->pvHead;
        if( psQueueHandle->pvReadFrom >= pu8Head+u32Size)
        {
                psQueueHandle->pvReadFrom = psQueueHandle->pvHead;
        }
        ( void ) FLib_MemCpy( pvItemFromQueue, psQueueHandle->pvReadFrom, psQueueHandle->u32ItemSize );
        uint8 *pu8ReadFrom = psQueueHandle->pvReadFrom;
        pu8ReadFrom += psQueueHandle->u32ItemSize;
        psQueueHandle->pvReadFrom = pu8ReadFrom;
        psQueueHandle->u32MessageWaiting--;        
#endif
        
        /* Decrease power manager activity count */
        PWRM_eFinishActivity();

        MICRO_INT_RESTORE_STATE();
    }
    else
    {
        return FALSE;
    }
    return TRUE;
#endif    
}

PUBLIC bool_t ZQ_bQueueIsEmpty ( void*    pu8QueueHandle )
{
    tszQueue *psQueueHandle = (tszQueue *)pu8QueueHandle;
    #ifdef CPU_MKW41Z512VHT4
    if (psQueueHandle->list.size == 0) return (TRUE);
    else return (FALSE);
    #else
    if (psQueueHandle->u32MessageWaiting == 0) return (TRUE);
    else return (FALSE);
    #endif
}


PUBLIC uint32 ZQ_u32QueueGetQueueSize ( void*    pu8QueueHandle )
{
    tszQueue *psQueueHandle = (tszQueue *)pu8QueueHandle;
    #ifdef CPU_MKW41Z512VHT4
    return (uint32)psQueueHandle->list.max;
    #else
    return psQueueHandle->u32Length;
    #endif
}

PUBLIC uint32 ZQ_u32QueueGetQueueMessageWaiting ( void*    pu8QueueHandle )
{
    tszQueue *psQueueHandle = (tszQueue *)pu8QueueHandle;
    #ifdef CPU_MKW41Z512VHT4
    return (uint32)psQueueHandle->list.size;
    #else
    return psQueueHandle->u32MessageWaiting;
    #endif
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
