/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
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

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitPins(void)
{
    /* Initialize LPUART4 pins below */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Affects PORTC_PCR14 register */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);
    /* Affects PORTC_PCR15 register */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    CLOCK_EnableClock(kCLOCK_PortE);
    /* QSPI0A-DATA3 */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt5);
    /* QSPI0A-SCLK */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt5);
    /* QSPI0A-DATA0 */
    PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt5);
    /* QSPI0A-DATA2 */
    PORT_SetPinMux(PORTE, 3U, kPORT_MuxAlt5);
    /* QSPI0A-DATA1 */
    PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt5);
    /* QSPI0A-SS0 */
    PORT_SetPinMux(PORTE, 5U, kPORT_MuxAlt5);
    /* QSPI0B-DATA3 */
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAlt5);
    /* QSPI0B-SCLK */
    PORT_SetPinMux(PORTE, 7U, kPORT_MuxAlt5);
    /* QSPI0B-DATA0 */
    PORT_SetPinMux(PORTE, 8U, kPORT_MuxAlt5);
    /* QSPI0B-DATA2 */
    PORT_SetPinMux(PORTE, 9U, kPORT_MuxAlt5);
    /* QSPI0B-DATA1 */
    PORT_SetPinMux(PORTE, 10U, kPORT_MuxAlt5);
    /* QSPI0B-SS0 */
    PORT_SetPinMux(PORTE, 11U, kPORT_MuxAlt5);
}
