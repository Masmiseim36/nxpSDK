/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
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
#if defined(MEM_STATISTICS_INTERNAL) || defined(MEM_MANAGER_BENCH)
#include "MemManagerInternal.h"
#endif /* MEM_STATISTICS MEM_MANAGER_BENCH*/
#include "MemManager.h"
#include "FunctionLib.h"
#include "board.h"

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
uint16_t gFreeMessagesCount;

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

#if !defined(gMemManagerLight) || (gMemManagerLight == 0)

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
extern uint32_t memHeap[heapSize_c/sizeof(uint32_t)];
uint32_t memHeap[heapSize_c/sizeof(uint32_t)];
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
static pools_t memPools[poolCount];
#ifdef MEM_STATISTICS
static pools_t  memPoolsSnapShot[poolCount];

#endif

#ifdef MEM_STATISTICS_INTERNAL
static memStats_t       MemStats;
#endif

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#if (defined(MEM_TRACKING) || (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)))

#ifndef NUM_OF_TRACK_PTR
#define NUM_OF_TRACK_PTR 1U
#endif

#define _block_size_ 0*
#define _number_of_blocks_ +
#define _eol_  +
#define _pool_id_(a)

#define mTotalNoOfMsgs_d (PoolsDetails_c 0)

#ifdef MEM_TRACKING
static const uint16_t mTotalNoOfMsgs_c = mTotalNoOfMsgs_d;
#ifdef MEM_STATISTICS
static blockTracking_t memTrack[mTotalNoOfMsgs_d];
#endif
#endif /*MEM_TRACKING*/

#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
/*memSourceTrack[] array holds the information of blocks allocated by LL. The blocks are identified by an index which
points to the lowest address of memSourceTrack[] array and consecutive indexes pointing to consecutive blocks in the array.
When a block (index=N) is allocated, the bit at position "word_ix" in "memSourceTrack[array_idx]" is set:
array_idx = N / 8
word_idx = N - (array_idx *8)
When a block is freed, the bit is cleared.
*/
static uint8_t memSourceTrack[((mTotalNoOfMsgs_d)+(MEM_SRC_TRACK_ELMT_SIZE-1))/MEM_SRC_TRACK_ELMT_SIZE] = {0};
static uint8_t  ll_pool_id = 0;
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#endif /* (MEM_TRACKING || (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))) */

/* START: Number of pool computation */
/* The below section is used to determine the exact number of entries (corresponding to the
number of different pool Ids */
#define _block_size_   0*
#define _number_of_blocks_  +0*
#define _eol_ +0+
#define _pool_id_(a)    +((a)/1)+

#define poolIdCount1 (PoolsDetails_c 0)

#if (poolIdCount1 == 0)
#define poolIdCount 1
#else /* (poolIdCount1 == 0) */
#undef _pool_id_
#define _pool_id_(a)    +((a)/2)+

#define poolIdCount2 (PoolsDetails_c 0)

#if (poolIdCount2 == 0)
#define poolIdCount 2
#else /* (poolIdCount2 == 0) */
#undef _pool_id_
#define _pool_id_(a)    +((a)/3)+

#define poolIdCount3 (PoolsDetails_c 0)

#if (poolIdCount3 == 0)
#define poolIdCount 3
#else /* (poolIdCount3 == 0) */
#undef _pool_id_
#define _pool_id_(a)    +((a)/4)+

#define poolIdCount4 (PoolsDetails_c 0)

#if (poolIdCount4 == 0)
#define poolIdCount 4
#else /* (poolIdCount4 == 0) */
#error "Maximum allowed pool number exceeded!"
#endif /* (poolIdCount4 == 0) */
#endif /* (poolIdCount3 == 0) */
#endif /* (poolIdCount2 == 0) */
#endif /* (poolIdCount1 == 0) */

/* STOP: Number of pool computation */

#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
/* memPoolHeapBaseAdd[poolId] holds the address of the first block in memHeap for poolId. */
static uint32_t memPoolHeapBaseAdd[poolIdCount] = {0};
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */

/* Memory pools address array is used to access directly the correct pool address in MEM_BufferAllocWithId function */
static pools_t *memPoolsTopAdd[poolIdCount];

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_


#ifdef MEM_STATISTICS
static uint16_t gFreeMessagesCountMin = 0xFFFF;
static uint16_t gTotalFragmentWaste = 0;
static uint16_t gMaxTotalFragmentWaste = 0;
#endif
/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

#ifdef MEM_STATISTICS_INTERNAL

static void MEM_InitMemStats(memStats_t* MemStats_)
{
    (void)memset(MemStats_, 0, sizeof(memStats_t));
    SystemCoreClockUpdate();
}

static void MEM_BufferAllocateMemStats(void* buffer, uint32_t time, uint32_t requestedSize)
{
    assert(NULL != buffer);

    MemStats.nb_alloc++;
    MemStats.ram_allocated += ((uint16_t)requestedSize + sizeof(listHeader_t));
    UPDATE_PEAK(MemStats.ram_allocated, MemStats.peak_ram_allocated);

    listHeader_t* pHeader;
    pHeader = (listHeader_t *)buffer-1;
    pools_t* pPools = ((pools_t *)pHeader->pParentPool);
    poolStat_t* poolStatistics = &(pPools->poolStatistics);
    uint16_t  poolId = pPools->poolId;

    /* if a requested pool is not supported, stats will be corrupted */
    assert(poolId < MAX_SUPPORTED_POOL);

    /* RAM lost per pool stats exist in poolStat_t */
    MemStats.ram_lost_per_pool[poolId] = poolStatistics->poolFragmentWaste;
    MemStats.ram_lost = 0U;
    for(uint16_t i = 0; i < MAX_SUPPORTED_POOL; i++)
    {
        /* compute total ram lost */
        MemStats.ram_lost += MemStats.ram_lost_per_pool[i];
    }
    UPDATE_PEAK(MemStats.ram_lost, MemStats.peak_ram_lost);

#ifdef MEM_MANAGER_BENCH
    if(time != 0U)
    {
        MemStats.last_alloc_time = time;
        MemStats.total_alloc_time += time;
        MemStats.average_alloc_time = MemStats.total_alloc_time / MemStats.nb_alloc;
        UPDATE_PEAK(time, MemStats.peak_alloc_time);
    }
    else /* alloc time is not correct, we bypass this allocation's data */
    {
        MemStats.nb_alloc--;
    }
#else
    NOT_USED(time);
#endif /* MEM_MANAGER_BENCH */
}

static void MEM_BufferFreeMemStats(void* buffer)
{
    assert(NULL != buffer);
    blockTracking_t *pTrack = NULL;

    /* blockTracking_t stores the requested size in a block */
    for(uint32_t i=0; i<mTotalNoOfMsgs_c; i++ )
    {
        if( (listHeader_t*)buffer == memTrack[i].blockAddr )
        {
            pTrack = &memTrack[i];
            break;
        }
    }

    if(NULL != pTrack)
    {
        MemStats.ram_allocated -= (pTrack->requested_size + sizeof(listHeader_t));

        listHeader_t* pHeader;
        pHeader = (listHeader_t *)buffer-1;
        pools_t *pPools = (pools_t *)pHeader->pParentPool;
        poolStat_t* poolStatistics = &(pPools->poolStatistics);
        uint16_t  poolId = pPools->poolId;

        assert(poolId < MAX_SUPPORTED_POOL);

        MemStats.ram_lost_per_pool[poolId] = poolStatistics->poolFragmentWaste;
        MemStats.ram_lost = 0U;
        for(uint16_t i = 0; i < MAX_SUPPORTED_POOL; i++)
        {
            MemStats.ram_lost += MemStats.ram_lost_per_pool[i];
        }
    }
    else
    {
        assert(0);
    }
}

#ifdef MEM_MANAGER_BENCH
static uint8_t systick_in_use = 0U;

static void MEM_StartSysTick()
{
    /* check if SysTick is in use */
    if(SysTick->CTRL != 0U)
    {
        systick_in_use = 1U;
    }
    else /* not in use, we can reset the counter */
    {
        /* set reload register to max value*/
        SysTick->LOAD  = SysTick_LOAD_RELOAD_Msk;
        /* Load the SysTick Counter Value */
        SysTick->VAL   = 0U;
        /* Enable SysTick Timer only */
        SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                         SysTick_CTRL_ENABLE_Msk;
    }
    /* waiting for reload */
    while(SysTick->VAL == 0);
}

static uint32_t MEM_GetSysTick()
{
    /* return the SysTick counter value */
    return (uint32_t)SysTick->VAL;
}

static void MEM_StopSysTick()
{
    if( systick_in_use == 1U )
    {
        systick_in_use = 0U;
    }
    else
    {
        /* disable the SysTick counter */
        SysTick->CTRL  = 0U;
    }
}
#endif /* MEM_MANAGER_BENCH */

#endif /* MEM_STATISTICS */

/* This function frees a memory block given the header of the allocated block. */
static memStatus_t MEM_FreeElmt(listHeader_t *pHeader, uint32_t savedLR)
{
    memStatus_t status = MEM_SUCCESS_c;
    pools_t *pParentPool = (pools_t *)pHeader->pParentPool;
    if( pHeader->link.list != NULL )
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
        status = MEM_FREE_ERROR_c;
    }
    else
    {
        gFreeMessagesCount++;

        (void)ListAddTail((listHandle_t)&pParentPool->anchor, (listElementHandle_t)&pHeader->link);
        pParentPool->allocatedBlocks--;

#ifdef MEM_STATISTICS
        MEM_ASSERT(pParentPool->poolStatistics.allocatedBlocks > 0U);
        pParentPool->poolStatistics.allocatedBlocks--;
#endif /*MEM_STATISTICS*/

#ifdef MEM_TRACKING
        listHeader_t *buffer = pHeader+1;
        (void)MEM_Track(buffer, MEM_TRACKING_FREE_c, savedLR, 0, NULL);
#endif /*MEM_TRACKING*/
    }

    return status;
}

#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
/* This function updates the memSourceTrack[] array by setting/clearing the bit corresponding to the
index of the selected block (index beeing determined by using address of block, size of block and size of list header.)*/
static void MEM_UpdateMemSourceTracking(uint32_t address, pools_t *pPools, bool_t is_allocate, uint8_t *allocIdx)
{
    uint8_t array_idx = 0;  /*Indicates which array element will hold the allocation/free information (1 bit). */
    uint8_t word_idx  = 0;  /*Indicates which bit position in the above selected array element will hold the allocation/free information (1 bit). */

    /* Update the allocation index using address = base_address of pool of given size - block_address(with header).
    Idea here is to find the index of the block to allocate/free among the pool of buffers of tehe selected size. */
    *allocIdx += (address / (pPools->blockSize+sizeof(listHeader_t)));

    /* Determine the position of the bit in the array to update. */
    array_idx = *allocIdx / MEM_SRC_TRACK_ELMT_SIZE;
    word_idx  = *allocIdx - (array_idx*MEM_SRC_TRACK_ELMT_SIZE);

    if (!is_allocate)
    {
        if (memSourceTrack[array_idx] & (1 << word_idx))
        {
            memSourceTrack[array_idx] &= ~(1 << word_idx);
        }
    }
    else
    {
        memSourceTrack[array_idx] |= (1 << word_idx);
    }
}
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

#if defined(MEM_STATISTICS_INTERNAL)
static void MEM_ReportMemStats(void)
{
    MEM_DBG_LOG("**************** MEM STATS REPORT **************");
    MEM_DBG_LOG("Nb Alloc:                  %d",       MemStats.nb_alloc);
    MEM_DBG_LOG("Current RAM allocated:     %d bytes", MemStats.ram_allocated);
    MEM_DBG_LOG("Peak RAM allocated:        %d bytes", MemStats.peak_ram_allocated);
    for(uint16_t i = 0; i < MAX_SUPPORTED_POOL; i++)
    {
        MEM_DBG_LOG("PoolID %d RAM lost:         %d bytes", i, MemStats.ram_lost_per_pool[i]);
    }
    MEM_DBG_LOG("Current RAM lost:          %d bytes", MemStats.ram_lost);
    MEM_DBG_LOG("Peak RAM lost:             %d bytes", MemStats.peak_ram_lost);
#ifdef MEM_MANAGER_BENCH
    MEM_DBG_LOG("************************************************");
    MEM_DBG_LOG("********* MEM MANAGER BENCHMARK REPORT *********");
    MEM_DBG_LOG("Last Alloc Time:           %d us",    MemStats.last_alloc_time);
    MEM_DBG_LOG("Average Alloc Time:        %d us",    MemStats.average_alloc_time);
    MEM_DBG_LOG("Peak Alloc Time:           %d us",    MemStats.peak_alloc_time);
#endif /* MEM_MANAGER_BENCH */
    MEM_DBG_LOG("************************************************");
}
#endif

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

#if defined(MEM_STATISTICS_INTERNAL)
    MEM_InitMemStats(&MemStats);
#endif

  for(;;)
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

    while(poolN != 0U)
    {
      /* Add block to list of free memory. */
      (void)ListAddTail((listHandle_t)&pPools->anchor, (listElementHandle_t)&((listHeader_t *)(uint32_t)pHeap)->link);
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
    pPools->nextBlockSize = (pPoolInfo+1)->blockSize;

    /* Set the pointer to the first memory pool with specific ID */
    if ((pPoolInfo->poolId < NumberOfElements(memPoolsTopAdd)) &&
        (memPoolsTopAdd[pPoolInfo->poolId] == NULL))
    {
        memPoolsTopAdd[pPoolInfo->poolId] = pPools;
#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
        memPoolHeapBaseAdd[pPoolInfo->poolId] = (uint32_t)(pPools->anchor.head);
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
    }

    if(pPools->nextBlockSize == 0U)
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

    for(;;)
    {
        if(size <= pPools->blockSize)
        {
            pTotalCount += ListGetSize((listHandle_t)&pPools->anchor);
        }

        if(pPools->nextBlockSize == 0U)
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
             It will call MEM_BufferAllocWithId.
* \param[in] numBytes - Size of buffer to allocate.
* \param[in] poolId - The ID of the pool where to search for a free buffer.
*
* \return Pointer to the allocated buffer, NULL if failed.
*
* \pre Memory manager must be previously initialized.
*
********************************************************************************** */

void* MEM_BufferAllocate(uint32_t numBytes, uint8_t  poolId)
{
    return MEM_BufferAllocWithId(numBytes, poolId, (uint32_t*) __get_LR());
}

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
void* MEM_BufferAllocWithId(uint32_t numBytes, uint8_t  poolId, void *pCaller)
{
#ifdef MEM_TRACKING
    /* Save the Link Register */
    volatile uint32_t savedLR = (uint32_t) __get_LR();
#endif
#if defined(MEM_TRACKING) || defined(MEM_DEBUG_OUT_OF_MEMORY) || defined(MEM_STATISTICS)
    uint16_t requestedSize = (uint16_t)numBytes;
#endif /*MEM_TRACKING*/

#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
    uint8_t     sourceId = GET_SOURCE_ID(poolId);
    uint32_t    baseAddress;
    uint8_t     allocIdx = 0;

    poolId = GET_POOL_ID(poolId);
    baseAddress = memPoolHeapBaseAdd[poolId];
    if (sourceId == LL_SOURCE_ID)
    {
        ll_pool_id = poolId;
    }
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */

    pools_t *pPools;
    listHeader_t *pBlock = NULL;

    if (poolId < NumberOfElements(memPoolsTopAdd))
    {
        pPools = memPoolsTopAdd[poolId];
    }
    else
    {
        pPools = memPools;
    }

    OSA_InterruptDisable();

#ifdef MEM_STATISTICS
    bool_t allocFailure = FALSE;
#ifdef MEM_MANAGER_BENCH
    uint32_t START_TIME = 0U, STOP_TIME = 0U, ALLOC_TIME = 0U;
    MEM_StartSysTick();
    START_TIME = MEM_GetSysTick();
#endif /* MEM_MANAGER_BENCH */
#endif /* MEM_STATISTICS */

    while(numBytes != 0U)
    {
        if( (numBytes <= pPools->blockSize) && (poolId == pPools->poolId) )
        {
            pBlock = (listHeader_t *)(uint32_t) ListRemoveHead((listHandle_t)&pPools->anchor);

            if(NULL != pBlock)
            {
#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
                if (sourceId == LL_SOURCE_ID)
                {
                    MEM_UpdateMemSourceTracking(((uint32_t)pBlock)-baseAddress, pPools, TRUE, &allocIdx);
                }
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */

                pBlock++;
                gFreeMessagesCount--;
                pPools->allocatedBlocks++;

#ifdef MEM_STATISTICS

#ifdef MEM_MANAGER_BENCH
                STOP_TIME = MEM_GetSysTick();
                MEM_StopSysTick();
                if(START_TIME < STOP_TIME)
                {
                    uint32_t max = SysTick->LOAD;
                    START_TIME += max;
                }
                ALLOC_TIME += START_TIME - STOP_TIME;
#endif /* MEM_MANAGER_BENCH */

                if(gFreeMessagesCount < gFreeMessagesCountMin)
                {
                    gFreeMessagesCountMin = gFreeMessagesCount;
                }

                pPools->poolStatistics.allocatedBlocks++;
                if ( pPools->poolStatistics.allocatedBlocks > pPools->poolStatistics.allocatedBlocksPeak )
                {
                    pPools->poolStatistics.allocatedBlocksPeak = pPools->poolStatistics.allocatedBlocks;
                }
                MEM_ASSERT(pPools->poolStatistics.allocatedBlocks <= pPools->poolStatistics.numBlocks);

#ifdef MEM_TRACKING
                (void)MEM_Track(pBlock, MEM_TRACKING_ALLOC_c, savedLR, requestedSize, pCaller);
#endif /*MEM_TRACKING*/

#ifdef MEM_MANAGER_BENCH
                MEM_StartSysTick();
                START_TIME = MEM_GetSysTick();
#endif /* MEM_MANAGER_BENCH */

#endif /*MEM_STATISTICS*/

                //MEM_DBG_LOG("%d %d %x", poolId, numBytes, pBlock);
                break;
            }
            else
            {
#ifdef MEM_STATISTICS

#ifdef MEM_MANAGER_BENCH
                STOP_TIME = MEM_GetSysTick();
                MEM_StopSysTick();
                if(START_TIME < STOP_TIME)
                {
                    uint32_t max = SysTick->LOAD;
                    START_TIME += max;
                }
                ALLOC_TIME += START_TIME - STOP_TIME;
#endif /* MEM_MANAGER_BENCH */

                if(!allocFailure)
                {
                    pPools->poolStatistics.allocationFailures++;
                    allocFailure = TRUE;
                }

#ifdef MEM_MANAGER_BENCH
                MEM_StartSysTick();
                START_TIME = MEM_GetSysTick();
#endif /* MEM_MANAGER_BENCH */

#endif /*MEM_STATISTICS*/
                if(numBytes > pPools->nextBlockSize)
                {
                    /* There are no free buffer that could hold the requested size. */
                    numBytes = 0U;
                    (void)numBytes;
                    break;
                }
            }
        }

#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
        if (sourceId == LL_SOURCE_ID)
        {
            baseAddress += (pPools->blockSize+sizeof(listHeader_t))*pPools->numBlocks;
            allocIdx += pPools->numBlocks;
        }
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
        /* Try next pool*/
        if(pPools->nextBlockSize != 0U)
        {
            pPools++;
        }
        else
        {
            /* The requested size does not fit in any of the configured buffer sizes (whether or not a free buffer is available). */
            numBytes = 0;
        }
    }

#ifdef MEM_STATISTICS_INTERNAL

#ifdef MEM_MANAGER_BENCH
    STOP_TIME = MEM_GetSysTick();
    MEM_StopSysTick();
    SystemCoreClockUpdate();
    if(START_TIME < STOP_TIME)
    {
        uint32_t max = SysTick->LOAD;
        START_TIME += max;
    }
    ALLOC_TIME += START_TIME - STOP_TIME;
    ALLOC_TIME = ALLOC_TIME / (SystemCoreClock / 1000000U);
    MEM_BufferAllocateMemStats((void*)pBlock, ALLOC_TIME, requestedSize);
#else
    MEM_BufferAllocateMemStats((void*)pBlock, 0U, requestedSize);
#endif /* MEM_MANAGER_BENCH */

    if( 0U == (MemStats.nb_alloc % NB_ALLOC_REPORT_THRESHOLD))
    {
        MEM_ReportMemStats();
    }
#endif /* MEM_STATISTICS */

#ifdef MEM_DEBUG_OUT_OF_MEMORY
    if (numBytes == 0U)
    {
        panic( 0, (uint32_t)MEM_BufferAllocWithId, 0, 0);
    }
#endif

    OSA_InterruptEnable();
    return (void*)pBlock;
}

#if 0
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
    memStatus_t status;

    if( pBuff == NULL )
    {
        status = MEM_FREE_ERROR_c;
    }
    else
    {
        pHeader = (listHeader_t *)pBuff-1;

        if( ((uint8_t*)pHeader < (uint8_t*)memHeap) || ((uint8_t*)pHeader > ((uint8_t*)memHeap + sizeof(memHeap))) )
        {
#ifdef MEM_DEBUG_INVALID_POINTERS
            panic( 0, (uint32_t)MEM_BufferFree, 0, 0);
#endif
            status = MEM_FREE_ERROR_c;
        }
        else
        {
            status = MEM_BufferFree(pBuff);
        }
    }
    return status;
}
#endif /* (defined MULTICORE_MEM_MANAGER) && ((MULTICORE_APPLICATION_CORE) || (MULTICORE_CONNECTIVITY_CORE)) */
#endif

memStatus_t MEM_BufferFree
(
void* buffer /* IN: Block of memory to free*/
)
{
    memStatus_t status = MEM_SUCCESS_c;
#ifdef MEM_TRACKING
    /* Save the Link Register */
    volatile uint32_t savedLR = (uint32_t) __get_LR();
#endif /*MEM_TRACKING*/

    listHeader_t *pHeader;
    pools_t *pParentPool;
    pools_t *pool;

#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
    uint32_t    baseAddress;
    uint8_t     allocIdx = 0;

    baseAddress = memPoolHeapBaseAdd[ll_pool_id];
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */

    //MEM_DBG_LOG("%x", buffer);

    if( buffer == NULL )
    {
        status = MEM_FREE_ERROR_c;
    }
    else
    {
#if defined(MEM_STATISTICS_INTERNAL)
        MEM_BufferFreeMemStats(buffer);
#endif /* MEM_STATISTICS */

        pHeader = (listHeader_t *)buffer-1;

        if( ((uint8_t*)pHeader < (uint8_t*)memHeap) || ((uint8_t*)pHeader > ((uint8_t*)memHeap + sizeof(memHeap))) )
        {
#if (defined MULTICORE_MEM_MANAGER) && (MULTICORE_CONNECTIVITY_CORE)
            status = MEM_BufferFreeOnMaster(buffer);
#elif (defined MULTICORE_MEM_MANAGER) && (MULTICORE_APPLICATION_CORE)
            status = MEM_BufferFreeOnSlave(buffer);
#else
#ifdef MEM_DEBUG_INVALID_POINTERS
            panic( 0, (uint32_t)MEM_BufferFree, 0, 0);
#endif
            status = MEM_FREE_ERROR_c;
#endif
        }
        else
        {
            OSA_InterruptDisable();

            pParentPool = (pools_t *)pHeader->pParentPool;
            pool = memPools;

            for(;;)
            {
                if (pParentPool == pool)
                {
                    break;
                }
#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
                if (ll_pool_id == pool->poolId)
                {
                    baseAddress += (pool->blockSize+sizeof(listHeader_t))*pool->numBlocks;
                    allocIdx += pool->numBlocks;
                }
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
                if(pool->nextBlockSize == 0U)
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
                    status = MEM_FREE_ERROR_c;
                    break;
                }
                pool++;
            }

            if (MEM_SUCCESS_c == status)
            {
#ifdef MEM_TRACKING
                status = MEM_FreeElmt(pHeader, savedLR);
#else /* MEM_TRACKING */
                status = MEM_FreeElmt(pHeader, 0);
#endif /* MEM_TRACKING */

#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
                if (ll_pool_id == pool->poolId)
                {
                    MEM_UpdateMemSourceTracking(((uint32_t)&pHeader->link)-baseAddress, pool, FALSE, &allocIdx);
                }
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
                OSA_InterruptEnable();
            }
        }
    }
    return status;
}


/*! *********************************************************************************
* \brief     Frees all allocated blocks by selected source and in selected pool.
*
* \param[in] poolId - Selected pool Id (4 LSBs of poolId parameter) and selected
*                     source Id (4 MSBs of poolId parameter).
*
* \return MEM_SUCCESS_c if deallocation was successful, MEM_FREE_ERROR_c if not.
*
*
********************************************************************************** */
memStatus_t MEM_BufferFreeAllWithId
(
    uint8_t poolId
)
{
#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
#ifdef MEM_TRACKING
    /* Save the Link Register */
    volatile uint32_t savedLR = (uint32_t) __get_LR();
#endif /* MEM_TRACKING */
    memStatus_t status = MEM_SUCCESS_c;
    uint32_t numBytes = 0xff;

    uint8_t     sourceId = GET_SOURCE_ID(poolId);
    poolId = GET_POOL_ID(poolId);

    uint32_t    baseAddress = memPoolHeapBaseAdd[poolId];
    uint8_t     allocIdx = 0;
    pools_t     *pPools;

    if (poolId < NumberOfElements(memPoolsTopAdd))
    {
        pPools = memPoolsTopAdd[poolId];
    }
    else
    {
        pPools = memPools;
    }

    if (sourceId != 0)
    {
        OSA_InterruptDisable();
        while(numBytes != 0U)
        {
            /* Pool matches the pool id of the block to be freed and not all blocks
            have been freed in that buffer pool. */
            if ((poolId == pPools->poolId) && (pPools->allocatedBlocks))
            {
                uint8_t array_idx = allocIdx / MEM_SRC_TRACK_ELMT_SIZE;
                uint8_t word_idx  = allocIdx - (array_idx*MEM_SRC_TRACK_ELMT_SIZE);

                /* Start tracking in the selected buffer pool which block allocated by LL was not freed. */
                for (int i=0; i<pPools->numBlocks; i++)
                {
                    if (memSourceTrack[array_idx] & (1 << word_idx))
                    {
                        /* Determine the address of the block allocated by LL. */
                        listHeader_t *pHeader = (listHeader_t*) (baseAddress+i*(pPools->blockSize+sizeof(listHeader_t)));
#ifdef MEM_TRACKING
                        status = MEM_FreeElmt(pHeader, savedLR);
#else /* MEM_TRACKING */
                        status = MEM_FreeElmt(pHeader, 0);
#endif /* MEM_TRACKING */
                        memSourceTrack[array_idx] &= ~(1 << word_idx);
                    }
                    if (pPools->allocatedBlocks == 0)
                    {
                        /* Leave as soon as all blocks for the selected pool of buffer of size = pPools->blockSize have been freed. */
                        break;
                    }

                    /* Handles the case where word_idx exceeds the boundary of memSourceTrack[] array element to
                    point to next valid bit position in memSourceTrack[]*/
                    word_idx++;
                    if (word_idx >= MEM_SRC_TRACK_ELMT_SIZE)
                    {
                       word_idx -= MEM_SRC_TRACK_ELMT_SIZE;
                       array_idx++;
                    }
                }
            }

            baseAddress += (pPools->blockSize+sizeof(listHeader_t))*pPools->numBlocks;
            allocIdx += pPools->numBlocks;

            if(pPools->nextBlockSize != 0U)
            {
                pPools++;
            }
            else
            {
                /* The requested size does not fit in any of the configured buffer sizes (whether or not a free buffer is available). */
                numBytes = 0;
            }
        }
        OSA_InterruptEnable();
    }

#else /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
    memStatus_t status = MEM_FREE_ERROR_c;
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
 return status;
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
    uint16_t size = 0U;
    if( buffer != NULL )
    {
        size = ((pools_t *)((listHeader_t *)buffer-1)->pParentPool)->blockSize;
    }

    return size;
}

void* MEM_BufferRealloc(void* buffer, uint32_t new_size)
{
    void* realloc_buffer = NULL;
    uint16_t block_size = 0U;

    assert(new_size <= 0x0000FFFFU); /* size will be casted to 16 bits */

    if(new_size == 0U)
    {
        /* new requested size is 0, free old buffer */
        (void)MEM_BufferFree(buffer);
        realloc_buffer = NULL;
    }
    else if(buffer == NULL)
    {
        /* input buffer is NULL simply allocate a new buffer and return it */
        realloc_buffer = MEM_BufferAllocWithId(new_size, 0U, (uint32_t*) __get_LR());
    }
    else
    {
        block_size = MEM_BufferGetSize(buffer);

        if((uint16_t)new_size <= block_size)
        {
            /* current buffer is large enough for the new requested size
               we can still use it */
            realloc_buffer = buffer;
        }
        else
        {
            /* not enough space in the current block, creating a new one */
            realloc_buffer = MEM_BufferAllocWithId(new_size, 0U, (uint32_t*) __get_LR());

            if(realloc_buffer != NULL)
            {
                /* copy input buffer data to new buffer */
                (void)FLib_MemCpy(realloc_buffer, buffer, (uint32_t)block_size);

                /* free old buffer */
                (void)MEM_BufferFree(buffer);
            }
        }
    }

    return realloc_buffer;
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */

#ifdef MEM_TRACKING

/*
 * MemManager Buffer Pool Optimizer
 *
 * Buffer usage in an application is a dynamic and fluid activity that can change
 * depending on a number of factors. Allocation sizes and peak allocations at
 * any point in time are two parameters the optimizer is attempting to determine.
 * The optimizer will use this information, on a per-allocation basis, to generate
 * an optimal pool configuration to support the peak allocation needs of the
 * application.
 *
 * To accomplish this, the optimizer tracks multiple views of memory usage at the
 * same time. These views are different size resolutions of all outstanding buffer
 * requests from 512B buckets down to 8byte buckets. This is done to have a better
 * understanding of overall peak buffer usage as well as size distribution of the
 * allocation requests. This also provides an opportunity to take advantage of
 * folding a buffer size request into the next larger buffer pool.
 *
 * Folding into a larger buffer pool provides memory size conservation opportunities
 * because there are per-buffer and per-pool overhead that can be saved in certain
 * cases.
 *
 * To use the optimizer, configure MEM_TRACKING and set POOL_TO_OPTIMIZE and
 * MAX_SUPPORTED_BUFFER_SIZE. Run the application through various usage scenarios.
 * The usage should be robust to ensure a good level of data collection over a
 * reasonable amount of time. Force a breakpoint and inspect the data gathering
 * structures.
 *
 * optimumPoolCfg array holds the last computed optimal pool configuration and can
 * be used to define the pool definition (AppPoolsDetails_c). For additional insight,
 * buffer_peak<n> arrays provide a peak view of the buffer allocation into various
 * bucket sizes with the finest granularity being buffer_peak8.
 *
 * The buffer_peak<n> arrays are provide a view of the data that allows one to zoom
 * in and out on the details. At the highest level (buffer_peak512), there is likely
 * only one bucket which indicates that maximum number of packets allocated at any
 * point of time during the profile run. A configuration could be made to create a
 * pool of this size using the peak value and the system would likely run fine as
 * 32 byte allocations would be satisfied with a 512byte buffer. To get a better
 * view of buffer needs, zooming in to finer granularity of bucket sizing will help
 * identify a better pool configuration.
 *
 * Because each bucket view represents the same state of the system at each allocation
 * point, any of the peak configurations would work for configuring the pool buffers.
 * As such, at each allocation, the profiler will pass through the data to calculate
 * the most RAM efficient pool configuration out of the various peak arrays and store
 * that into optimumPoolCfg.
 *
 * The pool configuration can be hand tuned to add a 10-20% margin by increasing some
 * (or all) of the pool bucket number configurations.
 *
 * Caveat: There is a time domain aspect to this analysis that the logic doesn't
 * capture. For example, there may be a single 400byte allocation request that occurs
 * within the time domain of a single function (allocated, used, and then freed). This
 * allocation will be recorded by the profiler. Later, a different 200 byte allocation
 * may occur which is also recorded. This allocation could have been folded into the
 * 400 byte allocation but the data tracking here doesn't really know this and will
 * result in suggesting a pool configuration with both of these buffers instead of one
 * 400 byte buffer for both. In general, the optimized pool configuration probably
 * saved more than enough memory for this to really matter but if it does, some
 * additional debugging can be done to understand the two allocations and their locality
 * in time for hand tuning of the profile configuration.
 *
 */

typedef struct optPool_tag
{
    uint16_t    size;
    uint16_t    cnt;
} optPool_t;

#define MAX_POOLS 32U
#define POOL_OVERHEAD        (sizeof(pools_t))
#define POOL_BUFFER_OVERHEAD (sizeof(listHeader_t))

/*
 * TODO: current implementation only assumes 512 byte buffers
 * as the largest size. Should add 1024 and 2048 but make them
 * conditional?
 */
#define MAX_8BYTE_SLICES     ((MAX_SUPPORTED_BUFFER_SIZE >> 3)+1)

static optPool_t optimumPoolCfg[MAX_POOLS];

static optPool_t tmp_pool[MAX_POOLS];
static uint32_t optimumPoolSize;

static uint8_t buffer_alloc8[MAX_8BYTE_SLICES];      /* Current outstanding allocations */
static uint8_t buffer_alloc16[MAX_8BYTE_SLICES/2];   /* Current outstanding allocations */
static uint8_t buffer_alloc32[MAX_8BYTE_SLICES/4];   /* Current outstanding allocations */
static uint8_t buffer_alloc64[MAX_8BYTE_SLICES/8];   /* Current outstanding allocations */
static uint8_t buffer_alloc128[MAX_8BYTE_SLICES/16]; /* Current outstanding allocations */
static uint8_t buffer_alloc256[MAX_8BYTE_SLICES/32]; /* Current outstanding allocations */
static uint8_t buffer_alloc512[MAX_8BYTE_SLICES/64]; /* Current outstanding allocations */
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
static uint8_t buffer_alloc1024[MAX_8BYTE_SLICES/128];   /* Current outstanding allocations */
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
static uint8_t buffer_alloc2048[MAX_8BYTE_SLICES/256];   /* Current outstanding allocations */
#endif

static uint8_t buffer_peak8[MAX_8BYTE_SLICES];       /* Peak outstanding allocations (per 8Byte bucket) */
static uint8_t buffer_peak16[MAX_8BYTE_SLICES/2];    /* Peak outstanding allocations (per 16Byte bucket) */
static uint8_t buffer_peak32[MAX_8BYTE_SLICES/4];    /* Peak outstanding allocations (per 32Byte bucket) */
static uint8_t buffer_peak64[MAX_8BYTE_SLICES/8];    /* Peak outstanding allocations (per 64Byte bucket) */
static uint8_t buffer_peak128[MAX_8BYTE_SLICES/16];  /* Peak outstanding allocations (per 128Byte bucket) */
static uint8_t buffer_peak256[MAX_8BYTE_SLICES/32];  /* Peak outstanding allocations (per 256Byte bucket) */
static uint8_t buffer_peak512[MAX_8BYTE_SLICES/64];  /* Peak outstanding allocations (per 512Byte bucket) */
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
static uint8_t buffer_peak1024[MAX_8BYTE_SLICES/128];    /* Peak outstanding allocations (per 1024Byte bucket) */
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
static uint8_t buffer_peak2048[MAX_8BYTE_SLICES/256];    /* Peak outstanding allocations (per 2048Byte bucket) */
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
static uint32_t buildPoolFromPeakArray(optPool_t *opool, uint8_t *peakArray, uint16_t arraySize, uint16_t bucketSize)
{
    uint32_t    totalSize = 0U;
    uint16_t    wsize = bucketSize/8U; // 8 byte window size

    uint16_t i, pcnt = 0U;
    int16_t widx;

    for (i=0; i<arraySize; i++)
    {
        if (peakArray[i] != 0U)
        {
            if (pcnt == MAX_POOLS)
            {
                totalSize = 0xFFFFFFFFU;
                break;
            }
            // Find max 8byte size in bucket range
            for (widx = (int16_t)wsize - 1; widx >= 0; widx--)
            {
                if ((buffer_peak8[i*wsize + (uint16_t)widx]) != 0U)
                {
                    opool->size = (i*wsize + (uint16_t)widx + 1U) * 8U;
                    break;
                }
            }
            opool->cnt = peakArray[i];
            totalSize += (uint32_t)(POOL_OVERHEAD) + (opool->cnt * (opool->size + (uint32_t)POOL_BUFFER_OVERHEAD));
            pcnt++;
            opool++;
        }
    }
    for(;pcnt<MAX_POOLS;pcnt++)
    {
        // clear out the rest of the pool structure
        opool->cnt = 0;
        opool++;
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

    optimumPoolSize = buildPoolFromPeakArray(&optimumPoolCfg[0], &buffer_peak512[0], (uint16_t)sizeof(buffer_peak512), 512);

#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024)
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak1024[0], (uint16_t)sizeof(buffer_peak1024), 1024);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048)
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak2048[0], (uint16_t)sizeof(buffer_peak2048), 2048);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
#endif
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak256[0], (uint16_t)sizeof(buffer_peak256), 256);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak128[0], (uint16_t)sizeof(buffer_peak128), 128);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak64[0], (uint16_t)sizeof(buffer_peak64), 64);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak32[0], (uint16_t)sizeof(buffer_peak32), 32);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak16[0], (uint16_t)sizeof(buffer_peak16), 16);
    if (tsize < optimumPoolSize)
    {
        optimumPoolSize = tsize;
        FLib_MemCpy(&optimumPoolCfg, &tmp_pool, sizeof(optimumPoolCfg));
    }
    tsize = buildPoolFromPeakArray(&tmp_pool[0], &buffer_peak8[0], (uint16_t)sizeof(buffer_peak8), 8);
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
        index = (size-1U)>>3U;

        if(alloc == MEM_TRACKING_ALLOC_c)
        {
            buffer_alloc8[index]++;
            if (buffer_alloc8[index] > buffer_peak8[index])
            {
                buffer_peak8[index] = buffer_alloc8[index];
            }
            buffer_alloc16[index/2U]++;
            if (buffer_alloc16[index/2U] > buffer_peak16[index/2U])
            {
                buffer_peak16[index/2U] = buffer_alloc16[index/2U];
            }
            buffer_alloc32[index/4U]++;
            if (buffer_alloc32[index/4U] > buffer_peak32[index/4U])
            {
                buffer_peak32[index/4U] = buffer_alloc32[index/4U];
            }
            buffer_alloc64[index/8U]++;
            if (buffer_alloc64[index/8U] > buffer_peak64[index/8U])
            {
                buffer_peak64[index/8U] = buffer_alloc64[index/8U];
            }
            buffer_alloc128[index/16U]++;
            if (buffer_alloc128[index/16U] > buffer_peak128[index/16U])
            {
                buffer_peak128[index/16U] = buffer_alloc128[index/16U];
            }
            buffer_alloc256[index/32U]++;
            if (buffer_alloc256[index/32U] > buffer_peak256[index/32U])
            {
                buffer_peak256[index/32U] = buffer_alloc256[index/32U];
            }
            buffer_alloc512[index/64U]++;
            if (buffer_alloc512[index/64U] > buffer_peak512[index/64U])
            {
                buffer_peak512[index/64U] = buffer_alloc512[index/64U];
            }
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024U)
            buffer_alloc1024[index/128U]++;
            if (buffer_alloc1024[index/128U] > buffer_peak1024[index/128U])
            {
                buffer_peak1024[index/128U] = buffer_alloc1024[index/128U];
            }
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048U)
            buffer_alloc2048[index/256U]++;
            if (buffer_alloc2048[index/256U] > buffer_peak2048[index/256U])
            {
                buffer_peak2048[index/256U] = buffer_alloc2048[index/256U];
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
            buffer_alloc16[index/2U]--;
            buffer_alloc32[index/4U]--;
            buffer_alloc64[index/8U]--;
            buffer_alloc128[index/16U]--;
            buffer_alloc256[index/32U]--;
            buffer_alloc512[index/64U]--;
#if (MAX_SUPPORTED_BUFFER_SIZE >= 1024U)
            buffer_alloc1024[index/128U]--;
#endif
#if (MAX_SUPPORTED_BUFFER_SIZE >= 2048U)
            buffer_alloc2048[index/256U]--;
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
  uint8_t status = 1U;
#ifdef MEM_STATISTICS
  poolStat_t * poolStatistics = (poolStat_t *)&((pools_t *)((void *)((listElementHandle_t)((void *)(block-1)->pParentPool))))->poolStatistics;
#endif

  uint16_t  poolId =(uint16_t)((pools_t *)((void *)((listElementHandle_t)((void *)(block-1)->pParentPool))))->poolId;

  for( i=0; i<mTotalNoOfMsgs_c; i++ )
  {
      if( block == memTrack[i].blockAddr )
      {
          pTrack = &memTrack[i];
          break;
      }
  }

  if( (NULL == pTrack) || (pTrack->allocStatus == alloc))
  {
#ifdef MEM_DEBUG
      panic( 0, (uint32_t)MEM_Track, 0, 0);
#endif
      status = 0U;
  }
  else
  {
      pTrack->allocStatus = alloc;
      pTrack->pCaller = (void*)((uint32_t *)((uint32_t)((uint32_t *)pCaller) & 0x7FFFFFFFUL));

      if(alloc == MEM_TRACKING_ALLOC_c)
      {
        pTrack->requested_size = requestedSize;

        profileBufferUsage(alloc, requestedSize, poolId);

        pTrack->fragmentWaste = pTrack->blockSize - requestedSize;
        pTrack->allocCounter++;
        pTrack->allocAddr = (void *)((uint32_t *)address);
        if( 0U != ((uint32_t)((uint32_t *)pCaller) & 0x80000000U) )
        {
            pTrack->timeStamp = 0xFFFFFFFFU;
        }
        else
        {
            pTrack->timeStamp = MEM_GetTimeStamp();
        }
#ifdef MEM_STATISTICS
        gTotalFragmentWaste += pTrack->fragmentWaste;
        if(gTotalFragmentWaste > gMaxTotalFragmentWaste)
        {
            gMaxTotalFragmentWaste = gTotalFragmentWaste;
            FLib_MemCpy(&memPoolsSnapShot[0], &memPools[0], sizeof(memPools));
        }

        poolStatistics->poolFragmentWaste += pTrack->fragmentWaste;
        if(poolStatistics->poolFragmentWaste > poolStatistics->poolFragmentWastePeak)
        {
          poolStatistics->poolFragmentWastePeak = poolStatistics->poolFragmentWaste;
        }
        if(pTrack->fragmentWaste < poolStatistics->poolFragmentMinWaste)
        {
          poolStatistics->poolFragmentMinWaste = pTrack->fragmentWaste;
        }
        if(pTrack->fragmentWaste > poolStatistics->poolFragmentMaxWaste)
        {
          poolStatistics->poolFragmentMaxWaste = pTrack->fragmentWaste;
        }
        if(poolStatistics->allocatedBlocks == poolStatistics->numBlocks)
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
        pTrack->freeAddr = (void *)((uint32_t *)address);
        pTrack->timeStamp = 0;
      }
  }
  return status;
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
    uint8_t result = 0U,status = 0U;

    if( (p < (uint8_t*)memHeap) || (p > ((uint8_t*)memHeap + sizeof(memHeap))) )
    {
        status = 0U;
    }
    else
    {
        while( 0U != pPollInfo->blockSize )
        {
            blockBytes = (uint32_t)pPollInfo->blockSize + (uint32_t)sizeof(listHeader_t);
            poolBytes  = blockBytes * pPollInfo->poolSize;

            /* Find the correct message pool */
            if( (p >= memAddr) && (p < memAddr + poolBytes) )
            {
                /* Check if the size to copy is greater then the size of the current block */
                if( size > pPollInfo->blockSize )
                {
#ifdef MEM_DEBUG
                    panic(0,0,0,0);
#endif
                    status = 1U;
                    break;
                }

                /* Find the correct memory block */
                for( i=0; i<pPollInfo->poolSize; i++ )
                {
                    if( (p >= memAddr) && (p < memAddr + blockBytes) )
                    {
                        result = 1U;
                        if( p + size > memAddr + blockBytes )
                        {
#ifdef MEM_DEBUG
                            panic(0,0,0,0);
#endif
                            status = 1U;
                            break;
                        }
                        else
                        {
                            status = 0U;
                            break;
                        }
                    }

                    memAddr += blockBytes;
                }

                if ( 1U == result)
                {
                    break;
                }
            }

            /* check next pool */
            memAddr += poolBytes;
            pPollInfo++;
        }
    }
    return status;
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

    if( (currentTime - lastTimestamp) >= MEM_CheckMemBufferInterval_c )
    {
        lastTimestamp = currentTime;
        j = 0;

        for( i = 0; i < mTotalNoOfMsgs_c; i++ )
        {
            pTrack = &memTrack[i];

            /* Validate the pParent first */
            pParentPool = (((listHeader_t *)(pTrack->blockAddr))-1)->pParentPool;
            if(pParentPool != &memPools[j])
            {
                if(j < NumberOfElements(memPools))
                {
                    j++;
                    if(pParentPool != &memPools[j])
                    {
                        panic(0,0,0,0);
                    }
                }
                else
                {
                    panic(0,0,0,0);
                }
            }

            /* Check if it should be freed  */
            OSA_InterruptDisable();
            memTrackingStatus_t allocStatus = pTrack->allocStatus;
            uint32_t timeStamp = pTrack->timeStamp;
            if((pTrack->timeStamp != 0xffffffffU ) &&
               (MEM_TRACKING_ALLOC_c == allocStatus) &&
               (currentTime > timeStamp))
            {
                t = currentTime - pTrack->timeStamp;
                if( t > MEM_CheckMemBufferThreshold_c )
                {
                    mpTrackTbl[trackCount++] = pTrack;
                    if(trackCount == NUM_OF_TRACK_PTR)
                    {
                        (void)mpTrackTbl; /* remove compiler warnings */
                        panic(0,0,0,0);
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
    void *pData = MEM_BufferAllocForever (len* val, 0);
    if (pData != NULL)
    {
        FLib_MemSet (pData, 0, len* val);
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

uint32_t MEM_GetHeapUpperLimit(void)
{
    return (uint32_t)0;
}

#endif

