/* The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "fsl_shell.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_NUMBERS 1U
#define LED_1_INIT() USER_LED_INIT(LOGIC_LED_OFF)
#define LED_1_ON() USER_LED_ON()
#define LED_1_OFF() USER_LED_OFF()
#define SHELL_Printf PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void Led_Init(void);

/* SHELL user send data callback */
void SHELL_SendDataCallback(uint8_t *buf, uint32_t len);

/* SHELL user receive data callback */
void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len);

static int32_t LedControl(p_shell_context_t context, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const shell_command_context_t xLedCommand = {"led",
                                                    "\r\n\"led arg1 arg2\":\r\n Usage:\r\n    arg1: 1|2|3|4...         "
                                                    "   Led index\r\n    arg2: on|off                Led status\r\n",
                                                    LedControl, 2};

/*******************************************************************************
 * Code
 ******************************************************************************/

void Led_Init(void)
{
    LED_1_INIT();
}

void SHELL_SendDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        PUTCHAR(*(buf++));
    }
}

void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        *(buf++) = GETCHAR();
    }
}

static int32_t LedControl(p_shell_context_t context, int32_t argc, char **argv)
{
    int32_t kLedIndex = ((int32_t)atoi(argv[1]));
    char *kLedCommand = argv[2];

    /* Check second argument to control led */
    switch (kLedIndex)
    {
#if defined(LED_NUMBERS) && LED_NUMBERS > 0
        case 1:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_1_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_1_OFF();
            }
            else
            {
                SHELL_Printf("Control conmmand is wrong!\r\n");
            }
            break;
#endif
#if defined(LED_NUMBERS) && LED_NUMBERS > 1
        case 2:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_2_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_2_OFF();
            }
            else
            {
                SHELL_Printf("Control conmmand is wrong!\r\n");
            }
            break;
#endif
#if defined(LED_NUMBERS) && LED_NUMBERS > 2
        case 3:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_3_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_3_OFF();
            }
            else
            {
                SHELL_Printf("Control conmmand is wrong!\r\n");
            }
            break;
#endif
#if defined(LED_NUMBERS) && LED_NUMBERS > 3
        case 4:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_4_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_4_OFF();
            }
            else
            {
                SHELL_Printf("Control conmmand is wrong!\r\n");
            }
            break;
#endif
        default:
            SHELL_Printf("LED index is wrong\r\n");
            break;
    }
    return 0;
}

/*! @brief Main function */
int main(void)
{
    shell_context_struct user_context;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Enable clock gate for GPIO1 */
    CLOCK_EnableClock(kCLOCK_Gpio1);    

    /* Init led */
    Led_Init();

    /* Init SHELL */
    SHELL_Init(&user_context, SHELL_SendDataCallback, SHELL_ReceiveDataCallback, SHELL_Printf, "SHELL>> ");

    /* Add new command to commands list */
    SHELL_RegisterCommand(&xLedCommand);

    SHELL_Main(&user_context);

    while (1)
    {
    }
}
