/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app_core0.h"
#include "mcmgr.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_hardfaultFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void Fault_handler()
{
    trdc_domain_error_t error;
    while (kStatus_Success == TRDC_GetAndClearFirstDomainError(EXAMPLE_TRDC_INSTANCE, &error))
    {
        APP_CheckAndResolveMrcAccessError(&error);
        APP_CheckAndResolveMbcAccessError(&error);
        g_hardfaultFlag = true;
    }
}
/*!
 * @brief BusFault_Handler
 */
void BusFault_Handler(void)
{
    Fault_handler();
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief HardFault_Handler
 */
void HardFault_Handler(void)
{
    Fault_handler();
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Print the initial banner. */
    PRINTF("\r\nTRDC example starts on primary core\r\n");
    PRINTF("In primary core example we use 1 domain for secure access only.\r\n");

    /* 1. Enable all access to all blocks. */
    APP_SetTrdcGlobalConfig();

    /* 2. In primary core example we use 1 domain for secure access only. */
    APP_SetTrdcAccessControl();

    /* 3. Configure the domain access control for secure domain. */
    APP_SetTrdcDacConfigSecureDomain();

    /* 4. Configure the PID so that it fits into the domain access control of secure domain. */
    APP_SetPid(1U);

    /* 5. MBC function demonstration. */
    PRINTF("Set the selected MBC block to non-secure for domain that is secure access only\r\n");
    /* Set the non-secure to true for secure domain to disable the memory access. */
    APP_SetMbcUnaccessible(EXAMPLE_TRDC_SECURE_DOMAIN, true);

    /* Touch the memory, there will be hardfault. */
    g_hardfaultFlag = false;

    APP_TouchMbcMemory();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }

    PRINTF("The MBC selected block is accessiable for secure master now\r\n");

    /* 6. MRC function demonstration. */
    PRINTF("Set the selected MRC region to non-secure for domain that is secure access only\r\n");
    /* Set the non-secure to true for secure domain to disable the memory access. */
    APP_SetMrcUnaccessible(EXAMPLE_TRDC_SECURE_DOMAIN, true);

    /* Touch the memory, there will be hardfault. */
    g_hardfaultFlag = false;

    APP_TouchMrcMemory();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }
    PRINTF("The MRC selected region is accessiable for secure master now\r\n");

    PRINTF("TRDC example succeeds on primary core\r\n");

    /* Clear the secondary core start flag */
    (*(volatile uint32_t *)EXAMPLE_SECONDARY_CORE_START_FLAG_ADDRESS) = 0U;

    /* Prepare all flags. */
    (*(volatile uint32_t *)EXAMPLE_NONSECURE_MRC_NEED_RESOLVE_FLAG_ADDRESS) = 0U;
    (*(volatile uint32_t *)EXAMPLE_NONSECURE_MRC_RESOLVED_FLAG_ADDRESS) = 0U;
    (*(volatile uint32_t *)EXAMPLE_NONSECURE_MBC_NEED_RESOLVE_FLAG_ADDRESS) = 0U;
    (*(volatile uint32_t *)EXAMPLE_NONSECURE_MBC_RESOLVED_FLAG_ADDRESS) = 0U;

    /* Start the secondary core. */
#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* This section ensures the secondary core image is copied from flash location to the target RAM memory.
       It consists of several steps: image size calculation, image copying and cache invalidation (optional for some
       platforms/cases). These steps are not required on MCUXpresso IDE which copies the secondary core image to the
       target memory during startup automatically. */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
    /* Invalidate cache for memory range the secondary core image has been copied to. */
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        L1CACHE_CleanInvalidateSystemCacheByRange((uint32_t)CORE1_BOOT_ADDRESS, core1_image_size);
    }
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY*/
#endif /* CORE1_IMAGE_COPY_TO_RAM */

    /* Boot Secondary core application */
    (void)PRINTF("Starting Secondary core.\r\n");
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 2, kMCMGR_Start_Synchronous);
    (void)PRINTF("The secondary core application has been started.\r\n");

    /* Primary core finished printing, the secondary core can start now. */
    (*(volatile uint32_t *)EXAMPLE_SECONDARY_CORE_START_FLAG_ADDRESS) = 1U;

    while (1)
    {
        if ((*(volatile uint32_t *)EXAMPLE_NONSECURE_MRC_NEED_RESOLVE_FLAG_ADDRESS) == 1U)
        {
            /* Resolve secondary core access issue for MRC memory */
            APP_ResolveSecondaryCoreMrcAccessError();

            /* Mark the issue resolved. */
            (*(volatile uint32_t *)EXAMPLE_NONSECURE_MRC_RESOLVED_FLAG_ADDRESS) = 1U;
            /* Clear the flag. */
            (*(volatile uint32_t *)EXAMPLE_NONSECURE_MRC_NEED_RESOLVE_FLAG_ADDRESS) = 0U;
        }
        if ((*(volatile uint32_t *)EXAMPLE_NONSECURE_MBC_NEED_RESOLVE_FLAG_ADDRESS) == 1U)
        {
            /* Resolve secondary core access issue for MBC memory */
            APP_ResolveSecondaryCoreMbcAccessError();

            /* Mark the issue resolved. */
            (*(volatile uint32_t *)EXAMPLE_NONSECURE_MBC_RESOLVED_FLAG_ADDRESS) = 1U;
            /* Clear the flag. */
            (*(volatile uint32_t *)EXAMPLE_NONSECURE_MBC_NEED_RESOLVE_FLAG_ADDRESS) = 0U;
        }
    }
}
