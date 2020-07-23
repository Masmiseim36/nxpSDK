/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* This is the header file for the Memory Manager interface.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include <stddef.h>

#include "EmbeddedTypes.h"
#include "GenericList.h"

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#ifndef gMemManagerLight
#define gMemManagerLight      0U
#endif


/*Defines pools by block size and number of blocks. Must be aligned to 4 bytes.*/
#ifndef PoolsDetails_c
#define PoolsDetails_c \
         _block_size_  64  _number_of_blocks_    8 _pool_id_(0) _eol_  \
         _block_size_ 128  _number_of_blocks_    2 _pool_id_(0) _eol_  \
         _block_size_ 256  _number_of_blocks_    6 _pool_id_(0) _eol_
#endif

/* If a buffer (that is not allocated forever using MEM_BufferAllocForever()) is not freed
   in MEM_CheckMemBufferThreshold_c ms, the device will enter into panic */
#ifndef MEM_CheckMemBufferThreshold_c
#define MEM_CheckMemBufferThreshold_c     120000UL /* ms */
#endif

/* How often the device should check if the above threshold expired */
#ifndef MEM_CheckMemBufferInterval_c
#define MEM_CheckMemBufferInterval_c      15000UL /* ms */
#endif

/* Default memory allocator */
#ifndef MEM_BufferAlloc
#define MEM_BufferAlloc(numBytes)   MEM_BufferAllocWithId(numBytes, 0, (uint32_t*)__get_LR())
#endif

/* Allocate a block from the memory pools forever.*/
#define MEM_BufferAllocForever(numBytes,poolId)   MEM_BufferAllocWithId(numBytes, poolId, (uint32_t*)((uintptr_t)__get_LR() | 0x80000000U ))

#ifdef MEM_TRACKING

/* It may be already defined by app but if not enabled,
enable it as MEM_TARCKING feature needs it. */
#ifndef MEM_STATISTICS
#define MEM_STATISTICS
#endif

/* Which pool to optimize */
#define POOL_TO_OPTIMIZE                0U
/* Maximum buffer size to track */
#define MAX_SUPPORTED_BUFFER_SIZE       512U
/* Maximum pool to track */

#endif /* MEM_TRACKING */

#define MAX_POOL_ID             3U

/* The MEM_TRACK_ALLOC_SOURCE allows to track blocks allocated by LL (functionality could be extended to other modules).
The feature is enabled by default. This tracking is needed because LL is sometimes requested to free all buffers it allocated
(on HCI_RESET command for instance).
On every LL allocation/free the array containing the tracking information (1 bit per block) will be updated (bit set on allocation
 and cleared when freed). On LL buffer free request, the MEM_BufferFreeAllWithId() function will be called and all unfreed LL blocks
will be released. */
#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
/*Each source id shall be unique. For now, only LL source is supported. */
#define LL_SOURCE_ID                    1U

#define SOURCE_ID_BIT_POS               4U
#define SOURCE_ID_MASK                  0xF0U
#define MEM_SRC_TRACK_ELMT_SIZE         8
#define GET_SOURCE_ID(poolId)           (((poolId) & SOURCE_ID_MASK) >> SOURCE_ID_BIT_POS)
#define SET_SOURCE_ID(poolId)           ((LL_SOURCE_ID << SOURCE_ID_BIT_POS) | ((poolId) & (~SOURCE_ID_MASK)))
#define GET_POOL_ID(poolId)             ((poolId) & MAX_POOL_ID)
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */

/* Debug Macros - stub if not defined */
#ifndef MEM_DBG_LOG
#define MEM_DBG_LOG(...)
#endif

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
extern uint16_t gFreeMessagesCount;

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
/*Defines statuses used in MEM_BufferAlloc and MEM_BufferFree*/
typedef enum
{
  MEM_SUCCESS_c = 0,                    /* No error occurred */
  MEM_INIT_ERROR_c,                     /* Memory initialization error */
  MEM_ALLOC_ERROR_c,                    /* Memory allocation error */
  MEM_FREE_ERROR_c,                     /* Memory free error */
  MEM_UNKNOWN_ERROR_c                   /* something bad has happened... */
}memStatus_t;

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
/*Initialises the Memory Manager.*/
memStatus_t MEM_Init(void);
/*Returns the number of available blocks that fit the given size.*/
uint32_t MEM_GetAvailableBlocks(uint32_t size);
/*Frees the givem buffer.*/
memStatus_t MEM_BufferFree(void* buffer);
/* Frees all buffers allocated in the selected pool Id (4 LSBs of poolId parameter)
and from the selected source Id (4 MSBs of poolId parameter).
This sourceId information is only used by LL and allows to track blocks allocated by LL. */
memStatus_t MEM_BufferFreeAllWithId(uint8_t poolId);
/*Returns the allocated buffer of the given size.*/
void* MEM_BufferAllocate(uint32_t numBytes , uint8_t  poolId);
/* Calls MEM_BufferAllocate and drops pCaller. Direct use of MEM_BufferAllocate is preferred.*/
/* The poolId information may contain in addition to the poolId information (4 LSBs of poolId),
a sourceId information (4 MSBs of poolId). This sourceId information is only used by LL and allows
to track blocks allocated by LL. */
void* MEM_BufferAllocWithId(uint32_t numBytes , uint8_t  poolId, void *pCaller);
/*Returns the size of a given buffer*/
uint16_t MEM_BufferGetSize(void* buffer);
/*MEM Manager calloc alternative implementation*/
void* MEM_CallocAlt(size_t len, size_t val);
/*MEM Manager free alternative implementation.*/
void MEM_FreeAlt(void* pData);

/*  MEM_BufferRealloc is based on standard C realloc() behavior
    If new requested size is 0, acts like MEM_BufferFree and returns NULL.
    If input pointer is NULL, acts like MEM_BufferAllocate and returns pointer
        to allocated buffer.
    If the new requested size is smaller/equal, returns input pointer.
    If new requested size is bigger, allocates a new buffer, copy the input
        buffer data to new buffer, frees the input buffer and returns pointer
        to new allocated buffer.
        If MEM_BufferAllocate fails, MEM_BufferRealloc returns a NULL pointer
        and doesn't free the input buffer */
void* MEM_BufferRealloc(void* buffer, uint32_t new_size);

#if (defined MULTICORE_MEM_MANAGER) && ((MULTICORE_APPLICATION_CORE) || (MULTICORE_CONNECTIVITY_CORE))
/*Free a buffer on the Master core*/
memStatus_t MEM_BufferFreeOnMaster(uint8_t * pBuff);
/*Free a buffer on the Slave core*/
memStatus_t MEM_BufferFreeOnSlave(uint8_t * pBuff);
#endif

/* Return the address after the last allocated block if MemManagerLight is used
   Return 0 in case of the legacy MemManager */
uint32_t MEM_GetHeapUpperLimit(void);


/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#if defined(MEM_STATISTICS) && defined(DEBUG_ASSERT)
#define MEM_ASSERT(condition) if(!(condition))while(TRUE);
#else
#define MEM_ASSERT(condition) (void)(condition);
#endif

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */

#ifdef MEM_STATISTICS
/*Statistics structure definition. Used by pools.*/
typedef struct poolStat_tag
  {
  uint16_t numBlocks;
  uint16_t allocatedBlocks;
  uint16_t allocatedBlocksPeak;
  uint16_t allocationFailures;
  uint16_t freeFailures;
#ifdef MEM_TRACKING
  uint16_t poolFragmentWaste;
  uint16_t poolFragmentWastePeak;
  uint16_t poolFragmentMinWaste;
  uint16_t poolFragmentMaxWaste;
#endif /*MEM_TRACKING*/
  } poolStat_t;

#endif /*MEM_STATISTICS*/

#ifdef MEM_TRACKING
/*Definition for alloc indicators. Used in buffer tracking.*/
typedef enum
{
  MEM_TRACKING_FREE_c = 0,
  MEM_TRACKING_ALLOC_c,
}memTrackingStatus_t;

/*Tracking structure definition.*/
typedef PACKED_STRUCT BlockTracking_tag
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
  memTrackingStatus_t allocStatus;  /*1 if currently allocated, 0 if currently free */
  uint32_t timeStamp;
  void *pCaller;
  uint16_t requested_size;
}blockTracking_t;
#endif /*MEM_TRACKING*/

/*Header description for buffers.*/
typedef struct
{
  listElement_t link;
  struct pools_tag *pParentPool;
}listHeader_t;

/*Buffer pools. Used by most functions*/
typedef struct pools_tag
{
  list_t anchor; /* MUST be first element in pools_t struct */
  uint16_t nextBlockSize;
  uint16_t blockSize;
  uint16_t  poolId;
#ifdef MEM_STATISTICS
  poolStat_t poolStatistics;
#endif /*MEM_STATISTICS*/
  uint8_t numBlocks;
  uint8_t allocatedBlocks;
}pools_t;

/*Buffer pool description. Used by MM_Init() for creating the buffer pools. */
typedef struct
{
  uint16_t blockSize;
  uint16_t poolSize;
  uint16_t poolId;
  uint8_t  padding[2];
}poolInfo_t;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
#ifdef MEM_TRACKING
#if !defined(gMemManagerLight) || (gMemManagerLight == 0)

uint8_t MEM_Track(listHeader_t *block, memTrackingStatus_t alloc, uint32_t address, uint16_t requestedSize, void *pCaller);
uint8_t MEM_BufferCheck(uint8_t *p, uint32_t size);
void MEM_CheckIfMemBuffersAreFreed(void);

/* The timestamp function used by MEM Manager for debug purpose.
   The timestamp must be in milliseconds! */
#if defined(__IAR_SYSTEMS_ICC__)
extern __weak uint32_t MEM_GetTimeStamp(void);
#elif defined(__GNUC__)
extern __attribute__((weak)) uint32_t MEM_GetTimeStamp(void);
#endif
#else /* MEM_TRACKING */
#define MEM_CalcOptimalPoolConfiguration(numBytes, poolId)
#endif /* MEM_TRACKING */

#endif /* gMemManagerLight */
#endif /* _MEM_MANAGER_H_ */
