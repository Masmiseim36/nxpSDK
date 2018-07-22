/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 /*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic // define objects in main module
#include "osObjects.h"  // RTOS object definitions
#include "cmsis_dv.h"

#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
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

uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetPllFllSelClkFreq();
}

uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetPllFllSelClkFreq();
}

uint32_t LPUART2_GetFreq(void)
{
    return CLOCK_GetPllFllSelClkFreq();
}

uint32_t LPUART3_GetFreq(void)
{
    return CLOCK_GetPllFllSelClkFreq();
}

uint32_t LPUART4_GetFreq(void)
{
    return CLOCK_GetPllFllSelClkFreq();
}

/*
 * main: initialize and start the system
 */
int main(void)
{
    osKernelInitialize(); // initialize CMSIS-RTOS

    edma_config_t edmaConfig;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetLpuartClock(0x1U);
    DMAMUX_Init(DMAMUX0);

    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DMA0, &edmaConfig);

    // initialize peripherals here

    // create 'thread' functions that start executing,
    // example: tid_name = osThreadCreate (osThread(name), NULL);

    cmsis_dv();
    osKernelStart(); // start thread execution
}
