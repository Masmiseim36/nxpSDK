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

#ifndef _NV_DATA_H_
#define _NV_DATA_H_

/*!
\file       nv_data.h
\brief      This is the header file for the  stack non volatile data module.
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "NVM_Interface.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/* Unique Nvm Data  Id's */
/* WARNING WARNING  WARNING  WARNING  WARNING  WARNING  WARNING  WARNING*/
/* For instantiable stacks  the most significant nibble is used for pan Id */

/*PAN0*/
                                                //0x0000
#define nvmId_ContextTable_c                    0x0001
#define nvmId_Dhcp6ClientParams_c               0x0002
#define nvmId_mDhcp6ServerBindingTbl_c          0x0003
#define nvmId_ThrNeighborsTbl_c                 0x0004
                                                //0x0005
#define nvmId_GlobalAddrTable6_c                0x0006
#define nvmId_mMplInstanceSet_c                 0x0007
#define nvmId_mMplSequenceNumber                0x0008
                                                //0x0009
#define nvmId_ip6RoutingTblEntry_c              0x000A
#define nvmId_macFilteringTable_c               0x000B
#define nvmId_ndCfgData_c                       0x000C
#define nvmId_ndPrefixList_c                    0x000D
#define nvmId_Dhcp6ServerCfg_c                  0x000E
#define nvmId_MulticastAddrTable_c              0x000F
#define nvmId_ThreadAttr_c                      0x0010
#define nvmId_EventsTbl_c                       0x0011
#define nvmId_MleOutgoingSecFrameCounter_c      0x0012
#define nvmId_macFilteringPolicy_c              0x0013
#define nvmId_MacOutgoingSecFrameCounter_c      0x0014
#define nvmId_ThrChildVersionTbl_c              0x0015
#define nvmId_ThrChildAddrRegTbl_c              0x0016
#define nvmId_ThrIdAssignTbl_c                  0x0017
#define nvmId_ThreadStringAttr_c                0x0018
                                                //0x0019
#define nvmId_Dhcp4ClientParams_c               0x001A
#define nvmId_GlobalAddrTable4_c                0x001B
                                                //0x001C
#define nvmId_ThrSlaacTempAddrTbl_c             0x001D

#define nvmId_ThreadActiveAttr_c                0x001E
#define nvmId_ThreadPendingAttr_c               0x001F

#define nvmId_mDhcp4ServerBindingTbl_c          0x0020

#define nvmId_ThreadMcastAddrRegTbl_c           0x0020
#define nvmId_Ip6FirewallTbl_c                  0x0021

#define nvmId_ServerDataPrefixAttr_c            0x0022
#define nvmId_ServerDataPrefixLenAttr_c         0x0023
#define nvmId_ServerDataBrSetAttr_c             0x0024
#define nvmId_ServerDataExtRouteSetAttr_c       0x0025
#define nvmId_ServerDataServiceSetAttr_c        0x0026
/*==================================================================================================
Public type definitions
==================================================================================================*/

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

#endif /* _NV_DATA_H_ */
