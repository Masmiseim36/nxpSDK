/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _RNDIS_H_
#define _RNDIS_H_  1

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#ifndef gRNDIS_MemPoolId_c
#define gRNDIS_MemPoolId_c  1
#endif
/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum{
   gRndis_Success_c               = 0,
   gRndis_InvalidParameter_c      = 1,
   gRndis_OutOfMemory_c           = 2,
   gRndis_InitError               = 3,
   gRndis_UsbError                = 4,
   gRndis_Timeout                 = 5
   }rndisStatus_t;

typedef enum
{
  rndisMCastMacAddType_IPv4      = 0,
  rndisMCastMacAddType_IPv6      = 1
}rndisMCastMacAddType_t;

typedef void(*pfRNDISRxCallback_t)(void* pPacket, uint16_t length);

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
rndisStatus_t RNDIS_Init();
rndisStatus_t RNDIS_Send(uint8_t* pBuff, uint16_t length);
rndisStatus_t RNDIS_SetDestMacAddress(uint8_t* pBuff);
rndisStatus_t RNDIS_SetRndisMacAddress(uint8_t* pBuff);
rndisStatus_t RNDIS_RegisterMulticastMacAddress(rndisMCastMacAddType_t addType, bool_t write, uint8_t* pBuff);
rndisStatus_t RNDIS_EnableIPv6(bool_t en);
rndisStatus_t RNDIS_EnableIPv4(bool_t en);
rndisStatus_t RNDIS_RegisterRxCallback(pfRNDISRxCallback_t pCallback);

#endif /* _RNDIS_H_ */
