/*! *********************************************************************************
 * \defgroup app
 * @{
 ********************************************************************************** */
/*!
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * \file
 *
 * This file is the app configuration file which is pre included.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _APP_PREINCLUDE_COMMON_H_
#define _APP_PREINCLUDE_COMMON_H_

/*!
 * Common configuration file for All wireless demo application
 * DO NOT EDIT
 * Application specific configuration shall be done in app_preinclude.h
 * Board specific configuration shall be done in board.h
 */

/* Number of bonded devices supported by the application. */
#ifndef gMaxBondedDevices_c
    /* Make sure that (gMaxBondedDevices_c * gBleBondDataSize_c) fits into the Flash area
     * reserved by the application for bond information. */
    #define gMaxBondedDevices_c         gAppMaxConnections_c
#endif /* gMaxBondedDevices_c */

#if (gAppUsePrivacy_d == 0)
#ifndef gMaxResolvingListSize_c
    /* Set minimal value in gMaxResolvingListSize_c */
    #define gMaxResolvingListSize_c     1
#endif /* gMaxResolvingListSize_c */
#endif

#if (gAppUseBonding_d) && (!gAppUsePairing_d)
    #error "Enable pairing to make use of bonding"
#endif

/* Number of devices identified by address to keep track of for Repeated Attempts */
#ifndef gRepeatedAttemptsNoOfDevices_c
    #define gRepeatedAttemptsNoOfDevices_c  (4U)
#endif

/* Minimum timeout after a pairing failure before the same peer can re-attempt it */
#ifndef gRepeatedAttemptsTimeoutMin_c
    #define gRepeatedAttemptsTimeoutMin_c   (10U) /* seconds */
#endif

/* Maximum timeout after a pairing failure before the same peer can re-attempt it */
#ifndef gRepeatedAttemptsTimeoutMax_c
    #define gRepeatedAttemptsTimeoutMax_c   (640U) /* seconds */
#endif
/*! *********************************************************************************
 *   Auto Configuration
 ********************************************************************************** */
/* Disable LEDs when enabling low power */
#if cPWR_UsePowerDownMode || gMWS_UseCoexistence_d
  #define gLEDSupported_d             0
#endif

#if gMWS_UseCoexistence_d
  #undef gKBD_KeysCount_c
  #define gKBD_KeysCount_c            1
#endif


/*! *********************************************************************************
 *   Default Buffer configurations in Memory Manager
 ********************************************************************************** */

/* If the number of connection is not mentioned, set it to 1 as in ble_config.h. The MemManager
 * requires to know the number of connection for LL buffer sizing */
#ifndef gAppMaxConnections_c
#ifndef GCOV_DO_COVERAGE
#define gAppMaxConnections_c           (1U)
#else /* GCOV_DO_COVERAGE */
#define gAppMaxConnections_c           (2U)
#endif /* GCOV_DO_COVERAGE */
#endif

/*! Number of credit-based channels supported */
#ifndef gL2caMaxLeCbChannels_c
#define gL2caMaxLeCbChannels_c         (2U)
#endif

/* Simultaneous EA chained reports.
 * This value must be overwritten by applications that enable BLE 5.0 Observer features */
#ifndef gGapSimultaneousEAChainedReports_c
#define gGapSimultaneousEAChainedReports_c  (0U)
#endif

/* Defines number of timers needed by the protocol stack */
#define gTmrStackTimers_c (3 + (gAppMaxConnections_c * 2) + gL2caMaxLeCbChannels_c + gGapSimultaneousEAChainedReports_c)

/* Default configuration for AppPoolsDetails_c */
#ifndef AppPoolsDetails_c
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6 _eol_  \
         _block_size_  64  _number_of_blocks_    3 _eol_  \
         _block_size_ 128  _number_of_blocks_    4 _eol_  \
         _block_size_ 512  _number_of_blocks_    4 _eol_
#endif /* AppPoolsDetails_c */

/*! *********************************************************************************
 *   NVM Module Configuration - gAppUseNvm_d shall be defined above as 1 or 0
 ********************************************************************************** */

#if gAppUseNvm_d
    /* configure NVM module */
    #define  gNvStorageIncluded_d                (1)
    #define  gNvFragmentation_Enabled_d          (1)
    #define  gNvUseFlexNVM_d                     (0)
    #if gNvUseFlexNVM_d
        #define  gUnmirroredFeatureSet_d         (0)
    #else
        #define  gUnmirroredFeatureSet_d         (1)
    #endif
    #if gNvFragmentation_Enabled_d
        /* Buffer size large enough to accommodate the maximum number of CCCDs for every device. */
        #define  gNvRecordsCopiedBufferSize_c    (gMaxBondedDevices_c * 16)
    #endif
#endif

/*! *********************************************************************************
 *   LlMem - gLlMemPoolId_c shall be defined above as 1
 ********************************************************************************** */
/*! *********************************************************************************
gLlUsePeriodicAdvertising_d:
---------------------------
If set to 1, periodic advertising from AE is enabled. If 0, periodic advertising is disabled.
gAppExtAdvEnable_d:
------------------
If set to 1, advertising extensions (extended + periodic) are enabled. If set to 0, AEs are disabled.
 ********************************************************************************** */
#ifndef gLlMemPoolId_c
/* If define is not set by application, use a common pool for app/host and LL. */
#define gLlMemPoolId_c 0
#else /* gLlMemPoolId_c */
/* Application set the flag, make sure it is valid. */
#if (gLlMemPoolId_c > 1)
#error Please select pool 0 or pool 1
#endif /* (gLlMemPoolId_c > 1) */
#endif /* gLlMemPoolId_c */

#if defined(gLlUsePeriodicAdvertising_d)
    /* check compile switch incompatibilities */
    #if defined(gAppExtAdvEnable_d)
        #if ((gAppExtAdvEnable_d == 0) && (gLlUsePeriodicAdvertising_d == 1))
            #error Compile switch incompatibility! gLlUsePeriodicAdvertising_d=1 shall not be used with gAppExtAdvEnable_d=0
        #endif /* ((gAppExtAdvEnable_d == 0) && (gLlUsePeriodicAdvertising_d == 1)) */
    #else
        /* Periodic advertising support needs extended advertising support. */
        #if (gLlUsePeriodicAdvertising_d == 1)
            #define   gAppExtAdvEnable_d   1
        #endif /* (gLlUsePeriodicAdvertising_d == 1) */
    #endif /*defined(gAppExtAdvEnable_d)*/
#endif /* (defined(gLlUsePeriodicAdvertising_d)) */

#if (defined(gLlScanPeriodicAdvertiserListSize_c) && !defined(gLlScanAdvertiserListSize_c))
    #if  (gLlScanPeriodicAdvertiserListSize_c != 0)
        #define gLlScanAdvertiserListSize_c (26-gLlScanPeriodicAdvertiserListSize_c)
    #endif /* (gLlScanPeriodicAdvertiserListSize_c != 0) */
#endif /* (defined(gLlScanPeriodicAdvertiserListSize_c) && !defined(gLlScanAdvertiserListSize_c)) */

#if (!defined(gAppExtAdvEnable_d))
    #define   gAppExtAdvEnable_d   0
#endif /* (!defined(gAppExtAdvEnable_d)) */

/* Defines L1Mem pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#if (gAppExtAdvEnable_d == 0)
    /*Large size events (<= 72 bytes).*/
    #define gLlBufferNbrLargeSizeEvent_c     (4)       //BT_FW_LE_EVENT_TYPE1_BUFFERS
    /*Medium size events (<= 32 bytes).*/
    #define gLlBufferNbrMediumSizeEvent_c    (4)       //BT_FW_LE_EVENT_TYPE2_BUFFERS
    /*Small size events (<= 12 bytes).*/
    #define gLlBufferNbrSmallSizeEvent_c     (6)       //BT_FW_LE_EVENT_TYPE3_BUFFERS
    /*Generic events (<= 72 bytes).*/
    #define gLlBufferGenericSizeEvent_c      (4)        //BT_FW_LE_EVENT_TYPE4_BUFFERS

    #define gLlCmdBuffer80Bytes_c            (1)

    /*If extended advertising is not set, use legacy settings for advertising*/
    #ifndef gLlMaxUsedAdvSet_c
        #define gLlMaxUsedAdvSet_c               1
    #endif
    #ifndef gLlMaxExtAdvDataLength_c
        #define gLlMaxExtAdvDataLength_c         31
    #endif
    #ifndef gLlUsePeriodicAdvertising_d
        #define gLlUsePeriodicAdvertising_d      0
    #endif

#if (gLlMemPoolId_c == 1)
#ifndef LlPoolsDetails_c
    #define LlPoolsDetails_c \
         _block_size_ 32   _number_of_blocks_    (gLlBufferNbrSmallSizeEvent_c+gLlBufferNbrMediumSizeEvent_c+((3+4)*gAppMaxConnections_c)) _pool_id_(1) _eol_ \
         _block_size_ 64   _number_of_blocks_    ((2*gAppMaxConnections_c)) _pool_id_(1) _eol_ \
         _block_size_ 80  _number_of_blocks_     (gLlBufferNbrLargeSizeEvent_c+gLlCmdBuffer80Bytes_c+gLlBufferGenericSizeEvent_c)  _pool_id_(1) _eol_ \
         _block_size_ 268  _number_of_blocks_    (gLlBufferNbrTxAclPkts+gLlBufferNbrRxAclPkts) _pool_id_(1) _eol_
#endif /* LlPoolsDetails_c */
#else  /* (gLlMemPoolId_c == 1) */
#ifdef LlPoolsDetails_c
#error Single pool is used, please do not define LlPoolsDetails_c in app_preinclude.h
#else
   #define LlPoolsDetails_c
#endif /* LlPoolsDetails_c */
#endif /* (gLlMemPoolId_c == 1) */
#else /* (gAppExtAdvEnable_d == 0) */
    /*Large size events (<= 288 bytes).*/
    #define gLlBufferNbrLargeSizeEvent_c     (10)       //BT_FW_LE_EVENT_TYPE1_BUFFERS
    /*Medium size events (<= 128 bytes).*/
    #define gLlBufferNbrMediumSizeEvent_c    (16)       //BT_FW_LE_EVENT_TYPE2_BUFFERS
    /*Small size events (<= 64 bytes).*/
    #define gLlBufferNbrSmallSizeEvent_c     (12)       //BT_FW_LE_EVENT_TYPE3_BUFFERS
    /*Generic events (<= 72 bytes).*/
    #define gLlBufferGenericSizeEvent_c      (6)        //BT_FW_LE_EVENT_TYPE4_BUFFERS

    #define gLlCmdBuffer288Bytes_c           (1)


/*
 * Default configuration for LlPoolsDetails_c
 *
 * The 128 bytes buffer pool has been changed to a 80 bytes buffer pool: 128 byte buffer for events whose size is > 64 & < 128 is not a good size
 *     because there are three events in this range with size = 72 (HCI_READ_LOCAL_SUPPORTED_COMMANDS & HCI_LE_READ_LOCAL_P256_PUBLIC_KEY) and
 *     size = 74 (HCI_VENDOR_DTM_RX_PKT_EVENT)
 *         => take 80 bytes buffer size instead
 */
#if (gLlMemPoolId_c == 1)
#ifndef LlPoolsDetails_c
    #define LlPoolsDetails_c \
         _block_size_ 32   _number_of_blocks_    ((3+4)*gAppMaxConnections_c) _pool_id_(1) _eol_ \
         _block_size_ 64   _number_of_blocks_    (gLlBufferNbrSmallSizeEvent_c+(2*gAppMaxConnections_c)) _pool_id_(1) _eol_ \
         _block_size_ 80  _number_of_blocks_     (gLlBufferGenericSizeEvent_c)  _pool_id_(1) _eol_ \
         _block_size_ 128  _number_of_blocks_     (gLlBufferNbrMediumSizeEvent_c)  _pool_id_(1) _eol_ \
         _block_size_ 268  _number_of_blocks_    (gLlBufferNbrTxAclPkts+gLlBufferNbrRxAclPkts) _pool_id_(1) _eol_
         _block_size_ 288  _number_of_blocks_    (gLlBufferNbrLargeSizeEvent_c+gLlCmdBuffer288Bytes_c) _pool_id_(1) _eol_
#endif /* LlPoolsDetails_c */
#else /* (gLlMemPoolId_c == 1) */
#ifdef LlPoolsDetails_c
#error Single pool is used, please do not define LlPoolsDetails_c in app_preinclude.h
#else
   #define LlPoolsDetails_c
#endif /* LlPoolsDetails_c */
#endif /* (gLlMemPoolId_c == 1) */
#endif /* (gAppExtAdvEnable_d == 0) */



 /*! *********************************************************************************
 *   Memory Pools Configuration
 ********************************************************************************** */

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.
 * DO NOT MODIFY THIS DIRECTLY. CONFIGURE AppPoolsDetails_c
 * If gMaxBondedDevices_c increases, adjust memory pools
*/
#define PoolsDetails_c \
	 AppPoolsDetails_c \
	 LlPoolsDetails_c


 /*! *********************************************************************************
 *   Xcvr Configuration
 ********************************************************************************** */
/* Define used to select proper timing for BLE LL (for KW38 A0). Define will not
 * be needed anymore for KW38 B0 samples only but for backward compatibility with
 * A0 samples, we need to keep it defined for A0 samples only.
*/
#define gXcvrAddTxOffset_d

/* Radio drivers support both Gen 3.5 and Gen 4.0. */
#ifndef RF_OSC_26MHZ
#define RF_OSC_26MHZ        0
#endif

#endif /* _APP_PREINCLUDE_COMMON_H_ */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
