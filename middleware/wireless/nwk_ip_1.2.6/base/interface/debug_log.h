/*
* The Clear BSD License
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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
#ifndef _DEBUG_LOG_H
#define _DEBUG_LOG_H
/*!=================================================================================================
\file       debug_log.h
\brief      This is the header file for the module to print debug messages and activate debug features.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include <stddef.h>
#include <stdarg.h>
/*==================================================================================================
Public macros
==================================================================================================*/
#if DEBUG_LOG
#if THR_ENABLE_MGMT_DIAGNOSTICS
/* Use this define to count specific errors returned by the MAC layer and reported through the
 * Thread Management Diagnostics module */
#ifndef DEBUG_MAC_ERRORS
    //#define DEBUG_MAC_ERRORS
#endif
#endif
/*==================================================================================================
Public type definitions
==================================================================================================*/
typedef enum
{
  DBG_MEM_TRACKING_FREE_c = 0,
  DBG_MEM_TRACKING_ALLOC_c,
}dbgMemTrackingStatus_t;

typedef PACKED_STRUCT dbgBlockTracking_tag
{
  void *blockAddr;                  /*Addr of Msg, not that this pointer is
                                     4 byte bigger than the addr in the pool
                                     has the header of the msg is 4 bytes */
  uint16_t blockSize;               /*Size of block in bytes.*/
  uint16_t fragmentWaste;           /*Size requested by allocator.*/
  void *allocAddr;                  /*Return address of last Alloc made */
  void *freeAddr;                   /*Return address of last Free made */
  uint16_t allocCounter;            /*No of time this msg has been allocated */
  uint16_t freeCounter;             /*No of time this msg has been freed */
  dbgMemTrackingStatus_t allocStatus;  /*1 if currently allocated, 0 if currently free */
  // uint8_t padding;
  uint32_t timeStamp;
  void *pCaller;
}dbgBlockTracking_t;

typedef struct dbgNVMSavings_tag
{
    uint32_t countSave;
    bool_t   dataIsAllocated;
    dbgBlockTracking_t lastAllocBlock; // Specify the last allocated message if (dataIsAllocated == TRUE)
    uint8_t*  pLastNonAllocAddr; // Specify the RAM address if (dataIsAllocated == FALSE)
}dbgNVMSavings_t;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
extern dbgNVMSavings_t  gDbgNVMSavings[];
extern const uint16_t   gDbgNVMSavingsSize;
#endif /*DEBUG_LOG*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/

#if DEBUG_LOG
void DBG_PrintLog( bool_t printTimeStamp, uint8_t* pString,
                   uint8_t* pBuf, size_t buflen);
void DBG_PrintTimeStamp(void);
void DBG_HexDump(uint8_t *pBuf, uint16_t buflen);
void DBG_Printf(char * format,...);
void DBG_WriteString(uint8_t* pBuf);
void DBG_PrintNBytes(uint8_t* pBuff, uint16_t n);
void DBG_MEMBufferCheck(uint8_t *p, uint32_t size);
void DBG_MsgCheck(void);
void DBG_CheckIfMemBuffersAreFreed(void);
#else
#define DBG_Printf(format,...)
#define DBG_WriteString(pBuf)
#define DBG_PrintLog(printTimeStamp, pString, pBuf, buflen)
#define DBG_MEMBufferCheck(p,size)
#define DBG_CheckIfMemBuffersAreFreed()
#endif /* DEBUG_LOG */
void DBG_CheckIP6Addresses(uint32_t ipIfId);
void DBG_CheckRestoreLeaderLed(void);
void DBG_Check(void);
void DBG_NVM(uint16_t nvmTableEntryId, uint16_t elementIndex, uint8_t saveRestoreAll);

/*================================================================================================*/
#endif
