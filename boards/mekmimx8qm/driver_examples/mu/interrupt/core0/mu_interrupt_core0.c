/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_lpuart.h"
#include "fsl_intmux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_MU CM4_0__MU0_B0
#define APP_MU_IRQHandler M4_0_MU0_B0_IRQHandler

#define CORE0_BOOT_CORE1 0
/* Flag indicates Core Boot Up*/
#define BOOT_FLAG 0x01U

/* Channel transmit and receive register */
#define CHN_MU_REG_NUM 0U

/* How many message is used to test message sending */
#define MSG_LENGTH 32U

/*
 * Use core 0 to boot core 1.
 */
#ifndef CORE0_BOOT_CORE1
#define CORE0_BOOT_CORE1 1
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t g_msgSend[MSG_LENGTH];
static uint32_t g_msgRecv[MSG_LENGTH];
volatile uint32_t g_curSend = 0;
volatile uint32_t g_curRecv = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function to fill the g_msgSend array.
 * This function set the g_msgSend values 0, 1, 2, 3...
 */
static void FillMsgSend(void)
{
    uint32_t i;
    for (i = 0U; i < MSG_LENGTH; i++)
    {
        g_msgSend[i] = i;
    }
}

/*!
 * @brief Function to clear the g_msgRecv array.
 * This function set g_msgRecv to be 0.
 */
static void ClearMsgRecv(void)
{
    uint32_t i;
    for (i = 0U; i < MSG_LENGTH; i++)
    {
        g_msgRecv[i] = 0U;
    }
}

/*!
 * @brief Function to validate the received messages.
 * This function compares the g_msgSend and g_msgRecv, if they are the same, this
 * function returns true, otherwise returns false.
 */
static bool ValidateMsgRecv(void)
{
    uint32_t i;
    for (i = 0U; i < MSG_LENGTH; i++)
    {
        PRINTF("Send: %d. Receive %d\r\n", g_msgSend[i], g_msgRecv[i]);

        if (g_msgRecv[i] != g_msgSend[i])
        {
            return false;
        }
    }
    return true;
}

void APP_MU_IRQHandler(void)
{
    uint32_t flag = 0;

    flag = MU_GetStatusFlags(APP_MU);
    if ((flag & kMU_Tx0EmptyFlag) == kMU_Tx0EmptyFlag)
    {
        if (g_curSend < MSG_LENGTH)
        {
            MU_SendMsgNonBlocking(APP_MU, CHN_MU_REG_NUM, g_msgSend[g_curSend++]);
        }
        else
        {
            MU_DisableInterrupts(APP_MU, kMU_Tx0EmptyInterruptEnable);
        }
    }
    if ((flag & kMU_Rx0FullFlag) == kMU_Rx0FullFlag)
    {
        if (g_curRecv < MSG_LENGTH)
        {
            g_msgRecv[g_curRecv++] = MU_ReceiveMsgNonBlocking(APP_MU, CHN_MU_REG_NUM);
        }
        else
        {
            MU_DisableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
        }
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    sc_ipc_t ipc;

    ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    if (sc_pm_set_resource_power_mode(ipc, SC_R_M4_0_INTMUX, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on INTMUX");
    }

    if (sc_pm_set_resource_power_mode(ipc, SC_R_M4_0_MU_0B, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on MU0_B");
    }

    NVIC_EnableIRQ(M4_0_MU0_B0_IRQn);

    /* Configure INTMUX to export CM4_0 MU0_A0 interrupt to other subsystem(CM4_1)*/
    INTMUX_Init(CM4_0__INTMUX);
    INTMUX_EnableInterrupt(CM4_0__INTMUX, 0U,
                           M4_0_INTMUX_SOURCE_MU0_A0_IRQn); /* Export CM4_0 MU0_A0 interrupt to INTMUX channel 0 */

    /* MUA init */
    MU_Init(APP_MU);
#if CORE0_BOOT_CORE1
    /* Boot core 1. */
    MU_BootCoreB(APP_MU, APP_CORE1_BOOT_MODE);
#endif

    /* Print the initial banner */
    PRINTF("\r\nMU example interrupt!\r\n");

    /* Wait Core 1 is Boot Up */
    while (BOOT_FLAG != MU_GetFlags(APP_MU))
    {
    }

    /* Fill the g_msgSend array before send */
    FillMsgSend();
    /* Clear the g_msgRecv array before receive */
    ClearMsgRecv();
    /* Enable transmit and receive interrupt */
    MU_EnableInterrupts(APP_MU, (kMU_Tx0EmptyInterruptEnable | kMU_Rx0FullInterruptEnable));
    /* Wait the data send and receive finish */
    while ((g_curSend < MSG_LENGTH) || (g_curRecv < MSG_LENGTH))
    {
    }

    /* Compare the data send and receive */
    if (true == ValidateMsgRecv())
    {
        PRINTF("MU example run succeed!");
    }
    else
    {
        PRINTF("MU example run Error!");
    }

    while (1)
    {
    }
}
