/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_mu.h"
#include "tee_fault_common.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t userOption = 0U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_WaitCore1Stat(uint32_t stat);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void Fault_Handler(void)
{
    switch (userOption)
    {
        case '0':
            PRINTF("Hardfault triggered by invalid data access\r\n");
            break;

        case '1':
            PRINTF("Hardfault triggered by invalid parameters\r\n");
            break;

        default:
            PRINTF("ERROR: HardFault happens with unknown reason\r\n");
            break;
    }

    PRINTF("Reset and rerun\r\n");

    while (1)
        ;
}

void HardFault_Handler(void)
{
    Fault_Handler();
}

void BusFault_Handler(void)
{
    Fault_Handler();
}

static void DEMO_WaitCore1Stat(uint32_t stat)
{
    while (stat != MU_ReceiveMsg(DEMO_MU, DEMO_MU_CH))
    {
    }
}

/*!
 * @brief Function to copy core1 image to execution address.
 */
static void DEMO_CopyCore1Image(void)
{
#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image  - not required on MCUXpresso IDE. MCUXpresso copies the secondary core
       image to the target memory during startup automatically */
    uint32_t core1_image_size = get_core1_image_size();

    PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
#endif
    memcpy((void *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
#endif
#endif
}

static void DEMO_PrimaryCoreInvalidDataAccess(void)
{
    /*
     * The DEMO_INVALID_DATA_ADDR is inaccessible for untrusted domain,
     * so the access results to hardfault.
     */
    (*(volatile uint32_t *)DEMO_INVALID_DATA_ADDR)++;

    /* Should never get here */
    while (1)
        ;
}

static void DEMO_PrimaryCoreInvalidParameters(void)
{
    /*
     * The DEMO_INVALID_DATA_ADDR is inaccessible for untrusted domain,
     * so the access results to hardfault.
     */
    PRINTF("%s\r\n", (char *)DEMO_INVALID_DATA_ADDR);

    /* Should never get here */
    while (1)
        ;
}

static void DEMO_SecondaryCoreInvalidDataAccess(void)
{
    MU_SendMsg(DEMO_MU, DEMO_MU_CH, DEMO_MU_MSG_INVALID_DATA_ACCESS);

    DEMO_WaitCore1Stat(DEMO_MU_MSG_INVALID_DATA_ACCESS_DONE);

    PRINTF("Secondary core hardfault triggered by invalid data access\r\n");
    PRINTF("Reset and rerun\r\n");

    while (1)
        ;
}

static void DEMO_SecondaryCoreInvalidParameters(void)
{
    MU_SendMsg(DEMO_MU, DEMO_MU_CH, DEMO_MU_MSG_INVALID_PARAM);

    DEMO_WaitCore1Stat(DEMO_MU_MSG_INVALID_PARAM_DONE);

    PRINTF("Secondary core hardfault triggered by invalid parameters\r\n");
    PRINTF("Reset and rerun\r\n");

    while (1)
        ;
}

int main(void)
{
    BOARD_InitHardware();

    DEMO_CopyCore1Image();

    PRINTF("TEE fault example start\r\n");

    MU_Init(DEMO_MU);

    DEMO_BootCore1();

    /* Wait for core 1 boot up. */
    DEMO_WaitCore1Stat(DEMO_MU_MSG_CORE1_READY);

    while (1)
    {
        PRINTF("Select the option:\r\n");
        PRINTF("0: Primary core invalid data access\r\n");
        PRINTF("1: Primary core invalid input parameters\r\n");
        PRINTF("2: Secondary core invalid data access\r\n");
        PRINTF("3: Secondary core invalid input parameters\r\n");

        userOption = 0U;
        userOption = GETCHAR();

        switch (userOption)
        {
            case '0':
                /* Switch to untrusted domain. */
                DEMO_SwitchToUntrustedDomain();
                DEMO_PrimaryCoreInvalidDataAccess();
                break;

            case '1':
                /* Switch to untrusted domain. */
                DEMO_SwitchToUntrustedDomain();
                DEMO_PrimaryCoreInvalidParameters();
                break;

            case '2':
                DEMO_SecondaryCoreInvalidDataAccess();
                break;

            case '3':
                DEMO_SecondaryCoreInvalidParameters();
                break;

            default:
                PRINTF("Invalid input, select again\r\n");
                break;
        }
    }
}
