/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This is the source file for the Memory Manager.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
#include "Panic.h"
#include "MemManager.h"
#include "FunctionLib.h"

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

#define _block_size_  {
#define _number_of_blocks_  ,
#define _pool_id_(a) , a ,
#define _eol_  },

extern poolInfo_t poolInfo[];
poolInfo_t poolInfo[] =
{
    PoolsDetails_c
    {0, 0, 0} /*termination tag*/
};

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#define _block_size_ (sizeof(listHeader_t)+
#define _number_of_blocks_ ) *
#define _eol_  +
#define _pool_id_(a)

#define heapSize_c (PoolsDetails_c 0)

/* Heap */
extern uint32_t memHeap[heapSize_c / sizeof(uint32_t)];
uint32_t memHeap[heapSize_c / sizeof(uint32_t)];
extern const uint32_t heapSize;
const uint32_t heapSize = heapSize_c;

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#define _block_size_ 0 *
#define _number_of_blocks_ + 0 *
#define _eol_  + 1 +
#define _pool_id_(a)

#define poolCount (PoolsDetails_c 0)

/* Memory pool info and anchors. */
extern pools_t memPools[poolCount];
pools_t memPools[poolCount];
/* Memory pools address array is used to access directly the correct pool address in MEM_BufferAllocWithId function */
extern pools_t *memPoolsTopAdd[poolCount];
pools_t *memPoolsTopAdd[poolCount];
#ifdef MEM_STATISTICS
pools_t  memPoolsSnapShot[poolCount];
#endif

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#ifdef MEM_TRACKING

#ifndef NUM_OF_TRACK_PTR
#define NUM_OF_TRACK_PTR 1
#endif

#define _block_size_ 0*
#define _number_of_blocks_ +
#define _eol_  +
#define _pool_id_(a)

#define mTotalNoOfMsgs_d (PoolsDetails_c 0)
static const uint16_t mTotalNoOfMsgs_c = mTotalNoOfMsgs_d;
blockTracking_t memTrack[mTotalNoOfMsgs_d];

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#endif /*MEM_TRACKING*/

/* Free messages counter. Not used by module. */
static uint16_t gFreeMessagesCount;

#ifdef MEM_STATISTICS
uint16_t gFreeMessagesCountMin = 0xFFFF;
uint16_t gTotalFragmentWaste = 0;
uint16_t gMaxTotalFragmentWaste = 0;
#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   This function initializes the message module private variables.
*          Must be called at boot time, or if device is reset.
*
* \param[in] none
*
* \return MEM_SUCCESS_c if initialization is successful. (It's always successful).
*
********************************************************************************** */
memStatus_t MEM_Init(void)
{
    poolInfo_t *pPoolInfo = poolInfo; /* IN: Memory layout information */
    pools_t *pPools = memPools;/* OUT: Will be initialized with requested memory pools. */
    uint8_t *pHeap = (uint8_t *)memHeap;/* IN: Memory heap.*/
    uint16_t poolN;
#ifdef MEM_TRACKING
    uint16_t memTrackIndex = 0;
#endif /*MEM_TRACKING*/

    gFreeMessagesCount = 0;
    FLib_MemSet(memPoolsTopAdd, 0U, sizeof(memPoolsTopAdd));

    for (;;)
    {
        poolN = pPoolInfo->poolSize;
        ListInit((listHandle_t)&pPools->anchor, poolN);
#ifdef MEM_STATISTICS
        pPools->poolStatistics.numBlocks = 0;
        pPools->poolStatistics.allocatedBlocks = 0;
        pPools->poolStatistics.allocatedBlocksPeak = 0;
        pPools->poolStatistics.allocationFailures = 0;
        pPools->poolStatistics.freeFailures = 0;
#ifdef MEM_TRACKING
        pPools->poolStatistics.poolFragmentWaste = 0;
        pPools->poolStatistics.poolFragmentWastePeak = 0;
        pPools->poolStatistics.poolFragmentMinWaste = 0xFFFF;
        pPools->poolStatistics.poolFragmentMaxWaste = 0;
#endif /*MEM_TRACKING*/
#endif /*MEM_STATISTICS*/

        while (poolN != 0U)
        {
            /* Add block to list of free memory. */
            (void)ListAddTail((listHandle_t)&pPools->anchor, (listElementHandle_t) & ((listHeader_t *)(uint32_t)pHeap)->link);
            ((listHeader_t *)(uint32_t)pHeap)->pParentPool = pPools;
#ifdef MEM_STATISTICS
            pPools->poolStatistics.numBlocks++;
#endif /*MEM_STATISTICS*/

            pPools->numBlocks++;
            gFreeMessagesCount++;
#ifdef MEM_TRACKING
            memTrack[memTrackIndex].blockAddr = (void *)(pHeap + sizeof(listHeader_t));
            memTrack[memTrackIndex].blockSize = pPoolInfo->blockSize;
            memTrack[memTrackIndex].fragmentWaste = 0;
            memTrack[memTrackIndex].allocAddr = NULL;
            memTrack[memTrackIndex].allocCounter = 0;
            memTrack[memTrackIndex].allocStatus = MEM_TRACKING_FREE_c;
            memTrack[memTrackIndex].freeAddr = NULL;
            memTrack[memTrackIndex].freeCounter = 0;
            memTrackIndex++;
#endif /*MEM_TRACKING*/

            /* Add block size (without list header)*/
            pHeap += pPoolInfo->blockSize + sizeof(listHeader_t);
            poolN--;
        }

        pPools->blockSize = pPoolInfo->blockSize;
        pPools->poolId = pPoolInfo->poolId;
        pPools->nextBlockSize = (pPoolInfo + 1)->blockSize;

        /* Set the pointer to the first memory pool with specific ID */
        if ((pPoolInfo->poolId < NumberOfElements(memPoolsTopAdd)) && 
            (memPoolsTopAdd[pPoolInfo->poolId] == NULL))
        {
            memPoolsTopAdd[pPoolInfo->poolId] = pPools;
        }

        if (pPools->nextBlockSize == 0U)
        {
            break;
        }

        pPools++;
        pPoolInfo++;
    }

    return MEM_SUCCESS_c;
}

/*! *********************************************************************************
* \brief    This function returns the number of available blocks greater or
*           equal to the given size.
*
* \param[in] size - Size of blocks to check for availability.
*
* \return Number of available blocks greater or equal to the given size.
*
* \pre Memory manager must be previously initialized.
*
********************************************************************************** */
uint32_t MEM_GetAvailableBlocks
(
    uint32_t size
)
{
    pools_t *pPools = memPools;
    uint32_t pTotalCount = 0;

    for (;;)
    {
        if (size <= pPools->blockSize)
        {
            pTotalCount += ListGetSize((listHandle_t)&pPools->anchor);
        }

        if (pPools->nextBlockSize == 0U)
        {
            break;
        }

        pPools++;
    }

    return  pTotalCount;
}

/*! *********************************************************************************
* \brief     Allocate a block from the memory pools. The function uses the
*            numBytes argument to look up a pool with adequate block sizes.
* \param[in] numBytes - Size of buffer to allocate.
*
* \return Pointer to the allocated buffer, NULL if failed.
*
* \pre Memory manager must be previously initialized.
*
********************************************************************************** */

/*! *********************************************************************************
* \brief     Allocate a block from the memory pools. The function uses the
*            numBytes argument to look up a pool with adequate block sizes.
* \param[in] numBytes - Size of buffer to allocate.
* \param[in] poolId - The ID of the pool where to search for a free buffer.
* \param[in] pCaller - pointer to the caller function (Debug purpose)
*
* \return Pointer to the allocated buffer, NULL if failed.
*
* \pre Memory manager must be previously initialized.
*
********************************************************************************** */
void* MEM_BufferAllocWithId
(
    uint32_t numBytes,
    uint8_t  poolId,
    void *pCaller
)
{
#ifdef MEM_TRACKING
    /* Save the Link Register */
    volatile uint32_t savedLR = (uint32_t) __get_LR();
#endif
#if defined(MEM_TRACKING) || defined(MEM_DEBUG_OUT_OF_MEMORY)
    uint16_t requestedSize = numBytes;
#endif /*MEM_TRACKING*/
#ifdef MEM_STATISTICS
    bool_t allocFailure = FALSE;
#endif

    pools_t *pPools;
    listHeader_t *pBlock;

    if (poolId < NumberOfElements(memPoolsTopAdd))
    {
        pPools = memPoolsTopAdd[poolId];
    }
    else
    {
        pPools = memPools;
    }

    OSA_InterruptDisable();


    while (numBytes != 0U)
    {
        if ( (numBytes <= pPools->blockSize) && (poolId == pPools->poolId) )
        {
            pBlock = (listHeader_t *)(uint32_t) ListRemoveHead((listHandle_t)&pPools->anchor);

            if (NULL != pBlock)
            {
                pBlock++;
                gFreeMessagesCount--;
                pPools->allocatedBlocks++;

#ifdef MEM_STATISTICS
                if (gFreeMessagesCount < gFreeMessagesCountMin)
                {
                    gFreeMessagesCountMin = gFreeMessagesCount;
                }

                pPools->poolStatistics.allocatedBlocks++;
                if ( pPools->poolStatistics.allocatedBlocks > pPools->poolStatistics.allocatedBlocksPeak )
                {
                    pPools->poolStatistics.allocatedBlocksPeak = pPools->poolStatistics.allocatedBlocks;
                }
                MEM_ASSERT(pPools->poolStatistics.allocatedBlocks <= pPools->poolStatistics.numBlocks);
#endif /*MEM_STATISTICS*/

#ifdef MEM_TRACKING
                MEM_Track(pBlock, MEM_TRACKING_ALLOC_c, savedLR, requestedSize, pCaller);
#endif /*MEM_TRACKING*/
                OSA_InterruptEnable();
                return pBlock;
            }
            else
            {
#ifdef MEM_STATISTICS
                if (!allocFailure)
                {
                    pPools->poolStatistics.allocationFailures++;
                    allocFailure = TRUE;
                }
#endif /*MEM_STATISTICS*/
                if (numBytes > pPools->nextBlockSize)
                {
                    break;
                }
                /* No more blocks of that size, try next size. */
                numBytes = pPools->nextBlockSize;
            }
        }
        /* Try next pool*/
        if (pPools->nextBlockSize != 0U)
        {
            pPools++;
        }
        else
        {
            numBytes = 0;
        }
    }

#ifdef MEM_DEBUG_OUT_OF_MEMORY
    if (requestedSize)
    {
        panic( 0, (uint32_t)MEM_BufferAllocWithId, 0, 0);
    }
#endif

    OSA_InterruptEnable();
    return NULL;
}

/*! *********************************************************************************
* \brief     Deallocate a memory block by putting it in the corresponding pool
*            of free blocks.
*
* \param[in] buffer - Pointer to buffer to deallocate.
*
* \return MEM_SUCCESS_c if deallocation was successful, MEM_FREE_ERROR_c if not.
*
* \pre Memory manager must be previously initialized.
*
* \remarks Never deallocate the same buffer twice.
*
********************************************************************************** */
#if (defined MULTICORE_MEM_MANAGER) && ((MULTICORE_APPLICATION_CORE) || (MULTICORE_CONNECTIVITY_CORE))
#if MULTICORE_APPLICATION_CORE
memStatus_t MEM_BufferFreeOnMaster
#elif MULTICORE_CONNECTIVITY_CORE
memStatus_t MEM_BufferFreeOnSlave
#endif /* MULTICORE_APPLICATION_CORE */
(
    uint8_t *pBuff
)
{
#ifdef MEM_TRACKING
    /* Save the Link Register */
    volatile uint32_t savedLR = (uint32_t) __get_LR();
#endif /*MEM_TRACKING*/
    listHeader_t *pHeader;

    if ( pBuff == NULL )
    {
        return MEM_FREE_ERROR_c;
    }

    pHeader = (listHeader_t *)pBuff - 1;

    if ( ((uint8_t*)pHeader < (uint8_t*)memHeap) || ((uint8_t*)pHeader > ((uint8_t*)memHeap + sizeof(memHeap))) )
    {
#ifdef MEM_DEBUG_INVALID_POINTERS
        panic( 0, (uint32_t)MEM_BufferFree, 0, 0);
#endif
        return MEM_FREE_ERROR_c;
    }
    else
    {
        return MEM_BufferFree(pBuff);
    }
}
#endif /* (defined MULTICORE_MEM_MANAGER) && ((MULTICORE_APPLICATION_CORE) || (MULTICORE_CONNECTIVITY_CORE)) */

memStatus_t MEM_BufferFree
(
    void* buffer /* IN: Block of memory to free*/
)
{
#ifdef MEM_TRACKING
    /* Save the Link Register */
    volatile uint32_t savedLR = (uint32_t) __get_LR();
#endif /*MEM_TRACKING*/
    listHeader_t *pHeader;
    pools_t *pParentPool;
    pools_t *pool;

    if ( buffer == NULL )
    {
        return MEM_FREE_ERROR_c;
    }

    pHeader = (listHeader_t *)buffer - 1;

    if ( ((uint8_t*)pHeader < (uint8_t*)memHeap) || ((uint8_t*)pHeader > ((uint8_t*)memHeap + sizeof(memHeap))) )
    {
#if (defined MULTICORE_MEM_MANAGER) && (MULTICORE_CONNECTIVITY_CORE)
        return MEM_BufferFreeOnMaster(buffer);
#elif (defined MULTICORE_MEM_MANAGER) && (MULTICORE_APPLICATION_CORE)
        return MEM_BufferFreeOnSlave(buffer);
#else
#ifdef MEM_DEBUG_INVALID_POINTERS
        panic( 0, (uint32_t)MEM_BufferFree, 0, 0);
#endif
        return MEM_FREE_ERROR_c;
#endif
    }

    OSA_InterruptDisable();

    pParentPool = (pools_t *)pHeader->pParentPool;
    pool = memPools;

    for (;;)
    {
        if (pParentPool == pool)
        {
            break;
        }
        if (pool->nextBlockSize == 0U)
        {
            /* The parent pool was not found! This means that the memory buffer is corrupt or
            that the MEM_BufferFree() function was called with an invalid parameter */
#ifdef MEM_STATISTICS
            pParentPool->poolStatistics.freeFailures++;
#endif /*MEM_STATISTICS*/
            OSA_InterruptEnable();
#ifdef MEM_DEBUG_INVALID_POINTERS
            panic( 0, (uint32_t)MEM_BufferFree, 0, 0);
#endif
            return MEM_FREE_ERROR_c;
        }
        pool++;
    }

    if ( pHeader->link.list != NULL )
    {
        /* The memory buffer appears to be enqueued in a linked list.
        This list may be the free memory buffers pool, or another list. */
#ifdef MEM_STATISTICS
        pParentPool->poolStatistics.freeFailures++;
#endif /*MEM_STATISTICS*/
        OSA_InterruptEnable();
#ifdef MEM_DEBUG_INVALID_POINTERS
        panic( 0, (uint32_t)MEM_BufferFree, 0, 0);
#endif
        return MEM_FREE_ERROR_c;
    }

    gFreeMessagesCount++;

    (void)ListAddTail((listHandle_t)&pParentPool->anchor, (listElementHandle_t)&pHeader->link);
    pParentPool->allocatedBlocks--;

#ifdef MEM_STATISTICS
    MEM_ASSERT(pParentPool->poolStatistics.allocatedBlocks > 0);
    pParentPool->poolStatistics.allocatedBlocks--;
#endif /*MEM_STATISTICS*/

#ifdef MEM_TRACKING
    MEM_Track(buffer, MEM_TRACKING_FREE_c, savedLR, 0, NULL);
#endif /*MEM_TRACKING*/
    OSA_InterruptEnable();
    return MEM_SUCCESS_c;
}

/*! *********************************************************************************
* \brief     Determines the size of a memory block
*
* \param[in] buffer - Pointer to buffer.
*
* \return size of memory block
*
* \pre Memory manager must be previously initialized.
*
********************************************************************************** */
uint16_t MEM_BufferGetSize
(
    void* buffer /* IN: Block of memory to free*/
)
{
    if ( buffer != NULL )
    {
        return ((pools_t *)((listHeader_t *)buffer - 1)->pParentPool)->blockSize;
    }

    return 0;
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

#ifdef MEM_TRACKING

/*
 * MemManager Buffer Pool Optimizer
 */

typedef struct optPool_tag
{
    uint16_t    size;
    uint16_t    cnt;
} optPool_t;

#define MAX_POOLS 32
#define POOL_OVERHEAD        (sizeof(pools_t))
#define POOL_BUFFER_OVERHEAD (sizeof(listHeader_t))

/*
 * TODO: current implementation only assumes 512 byte buffers
 * as the largest size. Should add 1024 and 2048 but make them
 * conditional?
 */
#define MAX_8BYTE_SLICES     ((MAX_SUPPORTED_BUFFER_SIZE >> 3)+1)

optPool_t optimumPoolCfg[MAX_POOLS] = {0};

optPool_t tmp_pool[MAX_POOLS] = {0};
uint32_t optimumPoolSize = 0;

uint8_t buffer_alloc8[MAX_8BYTE_SLICES] = {0};      /* Current outstanding allocations */
uint8_t buffer_alloc16[MAX_8BYTE_SLICES / 2] = {0}; /* Current outstanding allocations */
uint8_t buffer_alloc32[MAX_8BYTE_SLICES / 4] = {0}; /* Current outstanding allocations */
uint8_t buffer_alloc64[MAX_8BYTE_SLICES / 8] = {0}; /* Current outstanding allocations */
uint8_t buffer_alloc128[MAX_8BYTE_SLICES / 16] = {0}; /* Current outstanding allocations */
uint8_t buffer_alloc256[MAX_8BYTE_SLICES / 32] = {0}; /* Current outstanding allocations */
uint8_t buffer_alloc512[MAX_8BYTE_SLICES / 64] = {0}; /* Current outstanding allocations */
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
uint8_t buffer_alloc1024[MAX_8BYTE_SLICES / 128] = {0}; /* Current outstanding allocations */
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
uint8_t buffer_alloc2048[MAX_8BYTE_SLICES / 256] = {0}; /* Current outstanding allocations */
#endif

uint8_t buffer_peak8[MAX_8BYTE_SLICES] = {0};       /* Peak outstanding allocations (per 8Byte bucket) */
uint8_t buffer_peak16[MAX_8BYTE_SLICES / 2] = {0};  /* Peak outstanding allocations (per 16Byte bucket) */
uint8_t buffer_peak32[MAX_8BYTE_SLICES / 4] = {0};  /* Peak outstanding allocations (per 32Byte bucket) */
uint8_t buffer_peak64[MAX_8BYTE_SLICES / 8] = {0};  /* Peak outstanding allocations (per 64Byte bucket) */
uint8_t buffer_peak128[MAX_8BYTE_SLICES / 16] = {0}; /* Peak outstanding allocations (per 128Byte bucket) */
uint8_t buffer_peak256[MAX_8BYTE_SLICES / 32] = {0}; /* Peak outstanding allocations (per 256Byte bucket) */
uint8_t buffer_peak512[MAX_8BYTE_SLICES / 64] = {0}; /* Peak outstanding allocations (per 512Byte bucket) */
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
uint8_t buffer_peak1024[MAX_8BYTE_SLICES / 128] = {0};  /* Peak outstanding allocations (per 1024Byte bucket) */
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
uint8_t buffer_peak2048[MAX_8BYTE_SLICES / 256] = {0};  /* Peak outstanding allocations (per 2048Byte bucket) */
#endif

/*! *********************************************************************************
* \brief     Build the pool configuration from a particular peak array view of the
*        system.
*
* \param[in] opool - Pointer to optimized pool configuration array.
* \param[in] peakArray - pointer to peak bucket array.
* \param[in] arraySize - peakArray size
* \param[in] bucketSize - bucket size of peakArray.
*
* \return Returns RAM size footprint of pool configuration.
*
********************************************************************************** */
uint32_t buildPoolFromPeakArray(optPool_t *opool, uint8_t *peakArray, uint16_t arraySize, uint16_t bucketSize)
{
    uint32_t    totalSize = 0;
    uint16_t    wsize = bucketSize / 8; // 8 byte window size

    int i, pcnt, widx;

    for (i = 0, pcnt = 0; i < arraySize; i++)
    {
        if (peakArray[i] != 0)
        {
            if (pcnt == MAX_POOLS)
            {
                totalSize = 0xFFFFFFFFU;
                break;
            }
            // Find max 8byte size in bucket range
            for (widx = wsize - 1; widx >= 0; widx--)
            {
                if ((buffer_peak8[i * wsize + widx]) != 0)
                {
                    opool->size = (i * wsize + widx + 1) * 8;
                    break;
                }
            }
            opool->cnt = peakArray[i];
            totalSize += (POOL_OVERHEAD) + (opool->cnt * (opool->size + POOL_BUFFER_OVERHEAD));
            pcnt++;
            opool++;
        }
    }
    for (; pcnt < MAX_POOLS; pcnt++, opool++)
    {
        // clear out the rest of the pool structure
        opool->cnt = 0;
    }

    return totalSize;
}

/*! *********************************************************************************
* \brief     Calculate the optimal buffer pool configuration.
*
* Starting from the largest bucket sizing, calculate the optimal buffer pool
* configuration that results in the smallest RAM footprint.
*
********************************************************************************** */
static void calcOptimalPoolConfiguration(void)
{
    uint32_t tsize = 0;

    optimumPoolSize = buildPoolFromPeakArray(&optimumPoolCfg[0], &buffer_peak512[0], sizeof(buffer_peak512), 512);

#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak1024[0], sizeof(buffer_peak1024), 1024);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak2048[0], sizeof(buffer_peak2048), 2048);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
#endif
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak256[0], sizeof(buffer_peak256), 256);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak128[0], sizeof(buffer_peak128), 128);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak64[0], sizeof(buffer_peak64), 64);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak32[0], sizeof(buffer_peak32), 32);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak16[0], sizeof(buffer_peak16), 16);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak8[0], sizeof(buffer_peak8), 8);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
}

/*! *********************************************************************************
* \brief     This function updates the windowed view of the buffer allocation.
*
* \param[in] alloc - Indicates whether an allocation or free operation was performed
* \param[in] size - Indicates the buffer size to be allocated or to be freed.
* \param[in] poolId - Pool ID the alloc/free is targeting
*
********************************************************************************** */
static void profileBufferUsage(memTrackingStatus_t alloc, uint16_t size, uint16_t poolId)
{
    uint16_t index;

    if (poolId == POOL_TO_OPTIMIZE)
    {
        index = (size - 1) >> 3;

        if (alloc == MEM_TRACKING_ALLOC_c)
        {
            buffer_alloc8[index]++;
            if (buffer_alloc8[index] > buffer_peak8[index])
            {
                buffer_peak8[index] = buffer_alloc8[index];
            }
            buffer_alloc16[index / 2]++;
            if (buffer_alloc16[index / 2] > buffer_peak16[index / 2])
            {
                buffer_peak16[index / 2] = buffer_alloc16[index / 2];
            }
            buffer_alloc32[index / 4]++;
            if (buffer_alloc32[index / 4] > buffer_peak32[index / 4])
            {
                buffer_peak32[index / 4] = buffer_alloc32[index / 4];
            }
            buffer_alloc64[index / 8]++;
            if (buffer_alloc64[index / 8] > buffer_peak64[index / 8])
            {
                buffer_peak64[index / 8] = buffer_alloc64[index / 8];
            }
            buffer_alloc128[index / 16]++;
            if (buffer_alloc128[index / 16] > buffer_peak128[index / 16])
            {
                buffer_peak128[index / 16] = buffer_alloc128[index / 16];
            }
            buffer_alloc256[index / 32]++;
            if (buffer_alloc256[index / 32] > buffer_peak256[index / 32])
            {
                buffer_peak256[index / 32] = buffer_alloc256[index / 32];
            }
            buffer_alloc512[index / 64]++;
            if (buffer_alloc512[index / 64] > buffer_peak512[index / 64])
            {
                buffer_peak512[index / 64] = buffer_alloc512[index / 64];
            }
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
            buffer_alloc1024[index / 128]++;
            if (buffer_alloc1024[index / 128] > buffer_peak1024[index / 128])
            {
                buffer_peak1024[index / 128] = buffer_alloc1024[index / 128];
            }
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
            buffer_alloc2048[index / 256]++;
            if (buffer_alloc2048[index / 256] > buffer_peak2048[index / 256])
            {
                buffer_peak2048[index / 256] = buffer_alloc2048[index / 256];
            }
#endif

            //
            // At this point we would want to shift the peaks to figure
            // out what the optimum buffer allocation is. Note that
            // Overhead per buffer is 16 bytes and per pool is 20 bytes.
            //
            calcOptimalPoolConfiguration();
        }
        else
        {
            buffer_alloc8[index]--;
            buffer_alloc16[index / 2]--;
            buffer_alloc32[index / 4]--;
            buffer_alloc64[index / 8]--;
            buffer_alloc128[index / 16]--;
            buffer_alloc256[index / 32]--;
            buffer_alloc512[index / 64]--;
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
            buffer_alloc1024[index / 128]--;
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
            buffer_alloc2048[index / 256]--;
#endif
        }
    }

}

/*! *********************************************************************************
* \brief     This function updates the tracking array element corresponding to the given
*            block.
*
* \param[in] block - Pointer to the block.
* \param[in] alloc - Indicates whether an allocation or free operation was performed
* \param[in] address - Address where MEM_BufferAlloc or MEM_BufferFree was called
* \param[in] requestedSize - Indicates the requested buffer size  passed to MEM_BufferAlloc.
*                            Has no use if a free operation was performed.
*
* \return Returns TRUE if correct allocate or free operation was performed, FALSE if a
*         buffer was allocated or freed twice.
*
********************************************************************************** */
uint8_t MEM_Track(listHeader_t *block, memTrackingStatus_t alloc, uint32_t address, uint16_t requestedSize, void *pCaller)
{
    uint16_t i;
    blockTracking_t *pTrack = NULL;
#ifdef MEM_STATISTICS
    poolStat_t * poolStatistics = (poolStat_t *) & ((pools_t *)( (listElementHandle_t)(block - 1)->pParentPool ))->poolStatistics;
#endif

    uint16_t  poolId = (uint16_t)((pools_t *)( (listElementHandle_t)(block - 1)->pParentPool ))->poolId;

    for ( i = 0; i < mTotalNoOfMsgs_c; i++ )
    {
        if ( block == memTrack[i].blockAddr )
        {
            pTrack = &memTrack[i];
            break;
        }
    }

    if ( !pTrack || pTrack->allocStatus == alloc)
    {
#ifdef MEM_DEBUG
        panic( 0, (uint32_t)MEM_Track, 0, 0);
#endif
        return FALSE;
    }

    pTrack->allocStatus = alloc;
    pTrack->pCaller = (void*)((uint32_t)pCaller & 0x7FFFFFFF);

    if (alloc == MEM_TRACKING_ALLOC_c)
    {
        pTrack->requested_size = requestedSize;

        profileBufferUsage(alloc, requestedSize, poolId);

        pTrack->fragmentWaste = pTrack->blockSize - requestedSize;
        pTrack->allocCounter++;
        pTrack->allocAddr = (void *)address;
        if ( (uint32_t)pCaller & 0x80000000U )
        {
            pTrack->timeStamp = 0xFFFFFFFFU;
        }
        else
        {
            pTrack->timeStamp = MEM_GetTimeStamp();
        }
#ifdef MEM_STATISTICS
        gTotalFragmentWaste += pTrack->fragmentWaste;
        if (gTotalFragmentWaste > gMaxTotalFragmentWaste)
        {
            gMaxTotalFragmentWaste = gTotalFragmentWaste;
            FLib_MemCpy(&memPoolsSnapShot[0], &memPools[0], sizeof(memPools));
        }

        poolStatistics->poolFragmentWaste += pTrack->fragmentWaste;
        if (poolStatistics->poolFragmentWaste > poolStatistics->poolFragmentWastePeak)
        {
            poolStatistics->poolFragmentWastePeak = poolStatistics->poolFragmentWaste;
        }
        if (pTrack->fragmentWaste < poolStatistics->poolFragmentMinWaste)
        {
            poolStatistics->poolFragmentMinWaste = pTrack->fragmentWaste;
        }
        if (pTrack->fragmentWaste > poolStatistics->poolFragmentMaxWaste)
        {
            poolStatistics->poolFragmentMaxWaste = pTrack->fragmentWaste;
        }
        if (poolStatistics->allocatedBlocks == poolStatistics->numBlocks)
        {
            /* __asm("BKPT #0\n") ; */ /* cause the debugger to stop */
        }
#endif /*MEM_STATISTICS*/
    }
    else
    {
#ifdef MEM_STATISTICS
        poolStatistics->poolFragmentWaste -= pTrack->fragmentWaste;
        gTotalFragmentWaste -= pTrack->fragmentWaste;
#endif /*MEM_STATISTICS*/

        profileBufferUsage(alloc, pTrack->requested_size, poolId);

        pTrack->fragmentWaste = 0;
        pTrack->freeCounter++;
        pTrack->freeAddr = (void *)address;
        pTrack->timeStamp = 0;
    }

    return TRUE;
}

/*! *********************************************************************************
* \brief     This function checks for buffer overflow when copying multiple bytes
*
* \param[in] p    - pointer to destination.
* \param[in] size - number of bytes to copy
*
* \return 1 if overflow detected, else 0
*
********************************************************************************** */
uint8_t MEM_BufferCheck(uint8_t *p, uint32_t size)
{
    poolInfo_t *pPollInfo = poolInfo;
    uint8_t* memAddr = (uint8_t *)memHeap;
    uint32_t poolBytes, blockBytes, i;

    if ( (p < (uint8_t*)memHeap) || (p > ((uint8_t*)memHeap + sizeof(memHeap))) )
    {
        return 0;
    }

    while ( pPollInfo->blockSize )
    {
        blockBytes = pPollInfo->blockSize + sizeof(listHeader_t);
        poolBytes  = blockBytes * pPollInfo->poolSize;

        /* Find the correct message pool */
        if ( (p >= memAddr) && (p < memAddr + poolBytes) )
        {
            /* Check if the size to copy is greater then the size of the current block */
            if ( size > pPollInfo->blockSize )
            {
#ifdef MEM_DEBUG
                panic(0, 0, 0, 0);
#endif
                return 1;
            }

            /* Find the correct memory block */
            for ( i = 0; i < pPollInfo->poolSize; i++ )
            {
                if ( (p >= memAddr) && (p < memAddr + blockBytes) )
                {
                    if ( p + size > memAddr + blockBytes )
                    {
#ifdef MEM_DEBUG
                        panic(0, 0, 0, 0);
#endif
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }

                memAddr += blockBytes;
            }
        }

        /* check next pool */
        memAddr += poolBytes;
        pPollInfo++;
    }

    return 0;
}


/*! *********************************************************************************
* \brief     This function checks if the buffers are allocated for more than the
*            specified duration
*
********************************************************************************** */
void MEM_CheckIfMemBuffersAreFreed(void)
{
    uint32_t t;
    uint16_t i, j = 0;
    uint8_t trackCount = 0;
    pools_t *pParentPool;
    volatile blockTracking_t *pTrack;
    static volatile blockTracking_t *mpTrackTbl[NUM_OF_TRACK_PTR];
    static uint32_t lastTimestamp = 0;
    uint32_t currentTime = MEM_GetTimeStamp();

    if ( (currentTime - lastTimestamp) >= MEM_CheckMemBufferInterval_c )
    {
        lastTimestamp = currentTime;
        j = 0;

        for ( i = 0; i < mTotalNoOfMsgs_c; i++ )
        {
            pTrack = &memTrack[i];

            /* Validate the pParent first */
            pParentPool = (((listHeader_t *)(pTrack->blockAddr)) - 1)->pParentPool;
            if (pParentPool != &memPools[j])
            {
                if (j < NumberOfElements(memPools))
                {
                    j++;
                    if (pParentPool != &memPools[j])
                    {
                        panic(0, 0, 0, 0);
                    }
                }
                else
                {
                    panic(0, 0, 0, 0);
                }
            }

            /* Check if it should be freed  */
            OSA_InterruptDisable();
            if ((pTrack->timeStamp != 0xffffffffU ) &&
                    (pTrack->allocStatus == MEM_TRACKING_ALLOC_c) &&
                    (currentTime > pTrack->timeStamp))
            {
                t = currentTime - pTrack->timeStamp;
                if ( t > MEM_CheckMemBufferThreshold_c )
                {
                    mpTrackTbl[trackCount++] = pTrack;
                    if (trackCount == NUM_OF_TRACK_PTR)
                    {
                        (void)mpTrackTbl; /* remove compiler warnings */
                        panic(0, 0, 0, 0);
                        break;
                    }
                }
            }
            OSA_InterruptEnable();
        } /* end for */
    }
}

/*! *********************************************************************************
* \brief     Get time-stamp for memory operation: allocate/free.
*
* \return dummy time-stamp
*
********************************************************************************** */
#if defined(__IAR_SYSTEMS_ICC__)
__weak uint32_t MEM_GetTimeStamp(void)
#elif defined(__GNUC__)
__attribute__((weak)) uint32_t MEM_GetTimeStamp(void)
#endif
{
    return 0xFFFFFFFFU;
}
#endif /* MEM_TRACKING */

/*! ************************************************************************************************
\brief  MEM Manager calloc alternative implementation.

\param  [in]    len                     Number of blocks
\param  [in]    size                    Size of one block

\return         void*                   Pointer to the allocated buffer or NULL in case of error
************************************************************************************************* */
void* MEM_CallocAlt
(
    size_t len,
    size_t val
)
{
    void *pData = MEM_BufferAllocForever (len * val, 0);
    if (pData != NULL)
    {
        FLib_MemSet (pData, 0, len * val);
    }

    return pData;
}

/*! ************************************************************************************************
\brief  MEM Manager free alternative implementation.

\param  [in]    pData                   Pointer to the allocated buffer

\return         void
************************************************************************************************* */
void MEM_FreeAlt
(
    void* pData
)
{
    (void)MEM_BufferFree (pData);
}
