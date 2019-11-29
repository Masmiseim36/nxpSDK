/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */

#include "EmbeddedTypes.h"
#include "MpmInterface.h"
#include "Phy.h"

#include "FunctionLib.h"
#include "MemManager.h"


#if gMpmIncluded_d

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
uint8_t        mRegisteredPANs;
bool_t         mMpmExclusiveAccess;
panInfo_t      mPanInfo[gMpmMaxPANs_c];
panInfo_t     *pActivePANs[gMpmPhyPanRegSets_c];


/*! *********************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
********************************************************************************** */
static void MPM_SetPanSettingsInPhy( uint8_t panIndex );
static uint8_t MPM_AllocateResource( bool_t force, uint8_t panIdx );

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  This function determines the instance of the MAC associated with a PHY regSet
*
* \param[in]  regSet The PHY registry set
*
* \return  The instance of the MAC associated with a PHY registry set.
*
********************************************************************************** */
uint32_t MPM_GetMacInstanceFromRegSet(uint32_t regSet)
{
    if( pActivePANs[regSet] )
        return pActivePANs[regSet]->macInstance;

    return 0;
}

/*! *********************************************************************************
* \brief  This function determines the PHY regSet for the specified panIndex
*
* \param[in]  panIdx The index in mPanInfo[] table
*
* \return  The PHY registry set.
*
********************************************************************************** */
uint32_t MPM_GetRegSet( uint8_t panIdx )
{
    return mPanInfo[panIdx].phyRegSet;
}

/*! *********************************************************************************
* \brief  This function determines the PHY regSet for the specified panIndex
*
* \param[in]  pibId The id of the PHY PIB
* \param[in]  pValue The value of the PHY PIB
* \param[in]  panIdx The index in mPanInfo[] table
*
* \return  The status of the operation.
*
********************************************************************************** */
phyStatus_t MPM_SetPIB(phyPibId_t pibId, void* pValue, uint8_t panIdx)
{
    switch(pibId)
    {
#if gMpmUseDifferentTxPwrLevel_c
    case gPhyPibTransmitPower_c:
        if( *(uint8_t*)pValue > 32 )
        {
            return gPhyInvalidParameter_c;
        }
        mPanInfo[panIdx].pwrLevel = *(uint8_t*)pValue;
        break;
#endif
#if (gMpmMaxPANs_c > gMpmPhyPanRegSets_c)
    case gPhyPibCurrentChannel_c:
        if( (*(uint8_t*)pValue < 11) || (*(uint8_t*)pValue > 26) )
        {
            return gPhyInvalidParameter_c;
        }
        mPanInfo[panIdx].channel = *(uint8_t*)pValue;
        break;
    case gPhyPibLongAddress_c:
        FLib_MemCpy(&mPanInfo[panIdx].longAddr, pValue, sizeof(mPanInfo[panIdx].longAddr));
        break;
    case gPhyPibShortAddress_c:
        FLib_MemCpy(&mPanInfo[panIdx].shortAddr, pValue, sizeof(mPanInfo[panIdx].shortAddr));
        break;
    case gPhyPibPanId_c:
        FLib_MemCpy(&mPanInfo[panIdx].panId, pValue, sizeof(mPanInfo[panIdx].panId));
        break;
    case gPhyPibPanCoordinator_c:
        if( *(bool_t*)pValue )
            mPanInfo[panIdx].flags |= gMpmFlagPanCoord_c;
        else
            mPanInfo[panIdx].flags &= ~gMpmFlagPanCoord_c;
        break;
#endif
    case gPhyPibRxOnWhenIdle:
        if( *(bool_t*)pValue )
            mPanInfo[panIdx].flags |= gMpmFlagRxOnWhenIdle_c;
        else
            mPanInfo[panIdx].flags &= ~gMpmFlagRxOnWhenIdle_c;
        break;
    case gPhyPibPromiscuousMode_c:
        if( *(bool_t*)pValue )
            mPanInfo[panIdx].flags |= gMpmFlagPromiscuous_c;
        else
            mPanInfo[panIdx].flags &= ~gMpmFlagPromiscuous_c;
        break;
    default:
        return gPhyUnsupportedAttribute_c;
    }

    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function determines the PHY regSet for the specified panIndex
*
* \param[in]  pibId  The id of the PHY PIB
* \param[in]  pValue pointer to a location where to store the PIB value
* \param[in]  panIdx The index in mPanInfo[] table
*
* \return  The status of the operation.
*
********************************************************************************** */
phyStatus_t MPM_GetPIB(phyPibId_t pibId, void *pValue, uint8_t panIdx)
{
    switch(pibId)
    {
#if gMpmUseDifferentTxPwrLevel_c
    case gPhyPibTransmitPower_c:
        *(uint8_t*)pValue = mPanInfo[panIdx].pwrLevel;
        break;
#endif
#if (gMpmMaxPANs_c > gMpmPhyPanRegSets_c)
    case gPhyPibCurrentChannel_c:
        *(uint8_t*)pValue = mPanInfo[panIdx].channel;
        break;
    case gPhyPibLongAddress_c:
        FLib_MemCpy(pValue, &mPanInfo[panIdx].longAddr, sizeof(mPanInfo[panIdx].longAddr));
        break;
    case gPhyPibShortAddress_c:
        FLib_MemCpy(pValue, &mPanInfo[panIdx].shortAddr, sizeof(mPanInfo[panIdx].shortAddr));
        break;
    case gPhyPibPanId_c:
        FLib_MemCpy(pValue, &mPanInfo[panIdx].panId, sizeof(mPanInfo[panIdx].panId));
        break;
    case gPhyPibPanCoordinator_c:
        *(uint8_t*)pValue = !!(mPanInfo[panIdx].flags & gMpmFlagPanCoord_c);
        break;
#endif
    case gPhyPibRxOnWhenIdle:
        *(uint8_t*)pValue = !!(mPanInfo[panIdx].flags & gMpmFlagRxOnWhenIdle_c);
        break;
    case gPhyPibPromiscuousMode_c:
        *(uint8_t*)pValue = !!(mPanInfo[panIdx].flags & gMpmFlagPromiscuous_c);
        break;
    default:
        return gPhyUnsupportedAttribute_c;
    }

    return gPhySuccess_c;
}


/*! *********************************************************************************
* \brief  This function initializes the MPM module.
*
* \param[in]  None.
*
* \return  None
*
********************************************************************************** */
void MPM_Init( void )
{
    uint32_t i;

    mRegisteredPANs = 0;
    mMpmExclusiveAccess = FALSE;
    FLib_MemSet( mPanInfo, 0x00, sizeof(mPanInfo) );
    FLib_MemSet( pActivePANs, 0x00, sizeof(pActivePANs) );

    for(i=0; i<gMpmMaxPANs_c; i++)
      mPanInfo[i].phyRegSet = gMpmInvalidRegSet_c;

    PhyPpSetDualPanDwell( ((mDefaultDualPanDwellPrescaler_c << mDualPanDwellPrescalerShift_c) & mDualPanDwellPrescalerMask_c) |
                          ((mDefaultDualPanDwellTime_c       << mDualPanDwellTimeShift_c      ) & mDualPanDwellTimeMask_c) );
}

/*! *********************************************************************************
* \brief  This function prepare the Radio for a TX/CCA/ED operation
*
* \param[in]  macInstance The instance of the MAC
*
* \return  The status of the Operation
*
********************************************************************************** */
phyStatus_t MPM_PrepareForTx( instanceId_t macInstance )
{
    uint8_t panIdx = MPM_GetPanIndex(macInstance);

    if( TRUE == mMpmExclusiveAccess && mPanInfo[panIdx].locked <= 0 )
        return gPhyChannelBusy_c;

    /* Allocate HW Resources if necessary */
    if( mPanInfo[panIdx].phyRegSet == gMpmInvalidRegSet_c )
    {
        if( gMpmInvalidRegSet_c == MPM_AllocateResource( TRUE, panIdx ) )
            return gPhyChannelBusy_c;

        MPM_SetPanSettingsInPhy( panIdx );
    }

    /* Disable DualPan Auto Mode, and select the Active PAN */
    PhyPpSetDualPanAuto( FALSE );
    PhyPpSetDualPanActiveNwk( mPanInfo[panIdx].phyRegSet );
    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function checks if a PAN has the RxOnWhenIdle PIB set.
*         If an Rx needs to be started, it makes the propper settings in PHY.
*
* \param[in]  None.
*
* \return  phyStatus
*
********************************************************************************** */
phyStatus_t MPM_PrepareForRx( instanceId_t macInstance )
{
    uint32_t i, count = 0;
    uint32_t activePan = 0;

    if( gInvalidInstanceId_c != macInstance ) /* Rx */
    {
        i = MPM_GetPanIndex(macInstance);

        if( !mMpmExclusiveAccess || mPanInfo[i].locked )
        {
            /* Allocate HW Resources if necessary */
            if( mPanInfo[i].phyRegSet == gMpmInvalidRegSet_c )
            {
                if( gMpmInvalidRegSet_c == MPM_AllocateResource( TRUE, i ) )
                    return gPhyChannelBusy_c;

                MPM_SetPanSettingsInPhy( i );
            }

            count++;
            activePan = mPanInfo[i].phyRegSet;
        }
    }
    else /* RxOnWhenIdle */
    {
        for( i=0; i<gMpmPhyPanRegSets_c; i++)
        {
            if( (NULL != pActivePANs[i]) &&
                (pActivePANs[i]->flags & gMpmFlagRxOnWhenIdle_c) &&
                ( !mMpmExclusiveAccess || pActivePANs[i]->locked ) )
            {
                activePan = i;
                count++;
            }
        }
    }

    if( !count )
        return gPhyChannelBusy_c;

    /* Set the Active PAN and DualPan Auto mode if needed*/
    PhyPpSetDualPanActiveNwk( activePan );
    PhyPpSetDualPanAuto( count > 1 );
    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function returns the PAN index for a MAC instance
*
* \param[in]  macInstance The instance of the MAC
*
* \return  The PAN index or -1 if it was not found
*
********************************************************************************** */
int32_t  MPM_GetPanIndex( instanceId_t macInstance )
{
    uint32_t i;

    /* Get PAN Index for the macInstance */
    for( i=0; i<mRegisteredPANs; i++ )
    {
        if( mPanInfo[i].macInstance == macInstance )
            return i;
    }

    /* The instance of the MAC is not registered!
     * Register the current MAC instance if there is enough space.
     */
    if( mRegisteredPANs < gMpmMaxPANs_c )
    {
        mPanInfo[mRegisteredPANs].macInstance = macInstance;

        /* Try to allocate HW resource */
        mPanInfo[mRegisteredPANs].phyRegSet = MPM_AllocateResource( FALSE, i );
        mRegisteredPANs++;
    }
    else
    {
        i = -1;
    }

    return i;
}


/*! *********************************************************************************
* \brief  This function configures the MPM module
*
* \param[in]  pCfg pointer to a configuration structure
*
* \remarks
* The Dual PAN dwell time prescaller values: 0-3
* The Dual PAN dwell time values 0-63.
* The (dwellTime+1) represents multiples of the prescaller time base.
*
********************************************************************************** */
void MPM_SetConfig( mpmConfig_t *pCfg )
{
    PhyPpSetDualPanAuto ( FALSE );
    PhyPpSetDualPanDwell( pCfg->dwellTime );
    (void)MPM_AllocateResource( TRUE, MPM_GetPanIndex(pCfg->activeMAC) );
    PhyPpSetDualPanAuto ( pCfg->autoMode );
    if( PhyIsIdleRx(0) )
    {
        PhyPlmeForceTrxOffRequest();
        Radio_Phy_TimeRxTimeoutIndication(0);
    }
}

/*! *********************************************************************************
* \brief  This function returns the MPM configuration
*
* \param[in]  pCfg pointer to a configuration structure
*
* \return  None.
*
********************************************************************************** */
void MPM_GetConfig( mpmConfig_t *pCfg )
{
    pCfg->dwellTime  = PhyPpGetDualPanDwell();
    pCfg->activeMAC  = MPM_GetMacInstanceFromRegSet( PhyPpGetDualPanActiveNwk() );
    pCfg->autoMode   = PhyPpGetDualPanAuto();
}
#endif /* #if gMpmIncluded_d */

/*! *********************************************************************************
* \brief  This function Acquires a PAN for exclusive access.
*
* \param[in]  macInstance The instance of the MAC
*
* \return  The status of the operation
*
********************************************************************************** */
phyStatus_t MPM_AcquirePAN( instanceId_t macInstance )
{
#if gMpmIncluded_d
    int32_t panIndex = MPM_GetPanIndex(macInstance);

    /* Check if another PAN is acquired */
    if( TRUE == mMpmExclusiveAccess &&
        mPanInfo[panIndex].locked <= 0 )
    {
        return gPhyBusy_c;
    }

    if( mPanInfo[panIndex].locked == 0 )
    {
        mMpmExclusiveAccess = TRUE;
        if( PhyIsIdleRx(0) )
        {
            PhyPlmeForceTrxOffRequest();
            Radio_Phy_TimeRxTimeoutIndication(0);
        }
    }

    mPanInfo[panIndex].locked++;
#endif
    return gPhySuccess_c;
}

/*! *********************************************************************************
* \brief  This function Releases a PAN that was Acquired
*
* \param[in]  macInstance The instance of the MAC
*
* \return  The status of the operation
*
********************************************************************************** */
phyStatus_t MPM_ReleasePAN( instanceId_t macInstance )
{
#if gMpmIncluded_d
    uint8_t panIndex = MPM_GetPanIndex(macInstance);

    mPanInfo[panIndex].locked--;
    if( mPanInfo[panIndex].locked == 0 )
    {
        mMpmExclusiveAccess = FALSE;
        if( PhyIsIdleRx(0) )
        {
            PhyPlmeForceTrxOffRequest();
            Radio_Phy_TimeRxTimeoutIndication(0);
        }
    }
#endif
    return gPhySuccess_c;
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
#if gMpmIncluded_d
/*! *********************************************************************************
* \brief  This function alocates a registry set to a PAN.
*
* \param[in]  force If this parameter is TRUE, then another PAN can be preempted.
* \param[in]  panIdx The index into the MPM database
*
* \return  The register set allocated for the specified PAN.
*
********************************************************************************** */
static uint8_t MPM_AllocateResource( bool_t force, uint8_t panIdx )
{
    uint32_t i, regSet = gMpmInvalidRegSet_c;

    if( mPanInfo[panIdx].phyRegSet != gMpmInvalidRegSet_c )
      return mPanInfo[panIdx].phyRegSet;

    for( i=0; i<gMpmPhyPanRegSets_c; i++ )
    {
        if( NULL == pActivePANs[i] )
        {
            regSet = i;
            break;
        }
        else if( (force) &&
                 (pActivePANs[i]->flags == 0) &&
                 (pActivePANs[i]->locked <= 0) )
        {
            regSet = i;
        }
    }

    if( regSet != gMpmInvalidRegSet_c )
    {
        if( NULL != pActivePANs[regSet] )
        {
            pActivePANs[regSet]->phyRegSet = gMpmInvalidRegSet_c;
        }

        pActivePANs[regSet] = &mPanInfo[panIdx];
        pActivePANs[regSet]->phyRegSet = regSet;
    }

    return regSet;
}

/*! *********************************************************************************
* \brief  This function will store PAN settings in PHY.
*
* \param[in]  panIdx The index into the MPM database
*
* \return  None.
*
* \remarks Function assumes that the PAN is active!
*
********************************************************************************** */
static void MPM_SetPanSettingsInPhy( uint8_t panIndex )
{
    panInfo_t *pPAN = &mPanInfo[panIndex];

#if gMpmUseDifferentTxPwrLevel_c
    PhyPlmeSetPIBRequest(gPhyPibTransmitPower_c,   pPAN->pwrLevel,            pPAN->phyRegSet, 0 );
#endif

#if (gMpmMaxPANs_c > gMpmPhyPanRegSets_c)
    PhyPlmeSetPIBRequest(gPhyPibPromiscuousMode_c, !!(pPAN->flags & gMpmFlagPromiscuous_c),  pPAN->phyRegSet, 0 );
    PhyPlmeSetPIBRequest(gPhyPibRxOnWhenIdle,      !!(pPAN->flags & gMpmFlagRxOnWhenIdle_c), pPAN->phyRegSet, 0 );
    PhyPlmeSetPIBRequest(gPhyPibPanCoordinator_c,  !!(pPAN->flags & gMpmFlagPanCoord_c),     pPAN->phyRegSet, 0 );
    PhyPlmeSetPIBRequest(gPhyPibPanId_c,           pPAN->panId,     pPAN->phyRegSet, 0 );
    PhyPlmeSetPIBRequest(gPhyPibShortAddress_c,    pPAN->shortAddr, pPAN->phyRegSet, 0 );
    PhyPlmeSetPIBRequest(gPhyPibLongAddress_c,     pPAN->longAddr,  pPAN->phyRegSet, 0 );
    PhyPlmeSetPIBRequest(gPhyPibCurrentChannel_c,  pPAN->channel,   pPAN->phyRegSet, 0 );
#else
    (void)pPAN;
#endif
}
#endif /* gMpmIncluded_d */