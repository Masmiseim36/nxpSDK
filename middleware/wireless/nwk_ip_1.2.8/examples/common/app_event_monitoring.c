/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


/*!=================================================================================================
\file       app_monitoring.c
\brief      This is a public source file for the application monitoring system.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/


#if THR_ENABLE_EVENT_MONITORING
#include "app_stack_config.h"
#include "app_init.h"
#include "event_manager.h"
#include "thread_types.h"
#include "app_thread_config.h"
#include "thread_network.h"
#include "app_event_monitoring.h"

#if THREAD_USE_THCI
    #include "thci.h"
    #include "FsciInterface.h"
#endif

#if THREAD_USE_SHELL
    #include "shell_ip.h"
#endif
/*==================================================================================================
Private macros
==================================================================================================*/
#define gEvmMonitTypeMask_c             (0xFF000000)
#define gEvmMonitSetMask_c              (0xFFFF0000)
#define gEvmMonitModuleMask_c           (0x00FF0000)

#define gEvmModule6LoWPANAnyEvent_c     (gEvmModule6LoWPAN_c | gEvmMonitModuleMask_c)
#define gEvmModuleNdAnyEvent_c          (gEvmModuleNd_c | gEvmMonitModuleMask_c)
#define gEvmModuleThreadAnyEvent_c      (gEvmModuleThread_c | gEvmMonitModuleMask_c)

#define gEvmMonitorTableSize_c          0x03

#define gEvmAsyncronMonitoring_c        TRUE
/*==================================================================================================
Private type definitions
==================================================================================================*/
/*! Event Monitor table entry */
typedef struct eventMonitorEntry_tag
{
    uint32_t eventSet;          /* Event Set */
    instanceId_t instanceId;    /* Instance Id of the module which is registered to the event */
    pfFunction_t pfFunction;    /* Pointer to the callback function */
} eventMonitorEntry_t;

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static eventMonitorEntry_t maEventMonitorTable[gEvmMonitorTableSize_c];  /* maybe will be moved to NVNG */
/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private function prototypes
==================================================================================================*/
static void App_EventMonitorGetPayload(thrEvmParams_t *pEventParams, eventMonitorData_t *pEventMonitorData);
static void App_EventNwkScanGetPayload(thrEvmParams_t *pEventParams, eventMonitorData_t *pEventMonitorData);
/*==================================================================================================
Public functions
==================================================================================================*/

/*!*************************************************************************************************
\public
\fn     void APP_EventMonitor_Handler(uint8_t* pParam)
\brief  This function is used to monitor stack events.

\param  [in]    param   pointer to stack event

\return         void
***************************************************************************************************/
void APP_EventMonitor_Handler
(
    uint8_t* pParam
)
{

    thrEvmParams_t *pEvent = (thrEvmParams_t *)pParam;


    /* avoid end device events: allow/disallow to sleep */
    if((pEvent->code == gThrEv_GeneralInd_AllowDeviceToSleep_c) ||
       (pEvent->code == gThrEv_GeneralInd_DisallowDeviceToSleep_c))
    {
        return;
    }
   
    /* validate event monitor data */
    for(uint8_t i = 0; i< gEvmMonitorTableSize_c; i++)
    {
        if(((maEventMonitorTable[i].eventSet == (pEvent->code & gEvmMonitSetMask_c)) ||
            (maEventMonitorTable[i].eventSet == ((pEvent->code & gEvmMonitTypeMask_c)|gEvmMonitModuleMask_c)))&&
             ((maEventMonitorTable[i].instanceId == pEvent->thrInstId)||
                (maEventMonitorTable[i].instanceId == gEvmAnyInstanceId_c)||
                  (pEvent->thrInstId == (uint16_t)gEvmAnyInstanceId_c)) &&
                    (maEventMonitorTable[i].pfFunction))
        {
            uint16_t dataSize = pEvent->eventDataSize;

            if(pEvent->code == gThrEv_NwkScanCnf_Results_c)
            {
                thrNwkScanResults_t*  pScanResults = (thrNwkScanResults_t*)pEvent->pEventData;
                dataSize =  sizeof(thrNwkScanResults_t) + pScanResults->numOfEnergyDetectEntries +
                        (pScanResults->numOfNwkScanEntries * sizeof(thrNwkActiveScanEntry_t));
            }

            /* Copy the event monitor data to be sent to the monitoring handling
                    function - pParam will be freed by application callback */
            eventMonitorData_t *pEventData = NWKU_MEM_BufferAlloc(sizeof(eventMonitorData_t) + dataSize);

            if(pEventData)
            {
                pEventData->eventSet[0] = (uint8_t)(pEvent->code >> 16);
                pEventData->eventSet[1] = (uint8_t)(pEvent->code >> 24);
                pEventData->instanceId = (uint8_t)pEvent->thrInstId;
                pEventData->eventStatus[0] = (uint8_t)(pEvent->code);
                pEventData->eventStatus[1] = (uint8_t)(pEvent->code >> 8);
                pEventData->dataSize[0] = (uint8_t)(pEvent->eventDataSize);
                pEventData->dataSize[1] = (uint8_t)(pEvent->eventDataSize>>8);
                App_EventMonitorGetPayload(pEvent, pEventData);
#if gEvmAsyncronMonitoring_c
                if (FALSE == NWKU_SendMsg(maEventMonitorTable[i].pfFunction, (void *)pEventData, &appThreadMsgQueue))
                {
                    MEM_BufferFree((void *)pEventData);
                }
#else
                maEventMonitorTable[i].pfFunction(pEventData);
#endif /* gEvmAsyncronMonitoring_c */
            }
            i = gEvmMonitorTableSize_c;
        }
    }

}


/*!*************************************************************************************************
\fn    bool_t APP_EventMonitorRegister(uint32_t eventSet, pfFunction_t pfFunction, instanceId_t instanceId
                                  pfFunction_t pfFunction)
\brief Registers listening for an event dinamically(at run time).

\param [in]   eventSet      event set code (multiple events)
\param [in]   instanceId    instance Id of the module which is providing the event. Use gEvmAnyInstanceId_c
                            to be notified for all instance Ids
\param [in]   pfFunction    callback function for event
\param [out]  status        TRUE - Success, FALSE - failed
***************************************************************************************************/
bool_t APP_EventMonitorRegister
(
    uint32_t eventSet,
    instanceId_t instanceId,
    pfFunction_t pfFunction
)
{
   bool_t status = FALSE;
   uint8_t iEntry = THR_ALL_FFs8;

   /* Search for duplicate entry and free slots in the table */
   for(uint8_t i = 0; i<gEvmMonitorTableSize_c ; i++)
   {
        if(maEventMonitorTable[i].eventSet == 0 &&
           iEntry == THR_ALL_FFs8)
        {
           /* Mark free entry */
           iEntry = i;
        }
        if((eventSet == maEventMonitorTable[i].eventSet) &&
           ((instanceId == maEventMonitorTable[i].instanceId) ||
              (instanceId == gEvmAnyInstanceId_c)))
        {
            iEntry = i;
            i = gEvmMonitorTableSize_c;
        }

   }
   if(iEntry != THR_ALL_FFs8)
   {
        /* complete entry */
        maEventMonitorTable[iEntry].eventSet = eventSet;
        maEventMonitorTable[iEntry].instanceId = instanceId;
        maEventMonitorTable[iEntry].pfFunction = pfFunction;
        status = FALSE;
   }
   return status;
}

/*!*************************************************************************************************
\fn    bool_t APP_EventMonitorUnregister(uint32_t eventSet, instanceId_t instanceId)
\brief Unregisters listening for an event dinamically(at run time).

\param [in]   code          event code (single event) / event set code (multiple events)
\param [in]   instanceId    instance Id of the module which is providing the event. Use gEvmAnyInstanceId_c
                            to be notified for all instance Ids
\param [out]  status        TRUE - Success, FALSE - failed
***************************************************************************************************/
bool_t APP_EventMonitorUnregister
(
    uint32_t eventSet,
    instanceId_t instanceId
)
{
   bool_t status = TRUE;

   /* get event set entry */
   for(uint8_t i = 0; i<gEvmMonitorTableSize_c ; i++)
   {
        if((eventSet == maEventMonitorTable[i].eventSet) &&
           ((instanceId == maEventMonitorTable[i].instanceId) ||
              (instanceId == gEvmAnyInstanceId_c)))
        {
            /* clear entry */
            FLib_MemSet(&maEventMonitorTable[i], 0, sizeof(eventMonitorEntry_t));
        }
   }
   return status;
}

/*!*************************************************************************************************
\fn    uint16_t APP_InitEventMonitor(instanceId_t instanceId)
\brief  Init event monitoring

\param [in]   instanceId    thread instance Id
***************************************************************************************************/
void APP_InitEventMonitor(instanceId_t instanceId)
{

#if (THREAD_USE_THCI || THREAD_USE_SHELL)    
    pfFunction_t pfAppEvMonitor;
#endif    

    EVM_SetEventMonitorCb(APP_EventMonitor_Handler);
    FLib_MemSet(maEventMonitorTable, 0x00, sizeof(eventMonitorEntry_t)*gEvmMonitorTableSize_c);

    /* register events: */
#if THREAD_USE_THCI
    pfAppEvMonitor = (pfFunction_t)THCI_EvMonitor;
#elif THREAD_USE_SHELL
    SHELL_EvMonitorInit(instanceId);
    pfAppEvMonitor = (pfFunction_t)SHELL_EvMonitor;
#endif

#if (THREAD_USE_THCI || THREAD_USE_SHELL)
    APP_EventMonitorRegister(gEvmModuleThreadAnyEvent_c, instanceId, pfAppEvMonitor);
    APP_EventMonitorRegister(gEvmModule6LoWPANAnyEvent_c, instanceId, pfAppEvMonitor);
#if ND_ENABLED
    APP_EventMonitorRegister(gEvmModuleNdAnyEvent_c, instanceId, pfAppEvMonitor);
#endif /* ND_ENABLED */
#endif
}
/*!*************************************************************************************************
\fn    static void App_EventMonitorGetPayload(thrEvmParams_t *pEventParams,
                                                eventMonitorData_t *pEventMonitorData)
\brief

\param [in]   param    pointer to monitor event
***************************************************************************************************/
static void App_EventMonitorGetPayload(thrEvmParams_t *pEventParams, eventMonitorData_t *pEventMonitorData)
{
    if(pEventParams->code == gThrEv_NwkScanCnf_Results_c)
    {
        App_EventNwkScanGetPayload(pEventParams, pEventMonitorData);
    }
    else
    {
        FLib_MemCpy(pEventMonitorData->aData, pEventParams->pEventData, pEventParams->eventDataSize);
    }
}

/*!*************************************************************************************************
\fn    void App_EventNwkScanGetPayload(thrEvmParams_t *pEventParams, eventMonitorData_t *pEventMonitorData)
\brief

\param [in]   param    pointer to monitor event
***************************************************************************************************/
static void App_EventNwkScanGetPayload(thrEvmParams_t *pEventParams, eventMonitorData_t *pEventMonitorData)
{
    uint16_t dataSize = 0;
    if(pEventParams->eventDataSize)
    {
        thrNwkScanResults_t *pScanCnf = &pEventParams->pEventData->nwkScanCnf;
        if(pScanCnf)
        {
            FLib_MemCpy(&pEventMonitorData->aData[dataSize], &pScanCnf->scanInfo, sizeof(thrNwkScan_t));
            dataSize += sizeof(thrNwkScan_t);
            if(pScanCnf->numOfEnergyDetectEntries)
            {
                pEventMonitorData->aData[dataSize] = pScanCnf->numOfEnergyDetectEntries;
                dataSize++;
                /* copy energy detect list */
                FLib_MemCpy(&pEventMonitorData->aData[dataSize], pScanCnf->pEnergyDetectList, pScanCnf->numOfEnergyDetectEntries);
                dataSize += pScanCnf->numOfEnergyDetectEntries;

            }
            if(pScanCnf->numOfNwkScanEntries)
            {
                uint8_t i;
                pEventMonitorData->aData[dataSize] = pScanCnf->numOfNwkScanEntries;
                dataSize++;

                for(i = 0; i < pScanCnf->numOfNwkScanEntries; i++)
                {

                   FLib_MemCpy(&pEventMonitorData->aData[dataSize], &pScanCnf->nwkScanList[i],
                                   sizeof(thrNwkActiveScanEntry_t));
                   dataSize += sizeof(thrNwkActiveScanEntry_t);

                }
            }

            /* update payload size */
            pEventMonitorData->dataSize[0] = (uint8_t)(dataSize);
            pEventMonitorData->dataSize[1] = (uint8_t)(dataSize>>8);
        }
    }
}

#endif  /* THR_ENABLE_EVENT_MONITORING */

/*==================================================================================================
Private functions
==================================================================================================*/

/*==================================================================================================
Private debug functions
==================================================================================================*/
