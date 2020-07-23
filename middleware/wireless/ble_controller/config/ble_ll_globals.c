/*! *********************************************************************************
* \addtogroup BLE
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"
#include "ble_config.h"
#include "ble_ll_config.h"
#include "fsl_common.h" /* for assert support */
#include "MemManager.h"
#include "controller_interface.h"


#if (defined(CPU_MK64FN1M0VLL12) || defined(KW37A4_SERIES) || defined(KW37Z4_SERIES) || defined(KW38A4_SERIES) || defined(KW38Z4_SERIES) || defined(KW39A4_SERIES))
/************************************************************************************
*************************************************************************************
* Private constants & macros
*************************************************************************************
************************************************************************************/
#ifdef gLlNoBle5Feature
    #define gLlCtxtSize                     gLlConnectionCtxtSizeBle4_2
    #define gLlDupFilCtxtSize               gLlDupFilListInfoSizeBle4_2
    #define gLlMaxUsedCmdParamLength_c      gLlCmdBufferSizeBle4_2
#else /* gLlNoBle5Feature */
    #define gLlCtxtSize                     gLlConnectionCtxtSizeBle5
    #define gLlDupFilCtxtSize               gLlDupFilListInfoSizeBle5
    #define gLlMaxUsedCmdParamLength_c      gLlCmdBufferSizeBle5
#endif /* gLlNoBle5Feature */

#if (gLlMaxUsedCmdParamLength_c == 0U)
#error Invalid size for command buffer!
#endif

#define gExtAdvFeatureSupportMask_c (0x1U << 0x4U)
#define gPerAdvFeatureSupportMask_c (0x1U << 0x5U)

#if gLlUsePeriodicAdvertising_d
#define gPerAdvFeatureSupportVal_c (gPerAdvFeatureSupportMask_c)
#else
#define gPerAdvFeatureSupportVal_c (0x0U)
#endif

#if gAppExtAdvEnable_d
#define gExtAdvFeatureSupportVal_c (gExtAdvFeatureSupportMask_c)
#define gMaxUsedEventBufferSize_c   gLlGenEventBufferSizeExtAdv
#else
#define gExtAdvFeatureSupportVal_c (0x0U)
#define gMaxUsedEventBufferSize_c   gLlGenEventBufferSizeNoExtAdv
#endif

/* The number of max TX acl packets in the internal queue can be configured by setting the gLlBufferNbrTxAclPkts define
 in the app_preinclude.h header file. If not set the default value in ble_ll_config.h will be used. */
#define     gLlTotalInternalQueueSize    (gLlQueueSizeWithoutAclQueue+gLlBufferNbrTxAclPkts)

#define     gLlTotalPktQueueSize     (gLlBufferNbrRxAclPkts+gLlBufferNbrSmallSizeEvent_c+gLlBufferNbrMediumSizeEvent_c+gLlBufferNbrLargeSizeEvent_c+gLlBufferGenericSizeEvent_c)

/************************************************************************************
*************************************************************************************
* Public memory declarations - external references from Link Layer library
*************************************************************************************
************************************************************************************/
/* Structure used to pass configuration parameter to LL. A runtime check will be done
 to make sure parameter are within the allowed range of the linked LL library. */
const bleCtrlConfigParam appConfig =
{
    /* Extended advertising related info */
    .ext_adv_max_used_adv_data_length = gLlMaxExtAdvDataLength_c,
    .ext_adv_max_used_set = gLlMaxUsedAdvSet_c,
    .adv_set_ctxt_size_used = gLlAdvSetCtxtSize,
    .max_sync_engine_used = gLlMaxUsedSyncEngine_c,
    .sync_info_ctxt_size_used = gLlSyncInfoCtxtSize,
    .periodic_wl_size_used = gLlScanPeriodicAdvertiserListSize_c,
    .wl_size_used = gLlScanAdvertiserListSize_c,
    .wl_ctxt_size_used = gLlWlCtxtSize,
    .periodic_wl_ctxt_size_used = gLlPeriodicWlCtxtSize,
    /* Connection related info */
    .max_connections_used = gAppMaxConnections_c,
    .connection_ctxt_size_used = gLlCtxtSize,
    /* duplicated filtering info */
    .dup_fil_ctxt_size_used = gLlDupFilCtxtSize,
    .gBleMaxDupFilInfo = gLlScanDuplicateFiltListSize_c,
    /* TX acl packet info */
    .max_configured_ll_tx_acl_pkts = gLlBufferNbrTxAclPkts,
    /* RX acl packet info */
    .max_configured_ll_rx_acl_pkts = gLlBufferNbrRxAclPkts,
    .max_configured_num_event_buffers = gLlTotalPktQueueSize-gLlBufferNbrRxAclPkts,
    /* command buffer related info */
    .max_used_cmd_param_length_bytes = gLlMaxUsedCmdParamLength_c,
    .max_used_generic_event_length_bytes = gMaxUsedEventBufferSize_c,
    /* feature related info */
    .gBle5FeatureMask1_c = gExtAdvFeatureSupportMask_c | gPerAdvFeatureSupportMask_c,
    .gBle5FeatureEnable1_c = gExtAdvFeatureSupportVal_c | gPerAdvFeatureSupportVal_c,
#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
    .ll_mem_pool_id = (SET_SOURCE_ID(gLlMemPoolId_c))
#else /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
    .ll_mem_pool_id = gLlMemPoolId_c
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
};

/*Be carefull, do not change type of below arrays. Arrays are defined as uint32_t because 
of unaligned accesses that may occur with some compiler options/configurations. */
/* Buffers related to extended + periodic adv */
uint32_t    ext_adv_context_buffer[((gLlMaxUsedAdvSet_c*gLlAdvSetCtxtSize)+3U)/4U];
uint32_t    ext_adv_data_buffer[gLlMaxUsedAdvSet_c*((gLlMaxExtAdvDataLength_c+3U)/4U)];
#if gLlUsePeriodicAdvertising_d
uint32_t    ext_per_adv_data_buffer[gLlMaxUsedAdvSet_c*((gLlMaxExtAdvDataLength_c+3U)/4U)];
#else /* gLlUsePeriodicAdvertising_d */
/* periodic advertising buffer is used for legacy scan response data storage */
uint32_t    ext_per_adv_data_buffer[(gcGapMaxAdvertisingDataLength_c+3U)/4U];
#endif /* gLlUsePeriodicAdvertising_d */

/* Extended advertising uses FW white list filtering */
#if gAppExtAdvEnable_d
uint32_t    white_list_buffer[((gLlScanAdvertiserListSize_c*gLlWlCtxtSize)+3U)/4U];
#else /* (gLlScanAdvertiserListSize_c != 0) */
/* Extended advertising is not used: FW white list filtering does not apply. */
uint32_t    white_list_buffer[1];
#endif /* (gLlScanAdvertiserListSize_c != 0) */

#if (gLlScanPeriodicAdvertiserListSize_c != 0)
uint32_t    periodic_adv_list_buffer[((gLlScanPeriodicAdvertiserListSize_c*gLlPeriodicWlCtxtSize)+3U)/4U];
#else /* gLlScanPeriodicAdvertiserListSize_c != 0) */
uint32_t    periodic_adv_list_buffer[1];
#endif /* gLlScanPeriodicAdvertiserListSize_c != 0) */

/* Buffer related to connection */
uint32_t    connection_context_buffer[((gAppMaxConnections_c*gLlCtxtSize)+3U)/4U];
/* Buffer related to duplicate filtering */
uint32_t    dup_fil_context_buffer[((gLlScanDuplicateFiltListSize_c*gLlDupFilCtxtSize)+3U)/4U];
/* Buffer related to command */
uint32_t    g_lec_command_array[(gLlMaxUsedCmdParamLength_c+3U)/4U];
uint32_t    g_lec_generic_event_buffer[(gMaxUsedEventBufferSize_c+3U)/4U];
/* Buffer related to TX acl */
uint32_t    internal_queue[((gLlTotalInternalQueueSize*gLlQueueElmtSize)+3U)/4U];
uint32_t    tx_acl_queue_buffer[gAppMaxConnections_c*gLlBufferNbrTxAclPkts];
/* RX acl packet info */
uint32_t    hci_recvd_data_buffer[gLlBufferNbrRxAclPkts];
uint32_t    packet_queue[((gLlTotalPktQueueSize*gLlHciTdQueueElmtSize)+3U)/4U];
#if (gLlMaxUsedSyncEngine_c != 0)
uint32_t    ll_sync_info_buffer[((gLlMaxUsedSyncEngine_c*gLlSyncInfoCtxtSize)+3U)/4U];
#else /* (gLlMaxUsedSyncEngine_c != 0) */
uint32_t    ll_sync_info_buffer[1];
#endif /* (gLlMaxUsedSyncEngine_c != 0) */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
#endif /* (defined(CPU_MK64FN1M0VLL12) || defined(CPU_MKW38A512VFP4) || defined(CPU_MKW38A512VFT4) || defined(CPU_MKW38A512VHT4)) */

/*! *********************************************************************************
* @}
********************************************************************************** */
