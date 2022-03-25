/*! *********************************************************************************
 * \defgroup GATT_DB GATT_DB - GATT Database Interface and Definitions
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

#ifndef GATT_DATABASE_H
#define GATT_DATABASE_H

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
/*! Special value returned by GattDb_GetIndexOfHandle to signal that 
    an invalid attribute handle was given as parameter.  */
#define gGattDbInvalidHandleIndex_d     (0xFFFFU)
/*! Special value used to mark an invalid attribute handle. 
    Attribute handles are strictly positive. */
#define gGattDbInvalidHandle_d          (0x0000U)

#define    gPermissionNone_c                         0U       /*!< No permissions selected. */
    
/* Reading Permissions */
#define    gPermissionFlagReadable_c                 BIT0     /*!< Attribute can be read. */
/* if gPermissionFlagReadable_c == 1 */
#define    gPermissionFlagReadWithEncryption_c       BIT1     /*!< Attribute may be read only if link is encrypted. */
#define    gPermissionFlagReadWithAuthentication_c   BIT2     /*!< Attribute may be read only by authenticated peers. */
#define    gPermissionFlagReadWithAuthorization_c    BIT3     /*!< Attribute may be read only by authorized peers. */
/* endif */

/* Writing Permissions */
#define    gPermissionFlagWritable_c                 BIT4     /*!< Attribute can be written. */
/* if gPermissionFlagWritable_c == 1 */
#define    gPermissionFlagWriteWithEncryption_c      BIT5     /*!< Attribute may be written only if link is encrypted. */
#define    gPermissionFlagWriteWithAuthentication_c  BIT6     /*!< Attribute may be written only by authenticated peers. */
#define    gPermissionFlagWriteWithAuthorization_c   BIT7     /*!< Attribute may be written only by authorized peers. */
/* endif */

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Attribute structure */
typedef struct {
    uint16_t        handle;                         /*!< The attribute handle - cannot be 0x0000. The attribute handles need not be consecutive, but must be strictly increasing. */
    uint16_t        permissions;                    /*!< Attribute permissions as defined by the ATT. */
    uint32_t        uuid;                           /*!< The UUID should be read according to the gattDbAttribute_t.uuidType member: for 2-byte and 4-byte UUIDs, this contains the value of the UUID;
                                                         for 16-byte UUIDs, this is a pointer to the allocated 16-byte array containing the UUID. */
    uint8_t*        pValue;                         /*!< A pointer to allocated value array. */
    uint16_t        valueLength;                    /*!< The size of the value array. */
    uint16_t        uuidType : 2;                   /*!< Identifies the length of the UUID; values interpreted according to the \ref bleUuidType_t enumeration. */
    uint16_t        maxVariableValueLength : 10;    /*!< The maximum length of the attribute value array; if this is set to 0, then the attribute's length is fixed and cannot be changed. */
} gattDbAttribute_t;

/*! Bit fields for Characteristic properties */
typedef uint8_t gattCharacteristicPropertiesBitFields_t;
typedef enum {
    gGattCharPropNone_c                   = 0U,        /*!< No Properties selected. */
    gGattCharPropBroadcast_c              = BIT0,     /*!< Characteristic can be broadcast. */
    gGattCharPropRead_c                   = BIT1,     /*!< Characteristic can be read. */
    gGattCharPropWriteWithoutRsp_c        = BIT2,     /*!< Characteristic can be written without response. */
    gGattCharPropWrite_c                  = BIT3,     /*!< Characteristic can be written with response. */
    gGattCharPropNotify_c                 = BIT4,     /*!< Characteristic can be notified. */
    gGattCharPropIndicate_c               = BIT5,     /*!< Characteristic can be indicated. */
    gGattCharPropAuthSignedWrites_c       = BIT6,     /*!< Characteristic can be written with signed data. */
    gGattCharPropExtendedProperties_c     = BIT7      /*!< Extended Characteristic properties. */
} gattCharacteristicPropertiesBitFields_tag;

/*! Bit fields for attribute permissions */
typedef uint8_t gattAttributePermissionsBitFields_t;

/*! Attribute access type */
typedef enum {
    gAccessRead_c,      /*< Attempting to read the attribute. */
    gAccessWrite_c,     /*< Attempting to write the attribute. */
    gAccessNotify_c     /*< Attempting to notify the attribute. */
} gattDbAccessType_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
/*! The number of attributes in the GATT Database. */
extern uint16_t gGattDbAttributeCount_c;

/*! Reference to the GATT database */
extern gattDbAttribute_t* gattDatabase;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
* \brief   Returns the database index for a given attribute handle.
*
* \param[in] handle  The attribute handle.
*
* \return  The index of the given attribute in the database or gGattDbInvalidHandleIndex_d.
*
********************************************************************************** */
uint16_t GattDb_GetIndexOfHandle(uint16_t handle);

#ifdef __cplusplus
}
#endif 

#endif /* GATT_DATABASE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
