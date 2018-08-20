/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>
#include "fsl_debug_console.h"
#include "fsl_kpp.h"
/*
* This case tests the necessary API to enable the ENET transmission. It would call the functions sequentially in polling
* mode.
*
*/

#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_KPP_ACTIVE_COLUMROWS  (0x3D)
#define EXAMPLE_KPP_MATRX_NUM (4)
#define EXAMPLE_KPP_COLROW_START_INDX (2)
#define EXAMPLE_KPP KPP
#define EXAMPLE_KPP_KEYPRESS_LONG_NUM (0x3FF)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
char* keyMap[EXAMPLE_KPP_MATRX_NUM][EXAMPLE_KPP_MATRX_NUM] = {
  /* COL0    COL1    COL2    COL3   */
  { "SW1", "SW2", "SW3", "SW4"}, /* ROW0 */
  { "SW5", "SW6", "SW7", "SW8"}, /* ROW1 */
  { "SW9", "SW10", "SW11", "SW12" }, /* ROW2 */
  { "SW13", "SW14", "SW15", "SW16"}, /* ROW3 */
};
kpp_config_t kppConfig;
uint8_t read_keys[KPP_KEYPAD_COLUMNNUM_MAX] = {0};
extern char *keyMap[EXAMPLE_KPP_MATRX_NUM][EXAMPLE_KPP_MATRX_NUM];
volatile bool g_keypress = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void KPP_IRQHandler(void)
{
    uint16_t status = KPP_GetStatusFlag(EXAMPLE_KPP);
    if (status & kKPP_keyDepressInterrupt)
    {
        /* Disable interrupts. */
        KPP_DisableInterrupts(EXAMPLE_KPP, (kKPP_keyDepressInterrupt | kKPP_keyReleaseInterrupt));
        /* Scanning. */
        KPP_keyPressScanning(EXAMPLE_KPP, &read_keys[0], CLOCK_GetFreq(kCLOCK_CpuClk));
        /* Clear status. */
        KPP_ClearStatusFlag(EXAMPLE_KPP, (kKPP_keyDepressInterrupt | kKPP_keyReleaseInterrupt));
        KPP_SetSynchronizeChain(EXAMPLE_KPP, (kKPP_ClearKeyDepressSyncChain | kKPP_SetKeyReleasesSyncChain));
        /* Enable interrupts. */
        KPP_EnableInterrupts(EXAMPLE_KPP, kKPP_keyDepressInterrupt);
       g_keypress = true;
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

int main(void)
{
    kpp_config_t kppConfig;
    uint8_t rowOld = 0xFF;
    uint8_t colOld = 0xFF;
    uint32_t index = 0;
    bool g_printOnce = false;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n KPP Driver Example Start.\r\n");

    kppConfig.activeColumn = kppConfig.activeRow = EXAMPLE_KPP_ACTIVE_COLUMROWS;
    kppConfig.interrupt = kKPP_keyDepressInterrupt;

    KPP_Init(EXAMPLE_KPP, &kppConfig);

    while (1)
    {
        if (g_keypress)
        {
              /* Scanning and print the triggered key log out.*/
            for (uint32_t col = 0; col < EXAMPLE_KPP_MATRX_NUM; col++)
            {
                if ((read_keys[col + EXAMPLE_KPP_COLROW_START_INDX] & EXAMPLE_KPP_ACTIVE_COLUMROWS) != 0x00)
                {
                    for (uint8_t row = 0; row < EXAMPLE_KPP_MATRX_NUM; row++)
                    {
                        if ((read_keys[col + EXAMPLE_KPP_COLROW_START_INDX] &
                               (1 << (row + EXAMPLE_KPP_COLROW_START_INDX))) != 0)
                        {
                            if ((rowOld != row) || (colOld != col))
                            {
                                rowOld = row;
                                colOld = col;
                                index = 1;
                                g_printOnce = false;
                                PRINTF("\r\n Key %s was pressed.\r\n", keyMap[rowOld][colOld]);
                            }
                            else
                            {
                                index ++;
                                if ((index > EXAMPLE_KPP_KEYPRESS_LONG_NUM) && !g_printOnce)
                                {
                                    PRINTF("\r\n This was a long press.\r\n");
                                    g_printOnce = true;
                                }
                            }
                        }
                    }
                }
            }
            g_keypress = false;             
        }
    }
}
