/*! *********************************************************************************
 * \addtogroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* \brief     This file is a source file for nwk_ip initialization.
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework */
#include "EmbeddedTypes.h"


/* 6Lo */
#include "sixlowpan.h"
#include "ip_if_6LoBle.h"
#include "sixlowpan.h"
#include "sixlowpan_cfg.h"
#include "sixloble_interface.h"

/* ND */
#include "nd.h"
#include "nd_cfg.h"
#include "nd_sixlowpan.h"

/* IP */ 
#include "ip.h"
#include "ip6.h"
#include "ip_cfg.h"
#include "ip_if_management.h"

#include "session.h"
#include "coap.h"

#include "network_utils.h"
#include "nwk_ip_init.h"

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
static void Ip_Task(osaTaskParam_t argument);
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
OSA_TASK_DEFINE(Ip_Task, gIpStack_TaskPriority_c, 1, gIpStack_TaskStackSize_c, FALSE);

/*! Pointer to the Ip task message queue */
taskMsgQueue_t mIpTaskMsgQueue;

taskMsgQueue_t   mNwkIpMsgQueue = {{0}};
taskMsgQueue_t*  pNwkIpMsgQueue = NULL;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
taskMsgQueue_t* NwkIp_TaskMsgQueueInit(void);
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern taskMsgQueue_t mIpMsgQueue;
extern taskMsgQueue_t mSlwpMsgQueue;

/* stack nwk memory managers poolId */
const uint8_t gNwkPoolId = 0;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
osaStatus_t NwkIpManager_TaskInit(void)
{
    /* Already initialized? */
    if(mIpTaskMsgQueue.taskId)
    {
        return osaStatus_Error;
    }

    /* Initialization of task related */
    mIpTaskMsgQueue.taskEventId = OSA_EventCreate(TRUE);
    if( mIpTaskMsgQueue.taskEventId == NULL)
    {
        return osaStatus_Error;
    }

    /* Initialization of task message queue */
    MSG_InitQueue(&mIpTaskMsgQueue.msgQueue);

    pNwkIpMsgQueue = NwkIp_TaskMsgQueueInit();

    if( NULL == pNwkIpMsgQueue )
    {
        return osaStatus_Error;
    }
        
    mIpTaskMsgQueue.taskId = OSA_TaskCreate(OSA_TASK(Ip_Task), (osaTaskParam_t)NULL);

    if( NULL == mIpTaskMsgQueue.taskId )
    {
        return osaStatus_Error;
    }

    return osaStatus_Success;
}

static void Ip_Task(osaTaskParam_t argument)
{
    IP_Init();
    Session_Init();
    
#if COAP_ENABLED
    COAP_Init(pNwkIpMsgQueue);
#endif    
    
    while (1)
    {
        {
            osaEventFlags_t eventFlags;
            bool_t handleMsg;
            uint8_t i;
            taskMsgQueue_t* paMsgQueue[3];

            /* This will set the priority for the queues. Lower index means higher priority*/
            paMsgQueue[0] = pSlwpMsgQueue; //higher priority
            paMsgQueue[1] = pIpMsgQueue;
            paMsgQueue[2] = pNwkIpMsgQueue; 

            (void)OSA_EventWait(mIpTaskMsgQueue.taskEventId, NWKU_GENERIC_MSG_EVENT, FALSE, osaWaitForever_c,
                &eventFlags);

            handleMsg = TRUE;
            while(handleMsg)
            {
                for(i = 0; i < NumberOfElements(paMsgQueue); i++)
                {
                    handleMsg = NWKU_MsgHandler(paMsgQueue[i]);

                    /* For BareMetal, process only one message at a time */
                    if (!gUseRtos_c && handleMsg)
                    {
                        break;
                    }
                    if(handleMsg == TRUE)
                    {
                        break; 
                    }
                }

                /* For BareMetal, if there are pending messages send an event to handle the next messages */
                if (!gUseRtos_c)
                {
                    for(i = 0; i < NumberOfElements(paMsgQueue); i++)
                    {
                        if(MSG_Pending(&paMsgQueue[i]->msgQueue))
                        {
                            (void)OSA_EventSet(paMsgQueue[i]->taskEventId, NWKU_GENERIC_MSG_EVENT);
                            break;
                        }
                    }
                }
            }
        }

        /* For BareMetal break the while(1) after 1 run */
        if(gUseRtos_c == 0)
        {
            break;
        }
    }
}

/*!*************************************************************************************************
\fn Ip_TaskMsgQueueInit
\brief  Init ip task message queue (if enabled can create Ip Task)

\param  [in]    None


\retval [out]   taskMsgQueue_t* pointer to task message queue
***************************************************************************************************/
taskMsgQueue_t* Ip_TaskMsgQueueInit(void)
{

     pIpMsgQueue = &mIpMsgQueue;
     ListInit(&pIpMsgQueue->msgQueue,IP_TASK_MSG_QUEUE_SIZE);

     pIpMsgQueue->taskEventId = mIpTaskMsgQueue.taskEventId;
     pIpMsgQueue->taskId      = mIpTaskMsgQueue.taskId;
 
     return pIpMsgQueue;
}

/*!*************************************************************************************************
\fn SLWP_TaskMsgQueueInit
\brief  Init sixlowpan task message queue (if enabled can create SLWP Task)

\param  [in]    None


\retval [out]   taskMsgQueue_t* pointer to task message queue
***************************************************************************************************/
taskMsgQueue_t* SLWP_TaskMsgQueueInit(void)
{
     pSlwpMsgQueue = &mSlwpMsgQueue;
     ListInit(&pSlwpMsgQueue->msgQueue, SLWPCFG_MSG_QUEUE_SIZE);
     
     pSlwpMsgQueue->taskEventId = mIpTaskMsgQueue.taskEventId;
     pSlwpMsgQueue->taskId      = mIpTaskMsgQueue.taskId;

     return pSlwpMsgQueue;
}

/*!*************************************************************************************************
\fn AppNwk_TaskMsgQueueInit
\brief  Init App Network message queue

\param  [in]    None


\retval [out]   taskMsgQueue_t* pointer to task message queue
***************************************************************************************************/
taskMsgQueue_t* NwkIp_TaskMsgQueueInit(void)
{
     pNwkIpMsgQueue = &mNwkIpMsgQueue;
     ListInit(&pNwkIpMsgQueue->msgQueue, 20);
     
     pNwkIpMsgQueue->taskEventId = mIpTaskMsgQueue.taskEventId;
     pNwkIpMsgQueue->taskId      = mIpTaskMsgQueue.taskId;

     return pNwkIpMsgQueue;
}
/*! *********************************************************************************
* @}
********************************************************************************** */
