/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "fsl_semc.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_nand_flash.h"
#include "fsl_semc_nand_flash.h"
/*${header:end}*/

/*${function:start}*/
// semc_nand_timing_config_t timingConfig;

void delayUs(uint32_t delay_us)
{
    uint32_t s_tickPerMicrosecond = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000U;

    // Make sure this value is greater than 0
    if (!s_tickPerMicrosecond)
    {
        s_tickPerMicrosecond = 1;
    }
    delay_us = delay_us * s_tickPerMicrosecond;
    while (delay_us)
    {
        __NOP();
        delay_us--;
    }
}

semc_nand_config_t semcNandConfig = {
    .cePinMux          = kSEMC_MUXCSX0,                       /*!< The CE# pin mux setting. */
    .axiAddress        = EXAMPLE_SEMC_NAND_AXI_START_ADDRESS, /*!< The base address. */
    .axiMemsize_kbytes = 2 * 1024 * 1024,                     /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .ipgAddress        = EXAMPLE_SEMC_NAND_IPG_START_ADDRESS, /*!< The base address. */
    .ipgMemsize_kbytes = 2 * 1024 * 1024,                     /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .rdyactivePolarity = kSEMC_RdyActiveLow,                  /*!< Wait ready polarity. */
    .arrayAddrOption   = kSEMC_NandAddrOption_5byte_CA2RA3,
    .edoModeEnabled    = false,                               /*!< Address mode. */
    .columnAddrBitNum  = kSEMC_NandColum_12bit,               /*!< 12bit + 1bit to access the spare area. */
    .burstLen          = kSEMC_Nand_BurstLen64,               /*!< Burst length. */
    .portSize          = kSEMC_PortSize8Bit,                  /*!< Port size. */
    .timingConfig      = NULL,
};

semc_mem_nand_config_t semcMemConfig = {
    .semcNandConfig   = &semcNandConfig,
    .delayUS          = delayUs,
    .onfiVersion      = kNandOnfiVersion_1p0,
    .readyCheckOption = kNandReadyCheckOption_SR,
    .eccCheckType     = kNandEccCheckType_DeviceECC,
};

nand_config_t nandConfig = {
    .memControlConfig = (void *)&semcMemConfig,
    .driverBaseAddr   = (void *)EXAMPLE_SEMC,
};

void BOARD_InitMem(void)
{
    semc_config_t config;

    /* Initializes the MAC configure structure to zero. */
    memset(&config, 0, sizeof(semc_config_t));

    /*
       Get default configuration:
       config->queueWeight.queueaEnable = true;
       config->queueWeight.queuebEnable = true;
    */
    SEMC_GetDefaultConfig(&config);
    /* Disable queue B weight, which is for AXI bus access to SDRAM slave. */
    config.queueWeight.queuebEnable = false;
    /* Initialize SEMC. */
    SEMC_Init(SEMC, &config);
    /* Set SEMC clk source for NAND flash memory controller usage. */
    semcMemConfig.clkSrc_Hz = CLOCK_GetRootClockFreq(kCLOCK_Root_Semc);
}

void BOARD_ReconfigMPU(void)
{
    /* Disable code bus cache */
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        /* Enable the processor code bus to push all modified lines. */
        LMEM->PCCCR |= LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK | LMEM_PCCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PCCCR &= ~(LMEM_PCCCR_PUSHW0_MASK | LMEM_PCCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PCCCR &= ~LMEM_PCCCR_ENCACHE_MASK;
    }

    /* Disable system bus cache */
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        /* Enable the processor system bus to push all modified lines. */
        LMEM->PSCCR |= LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK | LMEM_PSCCR_GO_MASK;
        /* Wait until the cache command completes. */
        while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
        {
        }
        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        LMEM->PSCCR &= ~(LMEM_PSCCR_PUSHW0_MASK | LMEM_PSCCR_PUSHW1_MASK);
        /* Now disable the cache. */
        LMEM->PSCCR &= ~LMEM_PSCCR_ENCACHE_MASK;
    }

    /* Disable MPU */
    ARM_MPU_Disable();

    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write through. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Enables the processor system bus to invalidate all lines in both ways.
    and Initiate the processor system bus cache command. */
    LMEM->PSCCR |= LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK | LMEM_PSCCR_GO_MASK;
    /* Wait until the cache command completes */
    while ((LMEM->PSCCR & LMEM_PSCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PSCCR &= ~(LMEM_PSCCR_INVW0_MASK | LMEM_PSCCR_INVW1_MASK);
    /* Now enable the system bus cache. */
    LMEM->PSCCR |= LMEM_PSCCR_ENCACHE_MASK;

    /* Enables the processor code bus to invalidate all lines in both ways.
    and Initiate the processor code bus code cache command. */
    LMEM->PCCCR |= LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_GO_MASK;
    /* Wait until the cache command completes. */
    while ((LMEM->PCCCR & LMEM_PCCCR_GO_MASK) != 0U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    LMEM->PCCCR &= ~(LMEM_PCCCR_INVW0_MASK | LMEM_PCCCR_INVW1_MASK);
    /* Now enable the code bus cache. */
    LMEM->PCCCR |= LMEM_PCCCR_ENCACHE_MASK;
}

void BOARD_InitHardware(void)
{
    clock_root_config_t config = {0};
    config.mux                 = 7; /* SYS_PLL3_PFD0: 664.62MHz. */
    config.div                 = 4;

    BOARD_ConfigMPU();
    BOARD_ReconfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    /* Set semc clock to 166 MHz(rootclock/div: 664.62 / 4 = 166.1MHz).
       Note: SEMC max frequecny is 200MHz, its source selects from
        000 - OSC_RC_48M_DIV2(24MHz)
        001 - OSC_24M(24MHz)
        010 - OSC_RC_400M(400MHz)
        011 - OSC_RC_16M(16MHz)
        100 - SYS_PLL1_DIV5(200MHz)
        101 - SYS_PLL2(528MHz)
        110 - SYS_PLL2_PFD2(594MHz)
        111 - SYS_PLL3_PFD0(664.62MHz)
     */
    CLOCK_SetRootClock(kCLOCK_Root_Semc, &config);
    BOARD_InitMem();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
