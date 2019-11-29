/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
/*!=============================================================================
\file       bdb_DeviceCommissioning.c
\brief      BDB Out Of Band Commissioning.
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "FunctionLib.h"
#include "jendefs.h"
#include "bdb_api.h"
#include "bdb_ns.h"
#include "bdb_start.h"
#include "dbg.h"
#include "app_pdum_cfg.h"
#include "bdb_DeviceCommissioning.h"
#ifdef CPU_MKW41Z512VHT4
  #include "SecLib.h"
  #include "RNG_Interface.h"
#elif (defined JENNIC_CHIP_FAMILY_JN516x) || (defined JENNIC_CHIP_FAMILY_JN517x)
  #include "AHI_AES.h"
  #include "rnd_pub.h"
#else
  #include "aessw_ccm.h"
  #include "rnd_pub.h"
#endif
#include <string.h>
#include <stdlib.h>
#include "portmacro.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

extern PUBLIC void zps_pvAesGetKeyFromInstallCode(uint8 *pu8installCode,
                    uint16 u16installCodeLength,
                    AESSW_Block_u *puresult);
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: BDB_eOutOfBandCommissionGetDataEncrypted
 *
 * DESCRIPTION: 
 *              
 *
 * PARAMETERS:      
 *
 * RETURNS:
 *
 * EVENTS:   None
 *
 ****************************************************************************/
PUBLIC BDB_teStatus BDB_eOutOfBandCommissionGetDataEncrypted (  BDB_tsOobWriteDataToAuthenticate*     psSrcCredentials,
                                                                uint8*                                pu8ReturnAuthData,
                                                                uint16*                               puSize)
{
    AESSW_Block_u    uNonce;
    AESSW_Block_u    uKey;
    uint8*           pu8MicLocation;
    bool_t           bAesReturn;
    uint8*           pu8Key =  ( uint8* ) ZPS_pvNwkSecGetNetworkKey ( ZPS_pvAplZdoGetNwkHandle ( ) );
    uint64           u64Address =  ZPS_u64NwkNibGetExtAddr ( ZPS_pvAplZdoGetNwkHandle ( ) );
    ZPS_tsNwkNib*    psNib =  ZPS_psAplZdoGetNib( );

    /* Set incoming frame counters to 0 to allow all devices to rejoin into the network */
    FLib_MemSet( psNib->sTbl.pu32InFCSet, 0,
         ( sizeof(uint32) * psNib->sTblSize.u16NtActv ) );
    *puSize = 0;
    FLib_MemCpy ( &pu8ReturnAuthData[ 0 ] ,  &psSrcCredentials->u64ExtAddr, sizeof( uint64) );
    *puSize += 8;
    FLib_MemCpy ( &pu8ReturnAuthData[ *puSize ] ,  pu8Key , 16 );

    uNonce.au8[8] = (uint8) (psSrcCredentials->u64ExtAddr        & 0xFF);
    uNonce.au8[7] = (uint8)((psSrcCredentials->u64ExtAddr >>  8) & 0xFF);
    uNonce.au8[6] = (uint8)((psSrcCredentials->u64ExtAddr >> 16) & 0xFF);
    uNonce.au8[5] = (uint8)((psSrcCredentials->u64ExtAddr >> 24) & 0xFF);
    uNonce.au8[4] = (uint8)((psSrcCredentials->u64ExtAddr >> 32) & 0xFF);
    uNonce.au8[3] = (uint8)((psSrcCredentials->u64ExtAddr >> 40) & 0xFF);
    uNonce.au8[2] = (uint8)((psSrcCredentials->u64ExtAddr >> 48) & 0xFF);
    uNonce.au8[1] = (uint8)((psSrcCredentials->u64ExtAddr >> 56) & 0xFF);

    FLib_MemSet ( &uNonce.au8[9], 0, sizeof(uint8) * 7 );
    *puSize += 16;
    pu8MicLocation = &pu8ReturnAuthData [ *puSize];
    zps_pvAesGetKeyFromInstallCode ( psSrcCredentials->pu8InstallCode , 16, &uKey ) ;
#if (defined LITTLE_ENDIAN_PROCESSOR) && (defined JENNIC_CHIP_FAMILY) && (JENNIC_CHIP_FAMILY == JN517x)
    tsReg128 sKeyRevIn;
    vSwipeEndian(&uKey,&sKeyRevIn,TRUE);
    bAesReturn = bACI_WriteKey(&sKeyRevIn);
#else
    bAesReturn = bACI_WriteKey((tsReg128*)&uKey);
#endif
    if ( bAesReturn )
    {
        vACI_OptimisedCcmStar( TRUE,
                               4,
                               0,
                               16,
                               &uNonce,
                               &pu8ReturnAuthData [ 8 ],
                               &pu8ReturnAuthData [ 8 ],                // overwrite the i/p data
                               pu8MicLocation,                           // append to the o/p data
                               NULL );
    }
    else
    {
        return BDB_E_FAILURE;
    }
    *puSize +=  4;
    FLib_MemCpy ( &pu8ReturnAuthData [ *puSize ] ,
                              &u64Address,
                              sizeof( uint64 ) );
    *puSize +=  8;
    pu8ReturnAuthData [ *puSize ] =  psNib->sPersist.u8ActiveKeySeqNumber;
    *puSize += 1;
    pu8ReturnAuthData [ *puSize ] =  psNib->sPersist.u8VsChannel;
    *puSize += 1;
    FLib_MemCpy ( &pu8ReturnAuthData [ *puSize ] ,
             &psNib->sPersist.u16VsPanId,
             sizeof( uint16 ) );
    *puSize +=  2;
    FLib_MemCpy ( &pu8ReturnAuthData [ *puSize ] ,
             &psNib->sPersist.u64ExtPanId,
             sizeof( uint64 ) );
    *puSize +=  8;
    return BDB_E_SUCCESS;
}

/****************************************************************************
 *
 * NAME: BDB_eOutOfBandCommissionStartDevice
 *
 * DESCRIPTION: 
 *              
 *
 * PARAMETERS:      
 *
 * RETURNS:
 *
 * EVENTS:   None
 *
 ****************************************************************************/																
PUBLIC uint8 BDB_u8OutOfBandCommissionStartDevice ( BDB_tsOobWriteDataToCommission*    psStartupData )
{
    AESSW_Block_u    uKey = { { 0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c,
                             0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x30, 0x39 } };
    uint8            i;

    ZPS_tsNwkNib *psNib =  ZPS_psAplZdoGetNib();

    uint8 u8Status =  BDB_E_FAILURE;

    if (  psStartupData->pu8InstallCode )
    {
         zps_pvAesGetKeyFromInstallCode ( psStartupData->pu8InstallCode , 16, &uKey ) ;
    }
 
    /* Start with a known good state */
    ZPS_vDefaultStack();

    /* set universal configurations first */
    ZPS_vAplSecSetInitialSecurityState( ZPS_ZDO_PRECONFIGURED_LINK_KEY,
                                        uKey.au8,
                                        0x00,
                                        ZPS_APS_GLOBAL_LINK_KEY );

    ZPS_vNwkSetDeviceType ( ZPS_pvAplZdoGetNwkHandle(),
                           ( psStartupData->u8DeviceType + 1 ) );  /* coordinator is 1 - ED is 3 Router is 2*/
    ZPS_vSetZdoDeviceType ( psStartupData->u8DeviceType );     /* coordinator is 0 - ED is 2 and Router is 1 */

    ZPS_vNwkNibSetUpdateId ( ZPS_pvAplZdoGetNwkHandle() ,  psStartupData->u8NwkUpdateId );
    ZPS_eAplAibSetApsChannelMask ( psStartupData->u8Channel );
    ZPS_vNwkNibSetChannel ( ZPS_pvAplZdoGetNwkHandle(), psStartupData->u8Channel );
    ZPS_vNwkNibSetPanId ( ZPS_pvAplZdoGetNwkHandle(), psStartupData->u16PanId );
    ZPS_vNwkNibSetExtPanId( ZPS_pvAplZdoGetNwkHandle(), psStartupData->u64PanId );
    ZPS_eAplAibSetApsUseExtendedPanId ( psStartupData->u64PanId );

    if( psStartupData->u16ShortAddress != 0 && 
        psStartupData->u8DeviceType == ZPS_ZDO_DEVICE_COORD )
    {
        psStartupData->u16ShortAddress = 0;
    }

    if ( psStartupData->u8DeviceType != ZPS_ZDO_DEVICE_COORD && 
         (psStartupData->u16ShortAddress == 0 || psStartupData->u16ShortAddress > 0xFFFA )
       )
    {
        psStartupData->u16ShortAddress = (uint16)RND_u32GetRand( 1, 0xFFF7 ) ;
    }
   
    ZPS_vNwkNibSetNwkAddr ( ZPS_pvAplZdoGetNwkHandle(), psStartupData->u16ShortAddress);
    /* Add Key data into the NIB in first Key entry */
    ZPS_vNwkNibSetKeySeqNum ( ZPS_pvAplZdoGetNwkHandle(), psStartupData->u8ActiveKeySqNum );

    psNib->sTbl.psSecMatSet[0].u8KeySeqNum =  psStartupData->u8ActiveKeySqNum ;
    psNib->sTbl.psSecMatSet[0].u8KeyType =  ZPS_NWK_SEC_NETWORK_KEY;
    if(psStartupData->pu8NwkKey != NULL)
    {
        FLib_MemCpy (psNib->sTbl.psSecMatSet[0].au8Key, psStartupData->pu8NwkKey, ZPS_SEC_KEY_LENGTH);
    }
    else
    {
        /* If not Generate a Network Key */
        for ( i = 0; i < ZPS_SEC_KEY_LENGTH; i++ )
        {
            psNib->sTbl.psSecMatSet[0].au8Key[i] =  (uint8)(RND_u32GetRand256() & 0xFF);
        }

    }     
    FLib_MemSet( psNib->sTbl.pu32InFCSet, 0,
         ( sizeof(uint32) * psNib->sTblSize.u16NtActv ) );  

    /* Set trust centre address */
    if ( psStartupData->u64TrustCenterAddress  != ZPS_u64AplZdoGetIeeeAddr() &&
         psStartupData->u8DeviceType == ZPS_ZDO_DEVICE_COORD )
    {
        psStartupData->u64TrustCenterAddress = ZPS_u64AplZdoGetIeeeAddr();
    }
    ZPS_eAplAibSetApsTrustCenterAddress ( psStartupData->u64TrustCenterAddress );	

    ZPS_vSaveAllZpsRecords();


    if ( psStartupData->u8DeviceType == ZPS_ZDO_DEVICE_ENDDEVICE ) 
    {
        if ( psStartupData->u8RxOnWhenIdle )
        {
            ZPS_vAplAfSetMacCapability(0x88); /* Allocate address and Rx On When Idle is set */
        }
        psStartupData->bRejoin = TRUE;
    }

    /* Does this device need a rejoin - coordinator is just another router with an address of 0*/
    if ( !psStartupData->bRejoin ||
        ( psStartupData->u8DeviceType == ZPS_ZDO_DEVICE_COORD ))
    {
        BDB_tsBdbEvent sBdbEvent;
        ZPS_vSetNwkStateActive ( ZPS_pvAplZdoGetNwkHandle() );
        u8Status =  ZPS_eAplZdoStartRouter (  ( psStartupData->u8DeviceType != ZPS_ZDO_DEVICE_COORD ) );
        sBDB.sAttrib.bbdbNodeIsOnANetwork = TRUE;
        sBdbEvent.eEventType = BDB_EVENT_OOB_FORM_SUCCESS;
        APP_vBdbCallback(&sBdbEvent);

    }
    else
    {
       sBDB.sAttrib.bbdbNodeIsOnANetwork = FALSE;
       u8Status =  ZPS_eAplZdoRejoinNetwork ( TRUE );
       if( u8Status == ZPS_NWK_NLME_CFM_DEFERRED )
       {
           sBDB.sAttrib.bOutBandRejoinInprogress =  TRUE;
       }
    }
    return u8Status;
}

/****************************************************************************
 *
 * NAME: BDB_eOutOfBandCommissionGetData
 *
 * DESCRIPTION: 
 *              
 *
 * PARAMETERS:      
 *
 * RETURNS:
 *
 * EVENTS:   None
 *
 ****************************************************************************/
PUBLIC void BDB_vOutOfBandCommissionGetData ( BDB_tsOobReadDataToAuthenticate*      psReturnedCommissioningData )
{
    ZPS_tsNwkNib     *psNib =  ZPS_psAplZdoGetNib();
    uint8*           pu8Key =  ( uint8* ) ZPS_pvNwkSecGetNetworkKey ( ZPS_pvAplZdoGetNwkHandle ( ) );

    FLib_MemCpy ( psReturnedCommissioningData->au8Key , pu8Key, 16 );
    /* Set incoming frame counters to 0 to allow all devices to rejoin into the network */
    FLib_MemSet( psNib->sTbl.pu32InFCSet, 0,
         ( sizeof(uint32) * psNib->sTblSize.u16NtActv ) );
    psReturnedCommissioningData->u64TcAddress   =  ZPS_u64NwkNibGetExtAddr ( ZPS_pvAplZdoGetNwkHandle ( ) );
    psReturnedCommissioningData->u8ActiveKeySeq =  psNib->sPersist.u8ActiveKeySeqNumber;
    psReturnedCommissioningData->u8Channel      =  psNib->sPersist.u8VsChannel;
    psReturnedCommissioningData->u16ShortPanId  =  psNib->sPersist.u16VsPanId;
    psReturnedCommissioningData->u64PanId       =  psNib->sPersist.u64ExtPanId;

}

/****************************************************************************
 *
 * NAME: BDB_bOutOfBandCommissionGetKey
 *
 * DESCRIPTION: 
 *              
 *
 * PARAMETERS:      
 *
 * RETURNS:
 *
 * EVENTS:   None
 *
 ****************************************************************************/
PUBLIC bool_t BDB_bOutOfBandCommissionGetKey ( uint8*    pu8InstallCode, 
                                               uint8*    pu8EncKey,
                                               uint64    u64ExtAddress,
                                               uint8*    pu8DecKey,
                                               uint8*    pu8Mic )
{
    bool_t           bReturn;
    AESSW_Block_u    uNonce;
    AESSW_Block_u    uKey;

    /* Copy encrypted data into decryption buffer for in-situ decryption */
    FLib_MemCpy(pu8DecKey, pu8EncKey, 16);
    zps_pvAesGetKeyFromInstallCode ( pu8InstallCode , 16, &uKey ) ;
    /* Zero nonce then copy in data */
    FLib_MemSet (uNonce.au8, 0, sizeof(uNonce.au8));
    uNonce.au8[8] = (uint8) (u64ExtAddress        & 0xFF);
    uNonce.au8[7] = (uint8)((u64ExtAddress >>  8) & 0xFF);
    uNonce.au8[6] = (uint8)((u64ExtAddress >> 16) & 0xFF);
    uNonce.au8[5] = (uint8)((u64ExtAddress >> 24) & 0xFF);
    uNonce.au8[4] = (uint8)((u64ExtAddress >> 32) & 0xFF);
    uNonce.au8[3] = (uint8)((u64ExtAddress >> 40) & 0xFF);
    uNonce.au8[2] = (uint8)((u64ExtAddress >> 48) & 0xFF);
    uNonce.au8[1] = (uint8)((u64ExtAddress >> 56) & 0xFF);
#if (defined LITTLE_ENDIAN_PROCESSOR) && (defined JENNIC_CHIP_FAMILY) && (JENNIC_CHIP_FAMILY == JN517x)
    tsReg128 sKeyRevIn;
    vSwipeEndian(&uKey,&sKeyRevIn,TRUE);
    bReturn = bACI_WriteKey(&sKeyRevIn);
#else
    bReturn = bACI_WriteKey((tsReg128*)&uKey);
#endif
    if(bReturn)
    {
        vACI_OptimisedCcmStar(
            FALSE, /* TRUE=Encrypt / FALSE=Decrypt */
            4, /* Required number of checksum bytes */
            0, /* Length of authentication data in bytes */
            16, /* Length of input data in bytes */
            &uNonce, /* A pointer to the 128bit nonce data */
            NULL, /* Authentication data */
            pu8DecKey, /* Input and output data */
            pu8Mic, /* Checksum (MIC) */
            &bReturn); /* Authenticated flag */
        return bReturn;
    }
    else
    {
        return bReturn;
    }
}

/****************************************************************************
 *
 * NAME: BDB_vOutOfBandCommissionHandleEvent
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 *
 * RETURNS:
 *
 * EVENTS:   None
 *
 ****************************************************************************/
PUBLIC void BDB_vOutOfBandCommissionHandleEvent (  BDB_tsZpsAfEvent *psZpsAfEvent )
{
    BDB_tsBdbEvent sBdbEvent;
    if(sBDB.sAttrib.bOutBandRejoinInprogress)
    {
        if ( psZpsAfEvent->sStackEvent.eType ==  ZPS_EVENT_NWK_JOINED_AS_ROUTER || 
             psZpsAfEvent->sStackEvent.eType ==  ZPS_EVENT_NWK_JOINED_AS_ENDDEVICE )
        {
            sBDB.sAttrib.bbdbNodeIsOnANetwork = TRUE;
            sBDB.sAttrib.bOutBandRejoinInprogress =  FALSE;
            sBdbEvent.eEventType = BDB_EVENT_OOB_JOIN_SUCCESS;
            APP_vBdbCallback(&sBdbEvent);
        }

        if ( psZpsAfEvent->sStackEvent.eType == ZPS_EVENT_NWK_FAILED_TO_JOIN )
        {
            sBDB.sAttrib.bbdbNodeIsOnANetwork = FALSE;
            sBDB.sAttrib.bOutBandRejoinInprogress =  FALSE;
            sBdbEvent.eEventType = BDB_EVENT_OOB_FAIL;
            APP_vBdbCallback(&sBdbEvent);
        }
    }
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
