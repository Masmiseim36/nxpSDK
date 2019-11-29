/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the app configuration file which is pre included.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/* 
 * KSDK configuration 
 */
#define CPU_MKW41Z512VHT4               1


/* 
 * 802.15.4 PHY configuration 
 */
#define gPhyDefaultTxPowerLevel_d       6


/* 
 * 802.15.4 MAC configuration 
 */
#define gMacFeatureSet_d gMacFeatureSet_06BEGTSM0_d


/* 
 * Connectivity Framework configuration 
 */
#define gFsciIncluded_c                 1
#define gFSCI_IncludeMacCommands_c      1
#define gFSCI_IncludeLpmCommands_c      0
#define gUpgradeImageOnCurrentDevice_d  0
/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1
/* Default DCDC Mode used by the application */           
#define APP_DCDC_MODE                   gDCDC_Mode_Buck_c 
/* Enable the LowPower module */
#define cPWR_UsePowerDownMode           1
#define cPWR_DeepSleepMode              3
#define gSerialMgr_DisallowMcuSleep_d   1
#define gSerialMgrRxBufSize_c           256
#define gKBD_KeysCount_c                2
#define gLEDsOnTargetBoardCnt_c         4
#define gAspCapability_d                1

#define PoolsDetails_c \
         _block_size_  64  _number_of_blocks_    16 _pool_id_(0) _eol_  \
         _block_size_ 128  _number_of_blocks_    8 _pool_id_(0) _eol_  \
         _block_size_ 256  _number_of_blocks_    16 _pool_id_(0) _eol_

#endif /* __APP_PREINCLUDE_H__ */