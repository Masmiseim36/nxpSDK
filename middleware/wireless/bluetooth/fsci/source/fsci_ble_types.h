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

#ifndef FSCI_BLE_TYPES_H
#define FSCI_BLE_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

#include "ble_general.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

#define fsciBleGetBoolValueFromBuffer(value, pBuff) \
        (value) = (*(pBuff) == 0U) ? FALSE : TRUE;  \
        (pBuff) ++

#define fsciBleGetBufferFromBoolValue(value, pBuff) \
        *(pBuff) = ((value) == FALSE) ? 0U : 1U;    \
        (pBuff) ++

#define fsciBleGetUint8ValueFromBufferSigned(value, pBuff) \
        (value) = (int8_t)(*(pBuff));                      \
        (pBuff) ++

#define fsciBleGetUint8ValueFromBuffer(value, pBuff)    \
        (value) = *(pBuff);                             \
        (pBuff) ++

#define fsciBleGetBufferFromUint8ValueSigned(value, pBuff) \
        *(pBuff) = (uint8_t)(value);                       \
        (pBuff) ++

#define fsciBleGetBufferFromUint8Value(value, pBuff)    \
        *(pBuff) = (value);                             \
        (pBuff) ++

#define fsciBleGetUint16ValueFromBuffer(value, pBuff)   \
        (value) = Utils_ExtractTwoByteValue((pBuff));   \
        (pBuff) += 2U

#define fsciBleGetBufferFromUint16Value(value, pBuff)   \
        Utils_PackTwoByteValue((value), (pBuff));       \
        (pBuff) += 2U

#define fsciBleGetUint32ValueFromBuffer(value, pBuff)   \
        (value) = Utils_ExtractFourByteValue((pBuff));  \
        (pBuff) += 4U

#define fsciBleGetBufferFromUint32Value(value, pBuff)   \
        Utils_PackFourByteValue((value), (pBuff));      \
        (pBuff) += 4U

#define fsciBleGetUint128ValueFromBuffer(pValue, pBuff) \
        fsciBleGetArrayFromBuffer((pValue), (pBuff), 16)

#define fsciBleGetBufferFromUint128Value(pValue, pBuff) \
        fsciBleGetBufferFromArray((pValue), (pBuff), 16)

#define fsciBleGetArrayFromBuffer(pArray, pBuff, nbOfBytes) \
        FLib_MemCpy((void*)(pArray), (pBuff), (nbOfBytes)); \
        (pBuff) += (nbOfBytes)

#define fsciBleGetBufferFromArray(pArray, pBuff, nbOfBytes) \
        FLib_MemCpy((pBuff), (pArray), (nbOfBytes));        \
        (pBuff) += (nbOfBytes)

#define fsciBleGetEnumValueFromBuffer(value, pBuff, enumType)   \
        fsciBleGetArrayFromBuffer((uint8_t*)&(value), (pBuff), sizeof(enumType))

#define fsciBleGetBufferFromEnumValue(value, pBuff, enumType)   \
        fsciBleGetBufferFromArray((const uint8_t*)&(value), (pBuff), sizeof(enumType))

#define fsciBleGetAddressFromBuffer(pAddr, pBuff)   \
        fsciBleGetArrayFromBuffer((pAddr), (pBuff), gcBleDeviceAddressSize_c)

#define fsciBleGetBufferFromAddress(pAddr, pBuff)   \
        fsciBleGetBufferFromArray((pAddr), (pBuff), gcBleDeviceAddressSize_c)

#define fsciBleGetIrkFromBuffer(pAddr, pBuff)   \
        fsciBleGetArrayFromBuffer((pAddr), (pBuff), gcSmpIrkSize_c)

#define fsciBleGetBufferFromIrk(pAddr, pBuff)   \
        fsciBleGetBufferFromArray((pAddr), (pBuff), gcSmpIrkSize_c)

#define fsciBleGetCsrkFromBuffer(pAddr, pBuff)  \
        fsciBleGetArrayFromBuffer((pAddr), (pBuff), gcSmpCsrkSize_c)

#define fsciBleGetBufferFromCsrk(pAddr, pBuff)  \
        fsciBleGetBufferFromArray((pAddr), (pBuff), gcSmpCsrkSize_c)

#define fsciBleGetOobFromBuffer(pAddr, pBuff)   \
        fsciBleGetArrayFromBuffer((pAddr), (pBuff), gcSmpOobSize_c)

#define fsciBleGetBufferFromOob(pAddr, pBuff)   \
        fsciBleGetBufferFromArray((pAddr), (pBuff), gcSmpOobSize_c)

#define fsciBleGetDeviceIdBufferSize(pDeviceId) \
        sizeof(uint8_t)

#define fsciBleGetDeviceIdFromBuffer(pDeviceId, ppBuff) \
        fsciBleGetUint8ValueFromBuffer(*(pDeviceId), *(ppBuff))

#define fsciBleGetBufferFromDeviceId(pDeviceId, ppBuff) \
        fsciBleGetBufferFromUint8Value(*(pDeviceId), *(ppBuff))

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

uint16_t fsciBleGetUuidBufferSize
(
    bleUuidType_t uuidType
);

void fsciBleGetUuidFromBuffer
(
    bleUuid_t*      pUuid,
    uint8_t**       ppBuffer,
    bleUuidType_t   uuidType
);

void fsciBleGetBufferFromUuid
(
    const bleUuid_t*      pUuid,
    uint8_t**       ppBuffer,
    bleUuidType_t   uuidType
);

#ifdef __cplusplus
}
#endif

#endif /* FSCI_BLE_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
