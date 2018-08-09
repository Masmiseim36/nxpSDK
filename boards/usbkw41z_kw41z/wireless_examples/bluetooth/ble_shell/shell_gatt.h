/*! *********************************************************************************
 * \defgroup SHELL GATT
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
* This file is the interface file for the GATT Shell module
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

#ifndef _SHELL_GATT_H_
#define _SHELL_GATT_H_

/*************************************************************************************
**************************************************************************************
* Public types and macros
**************************************************************************************
*************************************************************************************/

typedef struct gattUuidNames_tag
{
    uint16_t    uuid;
    char*       name;
}gattUuidNames_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern const gattUuidNames_t mGattServices[];
extern const gattUuidNames_t mGattChars[];
extern const gattUuidNames_t mGattDescriptors[];
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

char* ShellGatt_GetServiceName
(
    uint16_t uuid16
);

char* ShellGatt_GetCharacteristicName
(
    uint16_t uuid16
);

char* ShellGatt_GetDescriptorName 
(
    uint16_t uuid16
);

int8_t  ShellGatt_Command
(
    uint8_t argc, 
    char * argv[]
);
  
/* SHELL GATT Events Callback */
void ShellGatt_ServerCallback 
(
   deviceId_t deviceId,
   gattServerEvent_t* pServerEvent
);

void ShellGatt_ClientCallback 
(
    deviceId_t              serverDeviceId,
    gattProcedureType_t     procedureType,
    gattProcedureResult_t   procedureResult,
    bleResult_t             error
);

void ShellGatt_NotificationCallback
(
    deviceId_t          serverDeviceId, 
    uint16_t            characteristicValueHandle,
    uint8_t*            aValue,
    uint16_t            valueLength
);

void ShellGatt_IndicationCallback
(
    deviceId_t          serverDeviceId, 
    uint16_t            characteristicValueHandle,
    uint8_t*            aValue,
    uint16_t            valueLength
);

#ifdef __cplusplus
}
#endif 


#endif /* _SHELL_GATT_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
