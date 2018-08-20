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
* This is a source file for FSCI Host BLE management.
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

#include "fsci_ble_types.h"


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

uint16_t fsciBleGetUuidBufferSize(bleUuidType_t uuidType)
{
    uint16_t dataSize;
    
    /* Get the variable size for the needed buffer (depending on UUID type) */
    switch(uuidType)
    {
        case gBleUuidType16_c:
            {
                /* UUID 16 bits */
                dataSize = sizeof(uint16_t);
            }
            break;
            
        case gBleUuidType32_c:
            {
                /* UUID 32 bits */
                dataSize = sizeof(uint32_t);
            }
            break;
            
        case gBleUuidType128_c:
            {
                /* UUID 128 bits */
                dataSize = 16U * sizeof(uint8_t);
            }
            break;
            
        default:
            dataSize = 0;
            break;
    }
    
    /* Return the needed buffer size */
    return dataSize;
} 


void fsciBleGetUuidFromBuffer(bleUuid_t* pUuid, uint8_t** ppBuffer, bleUuidType_t uuidType)
{
    /* Read UUID from buffer (depending on UUID type) */ 
    switch(uuidType)
    {
        case gBleUuidType16_c:
            {
                /* UUID 16 bits */
                fsciBleGetUint16ValueFromBuffer(pUuid->uuid16, *ppBuffer);
            }
            break;
            
        case gBleUuidType32_c:
            {
                /* UUID 32 bits */
                fsciBleGetUint32ValueFromBuffer(pUuid->uuid32, *ppBuffer);
            }
            break;
            
        case gBleUuidType128_c:
            {
                /* UUID 128 bits */
                fsciBleGetUint128ValueFromBuffer(pUuid->uuid128, *ppBuffer);
            }
            break;
            
        default:
            break;
    }
} 


void fsciBleGetBufferFromUuid(bleUuid_t* pUuid, uint8_t** ppBuffer, bleUuidType_t uuidType)
{
    /* Write UUID in buffer (depending on UUID type) */ 
    switch(uuidType)
    {
        case gBleUuidType16_c:
            {
                /* UUID 16 bits */
                fsciBleGetBufferFromUint16Value(pUuid->uuid16, *ppBuffer);
            }
            break;
            
        case gBleUuidType32_c:
            {
                /* UUID 32 bits */
                fsciBleGetBufferFromUint32Value(pUuid->uuid32, *ppBuffer);
            }
            break;
            
        case gBleUuidType128_c:
            {
                /* UUID 128 bits */
                fsciBleGetBufferFromUint128Value(&pUuid->uuid128, *ppBuffer);
            }
            break;
            
        default:
            break;
    }
} 
   
/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* @}
********************************************************************************** */
