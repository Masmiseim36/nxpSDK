/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file contains the OpGroups and OpCodes for the NVM module
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

#ifndef __NV_FSCI_COMMANDS_H__
#define __NV_FSCI_COMMANDS_H__

/*****************************************************************************
 *****************************************************************************
 * Include
 *****************************************************************************
 *****************************************************************************/

#include "EmbeddedTypes.h"
#include "FsciInterface.h"
#include "NVM_Interface.h"

#if gFsciIncluded_c && gNvStorageIncluded_d

/*****************************************************************************
 *****************************************************************************
 * Public macros
 *****************************************************************************
 *****************************************************************************/

/* Operation Groups */
#define gNV_FsciReqOG_d                 (0xA7)
#define gNV_FsciCnfOG_d                 (0xA8)

/* Operation Codes */
#define mFsciMsgNVSaveReq_c             (0xE4) /* Fsci-NVSave.Request.            */
#define mFsciMsgGetNVDataSetDescReq_c   (0xE5) /* Fsci-NVGetDataSetDesc.Request.  */
#define mFsciMsgGetNVCountersReq_c      (0xE6) /* Fsci-NVGetNvCounters.Request.   */
#define mFsciMsgSetNVMonitoringReq_c    (0xE9) /* Fsci-NVSetMonitoring.Request.   */
#define mFsciMsgNVWriteMonitoring_c     (0xEA) /* Fsci-NVWriteMonitoring.          */
#define mFsciMsgNVPageEraseMonitoring_c (0xEB) /* Fsci-NVPageEraseMonitoring.     */
#define mFsciMsgFormatNvmReq_c          (0xEC) /* Fsci-NVFormatReq.Request.       */
#define mFsciMsgRestoreNvmReq_c         (0xED) /* Fsci-NVRestoreReq.Request.      */
#define mFsciMsgRestoreMonitoring_c     (0xEE) /* Fsci-NVRestoreMonitoring.       */
#define mFsciMsgVirtualPageMonitoring_c (0xEF) /* Fsci-NVVirtualPageMonitoring.   */

#define mGetFsciInterfaceFromNvTableEntryId_d (0)

/*****************************************************************************
 *****************************************************************************
 * Public functions
 *****************************************************************************
 *****************************************************************************/
#if gNvmEnableFSCIRequests_c
  void   NV_FsciMsgHandler               (void*, void*, uint32_t);
  bool_t FSCI_MsgNVSaveReqFunc           (void*, uint32_t);
  bool_t FSCI_MsgGetNVDataSetDescReqFunc (void*, uint32_t);
  bool_t FSCI_MsgGetNVCountersReqFunc    (void*, uint32_t);
  bool_t FSCI_MsgSetNVMonitoring         (void*, uint32_t);
  bool_t FSCI_MsgNVFormatReq             (void*, uint32_t);
  bool_t FSCI_MsgNVRestoreReq            (void*, uint32_t);
#endif

#if gNvmEnableFSCIMonitoring_c
  void FSCI_MsgNVPageEraseMonitoring(uint32_t pageAddress, uint8_t status);
  void FSCI_MsgNVWriteMonitoring(uint16_t nvmTableEntryId, uint16_t elementIndex, uint8_t saveRestoreAll);
  void FSCI_MsgNVRestoreMonitoring(uint16_t nvmTableEntryId, bool_t bStart, uint8_t status);
  void FSCI_MsgNVVirtualPageMonitoring(bool_t bStart, uint8_t status);
#endif

#endif //gFsciIncluded_c
  
#endif /* #ifndef __NV_FSCI_COMMANDS_H__ */