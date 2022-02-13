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
#include "fsl_edma.h"
#include "fsl_dmamux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SEMC SEMC
#define EXAMPLE_SEMC_START_ADDRESS (0x80000000U)
#define EXAMPLE_SEMC_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Semc)

#define SEMC_EXAMPLE_DATALEN (0x1000U)


#define DCACHE_ENABLE 1
#define PREFETCH_BUFF_DIS 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t BOARD_InitSEMC(void);
void MeassureSpeed (uint32_t coreclk, uint32_t dsize);
void MeassureSpeed_Memcopy (uint32_t coreclk, uint32_t dsize);
void MeassureSpeed_CleanInvalidateCache (uint32_t coreclk, uint32_t dsize);
void MeassureSpeed_DMA (uint32_t coreclk, uint32_t dsize);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
static uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_ConfigMPU(void)
{
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	bool DCacheEnableFlag = false;
	// Disable D cache.
	if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
	{
		SCB_DisableDCache();
		DCacheEnableFlag = true;
	}
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
	volatile bool ICacheEnableFlag = false;
	// Disable I cache.
	if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
	{
		SCB_DisableICache();
		ICacheEnableFlag = true;
	}
#endif /* __ICACHE_PRESENT */

	/* Disable MPU */
	ARM_MPU_Disable();

	/* Region 9 setting: Memory with Normal type, not shareable, outer/inner write through */
	MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_64MB);

	/* Enable MPU */
	ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

	/* Enable I cache and D cache */
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
	if (DCacheEnableFlag)
	{
		/* Enable D cache. */
		SCB_EnableDCache();
	}
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
	if (ICacheEnableFlag)
	{
		/* Enable I cache. */
		SCB_EnableICache();
		ICacheEnableFlag = false;
	}
#endif /* __ICACHE_PRESENT */
}

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
	config.dqsMode = kSEMC_Loopbackdqspad; /* For more accurate timing. */
	SEMC_Init(SEMC, &config);

	/* Configure SDRAM. */
	sdramconfig.csxPinMux		   = kSEMC_MUXCSX0;
	sdramconfig.address			 = 0x80000000;
	sdramconfig.memsize_kbytes	  = 2 * 32 * 1024;	   /* 64MB = 2*32*1024*1KBytes*/
	sdramconfig.portSize			= kSEMC_PortSize32Bit; /*two 16-bit SDRAMs make up 32-bit portsize*/
	sdramconfig.burstLen			= kSEMC_Sdram_BurstLen8;
	sdramconfig.columnAddrBitNum	= kSEMC_SdramColunm_9bit;
	sdramconfig.casLatency		  = kSEMC_LatencyThree;
	sdramconfig.tPrecharge2Act_Ns   = 15; /* tRP 15ns */
	sdramconfig.tAct2ReadWrite_Ns   = 15; /* tRCD 15ns */
	sdramconfig.tRefreshRecovery_Ns = 70; /* Use the maximum of the (Trfc , Txsr). */
	sdramconfig.tWriteRecovery_Ns   = 2;  /* tWR 2ns */
	sdramconfig.tCkeOff_Ns =
		42; /* The minimum cycle of SDRAM CLK off state. CKE is off in self refresh at a minimum period tRAS.*/
	sdramconfig.tAct2Prechage_Ns	   = 40; /* tRAS 40ns */
	sdramconfig.tSelfRefRecovery_Ns	= 70;
	sdramconfig.tRefresh2Refresh_Ns	= 60;
	sdramconfig.tAct2Act_Ns			= 2; /* tRC/tRDD 2ns */
	sdramconfig.tPrescalePeriod_Ns	 = 160 * (1000000000 / clockFrq);
	sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; /* 64ms/8192 */
	sdramconfig.refreshUrgThreshold	= sdramconfig.refreshPeriod_nsPerRow;
	sdramconfig.refreshBurstLen		= 1;
	sdramconfig.delayChain			 = 2;

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
	volatile uint32_t tmp;
	uint32_t dsize = SEMC_EXAMPLE_DATALEN;
	/* Hardware initialize. */
	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	uint32_t coreclk = CLOCK_GetFreq(kCLOCK_CoreSysClk);
	
	/* Config cacheable attribute for SDRAM memory */
	APP_ConfigMPU();

#if !(defined(DCACHE_ENABLE) && DCACHE_ENABLE)
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
	if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
	{
		SCB_DisableDCache();
	}
#endif
#endif

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
	SysTick_Config(coreclk / 1000); // 1ms for one interrupt
	__enable_irq ();
	
	PRINTF("\r\n #####################################################");
	PRINTF("\r\n With Cache enabled, copy in a loop with 32-bit access");
	PRINTF("\r\n #####################################################\r\n");
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed (coreclk, dsize);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed (coreclk, 256);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed (coreclk, 96);

	PRINTF("\r\n #####################################");
	PRINTF("\r\n With Cache enabled, copy with memcopy");
	PRINTF("\r\n #####################################\r\n");
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_Memcopy (coreclk, dsize);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_Memcopy (coreclk, 256);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_Memcopy (coreclk, 96);

	PRINTF("\r\n ######################################################");
	PRINTF("\r\n With Cache disabled, copy in a loop with 32-bit access");
	PRINTF("\r\n ######################################################\r\n");
	if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
	{
		SCB_DisableDCache();
	}
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed (coreclk, dsize);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed (coreclk, 256);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed (coreclk, 96);

	PRINTF("\r\n ######################################");
	PRINTF("\r\n With Cache disabled, copy with memcopy");
	PRINTF("\r\n ######################################\r\n");
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_Memcopy (coreclk, dsize);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_Memcopy (coreclk, 256);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_Memcopy (coreclk, 96);

	while (SysTick->VAL > 100)
		;


	PRINTF("\r\n #####################################################################");
	PRINTF("\r\n With Cache enabled and invalidated, copy in a loop with 32-bit access");
	PRINTF("\r\n #####################################################################\r\n");
	#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
		SCB_EnableDCache(); /* Enable D cache. */
	#endif
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_CleanInvalidateCache (coreclk, dsize);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_CleanInvalidateCache (coreclk, 256);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_CleanInvalidateCache (coreclk, 96);

	while (SysTick->VAL > 100)
		;


	PRINTF("\r\n #####################################################################");
	PRINTF("\r\n With Cache enabled, copy by DMA");
	PRINTF("\r\n #####################################################################\r\n");
	DMAMUX_Init		   (DMAMUX0);
	DMAMUX_EnableAlwaysOn (DMAMUX0, 0, true);
	DMAMUX_EnableChannel  (DMAMUX0, 0);
	edma_config_t config;
	EDMA_GetDefaultConfig (&config);
	EDMA_Init			 (DMA0, &config);

	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_DMA (coreclk, dsize);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_DMA (coreclk, 256);
	while (SysTick->VAL > 0xFFFFFF-100);
	MeassureSpeed_DMA (coreclk, 96);
//*/

	while (1)
	{
	}
}

void MeassureSpeed (uint32_t coreclk, uint32_t dsize)
{
	//PRINTF("\r\n Start test SDRAM write performance!\r\n");
	// SDRAM write test
	uint32_t *ahb_addr = (uint32_t *)0x80000000;
	g_msCount = 0;
	uint32_t t1 = SysTick->VAL;
	for (uint32_t i = 0; i < dsize/4; i ++)
	{
		ahb_addr[i] = sdram_writeBuffer[i];
	}
	__asm("DSB");
	uint32_t t2 = SysTick->VAL;
	uint32_t g_ms = g_msCount;
	uint32_t time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	PRINTF("##SDRAM write performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
	
	/* invlide the cache range ahb_addr to ahb_addr+dsize */
	DCACHE_InvalidateByRange((uint32_t )ahb_addr,dsize);
	
	//PRINTF("\r\n Start test SDRAM read performance!\r\n");
	// SDRAM read test
	ahb_addr = (uint32_t *)0x80000000;
	g_msCount = 0;
	t1 = SysTick->VAL;
	for (uint32_t i = 0; i < dsize/4; i ++)
	{
		sdram_readBuffer[i] = ahb_addr[i];
	}
	__asm("DSB");
	t2 = SysTick->VAL;
	g_ms = g_msCount;
	time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	
	uint32_t errCount = 0;
	for (uint32_t i=0;i<dsize/4;i++)
	{
	  if (sdram_readBuffer[i] != sdram_writeBuffer[i])
	  {
		errCount++;
	  }
	}
	if (!errCount)
	{
	  //PRINTF("SDRAM read and write correctly!\r\n");
	}
	else
	{
	  PRINTF("SDRAM read and write fail!\r\n");
	}
	PRINTF("##SDRAM read performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
	
	/* invlide the cache range ahb_addr to ahb_addr+dsize */
	DCACHE_InvalidateByRange((uint32_t )ahb_addr,dsize);
}


void MeassureSpeed_CleanInvalidateCache (uint32_t coreclk, uint32_t dsize)
{
	//PRINTF("\r\n Start test SDRAM write performance!\r\n");
	// SDRAM write test
	uint32_t *ahb_addr = (uint32_t *)0x80000000;
	g_msCount = 0;
	uint32_t t1 = SysTick->VAL;
	for (uint32_t i = 0; i < dsize; i ++)
	{
		ahb_addr[i] = sdram_writeBuffer[i];
	}
//	SCB_CleanDCache ();
	SCB_CleanDCache_by_Addr (ahb_addr, dsize);
	__asm("DSB");
	uint32_t t2 = SysTick->VAL;
	uint32_t g_ms = g_msCount;
	uint32_t time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	PRINTF("##SDRAM write performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
	   
	//PRINTF("\r\n Start test SDRAM read performance!\r\n");
	// SDRAM read test
	ahb_addr = (uint32_t *)0x80000000;
	g_msCount = 0;
	t1 = SysTick->VAL;
//	SCB_InvalidateDCache ();
	SCB_CleanInvalidateDCache_by_Addr (ahb_addr, dsize);
	for (uint32_t i = 0; i < dsize/4; i ++)
	{
		sdram_readBuffer[i] = ahb_addr[i];
	}
	__asm("DSB");
	t2 = SysTick->VAL;
	g_ms = g_msCount;
	time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	
	uint32_t errCount = 0;
	for (uint32_t i=0;i<dsize/4;i++)
	{
	  if (sdram_readBuffer[i] != sdram_writeBuffer[i])
	  {
		errCount++;
	  }
	}
	if (!errCount)
	{
	  //PRINTF("SDRAM read and write correctly!\r\n");
	}
	else
	{
	  PRINTF("SDRAM read and write fail!\r\n");
	}
	PRINTF("##SDRAM read performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
}



void MeassureSpeed_Memcopy (uint32_t coreclk, uint32_t dsize)
{
	//PRINTF("\r\n Start test SDRAM write performance!\r\n");
	// SDRAM write test
	uint32_t *ahb_addr = (uint32_t *)0x80000000;
	g_msCount = 0;
	uint32_t t1 = SysTick->VAL;
	memcpy((uint32_t *)(ahb_addr),&sdram_writeBuffer[0],dsize);
	__asm("DSB");
	uint32_t t2 = SysTick->VAL;
	uint32_t g_ms = g_msCount;
	uint32_t time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	PRINTF("##SDRAM write performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
	
	/* invlide the cache range ahb_addr to ahb_addr+dsize */
	DCACHE_InvalidateByRange((uint32_t )ahb_addr,dsize);
	
	//PRINTF("\r\n Start test SDRAM read performance!\r\n");
	// SDRAM read test
	ahb_addr = (uint32_t *)0x80000000;
	g_msCount = 0;
	t1 = SysTick->VAL;
	memcpy(&sdram_readBuffer[0],(uint32_t *)(ahb_addr),dsize);
	__asm("DSB");
	t2 = SysTick->VAL;
	g_ms = g_msCount;
	time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	
	uint32_t errCount = 0;
	for (uint32_t i=0;i<dsize/4;i++)
	{
	  if (sdram_readBuffer[i] != sdram_writeBuffer[i])
	  {
		errCount++;
	  }
	}
	if(!errCount)
	{
	  //PRINTF("SDRAM read and write correctly!\r\n");
	}
	else
	{
	  PRINTF("SDRAM read and write fail!\r\n");
	}
	PRINTF("##SDRAM read performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
	
	/* invlide the cache range ahb_addr to ahb_addr+dsize */
	DCACHE_InvalidateByRange((uint32_t )ahb_addr,dsize);
}



void MeassureSpeed_DMA (uint32_t coreclk, uint32_t dsize)
{
	uint32_t *ahb_addr = (uint32_t *)0x80000000;
	// Erase memory area
	memset (ahb_addr, 0, dsize);

	edma_transfer_config_t transferConfig;
	EDMA_PrepareTransfer
	(
		&transferConfig,
		sdram_writeBuffer, sizeof(sdram_writeBuffer[0]), // srcAddr, srcWidth
		ahb_addr, sizeof(ahb_addr[0]),				   // destAddr, destWidth
		32, dsize,									   // bytesEachRequest, transferBytes
		kEDMA_MemoryToMemory							 // type
	);

	edma_handle_t hDMA;
	EDMA_CreateHandle   (&hDMA, DMA0, 0);
	EDMA_SubmitTransfer (&hDMA, &transferConfig);
//	EDMA_SetTransferConfig (DMA0, 0, &transferConfig, NULL);


//	PRINTF ("\r\n Start test SDRAM write performance!\r\n");
	// SDRAM write test
	g_msCount = 0;
	uint32_t t1 = SysTick->VAL;
	EDMA_StartTransfer (&hDMA);
	while ((DMA0->TCD[0].CSR & DMA_CSR_DONE_MASK) == 0)
		;
	__asm("DSB");
	uint32_t t2 = SysTick->VAL;
	uint32_t g_ms = g_msCount;
	uint32_t time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	PRINTF ("##SDRAM write performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
	

	EDMA_PrepareTransfer
	(
		&transferConfig,
		ahb_addr, sizeof(ahb_addr[0]),				   // srcAddr, srcWidth
		sdram_writeBuffer, sizeof(sdram_writeBuffer[0]), // destAddr, destWidth
		sizeof(sdram_writeBuffer[0]), dsize,			 // bytesEachRequest, transferBytes
		kEDMA_MemoryToMemory							 // type
	);
	EDMA_SubmitTransfer(&hDMA, &transferConfig);

	//PRINTF ("\r\n Start test SDRAM read performance!\r\n");
	// SDRAM read test
	ahb_addr = (uint32_t *)0x80000000;
	g_msCount = 0;
	t1 = SysTick->VAL;
	EDMA_StartTransfer (&hDMA);
	while ((DMA0->TCD[0].CSR & DMA_CSR_DONE_MASK) == 0)
		;
	__asm("DSB");
	t2 = SysTick->VAL;
	g_ms = g_msCount;
	time = (((uint64_t)(t1 - t2) * 1000000000) / coreclk);
	
	uint32_t errCount = 0;
	for (uint32_t i=0;i<dsize/4;i++)
	{
	  if (sdram_readBuffer[i] != sdram_writeBuffer[i])
	  {
		errCount++;
	  }
	}
	if (!errCount)
	{
//	  PRINTF ("SDRAM read and write correctly!\r\n");
	}
	else
	{
	  PRINTF("SDRAM read and write fail!\r\n");
	}
	PRINTF("##SDRAM read performance: t1: %06d; t2: %06d; diff: %d; ns: %d, datasize: %d byte; perf: %dMB/s; g_ms: %d\r\n",
		   t1, t2, t1 - t2, time, dsize, (dsize * 1000)/time, g_ms);
}