/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* DO NOT MODIFY THIS FILE!
*************************************************************************************
************************************************************************************/

#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

/************************************************************************************
*************************************************************************************
* Public macros - Do not modify directly! Override in app_preinclude.h if needed.
*************************************************************************************
************************************************************************************/
/* Number of bonded devices supported by the application.
*  Make sure that (gMaxBondedDevices_c * gBleBondDataSize_c) fits into the Flash area
*  reserved by the application for bond information.
*  Also, adjust the value of gNvRecordsCopiedBufferSize_c to be greater or equal to 
*  the number of elements of the largest NVM data set.
* (ex: gNvRecordsCopiedBufferSize_c >= (gMaxBondedDevices_c * gcGapMaximumSavedCccds_c)) */
#ifndef gMaxBondedDevices_c
#define gMaxBondedDevices_c             8
#endif

/*! Maximum number of entries in the controller resolving list. Adjust based on controller */
#ifndef gMaxResolvingListSize_c
#define gMaxResolvingListSize_c         8
#endif

/*! Maximum number of handles that can be registered for write notifications. */
#ifndef gMaxWriteNotificationHandles_c
#define gMaxWriteNotificationHandles_c        10
#endif

/*! Maximum number of handles that can be registered for read notifications. */
#ifndef gMaxReadNotificationHandles_c
#define gMaxReadNotificationHandles_c        10
#endif

/* Size of prepare write queue. Default value supports a 512-byte attribute transferred on maximum ATT_MTU. */
#ifndef gPrepareWriteQueueSize_c
#define gPrepareWriteQueueSize_c  (gAttMaxValueLength_c / (gAttMaxPrepWriteDataSize_d(gAttMaxMtu_c)) + 1U)
#endif

/* Preferred value for the maximum transmission number of payload octets to be
 * used for new connections.
 *
 * Range 0x001B - 0x00FB
 */
#ifndef gBleDefaultTxOctets_c
#define gBleDefaultTxOctets_c        0x00FB
#endif

/* Preferred value for the maximum packet transmission time to be
 * used for new connections.
 *
 * Range 0x0148 - 0x0848
 */
#ifndef gBleDefaultTxTime_c
#define gBleDefaultTxTime_c        	 0x0848
#endif

/* Timeout for Resolvable Private Address generation in Host
 *
 * Unit: 1 second
 * Range: 1 - 65535
 * Default: 900
 */
#ifndef gBleHostPrivacyTimeout_c
#define gBleHostPrivacyTimeout_c    900
#endif

/* Timeout for Resolvable Private Address generation in Controller
 * (Enhanced Privacy feature - BLE 4.2 only)
 *
 * Unit: 1 second
 * Range: 1 - 41400
 * Default: 900
 */
#ifndef gBleControllerPrivacyTimeout_c
#define gBleControllerPrivacyTimeout_c    900
#endif

/* Flag indicating whether device is set into LE Secure Connections Only Mode.
 * If this flag is overwritten as TRUE, then only LE Secure Connections Pairing is accepted.
 * Default: FALSE
 */
#ifndef gBleLeSecureConnectionsOnlyMode_c
#define gBleLeSecureConnectionsOnlyMode_c    (FALSE)
#endif

/* Flag indicating whether OOB channel used in LE Secure Connections pairing has MITM protection (BLE 4.2 only).
 * Default: FALSE
 */
#ifndef gBleLeScOobHasMitmProtection_c
#define gBleLeScOobHasMitmProtection_c    (FALSE)
#endif

/*! Number of maximum connections supported at application level. Do not modify this
    directly. Redefine it in app_preinclude.h if the application supports multiple
    connections */
#ifndef gAppMaxConnections_c
#define gAppMaxConnections_c             (1U)
#endif


/*! Number of credit-based channels supported */
#ifndef gL2caMaxLeCbChannels_c
#define gL2caMaxLeCbChannels_c           (2U)
#endif

/*! Nmber of LE_PSM supported */
#ifndef gL2caMaxLePsmSupported_c
#define gL2caMaxLePsmSupported_c         (1U)
#endif

/*! Maximum number of pending L2CA packets.
    This queue is used by the L2CAP layer to buffer packets when the LE controller cannot accept ACL Data packets anymore.
    Any new requests sent from the Host or application layer after this queue is full will generate a gBleOverflow_c event.
    Also, when the queue transitions to empty state, a gTxEntryAvailable_c generic event will be generated. */
#ifndef gMaxL2caQueueSize_c
#define gMaxL2caQueueSize_c              (3U)
#endif

#ifndef gMaxAdvReportQueueSize_c
#define gMaxAdvReportQueueSize_c         (30U)
#endif

/* The maximum number of BLE connection supported by platform */
#if defined(CPU_QN9080C)
    #define MAX_PLATFORM_SUPPORTED_CONNECTIONS     16

#elif (defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4) || defined(CPU_MKW36A512VFP4) || defined(CPU_MKW36A512VHT4) || \
       defined(CPU_MKW35Z512VHT4) || defined(CPU_MKW35A512VFP4) || \
       defined(CPU_K32W032S1M2CAx_cm0plus) || defined(CPU_K32W032S1M2VPJ_cm0plus) || \
       defined(CPU_K32W032S1M2CAx_cm4) || defined(CPU_K32W032S1M2VPJ_cm4) || \
       defined(CPU_MKW38A512VFT4) || defined (CPU_MKW38Z512VFT4) || defined(CPU_MKW39A512VFT4) || \
       defined(CPU_MKW37A512VFT4) || defined(CPU_MKW37Z512VFT4))
    #define MAX_PLATFORM_SUPPORTED_CONNECTIONS     8

#elif (defined(CPU_MKW41Z256VHT4) || defined(CPU_MKW41Z512CAT4) || defined(CPU_MKW41Z512VHT4) || \
       defined(CPU_MKW31Z256VHT4) || defined(CPU_MKW31Z512CAT4) || defined(CPU_MKW31Z512VHT4))
    #define MAX_PLATFORM_SUPPORTED_CONNECTIONS     2

#elif defined(CPU_MKW40Z160VHT4) || defined(CPU_MKW30Z160VHM4)
    #define MAX_PLATFORM_SUPPORTED_CONNECTIONS     1

#else
    #warning Undefined platform!
#endif

#if (gAppMaxConnections_c > MAX_PLATFORM_SUPPORTED_CONNECTIONS)
#error The number of connections configured by the application exceeds the number of connection supported by this platform.
#endif

#endif /* BLE_CONFIG_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
