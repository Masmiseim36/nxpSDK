/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*! *********************************************************************************
* \defgroup Mac Filtering Interface
* @{
********************************************************************************** */

#ifndef _MAC_FILTERING_H_
#define _MAC_FILTERING_H_

/*!
\file       mac_filtering.h
\brief      This is a header file for the mac filtering module. 
*/

/*==================================================================================================
Include Files
==================================================================================================*/

/* Network includes */
#include "mac_abs_types.h"
#include "thread_types.h"
/*==================================================================================================
Public macros
==================================================================================================*/

/*==================================================================================================
Public type definitions
==================================================================================================*/
/*! Mac filtering default policy enumeration */
typedef enum
{   
    mMacFilteringDisabled_c,              /*!< Default: Disabled */
    mMacFilteringDefaultPolicyReject_c,   /*!< Default: Reject   */
    mMacFilteringDefaultPolicyAccept_c,   /*!< Default: Accept   */
}macFilteringDefault_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern macFilteringNeighborData_t*  gaMacFilteringTable[];
extern uint32_t gThrMacFilterTlbSize;
/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_UpdateNeighbor(instanceId_t thrInstID, uint64_t extendedAddress,
                                  uint16_t shortAddress)
\brief  This function is used to update an entry in Mac filtering table

\param  [in]    thrInstID       thread instance id
\param  [in]    extendedAddress extended address
\param  [in]    shortAddress    short address

\return thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t MacFiltering_UpdateNeighbor(instanceId_t thrInstID, uint64_t extendedAddress, 
                                                     uint16_t shortAddress);
  
/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_AddNeighbor(instanceId_t thrInstID, uint64_t extendedAddress,
                                  uint16_t shortAddress, uint8_t linkIndicator, bool_t  blockNeighbor)
\brief  This function is used to add an entry in Mac filtering table

\param  [in]    thrInstID        thread instance id
\param  [in]    extendedAddress  extended address
\param  [in]    shortAddress     short address
\param  [in]    linkIndicator    link indicator
\param  [in]    blockNeighbor    boolean. If true - drop received packets from neighbor

\return thrStatus_t  Thread status
***************************************************************************************************/ 
thrStatus_t MacFiltering_AddNeighbor(instanceId_t thrInstID, uint64_t extendedAddress, 
                            uint16_t shortAddress, uint8_t linkIndicator, bool_t  blockNeighbor);
/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_RemoveNeighbor(instanceId_t thrInstID, uint64_t extendedAddress)
\brief  This function is used to remove an entry from Mac filtering table

\param  [in]    thrInstID       thread instance id
\param  [in]    extendedAddress extended address

\return thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t MacFiltering_RemoveNeighbor(instanceId_t thrInstID, uint64_t extendedAddress);
/*!*************************************************************************************************
\fn       bool_t MacFiltering_KeepPacket(instanceId_t thrInstID,  macAbsAddrModeType_t addressMode,
                                         uint64_t address,uint8_t *pLinkIndicator)
\brief    This function returns TRUE if the packet should be keep 

\param  [in]     addressMode     address mode
\param  [in]     address         address 
\param  [in]     pLinkIndicator  pointer to link indicator

\return  bool_t  boolean. If true - keep the packet
***************************************************************************************************/
bool_t MacFiltering_KeepPacket(macAbsAddrModeType_t addressMode, uint64_t address, uint8_t *pLinkIndicator);
/*!*************************************************************************************************
\fn     thrStatus_t MacFiltering_Active(instanceId_t thrInstID, macFilteringDefault_t filteringType)
\brief  This functions verify sets default filtering policy if mac filtering module is active 

\param  [in]    thrInstID          thread instance id
\param  [in]    filteringType      enable/disable filtering table

\return  thrStatus_t  Thread status
***************************************************************************************************/
thrStatus_t MacFiltering_Active(instanceId_t thrInstID, macFilteringDefault_t filteringType) ;
/*!*************************************************************************************************
\fn     macFilteringDefault_t MacFiltering_IsActive(instanceId_t thrInstID)
\brief  This functions returns default filtering policy

\param  [in]    thrInstID       thread instance id

\return  macFilteringDefault_t  Mac filtering default policy
***************************************************************************************************/
macFilteringDefault_t MacFiltering_IsActive(instanceId_t thrInstID);
/*!*************************************************************************************************
\fn     bool_t MacFiltering_Reset()
\brief  Disables and deletes the mac filtering table from NVM.

\return  bool_t   boolean. If true - reset success
***************************************************************************************************/
bool_t MacFiltering_Reset(void);

#ifdef __cplusplus
}
#endif

/*================================================================================================*/

#endif /* _MAC_FILTERING_H_ */
