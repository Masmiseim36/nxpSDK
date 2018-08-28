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

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void copy_to_ram(void);
static void clock_change(void);

#if defined(__ICCARM__)
#pragma section = "ramfunc_section"
#pragma section = "ramfunc_section_init"
void clock_change(void) @"ramfunc_section";
#elif defined(__CC_ARM)
extern uint32_t Load$$EXEC_m_ramfunc$$Base[];   /* Base address for loading ram function*/
extern uint32_t Load$$EXEC_m_ramfunc$$Length[]; /* Size of ram function */
extern uint32_t Image$$EXEC_m_ramfunc$$Base[];
void clock_change(void) __attribute__((section("ramfunc_section")));
#elif defined(__GNUC__)
extern uint32_t ramfunc_load_address[];
extern uint32_t ramfunc_length[];
extern uint32_t ramfunc_execution_address[];
void clock_change(void) __attribute__((section("ramfunc_section")));
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    BOARD_InitPins();
    uint32_t uartClkSrcFreq;

    /* SIM_SOPT2[27:26]:
     *  00: Clock Disabled
     *  01: MCGFLLCLK, or MCGPLLCLK, or IRC48M
     *  10: OSCERCLK
     *  11: MCGIRCCLK
     */
    CLOCK_SetLpuartClock(1);

    uartClkSrcFreq = CLOCK_GetFreq(kCLOCK_PllFllSelClk);

    DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);

    PRINTF("hello world.\r\n");

    /* Change QSPI clock frequency. Any clock change or QSPI configure change shall put into RAM */
    PRINTF("\r\nBegin to change QSPI clock frequency\r\n");
    copy_to_ram();
    clock_change();

    PRINTF("\r\nQSPI clock change finished!\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}

void copy_to_ram()
{
    uint8_t *codeRelocateRomStart;
    uint32_t codeRelocateSize;
    uint8_t *codeReloocateRamStart;
#if defined(__ICCARM__)
    codeRelocateRomStart = (uint8_t *)__section_begin("ramfunc_section_init");
    codeRelocateSize = (uint32_t)__section_size("ramfunc_section_init");
    codeReloocateRamStart = (uint8_t *)__section_begin("ramfunc_section");
#elif defined(__CC_ARM)
    codeRelocateRomStart = (uint8_t *)Load$$EXEC_m_ramfunc$$Base;
    codeRelocateSize = (uint32_t)Load$$EXEC_m_ramfunc$$Length;
    codeReloocateRamStart = (uint8_t *)Image$$EXEC_m_ramfunc$$Base;
#elif defined(__GNUC__)
    codeRelocateRomStart = (uint8_t *)ramfunc_load_address;
    codeRelocateSize = (uint32_t)ramfunc_length;
    codeReloocateRamStart = (uint8_t *)ramfunc_execution_address;
#endif

    while (codeRelocateSize)
    {
        *codeReloocateRamStart++ = *codeRelocateRomStart++;
        codeRelocateSize--;
    }
}

/* This function changes the QSPI frequency */
void clock_change()
{
    uint32_t val = QuadSPI0->MCR;

    /* Disable QSPI */
    QuadSPI0->MCR |= QuadSPI_MCR_MDIS_MASK;

    /* Change clock frequency */
    val &= ~QuadSPI_MCR_SCLKCFG_MASK;
    val |= QuadSPI_MCR_SCLKCFG(1);
    QuadSPI0->MCR = val;

    /* Re-enable QSPI */
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK;
}
