/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_trdc.h"
#include "fsl_soc_src.h"
#include "pin_mux.h"
#include "resource_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    Prepare_CM7(CORE1_KICKOFF_ADDRESS);
}

void DEMO_BootCore1(void)
{
    /* RT1180 Specific CM7 Kick Off operation */
    __attribute__((unused)) volatile uint32_t result1, result2;

    /* Trigger S401 */
    while ((MU_RT_S3MUA->TSR & MU_TSR_TE0_MASK) == 0)
        ; /*Wait TR empty*/
    MU_RT_S3MUA->TR[0] = 0x17d20106;
    while ((MU_RT_S3MUA->RSR & MU_RSR_RF0_MASK) == 0)
        ; /*Wait RR Full*/
    while ((MU_RT_S3MUA->RSR & MU_RSR_RF1_MASK) == 0)
        ; /*Wait RR Full*/

    /* Response from ELE must be always read */
    result1 = MU_RT_S3MUA->RR[0];
    result2 = MU_RT_S3MUA->RR[1];

    /* Deassert Wait */
    BLK_CTRL_S_AONMIX->M7_CFG =
        (BLK_CTRL_S_AONMIX->M7_CFG & (~BLK_CTRL_S_AONMIX_M7_CFG_WAIT_MASK)) | BLK_CTRL_S_AONMIX_M7_CFG_WAIT(0);
}

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

static void APP_SetPid(uint8_t pid)
{
    const trdc_pid_config_t pidConfig =
        {
            .pid = pid,
            .lock = kTRDC_PidUnlocked0
        };

    TRDC_SetPid(EXAMPLE_TRDC_INSTANCE, kTRDC1_MasterCM33, &pidConfig);
}

void DEMO_SwitchToUntrustedDomain(void)
{
    APP_SetPid(EXAMPLE_NONSECURE_PID);
}

static void release_trdc(void)
{
    uint32_t result;
    /*
     * Check ELE FW status
     */
    do
    {
        /*Wait TR empty*/
        while ((MU_APPS_S3MUA->TSR & MU_TSR_TE0_MASK) == 0)
            ;
        /* Send Get FW Status command(0xc5), message size 0x01 */
        MU_APPS_S3MUA->TR[0] = 0x17c50106;
        /*Wait RR Full*/
        while ((MU_APPS_S3MUA->RSR & MU_RSR_RF0_MASK) == 0)
            ;
        (void)MU_APPS_S3MUA->RR[0];
        /*Wait RR Full*/
        while ((MU_APPS_S3MUA->RSR & MU_RSR_RF1_MASK) == 0)
            ;
        /* Get response code, only proceed when result is 0xD6 which is S400_SUCCESS_IND. */
        result = MU_APPS_S3MUA->RR[1];
        /*Wait RR Full*/
        while ((MU_APPS_S3MUA->RSR & MU_RSR_RF2_MASK) == 0)
            ;
        (void)MU_APPS_S3MUA->RR[2];
    } while (result != 0xD6);

    /*
     * Send Release TRDC command
     */
    do
    {
        /*Wait TR empty*/
        while ((MU_APPS_S3MUA->TSR & MU_TSR_TE0_MASK) == 0)
            ;
        /* Send release RDC command(0xc4), message size 2 */
        MU_APPS_S3MUA->TR[0] = 0x17c40206;
        /*Wait TR empty*/
        while ((MU_APPS_S3MUA->TSR & MU_TSR_TE1_MASK) == 0)
            ;
        /* Release TRDC A(TRDC1, 0x74) to the RTD core(cm33, 0x1) */
        MU_APPS_S3MUA->TR[1] = 0x7401;
        /*Wait RR Full*/
        while ((MU_APPS_S3MUA->RSR & MU_RSR_RF0_MASK) == 0)
            ;
        (void)MU_APPS_S3MUA->RR[0];
        /*Wait RR Full*/
        while ((MU_APPS_S3MUA->RSR & MU_RSR_RF1_MASK) == 0)
            ;
        result = MU_APPS_S3MUA->RR[1];
    } while (result != 0xD6);
    do
    {
        /*Wait TR empty*/
        while ((MU_APPS_S3MUA->TSR & MU_TSR_TE0_MASK) == 0)
            ;
        /* Send release RDC command(0xc4), message size 2 */
        MU_APPS_S3MUA->TR[0] = 0x17c40206;
        /*Wait TR empty*/
        while ((MU_APPS_S3MUA->TSR & MU_TSR_TE1_MASK) == 0)
            ;
        /* Release TRDC W(TRDC2, 0x78) to the RTD core(cm33, 0x1) */
        MU_APPS_S3MUA->TR[1] = 0x7801;
        /*Wait RR Full*/
        while ((MU_APPS_S3MUA->RSR & MU_RSR_RF0_MASK) == 0)
            ;
        (void)MU_APPS_S3MUA->RR[0];
        /*Wait RR Full*/
        while ((MU_APPS_S3MUA->RSR & MU_RSR_RF1_MASK) == 0)
            ;
        result = MU_APPS_S3MUA->RR[1];
    } while (result != 0xD6);
}

static void APP_SetTrdcGlobalConfig(void)
{
    uint32_t i, j;

    /* 1. Get the hardware configuration of the EXAMPLE_TRDC_INSTANCE module. */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(TRDC1, &hwConfig);

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
            TRDC_MrcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }

        for (i = 0U; i < hwConfig.mbcNumber; i++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC1, &memAccessConfig, i, j);
        }
    }

    /* 4. Enable all read/write/execute access for MRC/MBC access control for the instance used in secondary core. */
    trdc_hardware_config_t hwConfig1;
    TRDC_GetHardwareConfig(TRDC2, &hwConfig1);

    for (j = 0U; j < 8U; j++)
    {
        for (i = 0U; i < hwConfig1.mrcNumber; i++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC2, &memAccessConfig, i, j);
        }

        for (i = 0U; i < hwConfig1.mbcNumber; i++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC2, &memAccessConfig, i, j);
        }
    }
}

static void APP_SetTrdcDacConfigSecureDomain(void)
{
    /* Configure the access control for CM33(master 1 for TRDC1) for EXAMPLE_TRDC_SECURE_DOMAIN. */
    trdc_processor_domain_assignment_t domainAssignment0 = {
        /* This configuration is affective only for EXAMPLE_TRDC_SECURE_DOMAIN. */
        .domainId = EXAMPLE_TRDC_SECURE_DOMAIN,
        /* Use the domian ID in this MDA configuration directly. */
        .domainIdSelect = kTRDC_DidMda,
        /* When all the bits in PID(ID of the pocessor) is masked by the pidMask,
           (In this demo pidMask is 0x7 meaning when PID is ranging from 0~7),
           the processor's access right to the resource is controled by the
           configuration of EXAMPLE_TRDC_SECURE_DOMAIN, which is secure access only. */
        .pidDomainHitConfig = kTRDC_pidDomainHitInclusive,
        .pidMask = 0x7U,
        /* Force the master with PID that fits into EXAMPLE_TRDC_SECURE_DOMAIN to be secure. */
        .secureAttr = kTRDC_MasterSecure,
        .pid  = 0U,            /* Not used. */
        .lock = false
    };
    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC1_MasterCM33,
                                      0U, &domainAssignment0);
}

static void APP_SetTrdcDacConfigNonsecureDomain(void)
{
    /* Configure the access control for the EXAMPLE_TRDC_NONSECURE_DOMAIN of CM33. */
    trdc_processor_domain_assignment_t domainAssignment = {
        .domainId = EXAMPLE_TRDC_NONSECURE_DOMAIN,
        /* Use the domian ID in this MDA field directly. */
        .domainIdSelect = kTRDC_DidMda,
        /* When NOT all the bits in PID(ID of the pocessor) is masked by the pidMask,
           (In this demo pidMask is 0x7 meaning when PID is larger than 7), the
           processor's access right to the resource is controled by the configuration of
           EXAMPLE_TRDC_NONSECURE_DOMAIN, which is non-secure access only. */
        .pidDomainHitConfig = kTRDC_pidDomainHitExclusive,
        .pidMask = 0x7U,
        /* Force the master that fits into EXAMPLE_TRDC_SECURE_DOMAIN to be non-secure. */
        .secureAttr = kTRDC_ForceNonSecure,
        .pid  = 0U,               /* Not used. */
        .lock = false
    };

    TRDC_SetProcessorDomainAssignment(EXAMPLE_TRDC_INSTANCE, (uint8_t)kTRDC1_MasterCM33,
                                      1U, &domainAssignment);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    release_trdc();
    APP_SetTrdcGlobalConfig();
    BOARD_InitTEE();

    APP_SetPid(EXAMPLE_SECURE_PID);
    APP_SetTrdcDacConfigSecureDomain();
    APP_SetTrdcDacConfigNonsecureDomain();

    BOARD_InitDebugConsole();
    NVIC_EnableIRQ(APP_MU_IRQn);
}
/*${function:end}*/
