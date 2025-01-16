/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app_core0.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
#include "fsl_ele_base_api.h"
/*${header:end}*/

/*${macro:start}*/
#define ELE_TRDC_AON_ID    0x74
#define ELE_TRDC_WAKEUP_ID 0x78
#define ELE_CORE_CM33_ID   0x1
#define ELE_CORE_CM7_ID    0x2

/*
 * Set ELE_STICK_FAILED_STS to 0 when ELE status check is not required,
 * which is useful when debug reset, where the core has already get the
 * TRDC ownership at first time and ELE is not able to release TRDC
 * ownership again for the following TRDC ownership request.
 */
#define ELE_STICK_FAILED_STS 1

#if ELE_STICK_FAILED_STS
#define ELE_IS_FAILED(x) (x != kStatus_Success)
#else
#define ELE_IS_FAILED(x) false
#endif
/*${macro:end}*/

/*${function:start}*/
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)__section_begin("__core1_image");
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

void BOARD_InitHardware(void)
{
    status_t sts;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    CLOCK_EnableClock(kCLOCK_Trdc);

    /*
     * Differernt communication memory type involves differernt XCACHE
     *    SDRAM(SEMC):        system cache
     *    Hyperram(FlexSPI2): code cache
     * So disable code and system cache here for compatibility.
     */
    XCACHE_DisableCache(XCACHE_PS);
    XCACHE_DisableCache(XCACHE_PC);

    /* Get ELE FW status */
    do
    {
        uint32_t ele_fw_sts;
        sts = ELE_BaseAPI_GetFwStatus(MU_RT_S3MUA, &ele_fw_sts);
    } while (sts != kStatus_Success);

    /* Enble CM7 */
    do
    {
        sts = ELE_BaseAPI_EnableAPC(MU_RT_S3MUA);
    } while (ELE_IS_FAILED(sts));

    /* Release TRDC A to CM33 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_AON_ID, ELE_CORE_CM33_ID);
    } while (ELE_IS_FAILED(sts));

    /* Release TRDC W to CM33 core */
    do
    {
        sts = ELE_BaseAPI_ReleaseRDC(MU_RT_S3MUA, ELE_TRDC_WAKEUP_ID, ELE_CORE_CM33_ID);
    } while (ELE_IS_FAILED(sts));
}

void APP_SetTrdcGlobalConfig(void)
{
    uint32_t i, j;

    TRDC_Init(EXAMPLE_TRDC_INSTANCE);

    /* 1. Get the hardware configuration of the EXAMPLE_TRDC_INSTANCE module. */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(EXAMPLE_TRDC_INSTANCE, &hwConfig);

    /* 2. Set control policies for MRC and MBC access control configuration registers. */
    trdc_memory_access_control_config_t memAccessConfig;
    (void)memset(&memAccessConfig, 0, sizeof(memAccessConfig));

    /* 3. Enable all read/write/execute access for MRC/MBC access control. */
    memAccessConfig.nonsecureUsrX  = 1U;
    memAccessConfig.nonsecureUsrW  = 1U;
    memAccessConfig.nonsecureUsrR  = 1U;
    memAccessConfig.nonsecurePrivX = 1U;
    memAccessConfig.nonsecurePrivW = 1U;
    memAccessConfig.nonsecurePrivR = 1U;
    memAccessConfig.secureUsrX     = 1U;
    memAccessConfig.secureUsrW     = 1U;
    memAccessConfig.secureUsrR     = 1U;
    memAccessConfig.securePrivX    = 1U;
    memAccessConfig.securePrivW    = 1U;
    memAccessConfig.securePrivR    = 1U;

    for (j = 0U; j < 8U; j++)
    {
        for (i = 0U; i < hwConfig.mrcNumber; i++)
        {
            TRDC_MrcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, i, j);
        }

        for (i = 0U; i < hwConfig.mbcNumber; i++)
        {
            TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_INSTANCE, &memAccessConfig, i, j);
        }
    }

    /* 4. Enable all read/write/execute access for MRC/MBC access control for the instance used in secondary core. */
    trdc_hardware_config_t hwConfig1;
    TRDC_GetHardwareConfig(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, &hwConfig1);

    for (j = 0U; j < 8U; j++)
    {
        for (i = 0U; i < hwConfig1.mrcNumber; i++)
        {
            TRDC_MrcSetMemoryAccessConfig(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, &memAccessConfig, i, j);
        }

        for (i = 0U; i < hwConfig1.mbcNumber; i++)
        {
            TRDC_MbcSetMemoryAccessConfig(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, &memAccessConfig, i, j);
        }
    }
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
    mrcRegionConfig.memoryAccessControlSelect = 0U;
    mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_MRC_INDEX;
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
    mrcRegionConfig.nseEnable = true; /* Enable nonsecure, meaning disable secure. */
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_NONSECURE_DOMAIN;
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
            mbcBlockConfig.domainIdx = j;
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

    /* TRDC settings to make sure both core can access communication memory in secure/non-secure mode */
    (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
    mrcRegionConfig.memoryAccessControlSelect = 0U;
    mrcRegionConfig.valid                     = true;
    mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_COMM_START_ADDR;
    mrcRegionConfig.endAddr                   = EXAMPLE_TRDC_COMM_END_ADDR;
    mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_COMM_MRC_INDEX;
    mrcRegionConfig.regionIdx                 = EXAMPLE_TRDC_COMM_MRC_REGION_INDEX;

    /* Set TRDC to make sure core0 & core1 could access the communication memory in any mode */
    mrcRegionConfig.nseEnable = false;
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_SECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_COMM_INSTANCE, &mrcRegionConfig);

    mrcRegionConfig.nseEnable = true;
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_NONSECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_COMM_INSTANCE, &mrcRegionConfig);

    mrcRegionConfig.nseEnable = false;
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_SECONDARY_CORE_SECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_COMM_INSTANCE, &mrcRegionConfig);

    mrcRegionConfig.nseEnable = true;
    mrcRegionConfig.domainIdx = EXAMPLE_TRDC_SECONDARY_CORE_NONSECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_COMM_INSTANCE, &mrcRegionConfig);
}

void APP_SetTrdcDacConfigSecureDomain(void)
{
    /* Configure the access control for CM33(master 1 for TRDC1) for EXAMPLE_TRDC_SECURE_DOMAIN. */
    trdc_processor_domain_assignment_t domainAssignment0 = {
        .domainId =
            EXAMPLE_TRDC_SECURE_DOMAIN,  /* This configuration is affective only for EXAMPLE_TRDC_SECURE_DOMAIN. */
        .domainIdSelect = kTRDC_DidMda,  /* Use the domian ID in this MDA configuration directly. */
        .pidDomainHitConfig =
            kTRDC_pidDomainHitInclusive, /* When all the bits in PID(ID of the pocessor) is masked by the pidMask,
                                           (In this demo pidMask is 0x7 meaning when PID is ranging from 0~7),
                                           the processor's access right to the resource is controled by the
                                           configuration of EXAMPLE_TRDC_SECURE_DOMAIN, which is secure access only. */
        .pidMask = 0x7U,
        .secureAttr =
            kTRDC_ForceSecure, /* Force the master with PID that fits into EXAMPLE_TRDC_SECURE_DOMAIN to be secure. */
        .pid  = 0U,            /* Not used. */
        .lock = false};
    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC1_MasterCM33, 0U, &domainAssignment0);
}

void APP_SetTrdcDacConfigNonsecureDomain(void)
{
    /* Configure the access control for CM33(master 1 for TRDC1) for EXAMPLE_TRDC_NONSECURE_DOMAIN. */
    trdc_processor_domain_assignment_t domainAssignment = {
        .domainId =
            EXAMPLE_TRDC_NONSECURE_DOMAIN, /* This configuration is affective only for EXAMPLE_TRDC_NONSECURE_DOMAIN. */
        .domainIdSelect = kTRDC_DidMda,    /* Use the domian ID in this MDA configuration directly. */
        .pidDomainHitConfig =
            kTRDC_pidDomainHitExclusive,   /* When NOT all the bits in PID(ID of the pocessor) is masked by the pidMask,
                                             (In this demo pidMask is 0x7 meaning when PID is larger than 7), the
                                             processor's access right to the resource is controled by the configuration of
                                             EXAMPLE_TRDC_NONSECURE_DOMAIN, which is non-secure access only. */
        .pidMask = 0x7U,
        .secureAttr =
            kTRDC_ForceNonSecure, /* Force the master that fits into EXAMPLE_TRDC_SECURE_DOMAIN to be non-secure. */
        .pid  = 0U,               /* Not used. */
        .lock = false};
    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC1_MasterCM33, 1U, &domainAssignment);
}

void APP_SetPid(uint8_t pid)
{
    const trdc_pid_config_t pidConfig = {.pid = pid, .lock = kTRDC_PidUnlocked0};
    TRDC_SetPid(EXAMPLE_TRDC_INSTANCE, kTRDC1_MasterCM33, &pidConfig);
}

void APP_SetMrcUnaccessible(uint8_t domain, bool nseEnable)
{
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
    if (error->controller == kTRDC_MemRegionChecker1)
    {
        PRINTF("Core0 MRC violent access at address: 0x%8X\r\n", error->address);

        /* Set the MRC region descriptor configuration and select the memory access control of all access for this
         * region */
        trdc_mrc_region_descriptor_config_t mrcRegionConfig;
        (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
        mrcRegionConfig.memoryAccessControlSelect = 0U;
        mrcRegionConfig.valid                     = true;
        mrcRegionConfig.nseEnable                 = false;
        mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_MRC_START_ADDRESS;
        mrcRegionConfig.endAddr                   = EXAMPLE_TRDC_MRC_END_ADDRESS;
        mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_MRC_INDEX;
        mrcRegionConfig.domainIdx                 = EXAMPLE_TRDC_SECURE_DOMAIN;
        mrcRegionConfig.regionIdx                 = EXAMPLE_TRDC_MRC_REGION_INDEX;
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
        PRINTF("Core0 MBC violent access at address: 0x%8X\r\n", error->address);

        /* Set the MBC slave memory block configuration and select the memory access control of no access for this
         * memory block */
        trdc_mbc_memory_block_config_t mbcBlockConfig;
        (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
        mbcBlockConfig.memoryAccessControlSelect = 0U;
        /* Disable NSE to enable secure access. */
        mbcBlockConfig.nseEnable      = false;
        mbcBlockConfig.mbcIdx         = EXAMPLE_TRDC_MBC_INDEX;
        mbcBlockConfig.domainIdx      = EXAMPLE_TRDC_SECURE_DOMAIN;
        mbcBlockConfig.slaveMemoryIdx = EXAMPLE_TRDC_MBC_SLAVE_INDEX;
        mbcBlockConfig.memoryBlockIdx = EXAMPLE_TRDC_MBC_MEMORY_INDEX;
        TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_INSTANCE, &mbcBlockConfig);
    }
}

void APP_ResolveSecondaryCoreMrcAccessError(void)
{
    trdc_domain_error_t error;
    TRDC_GetAndClearFirstSpecificDomainError(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, &error,
                                             EXAMPLE_TRDC_SECONDARY_CORE_NONSECURE_DOMAIN);
    PRINTF("Core1 MRC violent access at address: 0x%8X\r\n", error.address);

    trdc_mrc_region_descriptor_config_t mrcRegionConfig;
    (void)memset(&mrcRegionConfig, 0, sizeof(mrcRegionConfig));
    mrcRegionConfig.memoryAccessControlSelect = 0U;
    mrcRegionConfig.valid                     = true;
    mrcRegionConfig.startAddr                 = EXAMPLE_TRDC_SECONDARY_CORE_MRC_START_ADDRESS;
    mrcRegionConfig.endAddr                   = EXAMPLE_TRDC_SECONDARY_CORE_MRC_END_ADDRESS;
    mrcRegionConfig.mrcIdx                    = EXAMPLE_TRDC_SECONDARY_CORE_MRC_INDEX;
    mrcRegionConfig.regionIdx                 = EXAMPLE_TRDC_SECONDARY_CORE_MRC_REGION_INDEX;
    mrcRegionConfig.nseEnable                 = true;
    mrcRegionConfig.domainIdx                 = EXAMPLE_TRDC_SECONDARY_CORE_NONSECURE_DOMAIN;
    TRDC_MrcSetRegionDescriptorConfig(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, &mrcRegionConfig);

    /* Disable the master assignment for domain control. */
    TRDC_EnableProcessorDomainAssignment(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, (uint8_t)kTRDC2_MasterCM7AXI, 1U, false);
}

void APP_ResolveSecondaryCoreMbcAccessError(void)
{
    trdc_domain_error_t error;
    TRDC_GetAndClearFirstSpecificDomainError(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, &error,
                                             EXAMPLE_TRDC_SECONDARY_CORE_NONSECURE_DOMAIN);
    PRINTF("Core1 MBC violent access at address: 0x%8X\r\n", error.address);

    trdc_mbc_memory_block_config_t mbcBlockConfig;
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = 0U;
    mbcBlockConfig.mbcIdx                    = EXAMPLE_TRDC_SECONDARY_CORE_MBC_INDEX;
    mbcBlockConfig.slaveMemoryIdx            = EXAMPLE_TRDC_SECONDARY_CORE_MBC_SLAVE_INDEX;
    mbcBlockConfig.memoryBlockIdx            = EXAMPLE_TRDC_SECONDARY_CORE_MBC_MEMORY_INDEX;
    mbcBlockConfig.nseEnable                 = true;
    mbcBlockConfig.domainIdx                 = EXAMPLE_TRDC_SECONDARY_CORE_NONSECURE_DOMAIN;
    TRDC_MbcSetMemoryBlockConfig(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, &mbcBlockConfig);

    /* Disable the master assignment for domain control. */
    TRDC_EnableProcessorDomainAssignment(EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE, (uint8_t)kTRDC2_MasterCM7AHBP, 1U,
                                         false);
}

void SystemInitHook(void)
{
    Prepare_CM7(CORE1_KICKOFF_ADDRESS);
}
/*${function:end}*/
