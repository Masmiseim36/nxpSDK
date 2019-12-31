/*! *********************************************************************************
 * \addtogroup GATT_DB
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef GATT_DB_DYNAMIC_H
#define GATT_DB_DYNAMIC_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gatt_database.h"
#include "gatt_types.h"
#include "ble_sig_defines.h"
#include "ble_utils.h"

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
* \brief  Allocates smallest possible buffers and initializes an empty database.
*
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
* \remarks The application does not need to call this function if Ble_HostInitialize 
* or GattDb_Init is called.
*
********************************************************************************** */
bleResult_t GattDbDynamic_Init(void);

/*! *********************************************************************************
* \brief  Releases allocated buffers.
*
* \return  gBleSuccess_c or gBleInvalidState_c.
*
* \remarks This function executes synchronously.
* \remarks After the execution of this function, the external variable gattDatabase becomes NULL.
*
********************************************************************************** */
bleResult_t GattDbDynamic_ReleaseDatabase(void);

/*! *********************************************************************************
* \brief  Adds a Primary Service declaration into the database.
*
* \param[in]  desiredHandle     Desired attribute handle of the Service declaration. 
*                               Ignored if 0x0000 and allocated by the GATT Database
*                               Dynamic module.
* \param[in]  serviceUuidType   Service UUID type (16-bit, 32-bit or 128-bit).
* \param[in]  pServiceUuid      Service UUID.
* \param[out] pOutHandle        Resulting attribute handle of the Service declaration. Ignored if NULL.
*
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddPrimaryServiceDeclaration
(
    uint16_t              desiredHandle,
    bleUuidType_t         serviceUuidType,
    const bleUuid_t*      pServiceUuid,
    uint16_t*             pOutHandle
);

/*! *********************************************************************************
* \brief  Adds a Secondary Service declaration into the database.
*
* \param[in]  desiredHandle     Desired attribute handle of the Service declaration. 
*                               Ignored if 0x0000 and allocated by the GATT Database
*                               Dynamic module.
* \param[in]  serviceUuidType   Service UUID type (16-bit, 32-bit or 128-bit).
* \param[in]  pServiceUuid      Service UUID.
* \param[out] pOutHandle        Resulting attribute handle of the Service declaration. Ignored if NULL.
*
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddSecondaryServiceDeclaration
(
    uint16_t              desiredHandle,
    bleUuidType_t         serviceUuidType,
    const bleUuid_t*      pServiceUuid,
    uint16_t*             pOutHandle
);

/*! *********************************************************************************
* \brief  Adds an Include declaration into the database.
*
* \param[in]  includedServiceHandle     Handle of the included Service declaration.
* \param[in]  endGroupHandle            Last handle of the included Service.
* \param[in]  serviceUuidType           UUID type of the included Service.
* \param[in]  pServiceUuid              UUID of the included Service.
* \param[out] pOutHandle                Resulting attribute handle of the Include declaration. Ignored if NULL.
*
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddIncludeDeclaration
(
    uint16_t           includedServiceHandle,
    uint16_t           endGroupHandle,
    bleUuidType_t      serviceUuidType,
    const bleUuid_t*   pServiceUuid,
    uint16_t*          pOutHandle
);

/*! *********************************************************************************
* \brief  Adds a Characteristic declaration and its Value into the database.
*
* \param[in]  characteristicUuidType    Characteristic UUID type.
* \param[in]  pCharacteristicUuid       Characteristic UUID.
* \param[in]  characteristicProperties  Characteristic Properties.
* \param[in]  maxValueLength            If the Characteristic Value length is variable, 
*                                       this is the maximum length. For fixed lengths,
*                                       this must be set to 0.
* \param[in]  initialValueLength        Value length at initialization. Remains fixed
*                                       if maxValueLength is set to 0, otherwise cannot be
*                                       greater than maxValueLength.
* \param[in]  aInitialValue             Contains the initial value of the Characteristic.
* \param[in]  valueAccessPermissions    Access permissions for the value attribute.
* \param[out] pOutHandle                Resulting attribute handle of the Characteristic declaration. Ignored if NULL.
*                                       The Characteristic Value handle is the declaration handle plus one.
*
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharacteristicDeclarationAndValue
(
    bleUuidType_t                               characteristicUuidType,
    const bleUuid_t*                            pCharacteristicUuid,
    gattCharacteristicPropertiesBitFields_t     characteristicProperties,
    uint16_t                                    maxValueLength,
    uint16_t                                    initialValueLength,
    const uint8_t*                              aInitialValue,
    gattAttributePermissionsBitFields_t         valueAccessPermissions,
    uint16_t*                                   pOutHandle
);

/*! *********************************************************************************
* \brief  Adds a Characteristic descriptor into the database.
*
* \param[in]  descriptorUuidType            Descriptor UUID type.
* \param[in]  pDescriptorUuid               Descriptor UUID.
* \param[in]  descriptorValueLength         Fixed length of the descriptor attribute value.
* \param[in]  aInitialValue                 Initial value of the descriptor attribute.
* \param[in]  descriptorAccessPermissions   Access permissions for the descriptor attribute.
* \param[out] pOutHandle                    Resulting attribute handle of the Descriptor declaration. Ignored if NULL.
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharDescriptor
(
    bleUuidType_t                               descriptorUuidType,
    const bleUuid_t*                            pDescriptorUuid,
    uint16_t                                    descriptorValueLength,
    const uint8_t*                              aInitialValue,
    gattAttributePermissionsBitFields_t         descriptorAccessPermissions,
    uint16_t*                                   pOutHandle
);

/*! *********************************************************************************
* \brief  Adds a CCCD in the database.
*
* \param[out] pOutHandle        Resulting attribute handle of the CCCD declaration. Ignored if NULL.
*
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks If gBleOutOfMemory_c is returned, then you need to allocate more memory with
* GattDbDynamic_AllocateAttributes and/or GattDbDynamic_AllocateAttributeValues.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddCccd
(
    uint16_t*       pOutHandle
);

/*! *********************************************************************************
* \brief  Adds a Characteristic declaration with a Value contained in an universal 512-byte value buffer.
*
* \param[in]  characteristicUuidType    Characteristic UUID type.
* \param[in]  pCharacteristicUuid       Characteristic UUID.
* \param[in]  characteristicProperties  Characteristic Properties.
* \param[in]  valueAccessPermissions    Access permissions for the value attribute.
* \param[out] pOutHandle                Resulting attribute handle of the Characteristic declaration. Ignored if NULL.
*
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharDeclWithUniqueValue
(
    bleUuidType_t                               characteristicUuidType,
    const bleUuid_t*                            pCharacteristicUuid,
    gattCharacteristicPropertiesBitFields_t     characteristicProperties,
    gattAttributePermissionsBitFields_t         valueAccessPermissions,
    uint16_t*                                   pOutHandle
);

/*! *********************************************************************************
* \brief  Adds a Characteristic descriptor with a Value contained in an universal 512-byte value buffer.
*
* \param[in]  descriptorUuidType            Descriptor UUID type.
* \param[in]  pDescriptorUuid               Descriptor UUID.
* \param[in]  descriptorAccessPermissions   Access permissions for the descriptor attribute.
* \param[out] pOutHandle                    Resulting attribute handle of the Descriptor declaration. Ignored if NULL.
* \return  gBleSuccess_c or gBleOutOfMemory_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_AddCharDescriptorWithUniqueValue
(
    bleUuidType_t                               descriptorUuidType,
    bleUuid_t*                                  pDescriptorUuid,
    gattAttributePermissionsBitFields_t         descriptorAccessPermissions,
    uint16_t*                                   pOutHandle
);

/*! *********************************************************************************
* \brief  Removes a Service from the database.
*
* \param[in]  serviceHandle    Attribute handle of the Service declaration.
*
* \return  gBleSuccess_c or gGattDbInvalidHandle_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_RemoveService
(
    uint16_t serviceHandle
);

/*! *********************************************************************************
* \brief  Removes a Characteristic from the database.
*
* \param[in]  characteristicHandle    Attribute handle of the Characteristic declaration.
*
* \return  gBleSuccess_c or gGattDbInvalidHandle_c.
*
* \remarks This function executes synchronously.
*
********************************************************************************** */
bleResult_t GattDbDynamic_RemoveCharacteristic
(
    uint16_t characteristicHandle
);

#ifdef __cplusplus
}
#endif 

#endif /* GATT_DB_DYNAMIC_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
