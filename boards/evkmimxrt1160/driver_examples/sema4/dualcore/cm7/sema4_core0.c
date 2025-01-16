/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_sema4.h"
#include "fsl_mu.h"
#include "demo_common.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_INVALID_GATE_NUM 0xFFFFFFFFU

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile uint32_t s_lastNotifyGateNum = DEMO_INVALID_GATE_NUM;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_WaitCore1Stat(uint32_t stat);

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_SEMA4IRQHandler(void)
{
    uint32_t notifyStatus;
    uint32_t notifyGateNum;

    notifyStatus = SEMA4_GetGateNotifyStatus(DEMO_SEMA4, DEMO_PROC_NUM);

    if (0U == notifyStatus)
    {
        notifyGateNum = DEMO_INVALID_GATE_NUM;
    }
    else
    {
        notifyGateNum = 31U - __CLZ(notifyStatus);

        SEMA4_ResetGateNotify(DEMO_SEMA4, s_lastNotifyGateNum);
    }

    s_lastNotifyGateNum = notifyGateNum;

    SDK_ISR_EXIT_BARRIER;
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
static void APP_CopyCore1Image(void)
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

int main(void)
{
    uint32_t gateNum;

    BOARD_InitHardware();

    APP_CopyCore1Image();

    PRINTF("SEMA4 dual core example start\r\n");

    MU_Init(DEMO_MU);

    SEMA4_Init(DEMO_SEMA4);

    SEMA4_ResetAllGates(DEMO_SEMA4);

    SEMA4_ResetAllGateNotify(DEMO_SEMA4);

    DEMO_EnableSema4Interrupt();

    DEMO_BootCore1();

    /* Wait for core 1 boot up. */
    DEMO_WaitCore1Stat(DEMO_STAT_CORE1_READY);

    for (gateNum = 0U; gateNum < FSL_FEATURE_SEMA4_GATE_COUNT; gateNum++)
    {
        PRINTF("\r\nGate %d/%d: ", gateNum + 1, FSL_FEATURE_SEMA4_GATE_COUNT);

        SEMA4_EnableGateNotifyInterrupt(DEMO_SEMA4, DEMO_PROC_NUM, (1U << gateNum));

        /* Let core 1 lock gate. */
        MU_SendMsg(DEMO_MU, DEMO_MU_CH, DEMO_CMD_LOCK_GATE(gateNum));

        /* Wait for gate locked by core 1. */
        DEMO_WaitCore1Stat(DEMO_STAT_GATE_LOCKED(gateNum));

        /* Try to lock the gate, return error. */
        if (kStatus_Success == SEMA4_TryLock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM))
        {
            PRINTF("Gate should be locked by core1 but not.\r\n");
            break;
        }

        /* Let core 1 unlock the gate. */
        MU_SendMsg(DEMO_MU, DEMO_MU_CH, DEMO_CMD_UNLOCK_GATE(gateNum));

        /* Wait for gate locked by core 1. */
        DEMO_WaitCore1Stat(DEMO_STAT_GATE_UNLOCKED(gateNum));

        /* Try to lock the gate, return success. */
        if (kStatus_Success != SEMA4_TryLock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM))
        {
            PRINTF("Gate should be unlocked by core1 but not.\r\n");
            break;
        }

        SEMA4_DisableGateNotifyInterrupt(DEMO_SEMA4, DEMO_PROC_NUM, (1U << gateNum));

        /* Check the notify status. */
        if (s_lastNotifyGateNum != gateNum)
        {
            break;
        }

        PRINTF("Pass\r\n");
    }

    if (gateNum < FSL_FEATURE_SEMA4_GATE_COUNT)
    {
        PRINTF("\r\nExample finished with error\r\n");
    }
    else
    {
        PRINTF("\r\nExample finished successfully\r\n");
    }

    while (1)
    {
    }
}
