/*! *********************************************************************************
* \addtogroup HOST_BBOX_UTILITY
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the source file for the Host - Blackbox Utility management
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble.h"
#include "ble_general.h"

#if gFsciIncluded_c
#include "FsciInterface.h"
#include "FsciCommunication.h"
#endif

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#define gFsciHost_WaitForStatusTimeout_c     500 /* milliseconds */
#define mFsciHost_WaitForStatusTimeout_us_c  (gFsciHost_WaitForStatusTimeout_c * 1000)

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/* Flag which indicates if function has out parameters */
static volatile bool_t bFunctionHasOutParams = FALSE;

#if gFsciIncluded_c && gFsciHostSupport_c
extern clientPacket_t *pFsciHostSyncRsp;

#if gFsciHostSyncUseEvent_c
extern osaEventId_t    gFsciHostSyncRspEventId;
#endif
#endif

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

bleResult_t Ble_Initialize(gapGenericCallback_t gapGenericCallback)
{
    /* Init timestamp for out paramters polling */
    TMR_TimeStampInit();
    
    return Ble_HostInitialize(gapGenericCallback, NULL);;
}

bleResult_t Ble_GetCmdStatus(bool_t bHasOutParams)
{
    bleResult_t result = gBleSuccess_c;
#if gFsciHostSyncUseEvent_c    
    osaEventFlags_t fsciHostSyncRspEventFlags; 
#endif
    uint64_t tStamp = 0;
    
    /* Set out parameter flag if any */
    bFunctionHasOutParams = bHasOutParams;

    /* Wait for result from the serial interface */
    while( !pFsciHostSyncRsp )
    {
#if gFsciHostSyncUseEvent_c
        OSA_EventWait(gFsciHostSyncRspEventId, 0x00FFFFFF, FALSE, gFsciHost_WaitForStatusTimeout_c, &fsciHostSyncRspEventFlags);
#else
        tStamp = TMR_GetTimestamp();
        while( !pFsciHostSyncRsp )
        {
            FSCI_receivePacket((void*)fsciBleInterfaceId);
            if( TMR_GetTimestamp() - tStamp > mFsciHost_WaitForStatusTimeout_us_c )
            {
                break;
            }    
        }
#endif
    }
    
    if( pFsciHostSyncRsp )
    {
        result = (bleResult_t)pFsciHostSyncRsp->structured.payload[0];
      
        /* Check status and wait for outParameters */ 
        if( gBleSuccess_c == result )
        {
            tStamp = TMR_GetTimestamp();
            while( bFunctionHasOutParams )
            {
                FSCI_receivePacket((void*)fsciBleInterfaceId);
                if( TMR_GetTimestamp() - tStamp > mFsciHost_WaitForStatusTimeout_us_c )
                {
                    /* Timeout on the receiving the response */
                    result = gBleUnexpectedError_c;
                    break;
                }
            }  
        }
        
        /* Free FSCI packet */
        MEM_BufferFree(pFsciHostSyncRsp);
    }
    else
    {
        /* Timeout on the receiving the response */
        result = gBleUnexpectedError_c;
    }
    
    return result;
}


void Ble_OutParamsReady(void)
{
    /* Reset flag which indicates if function has out parameters */
    bFunctionHasOutParams = FALSE;
}

/*************************************************************************************
 *************************************************************************************
 * Private functions
 *************************************************************************************
 ************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
