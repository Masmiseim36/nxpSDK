/*! *********************************************************************************
* \defgroup MacGlobals Mac Globals
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the header file for the MacGlobals.c
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef _MAC_GLOBALS_H_
#define _MAC_GLOBALS_H_

#include "MacInterface.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/*! The maximum number of pending MAC requests. 0 = no limit */
#ifndef gMacInpuQueueLimit_d
#define gMacInpuQueueLimit_d  (10)
#endif

/********************************/
/*** MAC Security Tables sizes **/
/********************************/

/*! The number of keys used by a device. */
#ifndef gNumKeyTableEntries_c
#define gNumKeyTableEntries_c                       2
#endif

/*! Configure one entry for each KeyIdMode used for a specific key. */
#ifndef gNumKeyIdLookupListEntries_c
#define gNumKeyIdLookupListEntries_c                2
#endif

/*! Configure one entry for every device from which secured frames must be received. */
#ifndef gMAC2011_d
    #ifndef gNumKeyDeviceListEntries_c
    #define gNumKeyDeviceListEntries_c              2
    #endif
#else /* gMAC2011_d */
    #ifndef gNumDeviceDescriptorHandleListEntries_c
    #define gNumDeviceDescriptorHandleListEntries_c 2
    #endif
#endif /* gMAC2011_d */

/*! Configure one entry for every secured frame type. */
#ifndef gNumKeyUsageListEntries_c
#define gNumKeyUsageListEntries_c                   2
#endif

/*! Configure one entry for every device from which secured frames must be received. */
#ifndef gNumDeviceTableEntries_c
#define gNumDeviceTableEntries_c                    2
#endif

/*! For MAC internal use only.
 *  Do not change the following macro definition! */
#ifndef gNumDeviceAddrTableEntries_c
    #ifndef gMAC2011_d
        #if (gNumDeviceTableEntries_c > gNumKeyDeviceListEntries_c)
        #define gNumDeviceAddrTableEntries_c (gNumKeyDeviceListEntries_c + 2)
        #else
        #define gNumDeviceAddrTableEntries_c (gNumDeviceTableEntries_c)
        #endif
    #else
        #if (gNumDeviceTableEntries_c > gNumDeviceDescriptorHandleListEntries_c)
        #define gNumDeviceAddrTableEntries_c (gNumDeviceDescriptorHandleListEntries_c + 2)
        #else
        #define gNumDeviceAddrTableEntries_c (gNumDeviceTableEntries_c)
        #endif
    #endif
#endif

/*! Configure one entry for every secured frame type received. */
#ifndef gNumSecurityLevelTableEntries_c
#define gNumSecurityLevelTableEntries_c             2
#endif


/*******************************************/
/*** MAC LE Tables sizes and transactions **/
/***************************************** */

/*! The maximum number of entries in the CSL Synchronization table that specifies 
 *  how many devices can the MAC layer store synchronization details for 
 * (CSL listening period and timestamp of next channel sample). */
#ifndef gMacCslTableSize_c
#define gMacCslTableSize_c          (10)
#endif

/*! This constant defines the maximum number of CSL sequences that can be 
 *  present in the MAC layer when CSL is enabled. */
#ifndef gMacCslMaxSequencesCnt_c
#define gMacCslMaxSequencesCnt_c    (12)
#endif

/*! This constant defines the maximum number of entries in the RIT table 
 *  that specifies how many devices can the MAC layer store listening schedule
 *  for (timestamp of last received RIT Data Request command and listening schedule). */
#ifndef gMacRitTableSize_c
#define gMacRitTableSize_c          (10)
#endif

/*! This constant defines the maximum number of RIT sequences that can be 
 *  present in the MAC layer when RIT is enabled. */
#ifndef gMacRitMaxSequencesCnt_c
#define gMacRitMaxSequencesCnt_c    (12)
#endif


/****************************/
/*** MAC TSCH Tables sizes **/
/****************************/

/*! This constant defines the maximum number of channels that the MAC layer 
 *  can store to hop on when TSCH is enabled. */
#ifndef gMacHoppingSequenceTableEntries_c
#define gMacHoppingSequenceTableEntries_c   (128)
#endif

/*! This constant defines the maximum number of TSCH slotframes that the MAC layer can store. */
#ifndef gMacSlotframeTableEntries_c
#define gMacSlotframeTableEntries_c         (2)
#endif

/*! This constant defines the maximum number of TSCH links that the MAC layer can store. */
#ifndef gMacLinkTableEntries_c
#define gMacLinkTableEntries_c              (10)
#endif

/*! This constant defines the maximum number of TSCH time source neighbors 
 *  that the MAC layer can store when TSCH is enabled. These are configured 
 *  through the MLME-KEEP-ALIVE.Request primitive, and they must have the 
 *  Rx Link with timekeeping option set for the same node address. */
#ifndef gMacTschNeighborTableEntries_c
#define gMacTschNeighborTableEntries_c      (10)
#endif

/*! This constant defines the maximum number of PAN Coordinators that 
 *  the MAC layer can store during the TSCH Passive Scan procedure. 
 *  This information is required for TSCH service enablement following the scan procedure. */
#ifndef gMacTschMaxPanCoordSync_c
#define gMacTschMaxPanCoordSync_c           (1)       
#endif

/*! This constant defines the maximum number of Data Tx sequences 
 *  that can be present in the MAC layer when TSCH is enabled.*/
#ifndef gMacTschMaxTransactions_c
#define gMacTschMaxTransactions_c           (10)       
#endif


/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
/*! \cond DOXY_SKIP_TAG */
extern uint32_t gMacData[gMacInstancesCnt_c][(gMacInternalDataSize_c + 3)/sizeof(uint32_t)];
extern uint8_t gMacMaxIndirectTransactions;
extern uint8_t gMacMaxPendingReq;
extern const uint8_t gMacNoOfInstances;
extern const uint8_t gMacPoolId;
/*! \endcond */

#endif
/*! *********************************************************************************
* @}
********************************************************************************** */