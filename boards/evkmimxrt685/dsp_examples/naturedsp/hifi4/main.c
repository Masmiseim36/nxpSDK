/*
 * Copyright 2019-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_debug_console.h"
#include "srtm_naturedsp_test.h"

#include "NatureDSP_Signal.h"

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

int main(void)
{
    char version[30];

    BOARD_InitHardware();

    PRINTF("\r\n[DSP Main] Running NatureDSP library on DSP core\r\n");

    NatureDSP_Signal_get_library_version(version);
    PRINTF("[DSP Main] NatureDSP library version: %s\r\n", version);

    NatureDSP_Signal_get_library_api_version(version);
    PRINTF("[DSP Main] NatureDSP library API version: %s\r\n\r\n", version);

    TEST_FFT();
    TEST_VEC_DOT();
    TEST_VEC_ADD();
    TEST_VEC_MAX();
    TEST_IIR();
    TEST_FIR_BLMS();

    while (1)
    {
    }
}
