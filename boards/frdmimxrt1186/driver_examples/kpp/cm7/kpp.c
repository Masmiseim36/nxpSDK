/*
 * Copyright 2017, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdlib.h>
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_kpp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_KPP_KEYPRESS_LONG_NUM (0xFFFF)
   
#ifndef EXAMPLE_KPP_SYS_CLK_FREQ
#define EXAMPLE_KPP_SYS_CLK_FREQ CLOCK_GetFreq(kCLOCK_CpuClk)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
kpp_config_t kppConfig;
uint8_t read_keys_new[KPP_KEYPAD_COLUMNNUM_MAX] = {0};
uint8_t read_keys_old[KPP_KEYPAD_COLUMNNUM_MAX] = {0};
extern char *keyMap[EXAMPLE_KPP_MATRX_NUM][EXAMPLE_KPP_MATRX_NUM];
volatile bool g_keypress   = false;
volatile bool g_keyrelease = false;
volatile uint8_t g_keyPressedNum = 0U;
status_t stable = kStatus_Success;
/*******************************************************************************
 * Code
 ******************************************************************************/
void KPP_IRQHandler(void)
{
    uint16_t status = KPP_GetStatusFlag(EXAMPLE_KPP);
    
    if (status & kKPP_keyDepressInterrupt)
    {
        /* Disable interrupts. */
        KPP_DisableInterrupts(EXAMPLE_KPP, kKPP_keyDepressInterrupt | kKPP_keyReleaseInterrupt);
        /* Clear status. */
        KPP_ClearStatusFlag(EXAMPLE_KPP, kKPP_keyDepressInterrupt | kKPP_keyReleaseInterrupt);
        KPP_SetSynchronizeChain(EXAMPLE_KPP, kKPP_ClearKeyDepressSyncChain);

        g_keyPressedNum = 0;

        g_keypress = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

int main(void)
{
    kpp_config_t kppConfig;
    uint32_t index = 0;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\n KPP Driver Example Start.\r\n");

    kppConfig.activeColumn = kppConfig.activeRow = EXAMPLE_KPP_ACTIVE_COLUMROWS;
    kppConfig.interrupt                          = (kKPP_keyDepressInterrupt);

    KPP_Init(EXAMPLE_KPP, &kppConfig);

    stable = KPP_keyPressScanning(EXAMPLE_KPP, &read_keys_old[0], EXAMPLE_KPP_SYS_CLK_FREQ);

    while (1)
    {
        if (g_keypress)
        {
            index++;
            stable = KPP_keyPressScanning(EXAMPLE_KPP, &read_keys_new[0], EXAMPLE_KPP_SYS_CLK_FREQ);

            if (stable == kStatus_Success)
            {
                /* Scanning and print the triggered key log out.*/
                for (uint8_t col = 0; col < KPP_KEYPAD_COLUMNNUM_MAX; col++)
                {
                    if (read_keys_old[col] != read_keys_new[col])
                    {
                        index = 0;
                        for (uint8_t row = 0; row < KPP_KEYPAD_COLUMNNUM_MAX; row++)
                        {
                            if (((read_keys_old[col] ^ read_keys_new[col]) & (1 << row)) != 0)
                            {
                                uint8_t rowMap = row - EXAMPLE_KPP_COLROW_START_INDX;
                                uint8_t colMap = col - EXAMPLE_KPP_COLROW_START_INDX;

                                if ((read_keys_new[col] & (1 << row)) != 0)
                                {
                                    PRINTF("\r\n Key %s was pressed.\r\n", keyMap[rowMap][colMap]);
                                    KPP_SetSynchronizeChain(EXAMPLE_KPP, kKPP_ClearKeyDepressSyncChain);
                                    g_keyPressedNum++;
                                }
                                else
                                {
                                    PRINTF("\r\n Key %s was released.\r\n", keyMap[rowMap][colMap]);
                                    KPP_SetSynchronizeChain(EXAMPLE_KPP, kKPP_SetKeyReleasesSyncChain);
                                    g_keyPressedNum--;
                                }
                            }
                        }

                        read_keys_old[col] = read_keys_new[col];

                        if (g_keyPressedNum == 0U)
                        {
                            PRINTF("\r\n All keys released.\r\n");
                        }
                    }

                    if (g_keyPressedNum == 0U)
                    {
                            g_keypress = false;
                            KPP_ClearStatusFlag(EXAMPLE_KPP, kKPP_keyDepressInterrupt | kKPP_keyReleaseInterrupt);
                            KPP_EnableInterrupts(EXAMPLE_KPP, kKPP_keyDepressInterrupt);
                    }
                }
            }

            if (index > EXAMPLE_KPP_KEYPRESS_LONG_NUM)
            {
                index = 0;
                PRINTF("\r\n This was a long press.\r\n");
            }
        }
    }
}
