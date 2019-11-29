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

/*
 * SMAC
*/

#define gSmacUseSecurity_c 1
#define gMinOutputPower_c  (0x00)
#define gMaxOutputPower_c  (0x20)
#define gDefaultOutputPower_c  (0x06)


/* 
 * Connectivity Framework configuration 
 */
#define gFsciIncluded_c                 0
#define gKBD_KeysCount_c                1
#define gLEDsOnTargetBoardCnt_c         2
/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 0
/* Default DCDC Mode used by the application */           
#define APP_DCDC_MODE                   gDCDC_Mode_Buck_c
#define gEepromType_d gEepromDevice_InternalFlash_c
#define PoolsDetails_c                  _block_size_ 64 _number_of_blocks_ 10 _eol_ _block_size_ 128 _number_of_blocks_ 2 _eol_ _block_size_ 256 _number_of_blocks_ 10 _eol_

#endif /* __APP_PREINCLUDE_H__ */