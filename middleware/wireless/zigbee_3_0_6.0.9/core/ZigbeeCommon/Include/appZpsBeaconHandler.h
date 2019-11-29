/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _appZpsBeaconHandler_h_
#define _appZpsBeaconHandler_h_

/*!
\file       appZpsBeaconHandler.h
\brief      Zigbee Becon Handler Module Api
*/

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "jendefs.h"
#include "mac_sap.h"
#include "zps_apl_af.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define BF_BITMAP_BLACKLIST        0x1
#define BF_BITMAP_WHITELIST        0x2
#define BF_BITMAP_LQI              0x4
#define BF_BITMAP_CAP_ENDDEVICE    0x8
#define BF_BITMAP_CAP_ROUTER       0x10
#define BF_BITMAP_PERMIT_JOIN      0x20
#define BF_BITMAP_SHORT_PAN        0x40
#define BF_BITMAP_DEPTH            0x80
#define BF_BITMAP_PRIORITY_PARENT  0x100
#define BF_USED_BITMASK            0x01FF


#define BF_BCN_PL_PROTOCOL_ID(x)       ((x)[0])
#define BF_BCN_PL_STACK_PROFILE(x)     ((x)[1] & 0xf)
#define BF_PL_PROTOCOL_VER(x)          ((x)[1] >> 4)
#define BF_PL_RESERVED_BITS(x)         ((x)[2] & 0x3)
#define BF_BCN_PL_ZR_CAPACITY(x)       (((x)[2] >> 2) & 0x1)
#define BF_BCN_PL_ZED_CAPACITY(x)      (((x)[2] >> 7) & 0x1)
#define BF_BCN_PL_DEVICE_DEPTH(x)      (((x)[2] >> 3) & 0xf)
#define BF_BCN_PL_EXT_PAN_ID_PTR(x)    &((x)[3])
#define BF_ASSOC_PERMIT_BIT             15
#define BF_ASSOC_PERMIT_MASK            ((uint16)(0x1 << BF_ASSOC_PERMIT_BIT))
#define BF_GET_ASSOC_PERMIT(x)          (((x) & BF_ASSOC_PERMIT_MASK) >> BF_ASSOC_PERMIT_BIT)

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/

/* Bit Map  */
/*|    8      |    7      |    6     |      5      |    4        |        3      |      2    |      1        |      0        |*/
/*|   PRP     |    Depth  | Short PAN|  PermitJoin |  cap Router | cap EndDevice |    LQI    |  White List   |   BlackList   |*/
typedef struct
{
    uint64    *pu64ExtendPanIdList;
    uint16    u16Panid;
    uint16    u16FilterMap;
    uint8     u8ListSize;
    uint8     u8Lqi;
    uint8     u8Depth;    
} tsBeaconFilterType;

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void ZPS_bAppAddBeaconFilter(tsBeaconFilterType *psAppBeaconStruct);
PUBLIC void ZPS_bAppRemoveBeaconFilter(void);
PUBLIC void ZPS_bAppDiscoveryReceived(void);
#endif /* _appZpsBeaconHandler_h_ */

/* End of file $RCSfile: appZpsBeaconHandler.h,v $ *******************************************/

