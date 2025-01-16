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

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t cmd;
    uint32_t gateNum;

    BOARD_InitHardware();

    MU_Init(DEMO_MU);

    SEMA4_Init(DEMO_SEMA4);

    /* Notify core 0 that ready for the demo. */
    MU_SendMsg(DEMO_MU, DEMO_MU_CH, DEMO_STAT_CORE1_READY);

    /* Handle the command from core 0. */
    while (1)
    {
        cmd = MU_ReceiveMsg(DEMO_MU, DEMO_MU_CH);

        if (DEMO_IS_LOCK_CMD(cmd))
        {
            gateNum = DEMO_GET_LOCK_CMD_GATE(cmd);

            SEMA4_Lock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM);

            MU_SendMsg(DEMO_MU, DEMO_MU_CH, DEMO_STAT_GATE_LOCKED(gateNum));
        }
        else if (DEMO_IS_UNLOCK_CMD(cmd))
        {
            gateNum = DEMO_GET_LOCK_CMD_GATE(cmd);

            SEMA4_Unlock(DEMO_SEMA4, gateNum);

            MU_SendMsg(DEMO_MU, DEMO_MU_CH, DEMO_STAT_GATE_UNLOCKED(gateNum));
        }
        else
        {
            /* Empty. */
        }
    }
}
