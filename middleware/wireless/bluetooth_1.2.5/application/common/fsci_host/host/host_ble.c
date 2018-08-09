/*! *********************************************************************************
* \addtogroup HOST_BBOX_UTILITY
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* This file is the source file for the Host - Blackbox Utility management
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
