/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
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
