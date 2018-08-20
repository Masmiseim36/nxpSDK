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

#ifndef _FSCI_BLE_TYPES_H
#define _FSCI_BLE_TYPES_H

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
        (value) = (*(pBuff) == 0) ? FALSE : TRUE;   \
        (pBuff) ++
          
#define fsciBleGetBufferFromBoolValue(value, pBuff) \
        *(pBuff) = ((value) == FALSE) ? 0 : 1;      \
        (pBuff) ++
                  
#define fsciBleGetUint8ValueFromBuffer(value, pBuff)    \
        (value) = *(pBuff);                             \
        (pBuff) ++
          
#define fsciBleGetBufferFromUint8Value(value, pBuff)    \
        *(pBuff) = (value);                             \
        (pBuff) ++
                                       
#define fsciBleGetUint16ValueFromBuffer(value, pBuff)   \
        (value) = Utils_ExtractTwoByteValue((pBuff));   \
        (pBuff) += 2
          
#define fsciBleGetBufferFromUint16Value(value, pBuff)   \
        Utils_PackTwoByteValue((value), (pBuff));       \
        (pBuff) += 2               
                       
#define fsciBleGetUint32ValueFromBuffer(value, pBuff)   \
        (value) = Utils_ExtractFourByteValue((pBuff));  \
        (pBuff) += 4
          
#define fsciBleGetBufferFromUint32Value(value, pBuff)   \
        Utils_PackFourByteValue((value), (pBuff));      \
        (pBuff) += 4                          
                          
#define fsciBleGetUint128ValueFromBuffer(pValue, pBuff) \
        fsciBleGetArrayFromBuffer((pValue), (pBuff), 16)
          
#define fsciBleGetBufferFromUint128Value(pValue, pBuff) \
        fsciBleGetBufferFromArray((pValue), (pBuff), 16)

#define fsciBleGetArrayFromBuffer(pArray, pBuff, nbOfBytes) \
        FLib_MemCpy((pArray), (pBuff), (nbOfBytes));        \
        (pBuff) += (nbOfBytes)
          
#define fsciBleGetBufferFromArray(pArray, pBuff, nbOfBytes) \
        FLib_MemCpy((pBuff), (pArray), (nbOfBytes));        \
        (pBuff) += (nbOfBytes)
                                                                          
#define fsciBleGetEnumValueFromBuffer(value, pBuff, enumType)   \
        fsciBleGetArrayFromBuffer((uint8_t*)&(value), (pBuff), sizeof(enumType))          
          
#define fsciBleGetBufferFromEnumValue(value, pBuff, enumType)   \
        fsciBleGetBufferFromArray((uint8_t*)&(value), (pBuff), sizeof(enumType))            
                                                                                                  
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
    bleUuid_t*      pUuid, 
    uint8_t**       ppBuffer, 
    bleUuidType_t   uuidType
);

#ifdef __cplusplus
}
#endif 

#endif /* _FSCI_BLE_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */