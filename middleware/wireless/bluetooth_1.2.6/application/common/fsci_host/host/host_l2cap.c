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
* This file is the source file for the Host - Blackbox L2CAP Credit Based management
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

#include "fsci_ble_l2cap.h"
#include "host_ble.h"

#if gFsciIncluded_c
#include "FsciCommunication.h"
#endif

/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
            
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

bleResult_t L2ca_RegisterLeCbCallbacks(l2caLeCbDataCallback_t pCallback, l2caLeCbControlCallback_t pCtrlCallback)
{
    fsciBleSetL2capLeCbDataCallback(pCallback);
    fsciBleSetL2capLeCbControlCallback(pCtrlCallback);

    return gBleSuccess_c;
}


bleResult_t L2ca_RegisterLePsm(uint16_t lePsm, uint16_t lePsmMtu)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleL2capOpcodeGroup_c, gBleL2capStatusOpCode_c);    
    FsciL2capCmdMonitor(RegisterLePsm, lePsm, lePsmMtu);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;    
}


bleResult_t L2ca_DeregisterLePsm(uint16_t lePsm)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleL2capOpcodeGroup_c, gBleL2capStatusOpCode_c);  
    FsciL2capCmdMonitor(DeregisterLePsm, lePsm);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;    
}


bleResult_t L2ca_ConnectLePsm(uint16_t lePsm, deviceId_t deviceId, uint16_t initialCredits)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleL2capOpcodeGroup_c, gBleL2capStatusOpCode_c);  
    FsciL2capCmdMonitor(ConnectLePsm, lePsm, deviceId, initialCredits);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;    
}


bleResult_t L2ca_DisconnectLeCbChannel(deviceId_t deviceId, uint16_t channelId)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleL2capOpcodeGroup_c, gBleL2capStatusOpCode_c);  
    FsciL2capCmdMonitor(DisconnectLeCbChannel, deviceId, channelId);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;   
}


bleResult_t L2ca_CancelConnection(uint16_t lePsm, deviceId_t deviceId, l2caLeCbConnectionRequestResult_t refuseReason)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleL2capOpcodeGroup_c, gBleL2capStatusOpCode_c);  
    FsciL2capCmdMonitor(CancelConnection, lePsm, deviceId, refuseReason);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;    
}

 
bleResult_t L2ca_SendLeCbData(deviceId_t deviceId, uint16_t channelId, uint8_t*pPacket, uint16_t packetLength)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleL2capOpcodeGroup_c, gBleL2capStatusOpCode_c);  
    FsciL2capCmdMonitor(SendLeCbData, deviceId, channelId, pPacket, packetLength);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;    
}


bleResult_t L2ca_SendLeCredit(deviceId_t deviceId, uint16_t channelId, uint16_t credits)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleL2capOpcodeGroup_c, gBleL2capStatusOpCode_c);  
    FsciL2capCmdMonitor(SendLeCredit, deviceId, channelId, credits);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;    
}

/*************************************************************************************
 *************************************************************************************
 * Private functions
 *************************************************************************************
 ************************************************************************************/

 
/*! *********************************************************************************
* @}
********************************************************************************** */
