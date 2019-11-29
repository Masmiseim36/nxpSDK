/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/* 
 * KSDK configuration 
 */
#define CPU_MKW41Z512VHT4               1
#define FSL_RTOS_FREE_RTOS              1
#define gTotalHeapSize_c                12000

/* 
 * 802.15.4 PHY configuration 
 */
#define gPhyDefaultTxPowerLevel_d       6

/* 
 * 802.15.4 MAC configuration 
 */
#define gMacFeatureSet_d gMacFeatureSet_06M0_d


/* 
 * Connectivity Framework configuration 
 */
#define gFsciIncluded_c                 0
/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1
/* Default DCDC Mode used by the application */           
#define APP_DCDC_MODE                   gDCDC_Mode_Buck_c 
#define gSerialMgrUseUart_c             1
#define gSerialMgrRxBufSize_c           256
#define gKBD_KeysCount_c                2
#define gLEDsOnTargetBoardCnt_c         4
#define mEnterLowPowerWhenIdle_c        1
#define cPWR_DeepSleepMode              3
#define gNvStorageIncluded_d            1
#define gNvmTestActive_d                1
#define cPWR_UsePowerDownMode           1

#endif /* __APP_PREINCLUDE_H__ */

