/*! *********************************************************************************
 * \defgroup SHELL GATT
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* This file is the interface file for the GATT Shell module
*
* SPDX-License-Identifier: BSD-3-Clause
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
