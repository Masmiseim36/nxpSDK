/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_dac.h"
#include "fsl_dma.h"
#include "fsl_pit.h"
#include "fsl_dmamux.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC_BASEADDR DAC0
#define DAC_DATA_REG_ADDR 0x4003F000U
#define DEMO_DMAMUX_BASEADDR DMAMUX0
#define DEMO_DMA_CHANNEL 0U
#define DEMO_DMA_DAC_SOURCE 45U
#define DEMO_DMA_BASEADDR DMA0
#define DEMO_DMA_IRQ_ID DMA0_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define DEMO_DAC_USED_BUFFER_SIZE 32U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
* @brief Initialize the DMA.
*/
static void DMA_Configuration(void);
/*!
* @brief Initialize the DMAMUX.
*/
static void DMAMUX_Configuration(void);
/*!
 * @brief Initialize the PIT.
 */
static void PIT_Configuration(void);
/*!
* @brief Initialize the DAC.
*/
static void DAC_Configuration(void);
/*!
 * @brief Callback function for DMA.
 */
static void DMA_Callback(struct _dma_handle *handle, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
dma_handle_t g_DMA_Handle; /* DMA handler. */
dma_transfer_config_t g_transferConfig;
volatile uint32_t g_index = 0U; /* Index of the g_dacDataArray array. */
uint16_t g_dacDataArray[DEMO_DAC_USED_BUFFER_SIZE] = {
    0U,    401U,  799U,  1188U, 1567U, 1930U, 2275U, 2598U, 2895U, 3165U, 3405U, 3611U, 3783U, 3918U, 4016U, 4075U,
    4095U, 4075U, 4016U, 3918U, 3783U, 3611U, 3405U, 3165U, 2895U, 2598U, 2275U, 1930U, 1567U, 1188U, 799U,  401U};
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
* @brief Main function
*/
int main(void)
{
    /* Initialize hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("DAC CONTINUOUS PIT DMA DEMO\r\n");

    /* Initialize DMAMUX. */
    DMAMUX_Configuration();
    /* Initialize DMA. */
    DMA_Configuration();
    /* Initialize the HW trigger source. */
    PIT_Configuration();
    /* Initialize DAC. */
    DAC_Configuration();

    /* Generate continuous trigger signal to DAC. */
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    PRINTF("Please probe the DAC output with a oscilloscope.\r\n");
    while (1)
    {
    }
}

static void DMA_Configuration(void)
{
    DMA_Init(DEMO_DMA_BASEADDR);
    DMA_CreateHandle(&g_DMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    DMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);
    DMA_PrepareTransfer(&g_transferConfig, (void *)(g_dacDataArray + g_index), sizeof(uint16_t),
                        (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), DAC_DATH_COUNT * sizeof(uint16_t),
                        kDMA_MemoryToMemory);
    DMA_SubmitTransfer(&g_DMA_Handle, &g_transferConfig, kDMA_EnableInterrupt);
    /* Enable transfer. */
    DMA_StartTransfer(&g_DMA_Handle);
}

static void DMAMUX_Configuration(void)
{
    /* Configure DMAMUX. */
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_SetSource(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_DAC_SOURCE); /* Map ADC source to channel 0 */
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
}

/* Enable the trigger source of PIT. */
static void PIT_Configuration(void)
{
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    /* Init pit module */
    PIT_Init(PIT, &pitConfig);
    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(10000U, PIT_SOURCE_CLOCK));
}

static void DAC_Configuration(void)
{
    dac_config_t dacConfigStruct;
    dac_buffer_config_t dacBufferConfigStruct;

    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable output. */

    /* Configure the DAC buffer. */
    DAC_EnableBuffer(DEMO_DAC_BASEADDR, true);
    DAC_GetDefaultBufferConfig(&dacBufferConfigStruct);
    dacBufferConfigStruct.triggerMode = kDAC_BufferTriggerByHardwareMode;
    DAC_SetBufferConfig(DEMO_DAC_BASEADDR, &dacBufferConfigStruct);
    DAC_SetBufferReadPointer(DEMO_DAC_BASEADDR, 0U); /* Make sure the read pointer to the start. */

    /* Enable DMA. */
    DAC_EnableBufferInterrupts(DEMO_DAC_BASEADDR,
                               kDAC_BufferReadPointerBottomInterruptEnable | kDAC_BufferReadPointerTopInterruptEnable);
    DAC_EnableBufferDMA(DEMO_DAC_BASEADDR, true);
}

static void DMA_Callback(struct _dma_handle *handle, void *userData)
{
    /* Clear DMA interrupt flag. */
    DMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kDMA_TransactionsDoneFlag);
    /* Setup transfer. */
    g_index += DAC_DATH_COUNT;
    if (g_index == DEMO_DAC_USED_BUFFER_SIZE)
    {
        g_index = 0U;
    }
    DMA_PrepareTransfer(&g_transferConfig, (void *)(g_dacDataArray + g_index), sizeof(uint16_t),
                        (void *)DAC_DATA_REG_ADDR, sizeof(uint16_t), DAC_DATH_COUNT * sizeof(uint16_t),
                        kDMA_MemoryToMemory);
    DMA_SubmitTransfer(&g_DMA_Handle, &g_transferConfig, kDMA_EnableInterrupt);
    /* Enable transfer. */
    DMA_StartTransfer(&g_DMA_Handle);
}
