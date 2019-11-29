/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _BLE_ABS_TYPES_H
#define _BLE_ABS_TYPES_H
/*!=================================================================================================
\file       ble_abs_types.h
\brief      This contains the generic types for the Bluetooth Low Energy Abstraction module(s).
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"

/*==================================================================================================
Public macros
==================================================================================================*/

#define BLE_ADDR_SIZE  6
/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! Bluetooth Device Address - array of 6 bytes. */
typedef uint8_t bleAddr_t[BLE_ADDR_SIZE];

typedef struct bleDataReq_tag
{
    bleAddr_t      dstAddr;
    bool_t      isMCast;
    uint8_t *   pSdu;
    uint16_t    sduLength;
} bleDataReq_t;

typedef struct bleDataInd_tag
{
    instanceId_t        instanceId;
    bleAddr_t              dstAddr;
    bleAddr_t            srcAddr;
    uint8_t *           pSdu;
    uint16_t            sduLength;
} bleDataInd_t;

/*! Structure which holds the BLE requests function pointers */
typedef struct bleAbsRequests_tag
{
    /* BLE Request Functions */
    void (* BLE_DataReq)(bleDataReq_t * pParam, uint32_t upperLayerInstanceId);

    /* Implementation Specific Functions */
    uint32_t     (* GetMaxSduSize)   (bleDataReq_t *pParam);
    void         (* GetBleAddress)   (bleAddr_t aOutAddress);
    bool_t       (* GetPeerBleAddress)   (uint8_t deviceId, bleAddr_t aOutAddress);
} bleAbsRequests_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /* _BLE_ABS_TYPES_H */
