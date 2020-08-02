/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_dac.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC_BASEADDR DAC0

uint8_t g_msg[] =
    "DAC demo\r\n"
    "DAC output to control the sound of speaker.\r\n";

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
    dac_config_t dacConfigStruct;
    uint32_t dacData;
    uint8_t receiveBuff[8];
    uint8_t i, count;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable output. */
    DAC_SetBufferReadPointer(DEMO_DAC_BASEADDR, 0U);

    PRINTF("%s", g_msg);

    while (1)
    {
        PRINTF("\r\nInput value(0--4095): ");
        i = 0;
        count = 0;
        while (1)
        {
            receiveBuff[i] = GETCHAR();

            if (receiveBuff[i] == 0x08) /* backspace */
            {
                PUTCHAR(0x08);
                PUTCHAR(0x20);
                PUTCHAR(0x08);
                count--;
                i--;
            }
            else if ((receiveBuff[i] >= '0') && (receiveBuff[i] <= '9'))
            {
                if (count < 4)
                {
                    PUTCHAR(receiveBuff[i]);
                    receiveBuff[i] -= '0';
                    i++;
                    count++;
                }
            }
            else if (receiveBuff[i] == 0x0D) /* Enter */
            {
                break;
            }
            else
            {
            }
        }

        dacData = 0;
        for (i = 0; i < count; i++)
        {
            dacData = dacData * 10 + receiveBuff[i];
        }

        if (dacData > 4095)
        {
            PRINTF("\r\nThe input value is invalid.");
            continue;
        }

        /* DAC output to the speaker */
        DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, dacData);
    }
}
