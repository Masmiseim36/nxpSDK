/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
