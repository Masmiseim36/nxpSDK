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

#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
#include "fsl_debug_console.h"
#endif


#if defined(gMemManagerLight) && (gMemManagerLight == 1)

#ifndef cMemManagerLightReuseFreeBlocks
#define cMemManagerLightReuseFreeBlocks         1
#endif

#if defined(cMemManagerLightReuseFreeBlocks) && ( cMemManagerLightReuseFreeBlocks > 0 )
/* because a more restrictive on the size of the free blocks when cMemManagerLightReuseFreeBlocks
   is set, we need to enable a garbage collector to clean up the free block when possible  */
#ifndef gMemManagerLightFreeBlocksCleanUp
#define gMemManagerLightFreeBlocksCleanUp       1
#endif
#endif

#ifndef gMemManagerLightGuardsCheckEnable
#define gMemManagerLightGuardsCheckEnable       0
#endif

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

#define _block_size_ (sizeof(listHeader_t)+
#define _number_of_blocks_ ) *
#define _eol_  +
#define _pool_id_(a)

#define heapSize_c (PoolsDetails_c 0)

#define MEMMANAGER_BLOCK_INVALID        (uint16_t)0x0            /* Used to remove a block in the heap - debug only */
#define MEMMANAGER_BLOCK_FREE           (uint16_t)0xBA00     /* Mark a previous allocated block as free         */
#define MEMMANAGER_BLOCK_USED           (uint16_t)0xBABE     /* Mark the block as allocated                     */

#define BLOCK_HDR_SIZE                  (ROUNDUP_WORD(sizeof(blockHeader_t)))

#define ROUNDUP_WORD(__x)               ( ((((__x) - 1U)& ~0x3U) + 4U )& 0XFFFFFFFFU )

#define BLOCK_HDR_PREGUARD_SIZE         28U
#define BLOCK_HDR_PREGUARD_PATTERN      0x28U
#define BLOCK_HDR_POSTGUARD_SIZE        28U
#define BLOCK_HDR_POSTGUARD_PATTERN     0x39U

#if defined(gMemManagerLightGuardsCheckEnable) && (gMemManagerLightGuardsCheckEnable == 1)
#define gMemManagerLightAddPreGuard     1
#define gMemManagerLightAddPostGuard    1
#endif

#ifndef gMemManagerLightAddPreGuard
#define gMemManagerLightAddPreGuard     0
#endif

#ifndef gMemManagerLightAddPostGuard
#define gMemManagerLightAddPostGuard    0
#endif

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef struct blockHeader_s {
#if defined(gMemManagerLightAddPreGuard) && (gMemManagerLightAddPreGuard == 1)
    uint8_t                preguard[BLOCK_HDR_PREGUARD_SIZE];
#endif
    uint16_t               used;
#if defined(MEM_STATISTICS)
    uint16_t               buff_size;
#endif
    struct blockHeader_s * next;
    struct blockHeader_s * next_free;
    struct blockHeader_s * prev_free;
#ifdef MEM_TRACKING
    void                 * first_alloc_caller;
    void                 * second_alloc_caller;
#endif
#if defined(gMemManagerLightAddPostGuard) && (gMemManagerLightAddPostGuard == 1)
    uint8_t                postguard[BLOCK_HDR_POSTGUARD_SIZE];
#endif
}  blockHeader_t;

typedef struct freeBlockHeaderList_s {
    struct blockHeader_s* head;
    struct blockHeader_s* tail;
} freeBlockHeaderList_t;

typedef union void_ptr_tag
{
    uint32_t            raw_address;
    uint32_t*           address_ptr;
    void*               void_ptr;
    blockHeader_t *     block_hdr_ptr;
} void_ptr_t;

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */


/* Allocate memHeap array in the .heap section to ensure the size of the .heap section is large enough
   for the application (from app_preinclude.h)
   However, the real heap used at run time will cover all the .heap section so this area can be bigger
   than the requested heapSize_c - see memHeapEnd */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".heap"
static uint32_t memHeap[heapSize_c/sizeof(uint32_t)];
#elif defined(__GNUC__)
static uint32_t memHeap[heapSize_c/sizeof(uint32_t)]  __attribute__ ((section(".heap")));
#elif defined(__CC_ARM)
static uint32_t memHeap[heapSize_c/sizeof(uint32_t)] __attribute__ ((section(".heap")));
#else
#error "Compiler unknown!"
#endif

extern uint32_t       __HEAP_end__[];
static const uint32_t memHeapEnd         =  (uint32_t)&__HEAP_end__;

static freeBlockHeaderList_t FreeBlockHdrList;

static bool_t initialized = FALSE;

#ifdef MEM_STATISTICS_INTERNAL
static memStats_t       MemStats;
#endif /* MEM_STATISTICS_INTERNAL */

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
    void_ptr_t buffer_ptr;
    void_ptr_t blockHdr_ptr;
    blockHeader_t * BlockHdr;

    /* Using union to fix Misra */
    buffer_ptr.void_ptr = buffer;
    blockHdr_ptr.raw_address = buffer_ptr.raw_address - BLOCK_HDR_SIZE;
    BlockHdr = blockHdr_ptr.block_hdr_ptr;

    /* existing block must have a BlockHdr and a next BlockHdr */
    assert( (BlockHdr != NULL) && (BlockHdr->next != NULL) );

    MemStats.nb_alloc++;
    /* Sort the buffers by size, based on defined thresholds */
    if( requestedSize <= SMALL_BUFFER_SIZE )
    {
        MemStats.nb_small_buffer++;
        UPDATE_PEAK(MemStats.nb_small_buffer, MemStats.peak_small_buffer);
    }
    else if ( requestedSize <= LARGE_BUFFER_SIZE )
    {
        MemStats.nb_medium_buffer++;
        UPDATE_PEAK(MemStats.nb_medium_buffer, MemStats.peak_medium_buffer);
    }
    else
    {
        MemStats.nb_large_buffer++;
        UPDATE_PEAK(MemStats.nb_large_buffer, MemStats.peak_large_buffer);
    }
    /* the RAM allocated is the buffer size and the block header size*/
    MemStats.ram_allocated += (uint16_t)(requestedSize + BLOCK_HDR_SIZE);
    UPDATE_PEAK(MemStats.ram_allocated, MemStats.peak_ram_allocated);

    uint32_t block_size = 0U;
    block_size = (uint32_t)BlockHdr->next - (uint32_t)BlockHdr - BLOCK_HDR_SIZE;

    assert(block_size >= requestedSize);
    /* ram lost is the difference between block size and buffer size */
    MemStats.ram_lost += (uint16_t)(block_size - requestedSize);
    UPDATE_PEAK(MemStats.ram_lost, MemStats.peak_ram_lost);

    UPDATE_PEAK(((uint32_t)FreeBlockHdrList.tail + BLOCK_HDR_SIZE), MemStats.peak_upper_addr);

#ifdef MEM_MANAGER_BENCH
    if(time != 0U)
    {
        /* update mem stats used for benchmarking */
        MemStats.last_alloc_block_size = (uint16_t)block_size;
        MemStats.last_alloc_buff_size = (uint16_t)requestedSize;
        MemStats.last_alloc_time = (uint16_t)time;
        MemStats.total_alloc_time += time;
        MemStats.average_alloc_time = (uint16_t)(MemStats.total_alloc_time / MemStats.nb_alloc);
        UPDATE_PEAK((uint16_t)time, MemStats.peak_alloc_time);
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
    void_ptr_t buffer_ptr;
    void_ptr_t blockHdr_ptr;
    blockHeader_t * BlockHdr;

    /* Use union to fix Misra */
    buffer_ptr.void_ptr = buffer;
    blockHdr_ptr.raw_address = buffer_ptr.raw_address - BLOCK_HDR_SIZE;
    BlockHdr = blockHdr_ptr.block_hdr_ptr;

    /* Existing block must have a next block hdr */
    assert( (BlockHdr != NULL) && (BlockHdr->next != NULL) );

    MemStats.ram_allocated -= (uint16_t)(BlockHdr->buff_size + BLOCK_HDR_SIZE);
    /* Sort the buffers by size, based on defined thresholds */
    if( BlockHdr->buff_size <= SMALL_BUFFER_SIZE )
    {
        MemStats.nb_small_buffer--;
    }
    else if ( BlockHdr->buff_size <= LARGE_BUFFER_SIZE )
    {
        MemStats.nb_medium_buffer--;
    }
    else
    {
        MemStats.nb_large_buffer--;
    }

    uint16_t block_size = 0U;
    block_size = (uint16_t)((uint32_t)BlockHdr->next - (uint32_t)BlockHdr - BLOCK_HDR_SIZE);

    assert(block_size >= BlockHdr->buff_size);
    assert(MemStats.ram_lost >= (block_size - BlockHdr->buff_size));

    /* as the buffer is free, the ram is not "lost" anymore */
    MemStats.ram_lost -= (block_size - BlockHdr->buff_size);
}

#ifdef MEM_MANAGER_BENCH
static uint8_t systick_in_use = 0U;

static void MEM_StartSysTick(void)
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
    while(SysTick->VAL == 0U){;}
}

static uint32_t MEM_GetSysTick(void)
{
    /* return the SysTick counter value */
    return (uint32_t)(SysTick->VAL & SysTick_VAL_CURRENT_Msk);
}

static void MEM_StopSysTick(void)
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
#endif /* MEM_MANGER_BENCH */

#endif /* MEM_STATISTICS_INTERNAL */

#if defined(gMemManagerLightFreeBlocksCleanUp) && (gMemManagerLightFreeBlocksCleanUp == 1)
static void MEM_BufferFreeBlocksCleanUp(blockHeader_t * BlockHdr)
{
    blockHeader_t * NextBlockHdr = BlockHdr->next;
    blockHeader_t * NextFreeBlockHdr = BlockHdr->next_free;

    /* This function shouldn't be called on the last free block */
    assert( BlockHdr < FreeBlockHdrList.tail );

    while(NextBlockHdr == NextFreeBlockHdr)
    {
        if(NextBlockHdr == NULL)
        {
            assert(BlockHdr->next == BlockHdr->next_free);
            assert(BlockHdr->used == MEMMANAGER_BLOCK_FREE);
            /* pool is reached.  All buffers from BlockHdr to the pool are free
               remove all next buffers */
            BlockHdr->next = NULL;
            BlockHdr->next_free = NULL;
            FreeBlockHdrList.tail = BlockHdr;
            break;
        }
        NextBlockHdr = NextBlockHdr->next;
        NextFreeBlockHdr = NextFreeBlockHdr->next_free;
    }
}
#endif /* gMemManagerLightFreeBlocksCleanUp */

#if defined(gMemManagerLightGuardsCheckEnable) && (gMemManagerLightGuardsCheckEnable == 1)
static void MEM_BlockHeaderCheck(blockHeader_t *BlockHdr)
{
    bool_t  ret;

    ret = FLib_MemCmpToVal((const void*)&BlockHdr->preguard, BLOCK_HDR_PREGUARD_PATTERN, BLOCK_HDR_PREGUARD_SIZE);
    if ( ret == 0 )
    {
        MEM_DBG_LOG("Preguard Block Header Corrupted %x", BlockHdr);
    }
    assert(ret);

    ret = FLib_MemCmpToVal((const void*)&BlockHdr->postguard, BLOCK_HDR_POSTGUARD_PATTERN, BLOCK_HDR_POSTGUARD_SIZE);
    if ( ret == 0 )
    {
        MEM_DBG_LOG("Postguard Block Header Corrupted %x", BlockHdr);
    }
    assert(ret);
}

static void MEM_BlockHeaderSetGuards(blockHeader_t *BlockHdr)
{
    FLib_MemSet((void*)&BlockHdr->preguard,  BLOCK_HDR_PREGUARD_PATTERN,  BLOCK_HDR_PREGUARD_SIZE);
    FLib_MemSet((void*)&BlockHdr->postguard, BLOCK_HDR_POSTGUARD_PATTERN, BLOCK_HDR_POSTGUARD_SIZE);
}

#endif

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
    MEM_DBG_LOG("Small buffers:             %d",       MemStats.nb_small_buffer);
    MEM_DBG_LOG("Medium buffers:            %d",       MemStats.nb_medium_buffer);
    MEM_DBG_LOG("Large buffers:             %d",       MemStats.nb_large_buffer);
    MEM_DBG_LOG("Peak small:                %d ",      MemStats.peak_small_buffer);
    MEM_DBG_LOG("Peak medium:               %d ",      MemStats.peak_medium_buffer);
    MEM_DBG_LOG("Peak large:                %d ",      MemStats.peak_large_buffer);
    MEM_DBG_LOG("Current RAM allocated:     %d bytes", MemStats.ram_allocated);
    MEM_DBG_LOG("Peak RAM allocated:        %d bytes", MemStats.peak_ram_allocated);
    MEM_DBG_LOG("Current RAM lost:          %d bytes", MemStats.ram_lost);
    MEM_DBG_LOG("Peak RAM lost:             %d bytes", MemStats.peak_ram_lost);
    MEM_DBG_LOG("Peak Upper Address:        %x",       MemStats.peak_upper_addr);
#ifdef MEM_MANAGER_BENCH
    MEM_DBG_LOG("************************************************");
    MEM_DBG_LOG("********* MEM MANAGER BENCHMARK REPORT *********");
    MEM_DBG_LOG("Last Alloc Time:           %d us",    MemStats.last_alloc_time);
    MEM_DBG_LOG("Last Alloc Block Size:     %d bytes", MemStats.last_alloc_block_size);
    MEM_DBG_LOG("Last Alloc Buffer Size:    %d bytes", MemStats.last_alloc_buff_size);
    MEM_DBG_LOG("Average Alloc Time:        %d us",    MemStats.average_alloc_time);
    MEM_DBG_LOG("Peak Alloc Time:           %d us",    MemStats.peak_alloc_time);
#endif /* MEM_MANAGER_BENCH */
    MEM_DBG_LOG("************************************************");
}
#endif /* MEM_STATISTICS_INTERNAL */

memStatus_t MEM_Init(void)
{
    if(initialized == FALSE) 
    {
        initialized = TRUE;
        /* union to solve Misra 11.3 */
        void_ptr_t ptr;
        ptr.address_ptr = memHeap;
        blockHeader_t * firstBlockHdr;
        firstBlockHdr = ptr.block_hdr_ptr;

        //MEM_DBG_LOG("%x %d\r\n", memHeap, heapSize_c/sizeof(uint32_t));

        /* Init firstBlockHdr as a free block */
        firstBlockHdr->next = NULL;
        firstBlockHdr->used = MEMMANAGER_BLOCK_FREE;
        firstBlockHdr->next_free = NULL;
        firstBlockHdr->prev_free = NULL;

#if defined(MEM_STATISTICS)
        firstBlockHdr->buff_size = 0U;
#endif

        /* Init FreeBlockHdrList with firstBlockHdr */
        FreeBlockHdrList.head = firstBlockHdr;
        FreeBlockHdrList.tail = firstBlockHdr;

#if defined(gMemManagerLightGuardsCheckEnable) && (gMemManagerLightGuardsCheckEnable == 1)
        MEM_BlockHeaderSetGuards(firstBlockHdr);
#endif

#if defined(MEM_STATISTICS_INTERNAL)
        /* Init memory statistics */
        MEM_InitMemStats(&MemStats);
#endif
    }

    return MEM_SUCCESS_c;
}

void* MEM_BufferAllocWithId(uint32_t numBytes, uint8_t  poolId, void *pCaller)
{
#ifdef MEM_TRACKING
    void_ptr_t      BlockHdr_ptr;
#endif
    void_ptr_t      buffer_ptr;

    /* Alloc a buffer */
    buffer_ptr.void_ptr = MEM_BufferAllocate(numBytes, poolId);

#ifdef MEM_TRACKING
    if (buffer_ptr.void_ptr != NULL)
    {
        BlockHdr_ptr.raw_address = buffer_ptr.raw_address - BLOCK_HDR_SIZE;
        /* store caller */
        BlockHdr_ptr.block_hdr_ptr->second_alloc_caller = pCaller;
    }
#else
    NOT_USED(pCaller);
#endif

    return buffer_ptr.void_ptr;
}

void* MEM_BufferAllocate(uint32_t numBytes, uint8_t poolId)
{
    blockHeader_t* FreeBlockHdr         = FreeBlockHdrList.head;
    blockHeader_t* NextFreeBlockHdr     = FreeBlockHdr->next_free;
    blockHeader_t* PrevFreeBlockHdr     = FreeBlockHdr->prev_free;
    blockHeader_t* BlockHdrFound        = NULL;

#if defined(cMemManagerLightReuseFreeBlocks) && (cMemManagerLightReuseFreeBlocks > 0)
    blockHeader_t* UsableBlockHdr       = NULL;
#endif
    void* buffer                     = NULL;

    OSA_InterruptDisable();

#ifdef MEM_MANAGER_BENCH
    uint32_t START_TIME = 0U, STOP_TIME = 0U, ALLOC_TIME = 0U;

    MEM_StartSysTick();
    START_TIME = MEM_GetSysTick();
#endif /* MEM_MANAGER_BENCH */

    do
    {
        assert(FreeBlockHdr->used == MEMMANAGER_BLOCK_FREE);
        if(FreeBlockHdr->next != NULL)
        {
            uint32_t available_size;
            available_size = (uint32_t)FreeBlockHdr->next - (uint32_t)FreeBlockHdr - BLOCK_HDR_SIZE;
            /* if a next block hdr exists, it means (by design) that a next free block exists too
               Because the last block header at the end of the heap will always be free 
               So, the current block header cant be the tail, and the next free cant be NULL */
            assert(FreeBlockHdr < FreeBlockHdrList.tail);
            assert(FreeBlockHdr->next_free != NULL);

            if(available_size >= numBytes) /* enough space in this free buffer */ 
            {
#if defined(cMemManagerLightReuseFreeBlocks) && (cMemManagerLightReuseFreeBlocks > 0)
                /* this block could be used if the memory pool if full, so we memorize it */
                if (UsableBlockHdr == NULL)
                {
                    UsableBlockHdr = FreeBlockHdr;
                }
                /* to avoid waste of large blocks with small blocks, make sure the required size is big enough for the available block
                  otherwise, try an other block ! */
                if ( (available_size - numBytes) < (available_size >> cMemManagerLightReuseFreeBlocks))
#endif
                {
                    /* Found a matching free block */
                    FreeBlockHdr->used = MEMMANAGER_BLOCK_USED;
#if defined(MEM_STATISTICS_INTERNAL)
                    FreeBlockHdr->buff_size = (uint16_t)numBytes;
#endif
                    NextFreeBlockHdr = FreeBlockHdr->next_free;
                    PrevFreeBlockHdr = FreeBlockHdr->prev_free;

                    /* In the current state, the current block header can be anywhere
                       from list head to previous block of list tail */
                    if(FreeBlockHdrList.head == FreeBlockHdr)
                    {
                        FreeBlockHdrList.head = NextFreeBlockHdr;
                        NextFreeBlockHdr->prev_free = NULL;
                    }
                    else
                    {
                        assert(FreeBlockHdrList.head->next_free <= FreeBlockHdr);

                        NextFreeBlockHdr->prev_free = PrevFreeBlockHdr;
                        PrevFreeBlockHdr->next_free = NextFreeBlockHdr;
                    }

                    BlockHdrFound = FreeBlockHdr;
                    break;
                }
            }
        }
        else
        {
            /* last block in the heap, check if available space to allocate the block */
            uint32_t available_size;
            available_size = memHeapEnd - (uint32_t)FreeBlockHdr - BLOCK_HDR_SIZE;
            assert(FreeBlockHdr == FreeBlockHdrList.tail);

            if ( available_size >=  ( numBytes + BLOCK_HDR_SIZE ) )  /* need to keep the room for the next BlockHeader */
            {
                FreeBlockHdr->used = MEMMANAGER_BLOCK_USED;
#if defined(MEM_STATISTICS_INTERNAL)
                FreeBlockHdr->buff_size = (uint16_t)numBytes;
#endif
                FreeBlockHdr->next = (blockHeader_t *)ROUNDUP_WORD( ((uint32_t)FreeBlockHdr + BLOCK_HDR_SIZE + numBytes ));
                FreeBlockHdr->next_free = FreeBlockHdr->next;

                PrevFreeBlockHdr = FreeBlockHdr->prev_free;

                NextFreeBlockHdr = FreeBlockHdr->next_free;
                NextFreeBlockHdr->used = MEMMANAGER_BLOCK_FREE;
#if defined(MEM_STATISTICS_INTERNAL)
                NextFreeBlockHdr->buff_size = 0U;
#endif
                NextFreeBlockHdr->next = NULL;
                NextFreeBlockHdr->next_free = NULL;
                NextFreeBlockHdr->prev_free = PrevFreeBlockHdr;

                if(FreeBlockHdrList.head == FreeBlockHdr)
                {
                    assert(FreeBlockHdrList.head == FreeBlockHdrList.tail);
                    assert(PrevFreeBlockHdr == NULL);
                    /* last free block in heap was the only free block available
                       so now the first free block in the heap is the next one */
                    FreeBlockHdrList.head = FreeBlockHdr->next_free;
                }
                else
                {
                    /* update previous free block header to point its next
                       to the new free block */
                    PrevFreeBlockHdr->next_free = NextFreeBlockHdr;
                }

                /* new free block is now the tail of the free block list */
                FreeBlockHdrList.tail = NextFreeBlockHdr;

#if defined(gMemManagerLightGuardsCheckEnable) && (gMemManagerLightGuardsCheckEnable == 1)
                MEM_BlockHeaderSetGuards(NextFreeBlockHdr);
#endif

                BlockHdrFound = FreeBlockHdr;
            }
#if defined(cMemManagerLightReuseFreeBlocks) && (cMemManagerLightReuseFreeBlocks > 0)
            else if (UsableBlockHdr != NULL)
            {
                /* we found a free block that can be used */
                UsableBlockHdr->used = MEMMANAGER_BLOCK_USED;
#if defined(MEM_STATISTICS_INTERNAL)
                UsableBlockHdr->buff_size = (uint16_t)numBytes;
#endif
                NextFreeBlockHdr = UsableBlockHdr->next_free;
                PrevFreeBlockHdr = UsableBlockHdr->prev_free;

                /* In the current state, the current block header can be anywhere
                   from list head to previous block of list tail */
                if(FreeBlockHdrList.head == UsableBlockHdr)
                {
                    FreeBlockHdrList.head = NextFreeBlockHdr;
                    NextFreeBlockHdr->prev_free = NULL;
                }
                else
                {
                    assert(FreeBlockHdrList.head->next_free <= UsableBlockHdr);

                    NextFreeBlockHdr->prev_free = PrevFreeBlockHdr;
                    PrevFreeBlockHdr->next_free = NextFreeBlockHdr;
                }
                BlockHdrFound = UsableBlockHdr;
            }
#endif
            else
            {
                BlockHdrFound = NULL;
            }
            break;
        }
#if defined(gMemManagerLightGuardsCheckEnable) && (gMemManagerLightGuardsCheckEnable == 1)
        MEM_BlockHeaderCheck(FreeBlockHdr->next_free);
#endif
        FreeBlockHdr = FreeBlockHdr->next_free;
        /* avoid looping */
        assert(FreeBlockHdr != FreeBlockHdr->next_free);
    } while(true);
    //MEM_DBG_LOG("BlockHdrFound: %x", BlockHdrFound);

#ifdef MEM_DEBUG_OUT_OF_MEMORY
    assert( BlockHdrFound );
#endif
    
#ifdef MEM_MANAGER_BENCH
    STOP_TIME = MEM_GetSysTick();
    MEM_StopSysTick();
    SystemCoreClockUpdate();
    if(START_TIME < STOP_TIME)
    {
        uint32_t max = SysTick->LOAD;
        START_TIME += max;
    }
    ALLOC_TIME = START_TIME - STOP_TIME;
    ALLOC_TIME = ALLOC_TIME / (SystemCoreClock / 1000000U);
#endif /* MEM_MANAGER_BENCH */

    if ( BlockHdrFound != NULL )
    {
        void_ptr_t buffer_ptr;
#ifdef MEM_TRACKING
        void_ptr_t lr;
        lr.raw_address = __get_LR();
        BlockHdrFound-> first_alloc_caller = lr.void_ptr;
#endif
        buffer_ptr.raw_address = (uint32_t)BlockHdrFound + BLOCK_HDR_SIZE;
        buffer = buffer_ptr.void_ptr;
    }

#ifdef MEM_STATISTICS_INTERNAL
#ifdef MEM_MANAGER_BENCH
    MEM_BufferAllocateMemStats(buffer, ALLOC_TIME, numBytes);
#else
    MEM_BufferAllocateMemStats(buffer, 0, numBytes);
#endif

    if( (MemStats.nb_alloc % NB_ALLOC_REPORT_THRESHOLD) == 0U )
    {
        MEM_ReportMemStats();
    }
#endif /* MEM_STATISTICS_INTERNAL */

    OSA_InterruptEnable();

    return buffer;
}

memStatus_t MEM_BufferFree(void* buffer /* IN: Block of memory to free*/)
{
    memStatus_t ret = MEM_SUCCESS_c;
    void_ptr_t buffer_ptr;
    buffer_ptr.void_ptr = buffer;

    if( buffer == NULL )
    {
        ret = MEM_FREE_ERROR_c;
    }
    else
    {
        OSA_InterruptDisable();

        blockHeader_t * BlockHdr;
        BlockHdr = (blockHeader_t * )(buffer_ptr.raw_address - BLOCK_HDR_SIZE);

        /* assert checks */
        assert( BlockHdr->used == MEMMANAGER_BLOCK_USED );
        assert( BlockHdr->next != NULL );
        /* when allocating a buffer, we always create a FreeBlockHdr at
           the end of the buffer, so the FreeBlockHdrList.tail should always
           be at a higher address than current BlockHdr */
        assert( BlockHdr < FreeBlockHdrList.tail );

#if defined(gMemManagerLightGuardsCheckEnable) && (gMemManagerLightGuardsCheckEnable == 1)
        MEM_BlockHeaderCheck(BlockHdr->next);
#endif

        //MEM_DBG_LOG("%x %d", BlockHdr, BlockHdr->buff_size);

#if defined(MEM_STATISTICS_INTERNAL)
        MEM_BufferFreeMemStats(buffer);
#endif /* MEM_STATISTICS_INTERNAL */

        if(BlockHdr < FreeBlockHdrList.head)
        {
            /* BlockHdr is placed before FreeBlockHdrList.head so we can set it as
               the new head of the list */
            BlockHdr->next_free = FreeBlockHdrList.head;
            BlockHdr->prev_free = NULL;
            FreeBlockHdrList.head->prev_free = BlockHdr;
            FreeBlockHdrList.head = BlockHdr;
        }
        else
        {
            /* we want to find the previous free block header
               here, we cannot use prev_free as this information could be outdated
               so we need to run through the whole list to be sure to catch the
               correct previous free block header */
            blockHeader_t* PrevFreeBlockHdr = FreeBlockHdrList.head;
            while(PrevFreeBlockHdr->next_free < BlockHdr)
            {
                PrevFreeBlockHdr = PrevFreeBlockHdr->next_free;
            }
            /* insert the new free block in the list */
            BlockHdr->next_free = PrevFreeBlockHdr->next_free;
            BlockHdr->prev_free = PrevFreeBlockHdr;
            BlockHdr->next_free->prev_free = BlockHdr;
            PrevFreeBlockHdr->next_free = BlockHdr;
        }

        BlockHdr->used      = MEMMANAGER_BLOCK_FREE;
#if defined(MEM_STATISTICS_INTERNAL)
        BlockHdr->buff_size = 0U;
#endif

#if defined(gMemManagerLightFreeBlocksCleanUp) && (gMemManagerLightFreeBlocksCleanUp == 1)
        MEM_BufferFreeBlocksCleanUp(BlockHdr);
#endif

        OSA_InterruptEnable();
    }

    return ret;
}

memStatus_t MEM_BufferFreeAllWithId(uint8_t poolId)
{
    memStatus_t status = MEM_SUCCESS_c;
#if (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1))
#ifdef MEMMANAGER_NOT_IMPLEMENTED_YET

#endif /* MEMMANAGER_NOT_IMPLEMENTED_YET */
#else /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
    status = MEM_FREE_ERROR_c;
#endif /* (defined(MEM_TRACK_ALLOC_SOURCE) && (MEM_TRACK_ALLOC_SOURCE == 1)) */
 return status;
}

uint32_t MEM_GetHeapUpperLimit(void)
{
    /* There is always a free block at the end of the heap
       and this free block is the tail of the list */
    return ((uint32_t)FreeBlockHdrList.tail + BLOCK_HDR_SIZE);
}

uint16_t MEM_BufferGetSize(void* buffer)
{
    blockHeader_t * BlockHdr = NULL;
    uint16_t size;
    /* union used to fix Misra */
    void_ptr_t buffer_ptr;
    buffer_ptr.void_ptr = buffer;

    if(buffer != NULL)
    {
        BlockHdr = (blockHeader_t * )(buffer_ptr.raw_address - BLOCK_HDR_SIZE);
        /* block size is the space between current BlockHdr and next BlockHdr */
        size = (uint16_t)((uint32_t)BlockHdr->next - (uint32_t)BlockHdr - BLOCK_HDR_SIZE);
    }
    else
    {
        /* is case of a NULL buffer, we return 0U */
        size = 0U;
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
        realloc_buffer = MEM_BufferAllocate(new_size, 0U);
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
            realloc_buffer = MEM_BufferAllocate(new_size, 0);

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

#ifdef MEMMANAGER_NOT_IMPLEMENTED_YET
uint32_t MEM_GetAvailableBlocks(uint32_t size)
{
    /* Function not implemented yet */
    assert(0);

    return 0U;
}

void* MEM_CallocAlt(size_t len, size_t val)
{
    /* Function not implemented yet */
    assert(0);

    return NULL;
}

void MEM_FreeAlt(void* pData)
{
    /* Function not implemented yet */
    assert(0);
}

#endif
#endif
