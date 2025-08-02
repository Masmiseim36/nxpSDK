/*
 * Copyright 2023-2025 NXP
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

    BOARD_InitBootTEE();

    APP_SetPid(EXAMPLE_SECURE_PID);
    APP_SetTrdcDacConfigSecureDomain();
    APP_SetTrdcDacConfigNonsecureDomain();

    BOARD_InitDebugConsole();
    NVIC_EnableIRQ(APP_MU_IRQn);
}
/*${function:end}*/
