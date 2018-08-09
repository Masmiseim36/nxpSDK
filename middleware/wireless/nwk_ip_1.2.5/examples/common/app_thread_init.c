/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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


/*==================================================================================================
Include Files
==================================================================================================*/

/* Framework */
#include "NVM_Interface.h"
#include "RNG_Interface.h"
#include "SecLib.h"
#include "Flash_Adapter.h"
 
/* IP */
#include "ip.h"
#include "ip6.h"
#include "ip_cfg.h"
#include "session.h"
#include "trickle.h"
#include "sixlowpan.h"
#include "sixlowpan_cfg.h"
#include "network_utils.h"


/* Thread */
#include "thread_cfg.h"
#include "thread_types.h"
#include "thread_network.h"
#include "thread_utils.h"
#include "app_thread_config.h"
#include "coap.h"
#if DNS_ENABLED
    #include "dns_client.h"
#endif

#if STACK_THREAD

extern taskMsgQueue_t *mpThrTaskMsgQueue;
extern taskMsgQueue_t *mpThrIpTaskMsgQueue;

OSA_TASK_PROTO(IP_Task);
OSA_TASK_PROTO(SLWP_Task);
extern taskMsgQueue_t mIpMsgQueue;
extern taskMsgQueue_t mSlwpMsgQueue;
/*==================================================================================================
Private macros
==================================================================================================*/

#define gThrDefaultInstanceId_c 0
#define EXTENDED_802154_MAC_PREFIX_VALUE            0x00603700
#define EXTENDED_802154_MAC_PREFIX_MASK             0x000000FF
#define EXTENDED_802154_MAC_SUFFIX_VALUE            0x00000000
#define EXTENDED_802154_MAC_SUFFIX_MASK             THR_ALL_FFs32
#define UNIVERSAL_LOCAL_BIT_MASK                    0x0200000000000000 /* (1 << 57) */
/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

/* Thread Task */
OSA_TASK_DEFINE(THR_Task, OSA_PRIORITY_IDLE,  1, THREAD_TASK_STACK_SIZE, 0);

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*! Task message queue used by the Thread Stack */
taskMsgQueue_t mThreadStackMsgQueue;

uint32_t gaUniqueId[4];
#if !THREAD_ED_CONFIG
/*!< List of (EUI, PSKD) tuples */
list_t gThrExpectedJoinerList = {0};
#endif
/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void THR_Init(void)
\brief  Initialize Thread module.

\return      NONE
***************************************************************************************************/
void THR_Init
(
    void
)
{
    if(mpThrTaskMsgQueue == &mThreadStackMsgQueue)
    {
         /* protection for second call */
         return;
    }
    #if defined(SIM_UIDH)
    gaUniqueId[0] = SIM->UIDH;
#else
    gaUniqueId[0] = THR_ALL_FFs32;
#endif
#if SIM_UIDH_UID_MASK
    gaUniqueId[1] = SIM->UIDH;
#else
    gaUniqueId[1] = SIM->UIDMH;
#endif
#if SIM_UIDL_UID_MASK
    gaUniqueId[2] = SIM->UIDL;
#else
    gaUniqueId[2] = SIM->UIDML;
#endif
    gaUniqueId[3] = SIM->UIDL;

    mpThrTaskMsgQueue = &mThreadStackMsgQueue;

    /* Start Thread Stack Task */
    ListInit(&mpThrTaskMsgQueue->msgQueue,THREAD_TASK_MSG_QUEUE_SIZE);
    mpThrTaskMsgQueue->taskEventId = OSA_EventCreate(TRUE);
    mpThrTaskMsgQueue->taskId = OSA_TaskCreate(OSA_TASK(THR_Task), (osaTaskParam_t)NULL);
}
/*!*************************************************************************************************
\fn     void THR_Task(osaTaskParam_t argument)
\brief  Thread application task.

\param [in]  argument  Task private data

\return      NONE
***************************************************************************************************/
void THR_Task
(
    osaTaskParam_t argument
)
{
    static uint8_t thrInitialized = FALSE;

    if( !thrInitialized )
    {
        thrInitialized = TRUE;
        THR_InitAttributes(gThrDefaultInstanceId_c, pStackCfg[gThrDefaultInstanceId_c]);
        mpThrIpTaskMsgQueue = IP_Init();
        Session_Init();
        #if TRICKLE_ENABLED
        Trickle_Init(mpThrIpTaskMsgQueue);
        #endif
        #if COAP_ENABLED
        COAP_Init(mpThrIpTaskMsgQueue);
        #endif
        #if DNS_ENABLED
        /* DNS Client */
        DNS_ClientInit(mpThrIpTaskMsgQueue);
        #endif
    }

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
            paMsgQueue[2] = mpThrTaskMsgQueue; // lower priority

            (void)OSA_EventWait(mpThrTaskMsgQueue->taskEventId, NWKU_GENERIC_MSG_EVENT, FALSE, osaWaitForever_c,
                &eventFlags);

            handleMsg = TRUE;
            while(handleMsg)
            {
                for(i = 0; i < 3; i++)
                {
                    handleMsg = NWKU_MsgHandler(paMsgQueue[i]);

                    /* For BareMetal, process only one message at a time */
                    if (!gUseRtos_c && handleMsg)
                    {
                        break;
                    }

                    if(handleMsg == TRUE)
                    {
                        break; // break and handle the next message from the current queue
                    }
                    //if(handleMsg == FALSE), it will go to next queue
                }

                /* For BareMetal, if there are pending messages send an event to handle the next messages */
                if (!gUseRtos_c)
                {
                    for(i = 0; i < 3; i++)
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
\fn     taskMsgQueue_t *Ip_TaskMsgQueueInit(void)
\brief  Initialize IP task message queue (if enabled can create IP Task).

\return  [out]  taskMsgQueue_t *    Pointer to task message queue
***************************************************************************************************/
taskMsgQueue_t* Ip_TaskMsgQueueInit(void)
{

     pIpMsgQueue = &mIpMsgQueue;
     ListInit(&pIpMsgQueue->msgQueue,IP_TASK_MSG_QUEUE_SIZE);
     pIpMsgQueue->taskEventId = mpThrTaskMsgQueue->taskEventId;
     pIpMsgQueue->taskId      = mpThrTaskMsgQueue->taskId;
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
     ListInit(&pSlwpMsgQueue->msgQueue,SLWPCFG_MSG_QUEUE_SIZE);
     pSlwpMsgQueue->taskEventId = mpThrTaskMsgQueue->taskEventId;
     pSlwpMsgQueue->taskId      = mpThrTaskMsgQueue->taskId;
     return pSlwpMsgQueue;
}
/*!*************************************************************************************************
\fn     void THR_FactoryReset(void)
\brief  This function is used to reset device to factory default settings.

\return      NONE
***************************************************************************************************/
void THR_FactoryReset
(
    void
)
{
     /* should not call the NvFormat here. The function will be called
        when the gSwResetTimestamp is reached. Set only the flag here*/
     THR_TimeoutResetMcu(THR_RESET_MCU_TIMEOUT_MS, TRUE);
}
/*!*************************************************************************************************
\fn     void THR_TimeoutResetMcu(uint32_t timeoutMs, bool_t resetToFactory)
\brief  This function is used to reset the device after a specific timeout.

\param [in]  timeoutMs       Time expressed in milliseconds units.
       [in]  resetToFactory  If TRUE, the device will be reseted to factory

\return      NONE      
***************************************************************************************************/
void THR_TimeoutResetMcu(uint32_t timeoutMs, bool_t resetToFactory)
{
    APP_ResetMcuOnTimeout(timeoutMs, resetToFactory);
    EVM_EventNotify(gThrEv_GeneralInd_ResetMcuTimeout_c, NULL, 0, gEvmAnyInstanceId_c);
}
    
/*!*************************************************************************************************
\fn    uint64_t THR_GenerateExtendedAddress(bool_t privacyAddr)
\brief This function generates a random extended mac address

\param [in]  privacyAddr       TRUE if the address should be a privacy address

\retval      uint64_t          extended mac address
***************************************************************************************************/
uint64_t THR_GenerateExtendedAddress
(
    bool_t privacyAddr
)
{
    uint32_t macPrefix, macSuffix;
    uint64_t extendedAddr;
    bool_t isHwAddress = FALSE;

    if (FALSE == privacyAddr)
    {
        if (!FLib_MemCmpToVal(&(gHardwareParameters.ieee_802_15_4_address[0]), THR_ALL_FFs8, sizeof(gHardwareParameters.ieee_802_15_4_address)))
        {
            isHwAddress = TRUE;
            FLib_MemCpy(&extendedAddr, &(gHardwareParameters.ieee_802_15_4_address[0]), sizeof(gHardwareParameters.ieee_802_15_4_address));
        }
        else if ((gaUniqueId[2] == THR_ALL_FFs32) && (gaUniqueId[3] == THR_ALL_FFs32))
        {
            /* Generate random data */
            RNG_GetRandomNo(&macPrefix);
            RNG_GetRandomNo(&macSuffix);
        }
        else
        {
            sha256Context_t mCtx;
            SHA256_Hash(&mCtx, (uint8_t*)gaUniqueId, sizeof(gaUniqueId));
            FLib_MemCpy(&macSuffix, (uint8_t*)(&mCtx.hash), sizeof(macSuffix));
            FLib_MemCpy(&macPrefix, ((uint8_t*)(&mCtx.hash))+4, sizeof(macPrefix));
        }

        if (!isHwAddress)
        {
            macPrefix &= EXTENDED_802154_MAC_PREFIX_MASK;
            macPrefix |= EXTENDED_802154_MAC_PREFIX_VALUE;
        }
    }
    else
    {
        /* Generate random data */
        RNG_GetRandomNo(&macPrefix);
        RNG_GetRandomNo(&macSuffix);
    }

    if (!isHwAddress)
    {
        extendedAddr = (((uint64_t)macPrefix) << 32U) | macSuffix;

        if (FALSE == privacyAddr)
        {
            /* Set the Universal/Local bit to 0 - for Universal address*/
            extendedAddr &= ~UNIVERSAL_LOCAL_BIT_MASK;
        }
        else
        {
            /* Set the Universal/Local bit to 1 for Local address */
            extendedAddr |= UNIVERSAL_LOCAL_BIT_MASK;
        }
    }

    return extendedAddr;
}

/*!*************************************************************************************************
\fn    void THR_GetUniqueId(uint32_t *pOut)
\brief This function retrieves the board's unique id.

\param [in]  pOut               pointer to the memory area where the unique id will be stored

\return      none
***************************************************************************************************/
void THR_GetUniqueId
(
    uint32_t *pOut
)
{
    FLib_MemCpy(pOut, &gaUniqueId, sizeof(gaUniqueId));
}

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Private debug functions
==================================================================================================*/

#endif /* STACK_THREAD */
