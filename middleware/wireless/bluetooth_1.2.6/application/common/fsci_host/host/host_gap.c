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
* This file is the source file for the Host - Blackbox GAP management
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

#include "fsci_ble_gap.h"
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

bleResult_t Ble_HostInitialize
(
    gapGenericCallback_t            genericCallback,
    hciHostToControllerInterface_t  hostToControllerInterface
)
{
    FsciGapCmdMonitor(HostInitialize, genericCallback, hostToControllerInterface);
    
    return gBleSuccess_c;
}


bleResult_t Gap_RegisterDeviceSecurityRequirements
(
    gapDeviceSecurityRequirements_t* pSecurity
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(RegisterDeviceSecurityRequirements, pSecurity);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SetAdvertisingParameters
(
    gapAdvertisingParameters_t* pAdvertisingParameters
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(SetAdvertisingParameters, pAdvertisingParameters);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SetAdvertisingData
(
    gapAdvertisingData_t*         pAdvertisingData,
    gapScanResponseData_t*        pScanResponseData
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SetAdvertisingData, pAdvertisingData, pScanResponseData);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_StartAdvertising
(
    gapAdvertisingCallback_t    advertisingCallback,
    gapConnectionCallback_t     connectionCallback
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(StartAdvertising, advertisingCallback, connectionCallback);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_StopAdvertising(void)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);   
    FsciGapCmdMonitor(StopAdvertising);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_Authorize
(
    deviceId_t          deviceId,
    uint16_t            handle,
    gattDbAccessType_t  access
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(Authorize, deviceId, handle, access);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SaveCccd
(
    deviceId_t      deviceId,
    uint16_t        handle,
    gattCccdFlags_t cccd
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);   
    FsciGapCmdMonitor(SaveCccd, deviceId, handle, cccd);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_CheckNotificationStatus
(
    deviceId_t  deviceId,
    uint16_t    handle,
    bool_t*     pOutIsActive
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(CheckNotificationStatus, deviceId, handle, pOutIsActive);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_CheckIndicationStatus
(
    deviceId_t  deviceId,
    uint16_t    handle,
    bool_t*     pOutIsActive
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(CheckIndicationStatus, deviceId, handle, pOutIsActive);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_GetBondedStaticAddresses
(
    bleDeviceAddress_t* aOutDeviceAddresses,
    uint8_t             maxDevices,
    uint8_t*            pOutActualCount
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(GetBondedStaticAddresses, aOutDeviceAddresses, maxDevices, pOutActualCount);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_GetBondedDevicesIdentityInformation
(
    gapIdentityInformation_t* aOutIdentityAddresses, 
    uint8_t                   maxDevices,
    uint8_t*                  pOutActualCount
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(GetBondedDevicesIdentityInformation, aOutIdentityAddresses, maxDevices, pOutActualCount);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_Pair
(
    deviceId_t                  deviceId,
    gapPairingParameters_t*     pPairingParameters
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(Pair, deviceId, pPairingParameters);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SendSlaveSecurityRequest
(
    deviceId_t                  deviceId,
    bool_t                      bondAfterPairing,
    gapSecurityModeAndLevel_t   securityModeLevel
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SendSlaveSecurityRequest, deviceId, bondAfterPairing, securityModeLevel);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_EncryptLink
(
        deviceId_t              deviceId
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(EncryptLink, deviceId);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_AcceptPairingRequest
(
    deviceId_t                  deviceId,
    gapPairingParameters_t*     pPairingParameters
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(AcceptPairingRequest, deviceId, pPairingParameters);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_RejectPairing
(
    deviceId_t                          deviceId,
    gapAuthenticationRejectReason_t     reason
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);   
    FsciGapCmdMonitor(RejectPairing, deviceId, reason);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_EnterPasskey
(
    deviceId_t  deviceId,
    uint32_t    passkey
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);   
    FsciGapCmdMonitor(EnterPasskey, deviceId, passkey);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_ProvideOob
(
    deviceId_t      deviceId,
    uint8_t*        aOob
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(ProvideOob, deviceId, aOob);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_RejectPasskeyRequest
(
    deviceId_t deviceId
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(RejectPasskeyRequest, deviceId);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SendSmpKeys
(
    deviceId_t      deviceId,
    gapSmpKeys_t*   pKeys
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SendSmpKeys, deviceId, pKeys);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_RejectKeyExchangeRequest
(
    deviceId_t deviceId
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(RejectKeyExchangeRequest, deviceId);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_ProvideLongTermKey
(
    deviceId_t  deviceId,
    uint8_t*    aLtk,
    uint8_t     ltkSize
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(ProvideLongTermKey, deviceId, aLtk, ltkSize);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_DenyLongTermKey
(
    deviceId_t deviceId
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(DenyLongTermKey, deviceId);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_LoadEncryptionInformation
(
    deviceId_t                  deviceId,
    uint8_t*                    aOutLtk,
    uint8_t*                    pOutLtkSize
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(LoadEncryptionInformation, deviceId, aOutLtk, pOutLtkSize);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SetLocalPasskey
(
    uint32_t passkey
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SetLocalPasskey, passkey);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_StartScanning
(
    gapScanningParameters_t*    pScanningParameters,
    gapScanningCallback_t       scanningCallback,
    bool_t                      enableFilterDuplicates
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);   
    FsciGapCmdMonitor(StartScanning, pScanningParameters, scanningCallback, enableFilterDuplicates);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_StopScanning(void)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(StopScanning);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_Connect
(
    gapConnectionRequestParameters_t*   pParameters,
    gapConnectionCallback_t             connCallback
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(Connect, pParameters, connCallback);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_Disconnect
(
    deviceId_t deviceId
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(Disconnect, deviceId);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SaveCustomPeerInformation
(
    uint8_t  deviceId,
    void*    aInfo,
    uint16_t offset,
    uint16_t infoSize
)
{
    bleResult_t result = gBleSuccess_c;

    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SaveCustomPeerInformation, deviceId, aInfo, offset, infoSize);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_LoadCustomPeerInformation
(
    uint8_t  deviceId,
    void*    aOutInfo,
    uint16_t offset,
    uint16_t infoSize
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);    
    FsciGapCmdMonitor(LoadCustomPeerInformation, deviceId, aOutInfo, offset, infoSize);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_CheckIfBonded
(
    deviceId_t deviceId,
    bool_t* pOutIsBonded
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(CheckIfBonded, deviceId, pOutIsBonded);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_ReadWhiteListSize(void)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(ReadWhiteListSize);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_ClearWhiteList(void)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(ClearWhiteList);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_AddDeviceToWhiteList
(
    bleAddressType_t    addressType,
    bleDeviceAddress_t  address
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(AddDeviceToWhiteList, addressType, address);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_RemoveDeviceFromWhiteList
(
    bleAddressType_t    addressType,
    bleDeviceAddress_t  address
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(RemoveDeviceFromWhiteList, addressType, address);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_ReadPublicDeviceAddress(void)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(ReadPublicDeviceAddress);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_CreateRandomDeviceAddress
(
    uint8_t* aIrk,
    uint8_t* aRandomPart
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(CreateRandomDeviceAddress, aIrk, aRandomPart);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SaveDeviceName
(
    deviceId_t  deviceId,
    uchar_t*    aName,
    uint8_t     cNameSize
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SaveDeviceName, deviceId, aName, cNameSize);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_GetBondedDevicesCount
(
    uint8_t* pOutBondedDevicesCount
)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(GetBondedDevicesCount, pOutBondedDevicesCount);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_GetBondedDeviceName
(
    uint8_t     nvmIndex,
    uchar_t*    aOutName,
    uint8_t     maxNameSize
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(GetBondedDeviceName, nvmIndex, aOutName, maxNameSize);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_VerifyPrivateResolvableAddress
(
    uint8_t             nvmIndex,
    bleDeviceAddress_t  aAddress
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(VerifyPrivateResolvableAddress, nvmIndex, aAddress);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SetRandomAddress
(
    bleDeviceAddress_t aAddress
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SetRandomAddress, aAddress);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_RemoveBond
(
    uint8_t nvmIndex
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);   
    FsciGapCmdMonitor(RemoveBond, nvmIndex);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_RemoveAllBonds(void)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(RemoveAllBonds);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_ReadRadioPowerLevel
(
    gapRadioPowerLevelReadType_t    txPowerReadType,
    deviceId_t                      deviceId
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(ReadRadioPowerLevel, txPowerReadType, deviceId);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SetDefaultPairingParameters
(
    gapPairingParameters_t* pPairingParameters
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(SetDefaultPairingParameters, pPairingParameters);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_UpdateConnectionParameters
(
    deviceId_t  deviceId,
    uint16_t    intervalMin,
    uint16_t    intervalMax,
    uint16_t    slaveLatency,
    uint16_t    timeoutMultiplier,
    uint16_t    minCeLength,
    uint16_t    maxCeLength
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(UpdateConnectionParameters, deviceId, intervalMin, intervalMax, slaveLatency, timeoutMultiplier, minCeLength, maxCeLength);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_EnableUpdateConnectionParameters
(
    deviceId_t  deviceId,
    bool_t      enable
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(EnableUpdateConnectionParameters, deviceId, enable);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;   
}


bleResult_t Gap_UpdateLeDataLength
(
    deviceId_t  deviceId,
    uint16_t    txOctets,
    uint16_t    txTime
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(UpdateLeDataLength, deviceId, txOctets, txTime);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_LeScRegeneratePublicKey
(
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(LeScRegeneratePublicKey);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_LeScValidateNumericValue
(
    deviceId_t  deviceId,
    bool_t      valid
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(LeScValidateNumericValue, deviceId, valid);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;    
}


bleResult_t Gap_LeScGetLocalOobData
(
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);  
    FsciGapCmdMonitor(LeScGetLocalOobData);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_LeScSetPeerOobData
(
    deviceId_t              deviceId,
    gapLeScOobData_t*       pPeerOobData
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(LeScSetPeerOobData, deviceId, pPeerOobData);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;    
}


bleResult_t Gap_LeScSendKeypressNotification
(
    deviceId_t                  deviceId,
    gapKeypressNotification_t   keypressNotification
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(LeScSendKeypressNotification, deviceId, keypressNotification);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t Gap_SetScanMode
(
    gapScanMode_t           scanMode,
    gapAutoConnectParams_t* pAutoConnectParams
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(SetScanMode, scanMode, pAutoConnectParams);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;    
}


bleResult_t Gap_ControllerReset
(
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(ControllerReset);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;    
}


bleResult_t Gap_EnableHostPrivacy
(
    bool_t      enable,
    uint8_t*    aIrk
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(EnableHostPrivacy, enable, aIrk);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;    
}


bleResult_t Gap_EnableControllerPrivacy
(
    bool_t                      enable,
    uint8_t*                    aOwnIrk,
    uint8_t                     peerIdCount,
    gapIdentityInformation_t*   aPeerIdentities
)
{
    bleResult_t result = gBleSuccess_c;
        
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGapOpcodeGroup_c, gBleGapStatusOpCode_c);
    FsciGapCmdMonitor(EnableControllerPrivacy, enable, aOwnIrk, peerIdCount, aPeerIdentities);
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
