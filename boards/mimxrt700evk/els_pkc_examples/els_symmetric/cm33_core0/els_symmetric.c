/*
 * Copyright 2020-2021,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "mcuxCsslExamples.h"
#include "els_symmetric.h"
#include "mcux_els.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;
    uint32_t pass = 0u;
    uint32_t fail = 0u;

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Enable ELS and related clocks */
    if (ELS_PowerDownWakeupInit(ELS) != kStatus_Success)
    {
        PRINTF("\r\nELS init failed\r\n");
        return kStatus_Fail;
    }

    PRINTF("\r\nELS symmetric cipher example\r\n");
    PRINTF("\r\n============================\r\n");

    PRINTF("AES128-CBC encryption:");
    if (mcuxClEls_Cipher_Aes128_Cbc_Encrypt_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("AES128-ECB encryption:");
    if (mcuxClEls_Cipher_Aes128_Ecb_Encrypt_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("CMAC AES128:");
    if (mcuxClMacModes_Els_Cmac_Aes128_Oneshot_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("\r\n============================\r\n");
    PRINTF("RESULT: ");
    if (fail == 0)
    {
        PRINTF("All %d test PASS!!\r\n", pass);
    }
    else
    {
        PRINTF("%d / %d test PASSED, %d FAILED!!\r\n", pass, pass + fail, fail);
    }

    PRINTF("ELS example END \r\n");
    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
