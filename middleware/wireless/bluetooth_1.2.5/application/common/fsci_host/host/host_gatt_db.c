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
* This file is the source file for the Host - Blackbox GATT Database management
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

#include "fsci_ble_gatt_db_app.h"
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

bleResult_t GattDb_WriteAttribute
(
    uint16_t    handle,
    uint16_t    valueLength,
    uint8_t*    aValue
)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);  
    FsciGattDbCmdMonitor(WriteAttribute, handle, valueLength, aValue);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;
}


bleResult_t GattDb_ReadAttribute
(
    uint16_t    handle,
    uint16_t    maxBytes,
    uint8_t*    aOutValue,
    uint16_t*   pOutValueLength
)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(ReadAttribute, handle, maxBytes, aOutValue, pOutValueLength);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;
}


bleResult_t GattDb_FindServiceHandle
(
    uint16_t        startHandle,
    bleUuidType_t   uuidType,
    bleUuid_t*      pUuid,
    uint16_t*       pOutServiceHandle
)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(FindServiceHandle, startHandle, uuidType, pUuid, pOutServiceHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;
}


bleResult_t GattDb_FindCharValueHandleInService
(
    uint16_t        serviceHandle,
    bleUuidType_t   uuidType,
    bleUuid_t*      pUuid,
    uint16_t*       pOutCharValueHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(FindCharValueHandleInService, serviceHandle, uuidType, pUuid, pOutCharValueHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId); 
    
    return result;
}


bleResult_t GattDb_FindCccdHandleForCharValueHandle
(
    uint16_t        charValueHandle,
    uint16_t*       pOutCccdHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);    
    FsciGattDbCmdMonitor(FindCccdHandleForCharValueHandle, charValueHandle, pOutCccdHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId); 
    
    return result;
}


bleResult_t GattDb_FindDescriptorHandleForCharValueHandle
(
    uint16_t        charValueHandle,
    bleUuidType_t   uuidType,
    bleUuid_t*      pUuid,
    uint16_t*       pOutDescriptorHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(FindDescriptorHandleForCharValueHandle, charValueHandle, uuidType, pUuid, pOutDescriptorHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId); 
    
    return result;
}


bleResult_t GattDbDynamic_Init(void)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);  
    FsciGattDbCmdMonitor(InitDatabase);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;
}


bleResult_t GattDbDynamic_ReleaseDatabase(void)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);  
    FsciGattDbCmdMonitor(ReleaseDatabase);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;
}


bleResult_t GattDbDynamic_AddPrimaryServiceDeclaration
(
    bleUuidType_t   serviceUuidType,
    bleUuid_t*      pServiceUuid,
    uint16_t*       pOutHandle
)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(AddPrimaryServiceDeclaration, serviceUuidType, pServiceUuid, pOutHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);
    
    return result;
}


bleResult_t GattDbDynamic_AddSecondaryServiceDeclaration
(
    bleUuidType_t   serviceUuidType,
    bleUuid_t*      pServiceUuid,
    uint16_t*       pOutHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(AddSecondaryServiceDeclaration, serviceUuidType, pServiceUuid, pOutHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);
    
    return result;
}


bleResult_t GattDbDynamic_AddIncludeDeclaration
(
    uint16_t        includedServiceHandle,
    uint16_t        endGroupHandle,
    bleUuidType_t   serviceUuidType,
    bleUuid_t*      pServiceUuid,
    uint16_t*       pOutHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(AddIncludeDeclaration, includedServiceHandle, endGroupHandle, serviceUuidType, pServiceUuid, pOutHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);
    
    return result;
}


bleResult_t GattDbDynamic_AddCharacteristicDeclarationAndValue
(
    bleUuidType_t                               characteristicUuidType,
    bleUuid_t*                                  pCharacteristicUuid,
    gattCharacteristicPropertiesBitFields_t     characteristicProperties,
    uint16_t                                    maxValueLength,
    uint16_t                                    initialValueLength,
    uint8_t*                                    aInitialValue,
    gattAttributePermissionsBitFields_t         valueAccessPermissions,
    uint16_t*                                   pOutHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(AddCharacteristicDeclarationAndValue, characteristicUuidType, pCharacteristicUuid, 
                        characteristicProperties, maxValueLength, initialValueLength, 
                        aInitialValue, valueAccessPermissions, pOutHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);

    return result;
}


bleResult_t GattDbDynamic_AddCharacteristicDescriptor
(
    bleUuidType_t                               descriptorUuidType,
    bleUuid_t*                                  pDescriptorUuid,
    uint16_t                                    descriptorValueLength,
    uint8_t*                                    aInitialValue,
    gattAttributePermissionsBitFields_t         descriptorAccessPermissions,
    uint16_t*                                   pOutHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(AddCharacteristicDescriptor, descriptorUuidType, pDescriptorUuid, descriptorValueLength, 
                         aInitialValue, descriptorAccessPermissions, pOutHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;
}
  
  
bleResult_t GattDbDynamic_AddCccd(uint16_t* pOutHandle)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(AddCccd, pOutHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);
    
    return result;
}


bleResult_t GattDbDynamic_AddCharacteristicDeclarationWithUniqueValue
(
    bleUuidType_t                               characteristicUuidType,
    bleUuid_t*                                  pCharacteristicUuid,
    gattCharacteristicPropertiesBitFields_t     characteristicProperties,
    gattAttributePermissionsBitFields_t         valueAccessPermissions,
    uint16_t*                                   pOutHandle
)
{
    bleResult_t result = gBleSuccess_c;
    
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);
    FsciGattDbCmdMonitor(AddCharacteristicDeclarationWithUniqueValue, characteristicUuidType, pCharacteristicUuid, 
                        characteristicProperties, valueAccessPermissions, pOutHandle);
    result = Ble_GetCmdStatus(TRUE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    

    return result;
}


bleResult_t GattDbDynamic_RemoveService
(
    uint16_t serviceHandle
)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);  
    FsciGattDbCmdMonitor(RemoveService, serviceHandle);
    result = Ble_GetCmdStatus(FALSE);
    FSCI_HostSyncUnlock(fsciBleInterfaceId);    
    
    return result;        
}


bleResult_t GattDbDynamic_RemoveCharacteristic
(
    uint16_t characteristicHandle
)
{
    bleResult_t result = gBleSuccess_c;
  
    FSCI_HostSyncLock(fsciBleInterfaceId, gFsciBleGattDbAppOpcodeGroup_c, gBleGattDbAppStatusOpCode_c);  
    FsciGattDbCmdMonitor(RemoveCharacteristic, characteristicHandle);
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
