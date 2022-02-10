/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "fsl_semc.h"
#include "fsl_cache.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SEMC SEMC
#define EXAMPLE_SEMC_START_ADDRESS (0x80000000U)
#define EXAMPLE_SEMC_CLK_FREQ CLOCK_GetFreq(kCLOCK_SemcClk)

#define SEMC_EXAMPLE_DATALEN (0x1000U)
#define SEMC_EXAMPLE_WRITETIMES (1000U)


#define DCACHE_ENABLE 1
#define PREFETCH_BUFF_DIS 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t BOARD_InitSEMC(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t BOARD_InitSEMC(void)
{
    semc_config_t config;
    semc_sdram_config_t sdramconfig;
    uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;

    /* Initializes the MAC configure structure to zero. */
    memset(&config, 0, sizeof(semc_config_t));
    memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

    /* Initialize SEMC. */
    SEMC_GetDefaultConfig(&config);
    config.dqsMode = kSEMC_Loopbackdqspad;  /* For more accurate timing. */
    SEMC_Init(SEMC, &config);

    /* Configure SDRAM. */
    sdramconfig.csxPinMux = kSEMC_MUXCSX0;
    sdramconfig.address = 0x80000000;
    sdramconfig.memsize_kbytes = 32 * 1024; /* 32MB = 32*1024*1KBytes*/
    sdramconfig.portSize = kSEMC_PortSize16Bit;
	#if defined(FSL_FEATURE_SEMC_ERRATA_050577) && (FSL_FEATURE_SEMC_ERRATA_050577 == 0x01U)
	sdramconfig.burstLen = kSEMC_Sdram_BurstLen1;
	#else
    sdramconfig.burstLen = kSEMC_Sdram_BurstLen8;
	#endif
    sdramconfig.columnAddrBitNum = kSEMC_SdramColunm_9bit;
    sdramconfig.casLatency = kSEMC_LatencyThree;
    sdramconfig.tPrecharge2Act_Ns = 18;   /* Trp 18ns */
    sdramconfig.tAct2ReadWrite_Ns = 18;   /* Trcd 18ns */
    sdramconfig.tRefreshRecovery_Ns = 67; /* Use the maximum of the (Trfc , Txsr). */
    sdramconfig.tWriteRecovery_Ns = 12;   /* 12ns */
    sdramconfig.tCkeOff_Ns = 42;  /* The minimum cycle of SDRAM CLK off state. CKE is off in self refresh at a minimum period tRAS.*/
    sdramconfig.tAct2Prechage_Ns = 42; /* Tras 42ns */
    sdramconfig.tSelfRefRecovery_Ns = 67;
    sdramconfig.tRefresh2Refresh_Ns = 60;
    sdramconfig.tAct2Act_Ns = 60;
    sdramconfig.tPrescalePeriod_Ns = 160 * (1000000000 / clockFrq);
    sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; /* 64ms/8192 */
    sdramconfig.refreshUrgThreshold = sdramconfig.refreshPeriod_nsPerRow;
    sdramconfig.refreshBurstLen = 1;
    return SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
}

static volatile uint32_t g_msCount = 0;
void SysTick_Handler(void)
{
    g_msCount++;
}

/*!
 * @brief Main function
 */

int main(void)
{
    uint32_t time;
    volatile uint32_t tmp;
    uint32_t g_ms;
    uint32_t t1;
    uint32_t t2;
    uint32_t coreclk = 0;
    uint32_t dsize = 4*1024;
    uint32_t *ahb_addr = (uint32_t *)0x80000000;
    /* Hardware initialize. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    coreclk = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 29);
    /* Set semc clock to 163.86 MHz */
    CLOCK_SetMux(kCLOCK_SemcMux, 1);
    CLOCK_SetDiv(kCLOCK_SemcDiv, 1);   
#if( DCACHE_ENABLE ==1 )
    PRINTF("DCACHE is enabled!\r\n");
#else
    SCB_DisableDCache();
    PRINTF("DCACHE is disable!\r\n");
#endif    
    
#if PREFETCH_BUFF_DIS
    FLEXSPI->AHBCR &= ~FLEXSPI_AHBCR_PREFETCHEN_MASK;
    PRINTF("Prefetch is disable!\r\n");
#else
    PRINTF("Prefetch is enable!\r\n");
#endif  

    PRINTF("\r\n SEMC SDRAM Performance test Start!\r\n");
    if (BOARD_InitSEMC() != kStatus_Success)
    {
        PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
    }
    
    for (uint32_t i = 0; i < SEMC_EXAMPLE_DATALEN; i ++)
    {
      sdram_writeBuffer[i] = i;
    }
    
    /* configure system tick */
    SysTick_Config(coreclk / 1000); /* 1ms for one interrupt*/
    
    PRINTF("\r\n Start test SDRAM write performance!\r\n");
    /* sdram write test */
    ahb_addr = (uint32_t *)0x80000000;
    g_msCount = 0;
    t1 = SysTick->VAL;
#if 1
    for (uint32_t i = 0; i < dsize/4; i ++)
    {
        ahb_addr[i] = sdram_writeBuffer[i];
    }
    __asm("DSB");
#else
    memcpy((uint32_t *)(ahb_addr),&sdram_writeBuffer[0],dsize);
    __asm("DSB");
#endif
    t2 = SysTick->VAL;
    g_ms = g_msCount;
    time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
    PRINTF("##sdram write perf###t1: %d; t2: %d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
           t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
    
    /* invlide the cache range ahb_addr to ahb_addr+dsize */
    DCACHE_InvalidateByRange((uint32_t )ahb_addr,dsize);
    
    PRINTF("\r\n Start test SDRAM read performance!\r\n");
    /* sdram read test */
    ahb_addr = (uint32_t *)0x80000000;
    g_msCount = 0;
    t1 = SysTick->VAL;
#if 1
    for (uint32_t i = 0; i < dsize/4; i ++)
    {
        sdram_readBuffer[i] = ahb_addr[i];
    }
    __asm("DSB");
#else
    memcpy(&sdram_readBuffer[0],(uint32_t *)(ahb_addr),dsize);
    __asm("DSB");
#endif
    t2 = SysTick->VAL;
    g_ms = g_msCount;
    time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
    
    uint32_t errCount = 0;
    for(uint32_t i=0;i<dsize/4;i++)
    {
      if(sdram_readBuffer[i] != sdram_writeBuffer[i])
      {
        errCount++;
      }
    }
    if(!errCount)
    {
      PRINTF("sdram read and write correctly!\r\n");
    }
    else
    {
      PRINTF("sdram read and write fail!\r\n");
    }
    PRINTF("##sdram read perf###t1: %d; t2: %d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
           t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
    
    /* invlide the cache range ahb_addr to ahb_addr+dsize */
    DCACHE_InvalidateByRange((uint32_t )ahb_addr,dsize);
  
    
    /* hyper flash read test, need to ensure code programming to hyper flash with this test,
       otherwise, it will lead to hard fault */
    PRINTF("\r\n Start test Hyper Flash read performance!\r\n");   
    g_msCount = 0;
    ahb_addr = (uint32_t *)0x60000000;
    t1 = SysTick->VAL;
#if 1
    for (uint32_t i = 0; i < dsize/4; i ++)
    {
        sdram_readBuffer[i] = ahb_addr[i];
    }
    __asm("DSB");
#else
    memcpy(&sdram_readBuffer[0],(uint32_t *)(ahb_addr),dsize);
    __asm("DSB");
#endif
    t2 = SysTick->VAL;
    g_ms = g_msCount;
    time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
    PRINTF("##Hyper flash AHB read perf###t1: %d; t2: %d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
           t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
    
    /* invlide the cache range ahb_addr to ahb_addr+dsize */
    DCACHE_InvalidateByRange((uint32_t )ahb_addr,dsize);


    PRINTF("\r\n SEMC SDRAM Performance test End.\r\n");
    while (1)
    {
    }
}

