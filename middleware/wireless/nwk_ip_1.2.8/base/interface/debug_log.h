/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
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
