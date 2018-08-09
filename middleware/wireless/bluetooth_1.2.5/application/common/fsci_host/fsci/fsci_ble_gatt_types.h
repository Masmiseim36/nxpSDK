/*! *********************************************************************************
* \addtogroup FSCI_HOST_BLE
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
* This is a header file for FSCI Host BLE management.
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

#ifndef _FSCI_BLE_GATT_TYPES_H
#define _FSCI_BLE_GATT_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "fsci_ble.h"
#include "Panic.h"
#include "gatt_types.h"
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

#define fsciBleGattClientGetAttributeBufferSize(pAttribute) \
        (sizeof(uint16_t) + sizeof(bleUuidType_t) +         \
        fsciBleGetUuidBufferSize((pAttribute)->uuidType) +  \
        sizeof(uint16_t) + sizeof(uint16_t) + (pAttribute)->valueLength)

#define fsciBleGattClientEraseTmpInfo(bAllocated)   \
        fsciBleGattClientErasePermanentOrTmpInfo((bAllocated), TRUE)

#define fsciBleGattClientEraseInfo(bAllocated)  \
        fsciBleGattClientErasePermanentOrTmpInfo((bAllocated), FALSE)

#define fsciBleGattServerGetCharacteristicBufferSize(pCharacteristic)   \
        fsciBleGattClientGetCharacteristicBufferSize(pCharacteristic)

#define fsciBleGattServerGetCharacteristicFromBuffer(pCharacteristic, ppBuffer) \
        fsciBleGattClientGetCharacteristicFromBuffer((pCharacteristic), (ppBuffer))

#define fsciBleGattServerGetBufferFromCharacteristic(pCharacteristic, ppBuffer) \
        fsciBleGattClientGetBufferFromCharacteristic((pCharacteristic), (ppBuffer))
                                                                                                                     
#define fsciBleGattServerGetServerMtuChangedEventBufferSize(pServerMtuChangedEvent) \
        sizeof(uint16_t)

#define fsciBleGattServerGetServerMtuChangedEventFromBuffer(pServerMtuChangedEvent, ppBuffer)   \
        fsciBleGetUint16ValueFromBuffer((pServerMtuChangedEvent)->newMtu, *(ppBuffer))

#define fsciBleGattServerGetBufferFromServerMtuChangedEvent(pServerMtuChangedEvent, ppBuffer)   \
        fsciBleGetBufferFromUint16Value((pServerMtuChangedEvent)->newMtu, *(ppBuffer))

#define fsciBleGattServerGetServerAttributeWrittenEventBufferSize(pServerAttributeWrittenEvent) \
        (sizeof(uint16_t) + sizeof(uint16_t) + (pServerAttributeWrittenEvent)->cValueLength)
          
#define fsciBleGattServerGetServerLongCharacteristicWrittenEventBufferSize(pServerLongCharacteristicWrittenEvent) \
        (sizeof(uint16_t) + sizeof(uint16_t) + (pServerLongCharacteristicWrittenEvent)->cValueLength)          

#define fsciBleGattServerGetServerLongCharacteristicWrittenEventBufferSize(pServerLongCharacteristicWrittenEvent) \
        (sizeof(uint16_t) + sizeof(uint16_t) + (pServerLongCharacteristicWrittenEvent)->cValueLength)

#define fsciBleGattServerGetServerCccdWrittenEventBufferSize(pServerCccdWrittenEvent)   \
        (sizeof(uint16_t) + sizeof(gattCccdFlags_t))
          
#define fsciBleGattServerGetServerAttributeReadEventBufferSize(pServerAttributeReadEvent)   \
        sizeof(uint16_t)
          
#define fsciBleGattServerGetServerAttributeReadEventFromBuffer(pServerAttributeReadEvent, ppBuffer)   \
        fsciBleGetUint16ValueFromBuffer((pServerAttributeReadEvent)->handle, *(ppBuffer))

#define fsciBleGattServerGetBufferFromServerAttributeReadEvent(pServerAttributeReadEvent, ppBuffer)   \
        fsciBleGetBufferFromUint16Value((pServerAttributeReadEvent)->handle, *(ppBuffer))

#define fsciBleGattServerGetServerProcedureErrorBufferSize(pServerProcedureError)   \
        (sizeof(gattServerProcedureType_t) + sizeof(bleResult_t))

#define fsciBleGattServerFreeServerEvent(pServerEvent)  \
        MEM_BufferFree(pServerEvent)
          
#define fsciBleGattGetHandleRangeBufferSize(pHandleRange)    \
        (sizeof(uint16_t) + sizeof(uint16_t))

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

uint16_t* fsciBleGattClientAllocOutOrIoArraySize(void);

uint8_t* fsciBleGattClientAllocOutOrIoValue
(
    uint16_t maxValueLength
);

gattAttribute_t* fsciBleGattClientAllocOutOrIoAttributes
(
    uint8_t maxNbOfAtributes
);

void fsciBleGattClientGetAttributeFromBuffer
(
    gattAttribute_t*    pAttribute, 
    uint8_t**           ppBuffer
);

void fsciBleGattClientGetBufferFromAttribute
(
    gattAttribute_t*    pAttribute, 
    uint8_t**           ppBuffer
);

gattCharacteristic_t* fsciBleGattClientAllocOutOrIoCharacteristics
(
    uint8_t maxNbOfCharacteristics
);

uint16_t fsciBleGattClientGetCharacteristicBufferSize
(
    gattCharacteristic_t* pCharacteristic
);

void fsciBleGattClientGetCharacteristicFromBuffer
(
    gattCharacteristic_t*   pCharacteristic, 
    uint8_t**               ppBuffer
);

void fsciBleGattClientGetBufferFromCharacteristic
(
    gattCharacteristic_t*   pCharacteristic, 
    uint8_t**               ppBuffer
);

gattService_t* fsciBleGattClientAllocOutOrIoIncludedServices
(
    uint8_t maxNbOfIncludedServices
);

gattService_t* fsciBleGattClientAllocOutOrIoServices
(
    uint8_t maxNbOfServices
);

uint16_t fsciBleGattClientGetServiceBufferSize
(
    gattService_t* pService
);

void fsciBleGattClientGetServiceFromBuffer
(
    gattService_t*  pService, 
    uint8_t**       ppBuffer
);

void fsciBleGattClientGetBufferFromService
(
    gattService_t*  pService, 
    uint8_t**       ppBuffer
);

void fsciBleGattClientSaveServicesInfo
(
    deviceId_t     deviceId,
    gattService_t* pServices
);

void fsciBleGattClientSaveIncludedServicesInfo
(
    deviceId_t     deviceId,
    gattService_t* pIncludedServices
);

void fsciBleGattClientSaveCharacteristicsInfo
(
    deviceId_t            deviceId,
    gattCharacteristic_t* pCharacteristics
);

void fsciBleGattClientSaveDescriptorsInfo
(
    deviceId_t       deviceId,
    gattAttribute_t* pDescriptors
);

void fsciBleGattClientSaveValueInfo
(
    deviceId_t deviceId,
    uint8_t*   pValue
);

void fsciBleGattClientSaveArraySizeInfo
(
    deviceId_t deviceId,
    uint16_t*  pArraySize
);

gattService_t* fsciBleGattClientGetServicesInfo
(
    deviceId_t deviceId,
    bool_t     bAllocated
);

gattService_t* fsciBleGattClientGetIncludedServicesInfo
(
    deviceId_t deviceId,
    bool_t     bAllocated
);

gattCharacteristic_t* fsciBleGattClientGetCharacteristicsInfo
(
    deviceId_t deviceId,
    bool_t     bAllocated
);

gattAttribute_t* fsciBleGattClientGetDescriptorsInfo
(
    deviceId_t deviceId,
    bool_t     bAllocated
);

uint8_t* fsciBleGattClientGetValueInfo
(
    deviceId_t deviceId,
    bool_t     bAllocated
);

uint16_t* fsciBleGattClientGetArraySizeInfo
(
    deviceId_t deviceId,
    bool_t     bAllocated
);

void fsciBleGattClientKeepInfo
(
    bool_t bAllocatedInfo
);

void fsciBleGattClientErasePermanentOrTmpInfo
(
    bool_t bAllocatedInfo, 
    bool_t bTmp
);

void fsciBleGattServerGetServerAttributeWrittenEventFromBuffer
(
    gattServerAttributeWrittenEvent_t*  pServerAttributeWrittenEvent, 
    uint8_t**                           ppBuffer
);

void fsciBleGattServerGetBufferFromServerAttributeWrittenEvent
(
    gattServerAttributeWrittenEvent_t*  pServerAttributeWrittenEvent, 
    uint8_t**                           ppBuffer
);

void fsciBleGattServerGetServerLongCharacteristicWrittenEventFromBuffer
(
    gattServerLongCharacteristicWrittenEvent_t*	pServerLongCharacteristicWrittenEvent, 
    uint8_t**                                   ppBuffer
);

void fsciBleGattServerGetBufferFromServerLongCharacteristicWrittenEvent
(
    gattServerLongCharacteristicWrittenEvent_t*	pServerLongCharacteristicWrittenEvent, 
    uint8_t**                                   ppBuffer
);

void fsciBleGattServerGetServerCccdWrittenEventFromBuffer
(
    gattServerCccdWrittenEvent_t*   pServerCccdWrittenEvent, 
    uint8_t**                       ppBuffer
);

void fsciBleGattServerGetBufferFromServerCccdWrittenEvent
(
    gattServerCccdWrittenEvent_t*   pServerCccdWrittenEvent, 
    uint8_t**                       ppBuffer
);

void fsciBleGattServerGetServerProcedureErrorFromBuffer
(
    gattServerProcedureError_t* pServerProcedureError, 
    uint8_t**                   ppBuffer
);

void fsciBleGattServerGetBufferFromServerProcedureError
(
    gattServerProcedureError_t* pServerProcedureError, 
    uint8_t**                   ppBuffer
);

gattServerEvent_t* fsciBleGattServerAllocServerEventForBuffer
(
    gattServerEventType_t   eventType, 
    uint8_t*                pBuffer
);

uint16_t fsciBleGattServerGetServerEventBufferSize
(
    gattServerEvent_t* pServerEvent
);

void fsciBleGattServerGetServerEventFromBuffer
(
    gattServerEvent_t*  pServerEvent, 
    uint8_t**           ppBuffer
);

void fsciBleGattServerGetBufferFromServerEvent
(
    gattServerEvent_t*  pServerEvent, 
    uint8_t**           ppBuffer
);

void fsciBleGattGetHandleRangeFromBuffer
(
    gattHandleRange_t*  pHandleRange, 
    uint8_t**           ppBuffer
);

void fsciBleGattGetBufferFromHandleRange
(
    gattHandleRange_t*  pHandleRange, 
    uint8_t**           ppBuffer
);
 
#ifdef __cplusplus
}
#endif 

#endif /* _FSCI_BLE_GATT_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */