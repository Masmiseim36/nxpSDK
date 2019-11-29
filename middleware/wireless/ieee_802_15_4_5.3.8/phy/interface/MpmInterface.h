/*! *********************************************************************************
* \defgroup MpmInterface MPM Interface
* The Multi-PAN Manager (MPM) module is used to enable multiple MAC instances to access a single PHY.
* Most of the functions below are used internally by the PHY layer, so there is no need for a direct call.
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is a header file for the Multiple PAN Manager.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef __MPM_H__
#define __MPM_H__

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "PhyInterface.h"


/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/*! The maximun number of MAC instances that can be registered. 
    If set to 0, the MPM is disabled! */
#if !defined(gMpmMaxPANs_c)
#define gMpmMaxPANs_c                    (1)
#endif

/*! This define illustrates the state of the MPM functionality. It must not be modified directly. */
#define gMpmIncluded_d                   (gMpmMaxPANs_c > 1)

/*! Defines the number of hardware register sets that are used to store PAN-specific information (Hardware specific).*/
#define gMpmPhyPanRegSets_c              (2)

/*! Enable / disable different Tx power levels on every PAN. If enabled, the MPM will set the power level before the first Tx on a PAN.*/
#ifndef gMpmUseDifferentTxPwrLevel_c
#define gMpmUseDifferentTxPwrLevel_c     (0)
#endif

/*! Dual Pan Dwell Prescaler (0 - 3)

  value | timebase [ms]
  ------|--------------
     0  | 0.5
     1  | 2.5
     2  | 10
     3  | 50
*/
#ifndef mDefaultDualPanDwellPrescaler_c
#define mDefaultDualPanDwellPrescaler_c  (0x00) /* 0,5 ms */
#endif

/*! Dual Pan Dwell Timer (0 - 63).
    PAN switch time = (mDefaultDualPanDwellTime_c + 1) * mDefaultDualPanDwellPrescaler_c

  Prescaler timebase [ms] |  Timer range [ms]
  -------------------------|------------------
             0.5           |  0.5 - 32
             2.5           |  2.5 - 160
             10            |   10 - 640
             50            |   50 - 3200
*/
#ifndef mDefaultDualPanDwellTime_c
#define mDefaultDualPanDwellTime_c       (0x06)
#endif

/*! \cond DOXY_SKIP_TAG */
#ifndef gMpmAcquireIsBlocking_d
#define gMpmAcquireIsBlocking_d          (0)
#endif

#define gMpmInvalidRegSet_c (gMpmPhyPanRegSets_c)

#define mDualPanDwellPrescalerMask_c     (0x03)
#define mDualPanDwellPrescalerShift_c    (0)
#define mDualPanDwellTimeMask_c          (0xFC)
#define mDualPanDwellTimeShift_c         (2)


#if (gMpmMaxPANs_c > gMpmPhyPanRegSets_c)
    #error The number of PANs exceeds the number of HW registry sets! This feature is not supported yet.
#endif
/*! \endcond */

/*! MPM Flag: device is PAN Coordinator */
#define gMpmFlagPanCoord_c               (1 << 0)
/*! MPM Flag: device is in promiscuous RX mode */
#define gMpmFlagPromiscuous_c            (1 << 1)
/*! MPM Flag: device has RX on when idle enabled */
#define gMpmFlagRxOnWhenIdle_c           (1 << 2)

/*! This define is used to determine, whether the PAN specified as parameter is the active one. */
#define  MPM_isPanActive( panIdx ) (MPM_GetRegSet(panIdx) != gMpmInvalidRegSet_c)

/*! MPM helper macro. It is used to set the value of the dwellTime field of the mpmConfig_t */
#define mDualPanDwellTimerSetting(value, prescaler) ((((value) << (mDualPanDwellTimeShift_c)) & mDualPanDwellTimeMask_c) | \
                                                     (((prescaler) << (mDualPanDwellPrescalerShift_c)) & mDualPanDwellPrescalerMask_c))


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! MPM Configuration */
typedef PACKED_STRUCT mpmConfig_tag{
    bool_t  autoMode;  /*!< Enable / disable automatic PAN switch */
    uint8_t dwellTime; /*!< PAN switch dwell time: prescaler bits [1:0]; timer bits [7:2] */
    uint8_t activeMAC; /*!< Instance of the active MAC layer */
}mpmConfig_t;

/*! PAN information*/
typedef struct panInfo_tag{
    uint8_t        flags;       /*!< The state of relevant MAC PIBs */
    uint8_t        macInstance; /*!< Instance of the upper layer (MAC) */
    uint8_t        phyRegSet;   /*!< Id of the PAN register set */
    int8_t         locked;      /*!< PAN switch lock */
#if (gMpmMaxPANs_c > gMpmPhyPanRegSets_c)
    uint64_t       longAddr;    /*!< Device's Extended address */
    uint16_t       shortAddr;   /*!< Device's Short Address */
    uint16_t       panId;       /*!< Device's PAN Id value */
    uint8_t        channel;     /*!< PAN logical channel */
#endif
#if gMpmUseDifferentTxPwrLevel_c
    uint8_t        pwrLevel;    /*!< PAN TX Power Level */
#endif
}panInfo_t;

#ifdef __cplusplus
extern "C" {
#endif 

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#if gMpmIncluded_d
/*! *********************************************************************************
*   This function initializes the MPM module. 
*   This function is called at the initialization of the PHY layer so 
*   there is no need for an explicit call.
*
********************************************************************************** */
void MPM_Init( void );

/*! *********************************************************************************
*   This function is used by other layers to change the current MPM configuration. 
*   Read the MPM configuration first, modify it, and then write the updated configuration.
*
* \param[in]  pCfg  Pointer to the MPM config structure
*
********************************************************************************** */
void MPM_SetConfig( mpmConfig_t *pCfg );

/*! *********************************************************************************
*   This function is used by other layers to read the current MPM configuration.
*
* \param[in]  pCfg  Pointer to the MPM config structure
*
********************************************************************************** */
void MPM_GetConfig( mpmConfig_t *pCfg );

/*! *********************************************************************************
*   This function is used by the PHY to determine the MPM PAN index associated 
*   to a MAC instance.
*
* \param[in]  macInstance  Instance of the MAC
*
* \return PAN index if found, else -1
*
********************************************************************************** */
int32_t  MPM_GetPanIndex( instanceId_t macInstance );

/*! *********************************************************************************
*   This function is used by the PHY to determine the PAN register set associated 
*   to an MPM PAN index.
*
* \param[in]  panIdx  The MPM PAN index.
*
* \return The PAN register set.
*
********************************************************************************** */
uint32_t MPM_GetRegSet(uint8_t panIdx);

/*! *********************************************************************************
*   This function is used by the PHY to determine the MAC instance associated with 
*   the specified PAN register set.
*
* \param[in]  regSet  The PHY PAN register set.
*
* \return The instance of the MAC.
*
********************************************************************************** */
uint32_t MPM_GetMacInstanceFromRegSet(uint32_t regSet);

/*! *********************************************************************************
*   This function is called by the PHY layer before a new TX is initiated.
*
* \param[in]  macInstance  Instance of the MAC
*
* \return phyStatus_t
*
********************************************************************************** */
phyStatus_t MPM_PrepareForTx( instanceId_t macInstance );

/*! *********************************************************************************
*   This function is called by the PHY layer before a new RX is initiated.
*
* \param[in]  macInstance  Instance of the MAC
*
* \return phyStatus_t
*
********************************************************************************** */
phyStatus_t MPM_PrepareForRx( instanceId_t macInstance );

/*! *********************************************************************************
*   This function is called by the PHY layer when the value of a PIB is requested 
*   by the upper layer..
*
* \param[in]  pibId  The Id of the PIB.
* \param[in]  pValue Pointer to the PIB value.
* \param[in]  panIdx The index of the PAN on which the change occurred.
*
* \return phyStatus_t
*
********************************************************************************** */
phyStatus_t MPM_GetPIB(phyPibId_t pibId, void *pValue, uint8_t panIdx);

/*! *********************************************************************************
*   This function is called by the PHY layer when a PIB value is changed, 
*   to inform the MPM of the new PIB value.
*
* \param[in]  pibId  The Id of the PIB.
* \param[in]  pValue Pointer to the PIB value.
* \param[in]  panIdx The index of the PAN on which the change occurred.
*
* \return phyStatus_t
*
********************************************************************************** */
phyStatus_t MPM_SetPIB(phyPibId_t pibId, void *pValue, uint8_t panIdx);

#else /* #if gMpmIncluded_d */

#define MPM_Init()
#define MPM_SetConfig( pCfg )

#define MPM_GetPanIndex( macInstance )          0
#define MPM_GetRegSet( panIdx )                 0
#define MPM_GetMacInstanceFromRegSet( regSet )  0

#define MPM_PrepareForTx( macInstance )        gPhySuccess_c
#define MPM_PrepareForRx( macInstance )        gPhySuccess_c
#define MPM_GetPIB( pibId, pibValue, panIdx )  gPhySuccess_c
#define MPM_SetPIB( pibId, pibValue, panIdx )  gPhySuccess_c

#endif /* #if gMpmIncluded_d */

/*! *********************************************************************************
*   This function is used to request exclusive access to the PHY.
*
* \param[in]  macInstance  Instance of the MAC
*
* \return phyStatus_t
*
********************************************************************************** */
phyStatus_t MPM_AcquirePAN( instanceId_t macInstance );

/*! *********************************************************************************
*   This function is used to release exclusive access to the PHY.
*
* \param[in]  macInstance  Instance of the MAC
*
* \return phyStatus_t
*
********************************************************************************** */
phyStatus_t MPM_ReleasePAN( instanceId_t macInstance );

#ifdef __cplusplus
}
#endif 

#endif /*__MPM_H__ */
/*! *********************************************************************************
* @}
********************************************************************************** */