/*! *********************************************************************************
 * \defgroup FSCI_BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* This is a header file for FSCI BLE management.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef FSCI_BLE_GAP_TYPES_H
#define FSCI_BLE_GAP_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include <string.h>
#include "fsci_ble.h"
#include "gap_types.h"
#include "gap_interface.h"
#include "gatt_database.h"
#include "gatt_types.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

#define fsciBleGapFreeSmpKeys(pSmpKeys) \
        MEM_BufferFree(pSmpKeys)

#define fsciBleGapGetSecurityRequirementsBufferSize(pSecurityRequirements)  \
        (sizeof(gapSecurityModeAndLevel_t) + sizeof(bool_t) + sizeof(uint16_t))

#define fsciBleGapGetServiceSecurityRequirementsBufferSize(pSecurityRequirements)   \
        (sizeof(uint16_t) + fsciBleGapGetSecurityRequirementsBufferSize(pSecurityRequirements))

#define fsciBleGapGetDeviceSecurityRequirementsBufferSize(pDeviceSecurityRequirements)                              \
        (fsciBleGapGetSecurityRequirementsBufferSize((pDeviceSecurityRequirements)->pMasterSecurityRequirements) +  \
        sizeof(uint8_t) + (pDeviceSecurityRequirements)->cNumServices *                                             \
        fsciBleGapGetServiceSecurityRequirementsBufferSize((pDeviceSecurityRequirements)->aServiceSecurityRequirements))

#define fsciBleGapFreeDeviceSecurityRequirements(pDeviceSecurityRequirements)   \
        MEM_BufferFree(pDeviceSecurityRequirements)

#define fsciBleGapGetHandleListBufferSize(pHandleList)  \
        (sizeof(uint8_t) + (pHandleList)->cNumHandles * sizeof(uint16_T))

#define fsciBleGapGetConnectionSecurityInformationBufferSize(pConnectionSecurityInformation)    \
        (sizeof(gapSecurityModeAndLevel_t) +                                                    \
        fsciBleGapGetHandleListBufferSize((pConnectionSecurityInformation)->authorizedToRead) + \
        fsciBleGapGetHandleListBufferSize((pConnectionSecurityInformation)->authorizedToRead))

#define fsciBleGapGetPairingParametersBufferSize(pPairingParameters)        \
        (sizeof(bool_t) + sizeof(gapSecurityModeAndLevel_t) +               \
        sizeof(uint8_t) + sizeof(gapIoCapabilities_t) +  sizeof(bool_t) +   \
        sizeof(gapSmpKeyFlags_t) + sizeof(gapSmpKeyFlags_t) +               \
        sizeof(bool_t) + sizeof(bool_t))

#define fsciBleGapGetSlaveSecurityRequestParametersBufferSize(pSlaveSecurityRequestParameters)  \
        (sizeof(bool_t) + sizeof(gapSecurityModeAndLevel_t))

#define fsciBleGapGetAdvertisingParametersBufferSize(pSecurityRequirements)                     \
        (sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bleAdvertisingType_t) +                   \
        sizeof(bleAddressType_t) + sizeof(bleAddressType_t) + gcBleDeviceAddressSize_c +        \
        sizeof(gapAdvertisingChannelMapFlags_t) + sizeof(gapAdvertisingFilterPolicy_t) +   \
        sizeof(int8_t))

#define fsciBleGapGetExtAdvertisingParametersBufferSize(pAdvertisingParameters)               \
        (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t)          +   \
        sizeof(bleAddressType_t) + gcBleDeviceAddressSize_c + sizeof(bleAddressType_t)    +   \
        gcBleDeviceAddressSize_c + sizeof(gapAdvertisingChannelMapFlags_t)                +   \
        sizeof(gapAdvertisingFilterPolicy_t) + sizeof(bleAdvRequestProperties_t)          +   \
        sizeof(int8_t) + sizeof(gapLePhyMode_t) + sizeof(gapLePhyMode_t) + sizeof(uint8_t)+   \
        sizeof(bool_t))

#define fsciBleGapGetScanningParametersBufferSize(pScanningParameters)  \
        (sizeof(bleScanType_t) + sizeof(uint16_t) + sizeof(uint16_t) +  \
        sizeof(bleAddressType_t) + sizeof(bleScanningFilterPolicy_t) +  \
        sizeof(gapLePhyFlags_t))

#define fsciBleGapGetConnectionRequestParametersBufferSize(pConnectionRequestParameters)    \
        (sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bleInitiatorFilterPolicy_t) +         \
        sizeof(bleAddressType_t) + sizeof(bleAddressType_t) + gcBleDeviceAddressSize_c +    \
        sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) +         \
        sizeof(uint16_t) + sizeof(uint16_t) + sizeof(bool_t) + sizeof(gapLePhyFlags_t))

#define fsciBleGapGetConnectionParametersBufferSize(pConnectionParameters)  \
        (sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) +           \
        sizeof(bleMasterClockAccuracy_t))

#define fsciBleGapGetAdStructureBufferSize(pAdStructure)    \
        (sizeof(uint8_t) + (uint16_t)(pAdStructure)->length)

#define fsciBleGapFreeAdStructure(pAdStructure) \
        MEM_BufferFree(pAdStructure)

#define fsciBleGapFreeAdvertisingData(pAdvertisingData) \
        MEM_BufferFree(pAdvertisingData)

#define fsciBleGapAllocScanResponseDataForBuffer(pBuffer)   \
        (gapScanResponseData_t*)fsciBleGapAllocAdvertisingDataForBuffer(pBuffer)

#define fsciBleGapGetScanResponseDataFromBuffer(pScanResponseData, ppBuffer)    \
        fsciBleGapGetAdvertisingDataFromBuffer((gapAdvertisingData_t*)(pScanResponseData), (ppBuffer))

#define fsciBleGapGetScanResponseDataBufferSize(pScanResponseData)  \
        fsciBleGapGetAdvertisingDataBufferSize((gapAdvertisingData_t*)(pScanResponseData))

#define fsciBleGapGetBufferFromScanResponseData(pScanResponseData, ppBuffer)    \
        fsciBleGapGetBufferFromAdvertisingData((gapAdvertisingData_t*)(pScanResponseData), (ppBuffer))

#define fsciBleGapFreeScanResponseData(pScanResponseData)   \
        MEM_BufferFree(pScanResponseData)

#define fsciBleGapGetScannedDeviceBufferSize(pScannedDevice)                \
        (sizeof(bleAddressType_t) + gcBleDeviceAddressSize_c +              \
        sizeof(int8_t) + sizeof(uint8_t) + (uint16_t)(pScannedDevice)->dataLength +   \
        sizeof(bleAdvertisingReportEventType_t)+                            \
        sizeof(bool_t) + gcBleDeviceAddressSize_c + sizeof(bool_t))

#define fsciBleGapGetExtScannedDeviceBufferSize(pScannedDevice)               \
        (sizeof(bleAddressType_t) + sizeof(bleDeviceAddress_t) + sizeof(uint8_t) + \
        sizeof(bool_t) + sizeof(bleAdvReportEventProperties_t) + sizeof(int8_t) + \
        sizeof(int8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t) +   \
        sizeof(bool_t) + sizeof(bleAddressType_t) + sizeof(bleDeviceAddress_t) +  \
        sizeof(uint16_t) + (pScannedDevice)->dataLength)

#define fsciBleGapGetPeriodicScannedDeviceBufferSize(pScannedDevice)               \
        (sizeof(uint8_t) + sizeof(bleAddressType_t) + sizeof(bleDeviceAddress_t) + \
        sizeof(int8_t) + sizeof(int8_t) + sizeof(uint16_t) + (pScannedDevice)->dataLength)

#define fsciBleGapGetSyncLostBufferSize(pSyncLost)               \
        (sizeof(uint8_t) + sizeof(bleAddressType_t) + sizeof(bleDeviceAddress_t))

#define fsciBleGapFreeScannedDevice(pScannedDevice) \
        MEM_BufferFree(pScannedDevice)

#define fsciBleGapFreeScanningEvent(pScanningEvent) \
        MEM_BufferFree(pScanningEvent)

#define fsciBleGapGetConnectedEventBufferSize(pConnectedEvent)                              \
        (fsciBleGapGetConnectionParametersBufferSize(&(pConnectedEvent)->connParameters) +  \
        sizeof(bleAddressType_t) + gcBleDeviceAddressSize_c +                               \
        sizeof(bool_t) + gcBleDeviceAddressSize_c +                                         \
        sizeof(bool_t) + gcBleDeviceAddressSize_c)

#define fsciBleGapGetKeyExchangeRequestEventBufferSize(pKeyExchangeRequestEvent)    \
        (sizeof(gapSmpKeyFlags_t) + sizeof(uint8_t))

#define fsciBleGapGetKeysReceivedEventBufferSize(pKeysReceivedEvent)    \
        fsciBleGapGetSmpKeysBufferSize((pKeysReceivedEvent)->pKeys)

#define fsciBleGapGetKeysReceivedEventFromBuffer(pKeysReceivedEvent, ppBuffer)  \
        fsciBleGapGetSmpKeysFromBuffer((pKeysReceivedEvent)->pKeys, (ppBuffer))

#define fsciBleGapGetBufferFromKeysReceivedEvent(pKeysReceivedEvent, ppBuffer)  \
        fsciBleGapGetBufferFromSmpKeys((pKeysReceivedEvent)->pKeys, (ppBuffer))

#define fsciBleGapGetAuthenticationRejectedEventBufferSize(pAuthenticationRejectedEvent)    \
        sizeof(gapAuthenticationRejectReason_t)

#define fsciBleGapGetAuthenticationRejectedEventFromBuffer(pAuthenticationRejectedEvent, ppBuffer)  \
        fsciBleGetEnumValueFromBuffer((pAuthenticationRejectedEvent)->rejectReason, *(ppBuffer), gapAuthenticationRejectReason_t)

#define fsciBleGapGetBufferFromAuthenticationRejectedEvent(pAuthenticationRejectedEvent, ppBuffer)  \
        fsciBleGetBufferFromEnumValue((pAuthenticationRejectedEvent)->rejectReason, *(ppBuffer), gapAuthenticationRejectReason_t)

#define fsciBleGapGetLongTermKeyRequestEventBufferSize(pLongTermKeyRequestEvent)    \
        (sizeof(uint16_t) + sizeof(uint8_t) + (uint16_t)(pLongTermKeyRequestEvent)->randSize)

#define fsciBleGapGetEncryptionChangedEventBufferSize(pEncryptionChangedEvent)  \
        sizeof(bool_t)

#define fsciBleGapGetEncryptionChangedEventFromBuffer(pEncryptionChangedEvent, ppBuffer)    \
        fsciBleGetBoolValueFromBuffer((pEncryptionChangedEvent)->newEncryptionState, *(ppBuffer))

#define fsciBleGapGetBufferFromEncryptionChangedEvent(pEncryptionChangedEvent, ppBuffer)    \
        fsciBleGetBufferFromBoolValue((pEncryptionChangedEvent)->newEncryptionState, *(ppBuffer))

#define fsciBleGapGetDisconnectedEventBufferSize(pDisconnectedEvent)    \
        sizeof(gapDisconnectionReason_t)

#define fsciBleGapGetDisconnectedEventFromBuffer(pDisconnectedEvent, ppBuffer)  \
        fsciBleGetEnumValueFromBuffer((pDisconnectedEvent)->reason, *(ppBuffer), gapDisconnectionReason_t)

#define fsciBleGapGetBufferFromDisconnectedEvent(pDisconnectedEvent, ppBuffer)  \
        fsciBleGetBufferFromEnumValue((pDisconnectedEvent)->reason, *(ppBuffer), gapDisconnectionReason_t)

#define fsciBleGapGetInternalErrorBufferSize(pInternalError)    \
        (sizeof(bleResult_t) + sizeof(gapInternalErrorSource_t) + sizeof(uint16_t))

#define fsciBleGapGetAutoConnectParamsBufferSize(pAutoConnectParams)    \
        (sizeof(uint8_t) + sizeof(bool_t) +                             \
        (pAutoConnectParams)->cNumAddresses * sizeof(gapConnectionRequestParameters_t))

#define fsciBleGapFreeAutoConnectParams(pAutoConnectParams) \
        MEM_BufferFree(pAutoConnectParams)

#define fsciBleGapGetConnParameterUpdateRequestBufferSize(pConnParameterUpdateRequest)    \
        (sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t))

#define fsciBleGapGetConnParameterUpdateCompleteBufferSize(pConnParameterUpdateComplete)  \
        (sizeof(bleResult_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t))

#define fsciBleGapGetConnLeDataLengthChangedBufferSize(pConnLeDataLengthChanged)  \
        (4U * sizeof(uint16_t))

#define fsciBleGapGetConnLeScDisplayNumericValueBufferSize(pConnLeScDisplayNumericValue)  \
        sizeof(uint32_t)

#define fsciBleGapGetConnLeScKeypressNotificationBufferSize(pConnLeScKeypressNotification)  \
        sizeof(gapKeypressNotification_t)

#define fsciBleGapGetPeriodicAdvSyncReqParametersBufferSize(pReq)        \
        (sizeof(gapCreateSyncReqFilterPolicy_t) + sizeof(uint8_t) + sizeof(bleAddressType_t) \
        + gcBleDeviceAddressSize_c + 2U * sizeof(uint16_t))

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
**********************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

gapSmpKeys_t* fsciBleGapAllocSmpKeysForBuffer
(
    uint8_t* pBuffer
);

gapSmpKeys_t* fsciBleGapAllocSmpKeys
(
    void
);

uint16_t fsciBleGapGetSmpKeysBufferSize
(
    const gapSmpKeys_t* pSmpKeys
);

void fsciBleGapGetSmpKeysFromBuffer
(
    gapSmpKeys_t*   pSmpKeys,
    uint8_t**       ppBuffer
);

void fsciBleGapGetBufferFromSmpKeys
(
    const gapSmpKeys_t*   pSmpKeys,
    uint8_t**       ppBuffer
);

void fsciBleGapGetSecurityRequirementsFromBuffer
(
    gapSecurityRequirements_t*  pSecurityRequirements,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetBufferFromSecurityRequirements
(
    gapSecurityRequirements_t*  pSecurityRequirements,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetServiceSecurityRequirementsFromBuffer
(
    gapServiceSecurityRequirements_t*   pServiceSecurityRequirements,
    uint8_t**                           ppBuffer
);

void fsciBleGapGetBufferFromServiceSecurityRequirements
(
    gapServiceSecurityRequirements_t*   pServiceSecurityRequirements,
    uint8_t**                           ppBuffer
);

gapDeviceSecurityRequirements_t* fsciBleGapAllocDeviceSecurityRequirementsForBuffer
(
    uint8_t*    pBuffer
);

void fsciBleGapGetDeviceSecurityRequirementsFromBuffer
(
    gapDeviceSecurityRequirements_t*    pDeviceSecurityRequirements,
    uint8_t**                           ppBuffer
);

void fsciBleGapGetBufferFromDeviceSecurityRequirements
(
    const gapDeviceSecurityRequirements_t*    pDeviceSecurityRequirements,
    uint8_t**                           ppBuffer
);

void fsciBleGapGetHandleListFromBuffer
(
    gapHandleList_t*    pHandleList,
    uint8_t**           ppBuffer
);

void fsciBleGapGetBufferFromHandleList
(
    gapHandleList_t*    pHandleList,
    uint8_t**           ppBuffer
);

void fsciBleGapGetConnectionSecurityInformationFromBuffer
(
    gapConnectionSecurityInformation_t* pConnectionSecurityInformation,
    uint8_t**                           ppBuffer
);

void fsciBleGapGetBuffFromConnSecurityInformation
(
    gapConnectionSecurityInformation_t* pConnectionSecurityInformation,
    uint8_t**                           ppBuffer
);

void fsciBleGapGetPairingParametersFromBuffer
(
    gapPairingParameters_t* pPairingParameters,
    uint8_t**               ppBuffer
);

void fsciBleGapGetBufferFromPairingParameters
(
    const gapPairingParameters_t* pPairingParameters,
    uint8_t**               ppBuffer
);

void fsciBleGapGetSlaveSecurityRequestParametersFromBuffer
(
    gapSlaveSecurityRequestParameters_t*    pSlaveSecurityRequestParameters,
    uint8_t**                               ppBuffer
);

void fsciBleGapGetBufferFromSlaveSecurityRequestParameters
(
    gapSlaveSecurityRequestParameters_t*    pSlaveSecurityRequestParameters,
    uint8_t**                               ppBuffer
);

void fsciBleGapGetAdvertisingParametersFromBuffer
(
    gapAdvertisingParameters_t* pAdvertisingParameters,
    uint8_t**                   ppBuffer
);

#if defined(gBLE50_d) && (gBLE50_d == 1U)
void fsciBleGapGetExtAdvertisingParametersFromBuffer
(
    gapExtAdvertisingParameters_t* pAdvertisingParameters,
    uint8_t**                      ppBuffer
);
#endif

void fsciBleGapGetBuffFromAdvParameters
(
    const gapAdvertisingParameters_t* pAdvertisingParameters,
    uint8_t**                         ppBuffer
);

#if defined(gBLE50_d) && (gBLE50_d == 1U)
void fsciBleGapGetBufferFromExtAdvertisingParameters
(
    gapExtAdvertisingParameters_t* pAdvertisingParameters,
    uint8_t**                      ppBuffer
);
#endif

void fsciBleGapGetScanningParametersFromBuffer
(
    gapScanningParameters_t*    pScanningParameters,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetBuffFromScanParameters
(
    const gapScanningParameters_t*    pScanningParameters,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetConnectionRequestParametersFromBuffer
(
    gapConnectionRequestParameters_t*   pConnectionRequestParameters,
    uint8_t**                           ppBuffer
);

void fsciBleGapGetBuffFromConnReqParameters
(
    const gapConnectionRequestParameters_t*   pConnectionRequestParameters,
    uint8_t**                           ppBuffer
);

void fsciBleGapGetConnectionParametersFromBuffer
(
    gapConnectionParameters_t*  pConnectionParameters,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetBuffFromConnParameters
(
    gapConnectionParameters_t*  pConnectionParameters,
    uint8_t**                   ppBuffer
);

gapAdStructure_t* fsciBleGapAllocAdStructureForBuffer
(
    uint8_t* pBuffer
);

void fsciBleGapGetAdStructureFromBuffer
(
    gapAdStructure_t*   pAdStructure,
    uint8_t**           ppBuffer
);

void fsciBleGapGetBufferFromAdStructure
(
    gapAdStructure_t*   pAdStructure,
    uint8_t**           ppBuffer
);

gapAdvertisingData_t* fsciBleGapAllocAdvertisingDataForBuffer
(
    uint8_t* pBuffer
);

void fsciBleGapGetAdvertisingDataFromBuffer
(
    gapAdvertisingData_t*   pAdvertisingData,
    uint8_t**               ppBuffer
);

uint16_t fsciBleGapGetAdvertisingDataBufferSize
(
    const gapAdvertisingData_t* pAdvertisingData
);

void fsciBleGapGetBufferFromAdvertisingData
(
    const gapAdvertisingData_t*   pAdvertisingData,
    uint8_t**               ppBuffer
);

void fsciBleGapGetScannedDeviceFromBuffer
(
    gapScannedDevice_t* pScannedDevice,
    uint8_t**           ppBuffer
);

void fsciBleGapGetBufferFromScannedDevice
(
    gapScannedDevice_t* pScannedDevice,
    uint8_t**           ppBuffer
);


#if defined(gBLE50_d) && (gBLE50_d == 1U)
void fsciBleGapGetSyncLostFromBuffer
(
    gapSyncLostEventData_t* syncLost,
    uint8_t**               ppBuffer
);

void fsciBleGapGetBufferFromSyncLost
(
    gapSyncLostEventData_t* syncLost,
    uint8_t**               ppBuffer
);

void fsciBleGapGetExtScannedDeviceFromBuffer
(
    gapExtScannedDevice_t* pScannedDevice,
    uint8_t**              ppBuffer
);

void fsciBleGapGetBufferFromExtScannedDevice
(
    gapExtScannedDevice_t* pScannedDevice,
    uint8_t**              ppBuffer
);

void fsciBleGapGetPeriodicScannedDeviceFromBuffer
(
    gapPeriodicScannedDevice_t* pScannedDevice,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetBufferFromPerScannedDevice
(
    gapPeriodicScannedDevice_t* pScannedDevice,
    uint8_t**                   ppBuffer
);
#endif

void fsciBleGapGetConnectedEventFromBuffer
(
    gapConnectedEvent_t*    pConnectedEvent,
    uint8_t**               ppBuffer
);

void fsciBleGapGetBufferFromConnectedEvent
(
    gapConnectedEvent_t*    pConnectedEvent,
    uint8_t**               ppBuffer
);

void fsciBleGapGetKeyExchangeRequestEventFromBuffer
(
    gapKeyExchangeRequestEvent_t*   pKeyExchangeRequestEvent,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetBufferFromKeyExchangeRequestEvent
(
    gapKeyExchangeRequestEvent_t*   pKeyExchangeRequestEvent,
    uint8_t**                       ppBuffer
);

uint16_t fsciBleGapGetPairingCompleteEventBufferSize
(
    gapPairingCompleteEvent_t* pPairingCompleteEvent
);

void fsciBleGapGetPairComplEventFromBuffer
(
    gapPairingCompleteEvent_t*  pPairingCompleteEvent,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetBufferFromPairingCompleteEvent
(
    gapPairingCompleteEvent_t*  pPairingCompleteEvent,
    uint8_t**                   ppBuffer
);

void fsciBleGapGetLongTermKeyRequestEventFromBuffer
(
    gapLongTermKeyRequestEvent_t*   pLongTermKeyRequestEvent,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetBufferFromLongTermKeyRequestEvent
(
    gapLongTermKeyRequestEvent_t*   pLongTermKeyRequestEvent,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetInternalErrorFromBuffer
(
    gapInternalError_t* pInternalError,
    uint8_t**           ppBuffer
);

void fsciBleGapGetBufferFromInternalError
(
    gapInternalError_t* pInternalError,
    uint8_t**           ppBuffer
);

uint16_t fsciBleGapGetGenericEventBufferSize
(
    gapGenericEvent_t*  pGenericEvent
);

void fsciBleGapGetGenericEventFromBuffer
(
    gapGenericEvent_t*  pGenericEvent,
    uint8_t**           ppBuffer
);

void fsciBleGapGetBufferFromGenericEvent
(
    gapGenericEvent_t*  pGenericEvent,
    uint8_t**           ppBuffer
);

uint16_t fsciBleGapGetAdvertisingEventBufferSize
(
    gapAdvertisingEvent_t* pAdvertisingEvent
);

void fsciBleGapGetAdvertisingEventFromBuffer
(
    gapAdvertisingEvent_t*  pAdvertisingEvent,
    uint8_t**               ppBuffer
);

void fsciBleGapGetBuffFromAdvEvent
(
    gapAdvertisingEvent_t*  pAdvertisingEvent,
    uint8_t**               ppBuffer
);

gapScanningEvent_t* fsciBleGapAllocScanningEventForBuffer
(
    gapScanningEventType_t  eventType,
    uint8_t*                pBuffer
);

uint16_t fsciBleGapGetScanningEventBufferSize
(
    gapScanningEvent_t* pScanningEvent
);

void fsciBleGapGetScanningEventFromBuffer
(
    gapScanningEvent_t* pScanningEvent,
    uint8_t**           ppBuffer
);

void fsciBleGapGetBufferFromScanningEvent
(
    gapScanningEvent_t* pScanningEvent,
    uint8_t**           ppBuffer
);

gapConnectionEvent_t* fsciBleGapAllocConnectionEventForBuffer
(
    gapConnectionEventType_t    eventType,
    uint8_t*                    pBuffer
);

uint16_t fsciBleGapGetConnectionEventBufferSize
(
    gapConnectionEvent_t* pConnectionEvent
);

void fsciBleGapGetConnectionEventFromBuffer
(
    gapConnectionEvent_t*   pConnectionEvent,
    uint8_t**               ppBuffer
);

void fsciBleGapGetBufferFromConnectionEvent
(
    gapConnectionEvent_t*   pConnectionEvent,
    uint8_t**               ppBuffer
);

void fsciBleGapFreeConnectionEvent
(
    gapConnectionEvent_t* pConnectionEvent
);

gapAutoConnectParams_t* fsciBleGapAllocAutoConnectParamsForBuffer
(
    uint8_t* pBuffer
);

void fsciBleGapGetAutoConnectParamsFromBuffer
(
    gapAutoConnectParams_t* pAutoConnectParams,
    uint8_t**               ppBuffer
);

void fsciBleGapGetBufferFromAutoConnectParams
(
    gapAutoConnectParams_t* pAutoConnectParams,
    uint8_t**               ppBuffer
);

void fsciBleGapGetConnParamUpdateReqFromBuffer
(
    gapConnParamsUpdateReq_t* pConnParameterUpdateRequest,
    uint8_t**                 ppBuffer
);

void fsciBleGapGetBufferFromConnParameterUpdateRequest
(
    gapConnParamsUpdateReq_t*         pConnParameterUpdateRequest,
    uint8_t**                         ppBuffer
);

void fsciBleGapGetConnParameterUpdateCompleteFromBuffer
(
    gapConnParamsUpdateComplete_t*    pConnectionParameterUpdateComplete,
    uint8_t**                         ppBuffer
);

void fsciBleGapGetBuffFromConnParameterUpdateComplete
(
    gapConnParamsUpdateComplete_t*  pConnParameterUpdateComplete,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetConnLeDataLengthChangedFromBuffer
(
    gapConnLeDataLengthChanged_t*   pConnLeDataLengthChanged,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetBufferFromConnLeDataLengthChanged
(
    gapConnLeDataLengthChanged_t*   pConnLeDataLengthChanged,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetIdentityInformationFromBuffer
(
    gapIdentityInformation_t*       pIdentityInformation,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetBufferFromIdentityInformation
(
    const gapIdentityInformation_t*       pIdentityInformation,
    uint8_t**                       ppBuffer
);

#if defined(gBLE50_d) && (gBLE50_d == 1U)
void fsciBleGapGetPeriodicAdvSyncReqFromBuffer
(
    gapPeriodicAdvSyncReq_t*        pReq,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetBufferFromPerAdvSyncReq
(
    gapPeriodicAdvSyncReq_t*        pReq,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetBufferFromPerAdvParameters
(
    gapPeriodicAdvParameters_t*     pAdvertisingParameters,
    uint8_t**                       ppBuffer
);

void fsciBleGapGetPeriodicAdvParametersFromBuffer
(
    gapPeriodicAdvParameters_t*     pAdvertisingParameters,
    uint8_t**                       ppBuffer
);
#endif

#ifdef __cplusplus
}
#endif

#endif /* FSCI_BLE_GAP_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
