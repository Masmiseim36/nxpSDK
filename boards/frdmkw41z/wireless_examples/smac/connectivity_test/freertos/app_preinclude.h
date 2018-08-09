/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __APP_PREINCLUDE_H__
#define __APP_PREINCLUDE_H__

/* 
 * KSDK configuration 
 */
#define CPU_MKW41Z512VHT4               1
#define FSL_RTOS_FREE_RTOS              1
#define gTotalHeapSize_c                8000
/*
 * SMAC
*/

#define gSmacSupported                  1
#define CT_Feature_Xtal_Trim            1

/* 
 * Connectivity Framework configuration 
 */
#define gFsciIncluded_c                 0
#define gAspCapability_d                1
#define gKeyBoardSupported_d            0
#define gLEDsOnTargetBoardCnt_c         4
/* Enables / Disables the DCDC platform component */
#define gDCDC_Enabled_d                 1
/* Default DCDC Mode used by the application */           
#define APP_DCDC_MODE                   gDCDC_Mode_Buck_c
#define gEepromType_d gEepromDevice_InternalFlash_c
#define PoolsDetails_c                  _block_size_ 64 _number_of_blocks_ 10 _eol_ _block_size_ 128 _number_of_blocks_ 2 _eol_ _block_size_ 256 _number_of_blocks_ 10 _eol_

#endif /* __APP_PREINCLUDE_H__ */