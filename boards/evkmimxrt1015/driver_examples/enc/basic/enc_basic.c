/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_enc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    enc_config_t mEncConfigStruct;
    uint32_t mCurPosValue;

    BOARD_InitHardware();

    PRINTF("\r\nENC Basic Example.\r\n");

    /* Initialize the ENC module. */
    ENC_GetDefaultConfig(&mEncConfigStruct);
#if (defined(FSL_FEATURE_ENC_HAS_CTRL3) && FSL_FEATURE_ENC_HAS_CTRL3)
    /*
     * If there is CTRL3, the period measurement is enabled by default,
     * with this setting, the POSD is loaded to POSDH only when POSD
     * is read (calling ENC_GetPositionDifferenceValue).
     * In this project, the POSD is desired to be loaded to POSDH when
     * UPOS is read (calling ENC_GetPositionValue), so disable the
     * period measurement here.
     */
    mEncConfigStruct.enablePeriodMeasurementFunction = false;
#endif

    ENC_Init(DEMO_ENC_BASEADDR, &mEncConfigStruct);
    ENC_DoSoftwareLoadInitialPositionValue(DEMO_ENC_BASEADDR); /* Update the position counter with initial value. */

    PRINTF("Press any key to get the encoder values ...\r\n");

    while (1)
    {
        GETCHAR();
        PRINTF("\r\n");

        /* This read operation would capture all the position counter to responding hold registers. */
        mCurPosValue = ENC_GetPositionValue(DEMO_ENC_BASEADDR);

        /* Read the position values. */
        PRINTF("Current position value: %ld\r\n", mCurPosValue);
        PRINTF("Position differential value: %d\r\n", (int16_t)ENC_GetHoldPositionDifferenceValue(DEMO_ENC_BASEADDR));
        PRINTF("Position revolution value: %d\r\n", ENC_GetHoldRevolutionValue(DEMO_ENC_BASEADDR));
    }
}
