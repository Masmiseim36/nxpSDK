/*! *********************************************************************************
* \addtogroup FSCI BLE application
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
/* Framework / Drivers */
#include "LED.h"
#include "Keyboard.h"
#include "SerialManager.h"
#include "FsciInterface.h"
#include "FsciMacCommands.h"
#include "FsciAspCommands.h"

/* BLE Host Stack */
#include "fsci_ble_interface.h"

/*  Application */
#include "ble_802_15_4_fsci_black_box.h"

/* 802.15.4 */
#include "PhyInterface.h"
#include "MacInterface.h"

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

/* FSCI serial configuration structure */
static gFsciSerialConfig_t fsciConfigStruct = 
{
    .interfaceType      = APP_SERIAL_INTERFACE_TYPE,
    .interfaceChannel   = APP_SERIAL_INTERFACE_INSTANCE,
    .baudrate           = APP_SERIAL_INTERFACE_SPEED
};

/* This table contains indexes into the mFsciSerials[] table */
static uint8_t mFsciInterface[gMacInstancesCnt_c];
static instanceId_t mMacInstance[gMacInstancesCnt_c];


/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
resultType_t MCPS_NWK_SapHandler( mcpsToNwkMessage_t* pMsg, instanceId_t instanceId );
resultType_t MLME_NWK_SapHandler( nwkMessage_t* pMsg, instanceId_t instanceId );

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void App_Init(void)
{
    /* Init serial manager */
    SerialManager_Init();  
    
    /* Init FSCI */
    FSCI_Init(&fsciConfigStruct);
    
    /* Register BLE handlers in FSCI */
    fsciBleRegister(0);

    /* Bind to MAC layer */
    for(uint8_t i=0; i<gMacInstancesCnt_c; i++)
    {
        mFsciInterface[i] = i;
        mMacInstance[i] = BindToMAC((instanceId_t)i );
        Mac_RegisterSapHandlers(MCPS_NWK_SapHandler, MLME_NWK_SapHandler, mMacInstance[i]);
        fsciRegisterMac( mMacInstance[i], mFsciInterface[i] );      
    }

    //fsciRegisterAsp(0, 0);
}    


void BleApp_HandleKeys(key_event_t events)
{
    (void)events;
}


void BleApp_GenericCallback(gapGenericEvent_t* pGenericEvent)
{
}

/*! *********************************************************************************
* \brief  This is the SAP used by the MCPS to send messages to the upper layer 
*
* \param[in]  pMsg pointer to the MCPS message received from the MAC layer
* \param[in]  instanceId of the MAC
*
* \return  resultType_t.
*
********************************************************************************** */
resultType_t MCPS_NWK_SapHandler (mcpsToNwkMessage_t* pMsg, instanceId_t instanceId)
{
#if !gFsciHostMacSupport_c
    /* Monitor is performed in the Host MAC SAPs */  
    (void)FSCI_Monitor(gFSCI_McpsSapId_c, pMsg, gSuccess_c, mFsciInterface[instanceId] );
#endif

    MEM_BufferFree( pMsg );
    return gSuccess_c;
}

/*! *********************************************************************************
* \brief  This is the SAP used by the MLME to send messages to the upper layer 
*
* \param[in]  pMsg pointer to the MLME message received from the MAC layer
* \param[in]  instanceId of the MAC
*
* \return  resultType_t.
*
********************************************************************************** */
resultType_t MLME_NWK_SapHandler (nwkMessage_t* pMsg, instanceId_t instanceId)
{
    panDescriptorBlock_t* pTemp;
    panDescriptorBlock_t* pCrtPDBlock;

#if !gFsciHostMacSupport_c
    /* Monitor is performed in the Host MAC SAPs */
    (void)FSCI_Monitor(gFSCI_MlmeSapId_c, pMsg, gSuccess_c, mFsciInterface[instanceId] );
#endif

    if (gMlmeBeaconNotifyInd_c == pMsg->msgType)
    {
        MEM_BufferFree( pMsg->msgData.beaconNotifyInd.pBufferRoot );
    }

    if (gMlmeScanCnf_c == pMsg->msgType)
    {
        switch (pMsg->msgData.scanCnf.scanType)
        {
        case gScanModeED_c:
            MEM_BufferFree( pMsg->msgData.scanCnf.resList.pEnergyDetectList );
            break;
        case gScanModeActive_c:
        case gScanModePassive_c:
            pCrtPDBlock = pMsg->msgData.scanCnf.resList.pPanDescriptorBlockList;
            while (pCrtPDBlock)
            {
                pTemp = pCrtPDBlock;
                pCrtPDBlock = pCrtPDBlock->pNext;
                MEM_BufferFree( pTemp );
            }
            break;
        default:
            break;
        }
    }

    MEM_BufferFree( pMsg );
    return gSuccess_c;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
