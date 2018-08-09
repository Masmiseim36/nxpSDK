/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a source file for the FSCI Application.
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

#include "mac_fsci_black_box.h"

/* Drv */
#include "LED.h"
#include "Keyboard.h"

/* Fwk */
#include "SecLib.h"
#include "SerialManager.h"
#include "RNG_Interface.h"
#include "MemManager.h"
#include "TimersManager.h"
#include "FsciInterface.h"
#include "FsciCommands.h"
#include "FsciMacCommands.h"
#include "FsciAspCommands.h"
#if gUpgradeImageOnCurrentDevice_d
#include "OtaSupport.h"
#endif
#if gFSCI_IncludeLpmCommands_c
#include "PWR_Interface.h"
#endif

/* 802.15.4 */
#if !gFsciHostSupport_c
#include "PhyInterface.h"
#endif
#include "MacInterface.h"

/* KSDK */
#include "board.h"
#include "fsl_os_abstraction.h"


/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
resultType_t MCPS_NWK_SapHandler( mcpsToNwkMessage_t* pMsg, instanceId_t instanceId );
resultType_t MLME_NWK_SapHandler( nwkMessage_t* pMsg, instanceId_t instanceId );
static void App_HandleKeys(key_event_t events);
void main_task(uint32_t param);
void AppThread(uint32_t param);

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
#if gFsciIncluded_c
/* FSCI Interface Configuration structure */
static const gFsciSerialConfig_t mFsciSerials[] = {
    /* Baudrate,                        interface type,                  channel No,                virtual interface */
    {APP_SERIAL_INTERFACE_SPEED, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE,           0}  
};
#endif
/* This table contains indexes into the mFsciSerials[] table */
static uint8_t mFsciInterface[gMacInstancesCnt_c];
#if gFsciHostSupport_c
/* This table contains indexes into the mFsciSerials[] table for the MAC Black Box */
static uint8_t mFsciHostInterface[gMacInstancesCnt_c];
#endif
static instanceId_t mMacInstance[gMacInstancesCnt_c];


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

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
#if !gFsciHostSupport_c
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

#if !gFsciHostSupport_c
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

/*! *********************************************************************************
* \brief  This is the first task created by the OS. This task will initialize 
*         the system
*
* \param[in]  param
*
********************************************************************************** */
void main_task(uint32_t param)
{
    static uint8_t initialized = FALSE;
    uint32_t i;

    if( !initialized )
    {
        initialized = TRUE;
        hardware_init();
        
        /* Framework init */
        MEM_Init();
#if !gFsciHostSupport_c
        Phy_Init();
#endif
        TMR_Init();
        LED_Init();
        SecLib_Init();
        SerialManager_Init();
#if gFSCI_IncludeLpmCommands_c
        PWR_Init();
        PWR_DisallowDeviceToSleep();
#endif
#if gFsciIncluded_c
        FSCI_Init( (void*)mFsciSerials );
#if gUpgradeImageOnCurrentDevice_d
        OTA_RegisterToFsci( 0, NULL );
#endif
#endif
        RNG_Init(); /* RNG must be initialized after the PHY is Initialized */
        KBD_Init(App_HandleKeys);
        
        /* 802.15.4 MAC init */
        MAC_Init();
        
        /* Bind to MAC layer */
        for( i=0; i<gMacInstancesCnt_c; i++)
        {
            mFsciInterface[i] = i;
            mMacInstance[i] = BindToMAC((instanceId_t)i );
            Mac_RegisterSapHandlers(MCPS_NWK_SapHandler, MLME_NWK_SapHandler, mMacInstance[i]);
            fsciRegisterMac( mMacInstance[i], mFsciInterface[i] );
            
#if gFsciHostSupport_c
            /* Select serial interface for MAC black box 
               after gMacInstancesCnt_c positions in the FSCI interface array */
            mFsciHostInterface[i] = i + gMacInstancesCnt_c;
            fsciRegisterMacToHost( mMacInstance[i], mFsciHostInterface[i] );
#endif
        }
        
        fsciRegisterAsp(0, 0);
#if gFsciHostSupport_c
        fsciRegisterAspToHost(0, 0);
#endif
        
#if !gFSCI_IncludeLpmCommands_c
        LED_StartSerialFlash(LED1);
#endif
    }
    /* Call application task */
    AppThread( param );
}

/*! *********************************************************************************
* \brief  This function represents the Application task. 
*         This task reuses the stack alocated for the MainThread.
*
* \param[in]  argument
*
********************************************************************************** */
void AppThread (uint32_t param)
{
    while(1)
    {
#if gFSCI_IncludeLpmCommands_c
        if( PWR_CheckIfDeviceCanGoToSleep() )
        {
            PWR_EnterLowPower();
            FSCI_SendWakeUpIndication();
        }
        else
        {
            PWR_EnterSleep();
        }
#endif
        if (gUseRtos_c == 0)
        {
            break;
        } 
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static void App_HandleKeys(key_event_t events)
{
    (void)events;
}
