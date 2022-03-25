/*! *********************************************************************************
* \addtogroup BLE
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2021 NXP
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "att_interface.h"
#include "ble_config.h"
#include "gap_types.h"
#include "ModuleInfo.h"
#include "gatt_types.h"


/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
/* WARNING: Do not change these defines */
#define gAttConnStorageSize_c            (6U)
#define gActiveDevicesStorageSize_c      (92U)
#define gL2caLeCbChannelEntrySize_c      (40U)
#define gL2caLePsmEntrySize_c            (4U)

/************************************************************************************
*************************************************************************************
* Public function prototypes
*************************************************************************************
************************************************************************************/
bool_t Ble_CheckMemoryStorage(void);

/************************************************************************************
*************************************************************************************
* Public memory declarations - external references from Host library
*************************************************************************************
************************************************************************************/
extern const uint8_t gcGapMaximumBondedDevices_d;
const uint8_t gcGapMaximumBondedDevices_d = gMaxBondedDevices_c;
extern const uint8_t gcGapMaximumSavedCccds_d;
const uint8_t gcGapMaximumSavedCccds_d = gcGapMaximumSavedCccds_c;
extern bleBondIdentityHeaderBlob_t gaBondIdentityHeaderBlobs[gMaxBondedDevices_c];
bleBondIdentityHeaderBlob_t gaBondIdentityHeaderBlobs[gMaxBondedDevices_c];

extern const uint8_t gGapControllerResolvingListSize;
const uint8_t gGapControllerResolvingListSize = gMaxResolvingListSize_c;
extern gapIdentityInformation_t gaControllerPrivacyIdentities[gMaxResolvingListSize_c];
gapIdentityInformation_t gaControllerPrivacyIdentities[gMaxResolvingListSize_c];
extern gapCarSupport_t mCAR_Support[gMaxResolvingListSize_c];
gapCarSupport_t mCAR_Support[gMaxResolvingListSize_c];

extern const uint8_t gcGattMaxHandleCountForWriteNotifications_c;
const uint8_t gcGattMaxHandleCountForWriteNotifications_c = gMaxWriteNotificationHandles_c;
extern uint16_t gGattWriteNotificationHandles[gMaxWriteNotificationHandles_c];
uint16_t gGattWriteNotificationHandles[gMaxWriteNotificationHandles_c];
extern const uint8_t gcGattMaxHandleCountForReadNotifications_c;
const uint8_t gcGattMaxHandleCountForReadNotifications_c = gMaxReadNotificationHandles_c;
extern uint16_t gGattReadNotificationHandles[gMaxReadNotificationHandles_c];
uint16_t gGattReadNotificationHandles[gMaxReadNotificationHandles_c];

extern const uint8_t gcGapMaxServiceSpecificSecurityRequirements_c;
const uint8_t gcGapMaxServiceSpecificSecurityRequirements_c = gGapMaxServiceSpecificSecurityRequirements_c;
extern gapServiceSecurityRequirements_t gaServiceSecurityRequirements[gGapMaxServiceSpecificSecurityRequirements_c];
gapServiceSecurityRequirements_t gaServiceSecurityRequirements[gGapMaxServiceSpecificSecurityRequirements_c];

extern const uint16_t gcGattServerMtu_c;
const uint16_t gcGattServerMtu_c = gAttMaxMtu_c;

/* Queued Writes server globals, supporting gAppMaxConnections_c clients */
extern const uint8_t gcGattDbMaxPrepareWriteOperationsInQueue_c;
const uint8_t gcGattDbMaxPrepareWriteOperationsInQueue_c = gPrepareWriteQueueSize_c;
extern const uint8_t gcGattDbMaxPrepareWriteClients_c;
const uint8_t gcGattDbMaxPrepareWriteClients_c = gAppMaxConnections_c;
extern uint8_t gaGattDbPrepareWriteQueueIndexes[gAppMaxConnections_c];
uint8_t gaGattDbPrepareWriteQueueIndexes[gAppMaxConnections_c];
extern attPrepareQueueItem *gPrepareWriteQueues[gAppMaxConnections_c][gPrepareWriteQueueSize_c];
attPrepareQueueItem *gPrepareWriteQueues[gAppMaxConnections_c][gPrepareWriteQueueSize_c];

extern const uint16_t gGapDefaultTxOctets;
const uint16_t gGapDefaultTxOctets = gBleDefaultTxOctets_c;
extern const uint16_t gGapDefaultTxTime;
const uint16_t gGapDefaultTxTime = gBleDefaultTxTime_c;

extern const uint16_t gGapHostPrivacyTimeout;
const uint16_t gGapHostPrivacyTimeout = gBleHostPrivacyTimeout_c;
extern const uint16_t gGapControllerPrivacyTimeout;
const uint16_t gGapControllerPrivacyTimeout = gBleControllerPrivacyTimeout_c;
extern const bool_t gGapLeScOobHasMitmProtection;
const bool_t gGapLeScOobHasMitmProtection = gBleLeScOobHasMitmProtection_c;
extern const uint8_t gGapSimultaneousEAChainedReports;
const uint8_t gGapSimultaneousEAChainedReports = gGapSimultaneousEAChainedReports_c;

/* The following definitions are required by the VERSION_TAGS. DO NOT MODIFY or REMOVE */
extern const moduleInfo_t BLE_HOST_VERSION;
#if defined ( __IAR_SYSTEMS_ICC__ )
#pragma required=BLE_HOST_VERSION /* force the linker to keep the symbol in the current compilation unit */
uint8_t ble_dummy; /* symbol suppressed by the linker as it is unused in the compilation unit, but necessary because
                             to avoid warnings related to #pragma required */
#elif defined(__GNUC__)
static const moduleInfo_t *const dummy __attribute__((__used__)) = &BLE_HOST_VERSION;
#endif /* __IAR_SYSTEMS_ICC__ */

/* BLE Host connection storage */
const uint8_t gBleMaxActiveConnections = gAppMaxConnections_c;
extern uint32_t gAttConnStorage[(gAttConnStorageSize_c * gAppMaxConnections_c + 3) / 4];
uint32_t gAttConnStorage[(gAttConnStorageSize_c * gAppMaxConnections_c + 3) / 4];
extern uint32_t gActiveDevicesStorage[(gActiveDevicesStorageSize_c * gAppMaxConnections_c + 3) / 4];
uint32_t gActiveDevicesStorage[(gActiveDevicesStorageSize_c * gAppMaxConnections_c + 3) / 4];
const uint16_t gcConnectionEventMinDefault_c = gGapConnEventLengthMin_d; /*! Time = N * 0.625 ms */
const uint16_t gcConnectionEventMaxDefault_c = gGapConnEventLengthMax_d; /*! Time = N * 0.625 ms */
/* LE credit-based channels storage */
extern const uint8_t gcL2caMaxLePsmSupported;
const uint8_t gcL2caMaxLePsmSupported = gL2caMaxLePsmSupported_c;
extern const uint8_t gcL2caMaxLeCbChannels;
const uint8_t gcL2caMaxLeCbChannels = gL2caMaxLeCbChannels_c;
extern uint32_t gL2caPsmStorage[(gL2caLePsmEntrySize_c * gL2caMaxLePsmSupported_c + 3) / 4];
uint32_t gL2caPsmStorage[(gL2caLePsmEntrySize_c * gL2caMaxLePsmSupported_c + 3) / 4];
extern uint32_t gL2caCbChannelStorage[(gL2caLeCbChannelEntrySize_c * gL2caMaxLeCbChannels_c + 3) / 4];
uint32_t gL2caCbChannelStorage[(gL2caLeCbChannelEntrySize_c * gL2caMaxLeCbChannels_c + 3) / 4];

/* Private structure sizes used for run-time checks */
extern const uint16_t gAttConnEntrySize;
extern const uint16_t gActiveDevicesEntrySize;
extern const uint8_t gL2caLePsmEntrySize;
extern const uint8_t gL2caLeCbChannelEntrySize;

extern const uint8_t gMaxL2caQueueSize;
const uint8_t gMaxL2caQueueSize = gMaxL2caQueueSize_c;
extern const uint8_t gMaxAdvReportQueueSize;
const uint8_t gMaxAdvReportQueueSize = gMaxAdvReportQueueSize_c;

/* Timer interval after which the incomplete accumulated EA report is freed */
extern const uint32_t gFreeEAReportMs;
const uint32_t gFreeEAReportMs = gBleHostFreeEAReportTimeoutMs_c;

/* Service changed indication buffer */
extern gattHandleRange_t gServiceChangedIndicationStorage[gMaxBondedDevices_c];
gattHandleRange_t gServiceChangedIndicationStorage[gMaxBondedDevices_c];

extern const bool_t gAdvSetsUseSameRandomAddr;
const bool_t gAdvSetsUseSameRandomAddr = gBleHostExtAdvUseSameRandomAddr_c;

extern const bool_t gAutoRejectLtkRequestForUnbondedDevices;
const bool_t gAutoRejectLtkRequestForUnbondedDevices = gBleHostAutoRejectLtkRequestForUnbondedDevices_c;
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
bool_t Ble_CheckMemoryStorage(void)
{
    bool_t status = TRUE;

    if ((uint32_t)gAttConnEntrySize * (uint32_t)gAppMaxConnections_c > sizeof(gAttConnStorage))
    {
        status = FALSE;
    }

    if ((uint32_t)gActiveDevicesEntrySize * (uint32_t)gAppMaxConnections_c > sizeof(gActiveDevicesStorage))
    {
        status = FALSE;
    }

    if (gL2caLeCbChannelEntrySize > gL2caLeCbChannelEntrySize_c)
    {
        status = FALSE;
    }

    if (gL2caLePsmEntrySize > gL2caLePsmEntrySize_c)
    {
        status = FALSE;
    }

    return status;
}

/*! *********************************************************************************
* @}
********************************************************************************** */
