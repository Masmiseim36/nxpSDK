/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_cache.h"
#include "app_core1.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    L1CACHE_DisableDCache();
}

void APP_SetTrdcGlobalConfig(void)
{
    /* Has been settled in Core0 APP_SetTrdcGlobalConfig */
}

void APP_SetTrdcAccessControl(void)
{
    uint32_t i, j, m, n;

    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(EXAMPLE_TRDC_INSTANCE, &hwConfig);

    /* 1. Set the configuration for MRC. */
    trdc_mrc_region_descriptor_config_t mrcRegionConfig;
    (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
    /* First clear all the memory and control policies for all the resions of the MRC instance to be tested.
       Otherwise if any other region that has the same address but different control policy will affect the test result
       of the region to be tested.  */
    mrcRegionConfig.mrcIdx = EXAMPLE_TRDC_MRC_INDEX;
    for (i = 0; i < hwConfig.domainNumber; i++)
    {
        mrcRegionConfig.domainIdx = i;
        /* Get region count for current MRC instance */
        m = TRDC_GetMrcRegionNumber(EXAMPLE_TRDC_INSTANCE, EXAMPLE_TRDC_MRC_INDEX);
        for (j = 0U; j < m; j++)
        {
            mrcRegionConfig.regionIdx = j;
            TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);
        }
    }

    /* Configure the access to EXAMPLE_TRDC_SECURE_DOMAIN to secure access only. */
    mrcRegionConfig.memoryAccessControlSelect = 0U;
    mrcRegionConfig.valid                     = true;
    mrcRegionConfig.nseEnable                 = false; /* Disable nonsecure, meaning enable secure. */
    mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_MRC_INDEX;
    mrcRegionConfig.domainIdx                 = EXAMPLE_TRDC_SECURE_DOMAIN;
    mrcRegionConfig.regionIdx                 = EXAMPLE_TRDC_MRC_REGION_INDEX;
    mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_MRC_START_ADDRESS;
    mrcRegionConfig.endAddr                   = EXAMPLE_TRDC_MRC_END_ADDRESS;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);

    /* Configure the access to EXAMPLE_TRDC_NONSECURE_DOMAIN to non-secure access only. */
    mrcRegionConfig.mrcIdx    = EXAMPLE_TRDC_MRC_INDEX;
    mrcRegionConfig.startAddr = EXAMPLE_TRDC_MRC_START_ADDRESS;
    mrcRegionConfig.endAddr   = EXAMPLE_TRDC_MRC_END_ADDRESS;
    mrcRegionConfig.nseEnable = true; /* Enable nonsecure, meaning disable secure. */
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_NONSECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);

    /* Some variables are located in OCRAM1, make sure they can be accessed from secure and non-secure */
    mrcRegionConfig.mrcIdx    = 3U;
    mrcRegionConfig.regionIdx = 0;
    mrcRegionConfig.startAddr = 0x20480000UL;
    mrcRegionConfig.endAddr   = 0x204FFFFFUL;

    mrcRegionConfig.nseEnable = true; /* Enable nonsecure, meaning disable secure. */
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_NONSECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);

    mrcRegionConfig.nseEnable = false; /* Disable nonsecure, meaning enable secure. */
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_SECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);

    /* 2. Set the configuration for MBC. */
    trdc_slave_memory_hardware_config_t mbcHwConfig;
    trdc_mbc_memory_block_config_t mbcBlockConfig;
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = 0U;

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        mbcBlockConfig.mbcIdx = i;
        for (j = 0U; j < hwConfig.domainNumber; j++)
        {
            mbcBlockConfig.domainIdx = j;
            if (j == EXAMPLE_TRDC_NONSECURE_DOMAIN)
            {
                /* Configure the access to EXAMPLE_TRDC_NONSECURE_DOMAIN to non-secure access only. */
                mbcBlockConfig.nseEnable = true;
            }
            else if (j == EXAMPLE_TRDC_SECURE_DOMAIN)
            {
                /* Configure the access to EXAMPLE_TRDC_SECURE_DOMAIN to secure access only. */
                mbcBlockConfig.nseEnable = false;
            }
            else
            {
                /* Do not need to configure the other domains. */
                continue;
            }
            for (m = 0U; m < 4; m++)
            {
                TRDC_GetMbcHardwareConfig(EXAMPLE_TRDC_INSTANCE, &mbcHwConfig, i, m);
                if (mbcHwConfig.blockNum == 0U)
                {
                    break;
                }
                mbcBlockConfig.slaveMemoryIdx = m;
                for (n = 0U; n < mbcHwConfig.blockNum; n++)
                {
                    mbcBlockConfig.memoryBlockIdx = n;
                    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
                }
            }
        }
    }
}

void APP_SetTrdcDacConfigSecureDomain(void)
{
    /* Configure the access control for CM7 AXI and AHBP(master 0&1 for TRDC2) for EXAMPLE_TRDC_SECURE_DOMAIN. */
    trdc_processor_domain_assignment_t domainAssignment0 = {
        .domainId =
            EXAMPLE_TRDC_SECURE_DOMAIN,  /* This configuration is affective only for EXAMPLE_TRDC_SECURE_DOMAIN. */
        .domainIdSelect = kTRDC_DidMda,  /* Use the domian ID in this MDA configuration directly. */
        .pidDomainHitConfig =
            kTRDC_pidDomainHitInclusive, /* When all the bits in PID(ID of the pocessor) is masked by the pidMask,
                                           (In this demo pidMask is 0x7 meaning when PID is ranging from 0~7),
                                           the processor's access right to the resource, is controled by the
                                           configuration of EXAMPLE_TRDC_SECURE_DOMAIN, which is secure access only. */
        .pidMask = 0x7U,
        .secureAttr =
            kTRDC_ForceSecure, /* Force the master with PID that fits into EXAMPLE_TRDC_SECURE_DOMAIN to be secure. */
        .pid  = 3U,
        .lock = false};
    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC2_MasterCM7AXI, 0U, &domainAssignment0);
    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC2_MasterCM7AHBP, 0U, &domainAssignment0);
}

void APP_SetTrdcDacConfigNonsecureDomainMbc(void)
{
    /* Configure the access control for CM7 AHBP which controls the MBC(according to system block) used in this example.
     */
    trdc_processor_domain_assignment_t domainAssignment = {
        .domainId =
            EXAMPLE_TRDC_NONSECURE_DOMAIN, /* This configuration is affective only for EXAMPLE_TRDC_NONSECURE_DOMAIN. */
        .domainIdSelect = kTRDC_DidMda,    /* Use the domian ID in this MDA configuration directly. */
        .pidDomainHitConfig =
            kTRDC_pidDomainHitExclusive,   /* When NOT all the bits in PID(ID of the pocessor) is masked by the pidMask,
                                              (In this demo pidMask is 0x7 meaning when PID is larger than 7),
                                              the processor's access right to the resource is controled by the
                                              configuration of EXAMPLE_TRDC_NONSECURE_DOMAIN, which is non-secure access
                                              only. */
        .pidMask = 0x7U,
        .secureAttr =
            kTRDC_ForceNonSecure, /* Force the master that fits into EXAMPLE_TRDC_SECURE_DOMAIN to be non-secure. */
        .pid  = 8U,
        .lock = false};
    TRDC_EnableProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC2_MasterCM7AXI, 0U, false);
    TRDC_EnableProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC2_MasterCM7AHBP, 0U, false);
    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC2_MasterCM7AHBP, 1U, &domainAssignment);
}

void APP_SetTrdcDacConfigNonsecureDomainMrc(void)
{
    /* Configure the access control for CM7 AXI which controls the MRC(according to system block) used in this example.
     */
    trdc_processor_domain_assignment_t domainAssignment = {
        .domainId =
            EXAMPLE_TRDC_NONSECURE_DOMAIN, /* This configuration is affective only for EXAMPLE_TRDC_NONSECURE_DOMAIN. */
        .domainIdSelect = kTRDC_DidMda,    /* Use the domian ID in this MDA configuration directly. */
        .pidDomainHitConfig =
            kTRDC_pidDomainHitExclusive,   /* When NOT all the bits in PID(ID of the pocessor) is masked by the pidMask,
                                              (In this demo pidMask is 0x7 meaning when PID is larger than 7), the
                                              processor's access right to the resource is controled by the configuration
                                              of EXAMPLE_TRDC_NONSECURE_DOMAIN, which is non-secure access only. */
        .pidMask = 0x7U,
        .secureAttr =
            kTRDC_ForceNonSecure, /* Force the master that fits into EXAMPLE_TRDC_SECURE_DOMAIN to be non-secure. */
        .pid  = 8U,
        .lock = false};
    TRDC_EnableProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC2_MasterCM7AHBP, 1U, false);
    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC2_MasterCM7AXI, 1U, &domainAssignment);
}

void APP_SetMrcUnaccessible(uint8_t domain, bool nseEnable)
{
    /* Set the MRC region descriptor configuration and select the memory access control of no access for this region. */
    trdc_mrc_region_descriptor_config_t mrcRegionConfig;
    (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
    mrcRegionConfig.memoryAccessControlSelect = 0;
    mrcRegionConfig.valid                     = true;
    mrcRegionConfig.nseEnable                 = nseEnable;
    mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_MRC_START_ADDRESS;
    mrcRegionConfig.endAddr                   = EXAMPLE_TRDC_MRC_END_ADDRESS;
    mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_MRC_INDEX;
    mrcRegionConfig.domainIdx                 = domain;
    mrcRegionConfig.regionIdx                 = EXAMPLE_TRDC_MRC_REGION_INDEX;

    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);
}

void APP_TouchMrcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)EXAMPLE_TRDC_MRC_START_ADDRESS);
}

void APP_CheckAndResolveMrcAccessError(trdc_domain_error_t *error)
{
    if (error->controller == kTRDC_MemRegionChecker6)
    {
        PRINTF("Core1 MRC violent access at address: 0x%8X\r\n", error->address);

        /* Set the MRC region descriptor configuration and select the memory access control of all access for this
         * region */
        trdc_mrc_region_descriptor_config_t mrcRegionConfig;
        (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
        mrcRegionConfig.memoryAccessControlSelect = 0U;
        mrcRegionConfig.valid                     = true;
        mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_MRC_START_ADDRESS;
        mrcRegionConfig.endAddr                   = EXAMPLE_TRDC_MRC_END_ADDRESS;
        mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_MRC_INDEX;
        mrcRegionConfig.regionIdx                 = EXAMPLE_TRDC_MRC_REGION_INDEX;
        /* Disable nseEnable meaning enable secure access for EXAMPLE_TRDC_SECURE_DOMAIN. */
        mrcRegionConfig.nseEnable = false;
        mrcRegionConfig.domainIdx = EXAMPLE_TRDC_SECURE_DOMAIN;
        TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_INSTANCE, &mrcRegionConfig);
    }
}

void APP_SetMbcUnaccessible(uint8_t domain, bool nseEnable)
{
    trdc_mbc_memory_block_config_t mbcBlockConfig;
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = 0U;
    mbcBlockConfig.nseEnable                 = nseEnable;
    mbcBlockConfig.mbcIdx                    = EXAMPLE_TRDC_MBC_INDEX;
    mbcBlockConfig.domainIdx                 = domain;
    mbcBlockConfig.slaveMemoryIdx            = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
    mbcBlockConfig.memoryBlockIdx            = EXAMPLE_TRDC_MBC_MEMORY_INDEX;

    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
}

void APP_TouchMbcMemory(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)EXAMPLE_TRDC_MBC_START_ADDRESS);
}

void APP_CheckAndResolveMbcAccessError(trdc_domain_error_t *error)
{
    if (error->controller == kTRDC_MemBlockController0)
    {
        PRINTF("Core1 MBC violent access at address: 0x%8X\r\n", error->address);

        trdc_mbc_memory_block_config_t mbcBlockConfig;
        (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
        mbcBlockConfig.memoryAccessControlSelect = 0U;
        mbcBlockConfig.mbcIdx                    = EXAMPLE_TRDC_MBC_INDEX;
        mbcBlockConfig.slaveMemoryIdx            = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
        mbcBlockConfig.memoryBlockIdx            = EXAMPLE_TRDC_MBC_MEMORY_INDEX;
        /* Disable nseEnable meaning enable secure access for EXAMPLE_TRDC_SECURE_DOMAIN. */
        mbcBlockConfig.nseEnable = false;
        mbcBlockConfig.domainIdx = EXAMPLE_TRDC_SECURE_DOMAIN;
        TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
    }
}
/*${function:end}*/
