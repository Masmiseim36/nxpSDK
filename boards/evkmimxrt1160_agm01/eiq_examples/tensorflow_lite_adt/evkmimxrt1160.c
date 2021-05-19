/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file  evkmimxrt1160.c
 * @brief The  evkmimxrt1160.c file defines GPIO pins and I2C CMSIS utilities for  evkmimxrt1160 board.
 */

#include "evkmimxrt1160.h"
#include "fsl_gpc.h"

/*
 * Execute the instrument to enter low power.
 */
static void stop(void)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __asm("WFI");
}

/*!
 * @brief Configures the system to WAIT power mode.
 *        API name used from Kinetis family to maintain compatibility.
 *
 * @param Power peripheral base address (dummy).
 * @return Configuration error code.
 */
status_t SMC_SetPowerModeWait(void *arg)
{
#if __CORTEX_M == 7
    GPC_CM_SetNextCpuMode(GPC_CPU_MODE_CTRL_0, kGPC_WaitMode);
#endif
#if __CORTEX_M == 4
    GPC_CM_SetNextCpuMode(GPC_CPU_MODE_CTRL_1, kGPC_WaitMode);
#endif
    stop();

    return kStatus_Success;
}

/*!
 * @brief Configures the system to VLPR power mode.
 *        API name used from Kinetis family to maintain compatibility.
 *
 * @param Power peripheral base address (dummy).
 * @return Configuration error code.
 */
status_t SMC_SetPowerModeVlpr(void *arg)
{
#if __CORTEX_M == 7
    GPC_CM_SetNextCpuMode(GPC_CPU_MODE_CTRL_0, kGPC_WaitMode);
#endif
#if __CORTEX_M == 4
    GPC_CM_SetNextCpuMode(GPC_CPU_MODE_CTRL_1, kGPC_WaitMode);
#endif
    stop();

    return kStatus_Success;
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_uart_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPUART1_GetFreq(void)
{
    return BOARD_DebugConsoleSrcFreq();
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_i2c_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPI2C1_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_OscRc48MDiv2);
}

/*! @brief       Determines the Clock Frequency feature.
 *  @details     The Clock Frequecny computation API required by fsl_spi_cmsis.c.
 *  @param[in]   void
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      uint32_t Returns the clock frequency .
 */
uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetFreqFromObs(CCM_OBS_LPSPI1_CLK_ROOT);
}
