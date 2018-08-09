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

#ifndef ZQUEUE_H_
#define ZQUEUE_H_

/*!
\file       ZQueue.h
\brief      Zigbee Queue module api
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "string.h"

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#ifdef CPU_MKW41Z512VHT4
#include "GenericList.h"
typedef struct
{
    list_t list;
    uint32 u32ItemSize; 
}tszQueue;
#else
typedef struct
{
    uint32 u32Length;                    /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    uint32 u32ItemSize;                /*< The size of each items that the queue will hold. */
    uint32 u32MessageWaiting;
    void  *pvHead;                    /*< Points to the beginning of the queue storage area. */
    void  *pvWriteTo;                    /*< Points to the free next place in the storage area. */
    void  *pvReadFrom;                /*< Points to the free next place in the storage area. */
}tszQueue;
#endif


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void ZQ_vQueueCreate (tszQueue *psQueueHandle, const uint32 uiQueueLength, const uint32 uiItemSize, uint8* pu8StartQueue);
PUBLIC bool_t ZQ_bQueueSend(void *pvQueueHandle, const void *pvItemToQueue);
PUBLIC bool_t ZQ_bQueueReceive(void *pvQueueHandle, void *pvItemFromQueue);
PUBLIC bool_t ZQ_bQueueIsEmpty(void *pvQueueHandle);
PUBLIC uint32 ZQ_u32QueueGetQueueSize(void *pvQueueHandle);
PUBLIC uint32 ZQ_u32QueueGetQueueMessageWaiting ( void*    pu8QueueHandle );
#endif /*ZQUEUE_H_*/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
