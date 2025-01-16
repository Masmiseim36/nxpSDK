/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app_core1.h"
#include "mcmgr.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_SECURE 0
#define DEMO_NONSECURE_MBC 1
#define DEMO_NONSECURE_MRC 2
#define DEMO_OTHER 3
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_INIT(volatile static bool g_hardfaultFlag) = false;
AT_NONCACHEABLE_SECTION_INIT(volatile static uint8_t g_demo) = DEMO_SECURE;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}

void Fault_handler()
{
    if (g_demo == DEMO_SECURE)
    {
        trdc_domain_error_t error;
        while (kStatus_Success == TRDC_GetAndClearFirstDomainError(EXAMPLE_TRDC_INSTANCE, &error))
        {
            APP_CheckAndResolveMrcAccessError(&error);
            APP_CheckAndResolveMbcAccessError(&error);
            g_hardfaultFlag = true;
        }
    }
    else if (g_demo == DEMO_NONSECURE_MRC)
    {
        /* Set the flag to let the primary core to resolve the issue. */
        (*(volatile uint32_t *)EXAMPLE_NONSECURE_MRC_NEED_RESOLVE_FLAG_ADDRESS) = 1U;
        /* Clear the g_demo to prevent setting the flag again by mistake after primary core has resolved it. */
        g_demo = DEMO_OTHER;
    }
    else if (g_demo == DEMO_NONSECURE_MBC)
    {
        /* Set the flag to let the primary core to resolve the issue. */
        (*(volatile uint32_t *)EXAMPLE_NONSECURE_MBC_NEED_RESOLVE_FLAG_ADDRESS) = 1U;
        /* Clear the g_demo to prevent setting the flag again by mistake after primary core has resolved it. */
        g_demo = DEMO_OTHER;
    }
    else
    {
        /* Do nothing. */
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
    uint32_t startupData;
    mcmgr_status_t status;

    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(&startupData);
    } while (status != kStatus_MCMGR_Success);

    /* Wait for the primary core to finished printing. */
    while ((*(volatile uint8_t *)EXAMPLE_SECONDARY_CORE_START_FLAG_ADDRESS) == 0U)
    {
    }

    PRINTF("\r\nIn secondary core demo we use 2 domains, one is for secure access only, and the other is for non-secure access only.\r\n");

    /* 1. Enable all access to all blocks. */
    APP_SetTrdcGlobalConfig();

    /* 2. In secondary core demo we use 2 domains, one is for secure access only, and the other is for non-secure access only. */
    APP_SetTrdcAccessControl();

    PRINTF("Secondary core secure access demo starts.\r\n");
    /* 3. Access demo for secure master. */
    /* 3.1 Configure the domain access control for secure domain. */
    APP_SetTrdcDacConfigSecureDomain();

    /* 3.2 MBC function demonstration. */
    PRINTF("Set the selected MBC block to non-secure for domain that is secure access only\r\n");
    /* Set the non-secure to true for secure domain to disable the memory access. */
    APP_SetMbcUnaccessible(EXAMPLE_TRDC_SECURE_DOMAIN, true);

    /* Touch the MBC, there will be hardfault. */
    g_hardfaultFlag = false;

    __DSB();

    APP_TouchMbcMemory();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }

    PRINTF("The MBC selected block is accessiable for secure master now\r\n");

    /* 3.3 MRC function demonstration. */
    PRINTF("Set the selected MRC region to non-secure for domain that is secure access only\r\n");
    /* Set the non-secure to true for secure domain to disable the memory access. */
    APP_SetMrcUnaccessible(EXAMPLE_TRDC_SECURE_DOMAIN, true);

    /* Touch the MRC, there will be hardfault. */
    g_hardfaultFlag = false;

    __DSB();

    APP_TouchMrcMemory();

    /* Wait for the hardfault occurs. */
    while (!g_hardfaultFlag)
    {
    }
    PRINTF("The MRC selected region is accessiable for secure master now\r\n");

    PRINTF("Secondary core non-secure access demo starts.\r\n");
    /* 4. Access demo for non-secure master. */
    /* 4.1 Configure the MBC accessibility. */
    PRINTF("Set the selected MBC block to non-secure for domain that is secure access only\r\n");
    /* Set the non-secure to false for non-secure domain to disable the memory access. */
    APP_SetMbcUnaccessible(EXAMPLE_TRDC_NONSECURE_DOMAIN, false);

    /* 4.2 Configure the MRC accessibility. */
    PRINTF("Set the selected MRC region to non-secure for domain that is secure access only\r\n");
    /* Set the non-secure to false for non-secure domain to disable the memory access. */
    APP_SetMrcUnaccessible(EXAMPLE_TRDC_NONSECURE_DOMAIN, false);

    /* 4.3 Cofigure the domain access control for MBC for non-secure domain.
       Notice once this configuration is done, the processor master is forced to be non-secure,
       thus can no-loner access the TRDC register, because TRDC is secure access only.*/
    g_demo = DEMO_NONSECURE_MBC;
    APP_SetTrdcDacConfigNonsecureDomainMbc();

    /* 4.4 Touch the MBC, there will be hardfault. */
    __DSB();
    APP_TouchMbcMemory();

    /* Wait for the primary core to change TRDC configuration to resolve the access error. */
    while ((*(volatile uint32_t *)EXAMPLE_NONSECURE_MBC_RESOLVED_FLAG_ADDRESS) == 0U)
    {
    }

    PRINTF("The MBC selected block is accessiable for non-secure master now\r\n");

    /* 4.5 Cofigure the domain access control for MRC for non-secure domain.
       Notice once this configuration is done, the processor master is forced to be non-secure,
       thus can no-loner access the TRDC register, because TRDC is secure access only. */
    g_demo = DEMO_NONSECURE_MRC;
    APP_SetTrdcDacConfigNonsecureDomainMrc();

    /* 4.6 Touch the MRC, there will be hardfault. */
    __DSB();
    APP_TouchMrcMemory();

    /* Wait for the primary core to change TRDC configuration to resolve the access error. */
    while ((*(volatile uint32_t *)EXAMPLE_NONSECURE_MRC_RESOLVED_FLAG_ADDRESS) == 0U)
    {
    }
    PRINTF("The MRC selected region is accessiable for non-secure master now\r\n");

    PRINTF("TRDC example succeeds on secondary core\r\n");

    while (1)
    {
    }
}
