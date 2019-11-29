/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
#ifndef BDB_DEVICE_COMMISSIONING_INCLUDED
#define BDB_DEVICE_COMMISSIONING_INCLUDED

/*!
\file       bdb_DeviceCommissioning.h
\brief      BDB Out of Band commissioning API
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "bdb_api.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define BDB_COORDINATOR               ZPS_ZDO_DEVICE_COORD
#define BDB_ROUTER                    ZPS_ZDO_DEVICE_ROUTER
#define BDB_END_DEVICE                ZPS_ZDO_DEVICE_ENDDEVICE
#define BDB_AUTH_DATA_LENGTH          49

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct {
	uint64    u64PanId;	
	uint64    u64TrustCenterAddress;
	uint8*    pu8NwkKey;
	uint8*    pu8InstallCode;  /* initial commissioning key */
	uint16    u16PanId;
	uint16    u16ShortAddress;
	bool_t    bRejoin;
	uint8     u8ActiveKeySqNum;
    uint8     u8DeviceType;
	uint8     u8RxOnWhenIdle;
	uint8     u8Channel;
	uint8     u8NwkUpdateId;
}  BDB_tsOobWriteDataToCommission;

typedef struct {
	uint64    u64ExtAddr;
	uint8*    pu8InstallCode; /* assumes 16 byte install codes only */
}  BDB_tsOobWriteDataToAuthenticate;

typedef struct {
#if defined(__IAR_SYSTEMS_ICC__)
#pragma pack(push, 16)
    uint8     au8Key [ 16 ]; 
#pragma pack(pop)
#else
    uint8     au8Key [ 16 ] __attribute__ ((aligned (16))); 
#endif
	uint64    u64TcAddress;
	uint64    u64PanId;
	uint16    u16ShortPanId;
	uint8     u8ActiveKeySeq;
	uint8     u8Channel;
}  BDB_tsOobReadDataToAuthenticate;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC BDB_teStatus BDB_eOutOfBandCommissionGetDataEncrypted (  BDB_tsOobWriteDataToAuthenticate*     psSrcCredentials,
                                                                uint8*                                pu8ReturnAuthData,
                                                                uint16*                               puSize);
																
PUBLIC uint8 BDB_u8OutOfBandCommissionStartDevice ( BDB_tsOobWriteDataToCommission*    psStartupData );

PUBLIC void BDB_vOutOfBandCommissionGetData ( BDB_tsOobReadDataToAuthenticate*      psReturnedCommissioningData );

PUBLIC bool_t BDB_bOutOfBandCommissionGetKey ( uint8*    pu8InstallCode, 
                                               uint8*    pu8EncKey,
                                               uint64    u64ExtAddress,
                                               uint8*    pu8DecKey,
                                               uint8*    pu8Mic );

PUBLIC void BDB_vOutOfBandCommissionHandleEvent (  BDB_tsZpsAfEvent *psZpsAfEvent );
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


#endif  /* BDB_DEVICE_COMMISSIONING_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/






