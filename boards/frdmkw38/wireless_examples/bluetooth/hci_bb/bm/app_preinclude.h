/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _APP_PREINCLUDE_H_
#define _APP_PREINCLUDE_H_


/*! *********************************************************************************
 *     Board Configuration
 ********************************************************************************** */
 /* Defines the number of required keys for the keyboard module */
#define gKeyBoardSupported_d    0

#define gKBD_KeysCount_c        0

/* Specifies the number of required LEDs for the LED module */
#define gLEDsOnTargetBoardCnt_c 4

#define  gLEDBlipEnabled_d     0

/*! *********************************************************************************
 * 	App Configuration
 ********************************************************************************** */

/*! *********************************************************************************
 *     Framework Configuration
 ********************************************************************************** */
/* Defines Num of Serial Manager interfaces */
#define gSerialManagerMaxInterfaces_c   1

/* Defines Size for Timer Task*/
#define gTmrTaskStackSize_c  384

/* Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#ifndef GCOV_DO_COVERAGE
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6  _pool_id_(0) _eol_  \
         _block_size_  64  _number_of_blocks_    3  _pool_id_(0) _eol_  \
         _block_size_ 128  _number_of_blocks_   10  _pool_id_(0) _eol_  \
         _block_size_ 512  _number_of_blocks_   40  _pool_id_(0) _eol_
#else /* GCOV_DO_COVERAGE */
#define AppPoolsDetails_c \
         _block_size_  32  _number_of_blocks_    6  _pool_id_(0) _eol_  \
         _block_size_  64  _number_of_blocks_    3  _pool_id_(0) _eol_  \
         _block_size_ 128  _number_of_blocks_    5  _pool_id_(0) _eol_  \
         _block_size_ 512  _number_of_blocks_    5  _pool_id_(0) _eol_
#endif /* GCOV_DO_COVERAGE */

/* Defines number of timers needed by the application */
#define gTmrApplicationTimers_c         0

/* Set this define TRUE if the PIT frequency is an integer number of MHZ */
#define gTMR_PIT_FreqMultipleOfMHZ_d    0

           /*
 * TimerManager Configuration
 */
#define FSL_OSA_BM_TIMER_CONFIG FSL_OSA_BM_TIMER_SYSTICK
#define gTimestamp_Enabled_d                    0
#define gTMR_PIT_Timestamp_Enabled_d 0
#define gTMR_EnableLowPowerTimers_d             1
#define gTMR_EnableMinutesSecondsTimers_d       0

/*! *********************************************************************************
 *     RTOS Configuration
 ********************************************************************************** */
/* Defines number of OS events used */
#define osNumberOfEvents        6

/*! *********************************************************************************
 *     BLE Stack Configuration
 ********************************************************************************** */

#define gUseHciTransportUpward_d    1

/*! *********************************************************************************
 *     BLE LL Configuration
 ***********************************************************************************/
/*  ble_ll_config.h file lists the parameters with their default values. User can override
 *    the parameter here by defining the parameter to a user defined value. */
/* enable periodic advertiser list */
#define gAppExtAdvEnable_d                   1
#define gLlScanPeriodicAdvertiserListSize_c (8U)
/* disable autonomous feature exchange */
#define gL1AutonomousFeatureExchange_d 0

#ifdef GCOV_DO_COVERAGE
#define gLlMaxExtAdvDataLength_c             512U	//255U
#endif /* GCOV_DO_COVERAGE */


/*
 * Specific configuration of LL pools by block size and number of blocks for this application.
 * Optimized using the MEM_OPTIMIZE_BUFFER_POOL feature in MemManager,
 * we find that the most optimized combination for LL buffers.
 *
 * If LlPoolsDetails_c is not defined, default LL buffer configuration in app_preinclude_common.h
 * will be applied.
 */

/* Include common configuration file and board configuration file */
#include "app_preinclude_common.h"
#endif /* _APP_PREINCLUDE_H_ */
