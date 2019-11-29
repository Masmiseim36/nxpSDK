/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       debug_log.c
\brief      Use this module to print debug messages and activate debug features.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "debug_log.h"

#if DEBUG_LOG
#include "thread_mgmt.h"
#include "thread_attributes.h"
#include "EmbeddedTypes.h"
#include "FunctionLib.h"
#include "TimersManager.h"
#include "MemManager.h"
#include "Panic.h"
#include "fsl_os_abstraction.h"
#include "app_stack_config.h"
#if NVM_NG_ENABLED
#include "nv_data.h"
#endif
#include "ip6.h"


/*==================================================================================================
Private macros
==================================================================================================*/

/* If a buffer (that is not allocated forever using MEM_BufferAllocForever()) is not freed
   in DBG_CheckMemBufferThreshold_c ms, the device will enter into panic */
#define DBG_CheckMemBufferThreshold_c     120000 /* ms */
/* How often the device should check if the above threshold expired */
#define DBG_CheckMemBufferInterval_c      15000 /* ms */

#if THREAD_USE_SHELL
    #undef SHELL_USE_PRINTF
    #define SHELL_USE_PRINTF 1
    //#include "shell.h"
    extern uint16_t shell_printf(char * format,...);
    extern void shell_write(char *pBuff);
    extern void shell_writeN(char *pBuff, uint16_t n);
#else
    #define shell_printf(a, ...)
    #define shell_write(a)
    #define shell_writeN(b,n)
#endif

#define NUM_OF_TRACK_PTR  1

/*==================================================================================================
Private type definitions
==================================================================================================*/

/*==================================================================================================
Private prototypes
==================================================================================================*/
static void DBG_MEMBuffCheckTimeout(void    *pParam);
/*==================================================================================================
Private global variables declarations
==================================================================================================*/
volatile bool_t mCheckBuffIfAreFreed = FALSE;
/*==================================================================================================
Public global variables declarations
==================================================================================================*/
#ifdef MEM_TRACKING

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#define _block_size_ 0 *
#define _number_of_blocks_ + 0 *
#define _eol_  + 1 +
#define _pool_id_(a)

uint16_t const poolCount = (PoolsDetails_c 0);

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

#define _block_size_ 0*
#define _number_of_blocks_ +
#define _eol_  +
#define _pool_id_(a)

uint16_t const mTotalNoOfMsgs_d  = (PoolsDetails_c 0);

#undef _block_size_
#undef _number_of_blocks_
#undef _eol_
#undef _pool_id_

extern const uint32_t heapSize;
extern uint8_t memHeap[];
extern blockTracking_t memTrack[];
extern pools_t memPools[];


#endif /* end MEM_TRACKING */


/*==================================================================================================
Public functions
==================================================================================================*/
void DBG_Printf(char * format,...)
{
#if DEBUG_LOG
    va_list args;
    va_start(args, format);
    shell_printf(format,args);
    va_end(args);

#endif
}


void DBG_WriteString(uint8_t* pBuf)
{
#if DEBUG_LOG
  shell_write((char*)pBuf);
#endif
}


void DBG_PrintNBytes(uint8_t* pBuff, uint16_t n)
{
#if DEBUG_LOG
  shell_writeN((char*)pBuff, n);
#endif
}


void DBG_HexDump(uint8_t *pBuf, uint16_t buflen) {
#if DEBUG_LOG
  int n = 0;

  while (buflen--) {
    shell_printf("%02X ", *pBuf++);

    n++;
    if (n % 8 == 0) {
      if (n % 16 == 0)
    shell_printf("\n\r");
      else
    shell_printf(" ");
    }
  }
#endif
}


void DBG_PrintLog( bool_t printTimeStamp,  uint8_t* pString,
                    uint8_t* pBuf, size_t buflen)
{
#if DEBUG_LOG
    if(printTimeStamp)
    {
        DBG_PrintTimeStamp();
    }

    if(pString)
    {
        //print string message
        DBG_WriteString(pString);
        DBG_WriteString((uint8_t*)"\n\r");
    }

    if(pBuf)
    {
            //print hex dump
        DBG_HexDump(pBuf, buflen);
        DBG_WriteString((uint8_t*)"\n\r");
    }
#endif
}

uint32_t MEM_GetTimeStamp(void)
{
#if DEBUG_LOG
    return   ((uint32_t)((uint64_t)TMR_GetTimestamp()/(uint64_t)1000));
#endif
}

void DBG_PrintTimeStamp(void)
{
#if DEBUG_LOG && THREAD_USE_SHELL
    uint64_t time =  (TMR_GetTimestamp()/1000);
    shell_printf("\n\rTime stamp ms: %lld\n\r", time);
#endif
}


 /*! *********************************************************************************
* \brief
* \param[in]
*
* \return
********************************************************************************** */
void DBG_MsgCheck(void)
{
#if MEM_TRACKING && DEBUG_LOG
    uint16_t i, j;
    uint8_t* pHead;
    pools_t* pParentPool;

    OSA_InterruptDisable();
    for(i=0; i< poolCount; i++)
    {
        pHead = (uint8_t*)(memPools[i].anchor.head);
        if( (pHead != NULL) &&
            ((pHead < memHeap) || (pHead > (memHeap + heapSize)) )
          )
        {
            panic(0,0,0,0);
        }
    }

    j = 0;
    for(i=0; i<mTotalNoOfMsgs_d; i++)
    {
        pParentPool = (((listHeader_t *)(memTrack[i].blockAddr))-1)->pParentPool;

        if(pParentPool != &memPools[j])
        {
          if(j < poolCount)
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
    }
  OSA_InterruptEnable();
#endif
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
void DBG_MEMBufferCheck(uint8_t *p, uint32_t size)
{
#if MEM_TRACKING && DEBUG_LOG
    uint32_t i;
    OSA_InterruptDisable();
    if( (p < (uint8_t*)memHeap) || (p > ((uint8_t*)memHeap + heapSize)) )
    {
        OSA_InterruptEnable();
        return;
    }
    for(i=0; i<mTotalNoOfMsgs_d; i++)
    {
        if( p >= (uint8_t*)memTrack[i].blockAddr &&
            p < (uint8_t*)memTrack[i+1].blockAddr )
        {
            if( (p+size) > ((uint8_t*)memTrack[i+1].blockAddr - sizeof(listHeader_t)) )
            {
                panic(0,0,0,0);
            }

            break;
        }
    }
    OSA_InterruptEnable();
#endif
}

static void DBG_MEMBuffCheckTimeout(void    *pParam)
{
    (void)pParam;
    mCheckBuffIfAreFreed = TRUE;
}


void DBG_CheckIfMemBuffersAreFreed(void)
{
#if MEM_TRACKING && DEBUG_LOG

    static tmrTimerID_t    gBuffTmrId = gTmrInvalidTimerID_c;

    if(TRUE == mCheckBuffIfAreFreed)
    {
        uint16_t i, j = 0;
        volatile blockTracking_t *pTrack;
        static volatile blockTracking_t *mpTrackTbl[NUM_OF_TRACK_PTR];
        uint8_t* pHead;
        pools_t* pParentPool;
        uint8_t trackCount = 0;

        /*Reset the flag */
        mCheckBuffIfAreFreed = FALSE;

        /* Validate the pHead for each pool */
        for(i = 0; i < poolCount; i++)
        {
            OSA_InterruptDisable();
            pHead = (uint8_t*)(memPools[i].anchor.head);
            if( (pHead != NULL) &&
                ((pHead < memHeap) || (pHead > (memHeap + heapSize)) )
            )
            {
                panic(0,0,0,0);
            }
            OSA_InterruptEnable();

        } /* end for */


        for( i = 0; i < mTotalNoOfMsgs_d; i++ )
        {
            OSA_InterruptDisable();

            /* Validate the pParent first */
            pParentPool = (((listHeader_t *)(memTrack[i].blockAddr))-1)->pParentPool;
            if(pParentPool != &memPools[j])
            {
                if(j < poolCount)
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
            pTrack = &memTrack[i];
            if((pTrack->timeStamp != 0xffffffff ) &&
                (pTrack->allocStatus == MEM_TRACKING_ALLOC_c))
            {
                uint32_t t = MEM_GetTimeStamp();
                t = t - pTrack->timeStamp;
                if(t > DBG_CheckMemBufferThreshold_c)
                {
                    mpTrackTbl[trackCount++] = pTrack;
                    if(trackCount == NUM_OF_TRACK_PTR)
                    {
                        (void)mpTrackTbl; /* remove compiler warnings */
                        panic(0,0,0,0);
                    }
                }
            }
            OSA_InterruptEnable();
        } /* end for */
    }
    else if(gBuffTmrId == gTmrInvalidTimerID_c) /* Start the timer interval if not started */
    {
        gBuffTmrId = TMR_AllocateTimer();
        if(gBuffTmrId == gTmrInvalidTimerID_c)
        {
            panic(0,0,0,0);
        }

        TMR_StartTimer(gBuffTmrId, gTmrLowPowerIntervalMillisTimer_c, DBG_CheckMemBufferInterval_c,
            DBG_MEMBuffCheckTimeout, NULL);
    }


#endif
}
/***************************************************************************************************
***************************************************************************************************/
void HandlerC(unsigned long *hardfault_args)
{
    volatile unsigned long stacked_r0 = 0;
    volatile unsigned long stacked_r1 = 0;
    volatile unsigned long stacked_r2 = 0;
    volatile unsigned long stacked_r3 = 0;
    volatile unsigned long stacked_r12 = 0;
    volatile unsigned long stacked_lr = 0;
    volatile unsigned long stacked_pc = 0;
    volatile unsigned long stacked_psr = 0;
    volatile unsigned long _CFSR = 0;
    volatile unsigned long _HFSR = 0;
    volatile unsigned long _DFSR = 0;
    volatile unsigned long _AFSR = 0;
    volatile unsigned long _BFAR = 0;
    volatile unsigned long _MMAR = 0;

    /* suppress warnings about unused variables */
    (void)stacked_r0;
    (void)stacked_r1;
    (void)stacked_r2;
    (void)stacked_r3;
    (void)stacked_r12;
    (void)stacked_lr;
    (void)stacked_pc;
    (void)stacked_psr;
    (void)_CFSR;
    (void)_HFSR;
    (void)_DFSR;
    (void)_AFSR;
    (void)_BFAR;
    (void)_MMAR;

    stacked_r0 = ((unsigned long)hardfault_args[0]);
    stacked_r1 = ((unsigned long)hardfault_args[1]);
    stacked_r2 = ((unsigned long)hardfault_args[2]);
    stacked_r3 = ((unsigned long)hardfault_args[3]);
    stacked_r12 = ((unsigned long)hardfault_args[4]);
    stacked_lr = ((unsigned long)hardfault_args[5]);
    stacked_pc = ((unsigned long)hardfault_args[6]);
    stacked_psr = ((unsigned long)hardfault_args[7]);

    /* Configurable Fault Status Register */
    /* Consists of MMSR, BFSR and UFSR */
    _CFSR = (*((volatile unsigned long *)(0xE000ED28)));

    /* Hard Fault Status Register */
    _HFSR = (*((volatile unsigned long *)(0xE000ED2C)));

    /* Debug Fault Status Register */
    _DFSR = (*((volatile unsigned long *)(0xE000ED30)));

    /* Auxiliary Fault Status Register */
    _AFSR = (*((volatile unsigned long *)(0xE000ED3C)));

    /* Read the Fault Address Registers. These may not contain valid values.
     * Check BFARVALID/MMARVALID to see if they are valid values
     * MemManage Fault Address Register
     */
    _MMAR = (*((volatile unsigned long *)(0xE000ED34)));
    /* Bus Fault Address Register */
    _BFAR = (*((volatile unsigned long *)(0xE000ED38)));

    __asm("BKPT #0\n") ; /* cause the debugger to stop */
}
#endif


void HardFault_Handler
(
    void
)
{
#if DEBUG_LOG
    /* activate handler for more debug info */
    //extern void __HwFault(void);
    //__HwFault();
    //HandlerC(0);
#if __ICCARM__
#include "intrinsics.h"
    uint32_t sp = __get_SP();
    uint32_t psr = __get_PSR();
    uint32_t psp = __get_PSP();
    uint32_t lr = __get_LR();
    uint32_t ipsr = __get_IPSR();
    //uint32_t faultmask = __get_FAULTMASK();

    uint32_t addr = *((uint32_t*)(__get_PSP() + 0x18));
    (void)addr;
#elif __GNUC__

    volatile register uint32_t psp;
    __asm volatile ("MRS %0, psp\n"  : "=r" (psp) );

    /* TBD for GCC */
    volatile uint32_t addr = *((uint32_t*)(psp + 0x18));
    (void)addr;
#endif
    __asm("BKPT #0\n") ;
    
#else
    __asm("BKPT #0\n") ; /* cause the debugger to stop */
#endif    
}



void DBG_CheckIP6Addresses(uint32_t ipIfId)
{
#if DEBUG_LOG
    extern const uint32_t globalAddrTable6Size;
    extern ip6IfAddrData_t* aGlobalAddrTable6[];
    uint16_t countMLprefix = 0;
    uint16_t countLLprefix = 0;
    uint8_t allzeros[] = {0,0,0,0,0,0,0,0};
    ipAddr_t* pGAddr;


    if(ipIfId != 0xff)
    {
        for(uint32_t i=0; i<globalAddrTable6Size; i++)
        {
            pGAddr = &aGlobalAddrTable6[i]->ip6Addr;

            if((ADDR_STATE_GET(aGlobalAddrTable6[i]->ip6AddrTypeAndState) != ip6AddrStateNotUsed_c) &&
                (aGlobalAddrTable6[i]->ipIfId == ipIfId))
            {
                /* Check LL64*/
                if((pGAddr->addr32[0] == 0x000080FE) && (pGAddr->addr32[1] == 0) )
                {
                    countLLprefix++;
                    if(countLLprefix > 1)
                    {
                        panic(0,0,0,0);
                    }
                }

                /* Check ML addresses */
                if( !FLib_MemCmp(&gpaThrActiveAttr[0]->MLprefix.prefix.addr8[0], allzeros, 8) &&
                    FLib_MemCmp(&pGAddr->addr8[0], &gpaThrActiveAttr[0]->MLprefix.prefix.addr8[0],8)
                    && !(NWKU_IsIPAddrBasedOnShort(pGAddr) && (pGAddr->addr8[14] == 0xFC))) /* do not count the anycast address */
                {
                    countMLprefix++;
                    if(countMLprefix > 2)
                    {
                        panic(0,0,0,0);
                    }

                }

            }

        }
    }
#endif
}

void DBG_CheckRestoreLeaderLed(void)
{
#if !THREAD_ED_CONFIG
#ifndef USB_KW24D
    extern uint32_t leaderLedTimestamp;
    extern void App_RestoreLeaderLedCb(void* param);
    extern uint64_t TMR_GetTimestamp(void);

    if((leaderLedTimestamp) &&
       (leaderLedTimestamp < (TMR_GetTimestamp()/1000000)))
    {
        leaderLedTimestamp = 0;
        /* restore leader LED */
        App_RestoreLeaderLedCb(NULL);
    }
#endif
#endif
}

void DBG_Check(void)
{
#if DEBUG_LOG   
    /* Check if memory blocks are freed after a time interval - default disabled */
    DBG_CheckIfMemBuffersAreFreed();
    /* Check timeout for leader Led restore - default enabled */
    DBG_CheckRestoreLeaderLed();
#endif    
}
#if NVM_NG_ENABLED
void DBG_NVM(uint16_t nvmTableEntryId, uint16_t elementIndex, uint8_t saveRestoreAll)
{
#if DEBUG_LOG
    uint16_t tableEntryIndex = 0;
    NVM_DataEntry_t* pNVM_DataEntry = NULL;

    if (gNVM_TABLE_entries_c > (gNvTableEntriesCountMax_c - 1))
    {
        panic(0,0,0,0);
    }

    /* Find index entry in pNVM_DataTable */
    while(gNVM_TABLE_entries_c > tableEntryIndex)
    {
        if(pNVM_DataTable[tableEntryIndex].DataEntryID == nvmTableEntryId)
        {
            pNVM_DataEntry = &pNVM_DataTable[tableEntryIndex];
            break;
        }
        tableEntryIndex++;
    }

    if(pNVM_DataEntry == NULL)
    {
        panic(0,0,0,0);
    }

    gDbgNVMSavings[tableEntryIndex].countSave++;

    if(gNVM_MirroredInRam_c != pNVM_DataEntry->DataEntryType)
    {
        uint32_t i;
        uint8_t* p = (uint8_t*)((uint8_t**)pNVM_DataEntry->pData)[elementIndex];

        OSA_InterruptDisable();
        if( (p >= (uint8_t*)memHeap) && (p <= ((uint8_t*)memHeap + heapSize)) )
        {
            for(i=0; i<mTotalNoOfMsgs_d; i++)
            {
                if( p >= (uint8_t*)memTrack[i].blockAddr &&
                    p < (uint8_t*)memTrack[i+1].blockAddr )
                {
                    gDbgNVMSavings[tableEntryIndex].dataIsAllocated = TRUE;
                    FLib_MemCpy(&gDbgNVMSavings[tableEntryIndex].lastAllocBlock,
                        &memTrack[i], sizeof(blockTracking_t));
                    break;
                }
            }
        }
        OSA_InterruptEnable();
    }
    else
    {
        uint8_t* p;

        p = (uint8_t*)(pNVM_DataEntry->pData);
        if(saveRestoreAll == FALSE)
        {
          p = p +(elementIndex * pNVM_DataEntry->ElementSize);
        }

        if( (p >= (uint8_t*)memHeap) && (p <= ((uint8_t*)memHeap + heapSize)) )
        {
            panic(0,0,0,0);
        }

        gDbgNVMSavings[tableEntryIndex].dataIsAllocated = FALSE;
        gDbgNVMSavings[tableEntryIndex].pLastNonAllocAddr = p;

    }

#endif

}
#endif /* NVM_NG_ENABLED */


/*==================================================================================================
Private debug functions
==================================================================================================*/



