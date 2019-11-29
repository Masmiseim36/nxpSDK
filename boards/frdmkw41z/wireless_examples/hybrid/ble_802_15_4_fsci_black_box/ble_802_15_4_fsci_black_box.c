/*! *********************************************************************************
* \addtogroup FSCI BLE application
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* SPDX-License-Identifier: BSD-3-Clause
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
