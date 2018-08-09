/*
* The Clear BSD License
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






