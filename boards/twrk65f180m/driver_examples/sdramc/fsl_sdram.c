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

#include "fsl_sdram.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Defines the sdram nop delay times in initialize sequence for about 1us. */
#define SDRAM_DELAYTIMES_ONEMICROSECOND (125U)

/*******************************************************************************
* Prototypes
******************************************************************************/

static status_t SDRAM_InitSequence(SDRAM_Type *base,
                               sdramc_block_selection_t whichBlock,
                               sdram_burst_len_t burstLen,
                               sdram_burst_type_t burstType,
                               sdram_cas_latency_t casLatency,
                               sdram_operation_mode_t opMode,
                               sdram_write_burst_mode_t writeBurstMode);

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t SDRAM_Init(SDRAM_Type *base, uint32_t busClock_Hz)
{
    sdramc_config_t config;
    sdramc_refresh_config_t refConfig;
    sdramc_blockctl_config_t ctlConfig;

    /* SDRAM refresh timing configuration. */
    refConfig.refreshTime = kSDRAMC_RefreshThreeClocks;
    /* Refresh time 4096 rows/ 64ms. */
    refConfig.sdramRefreshRow = 15625;
    refConfig.busClock_Hz = busClock_Hz;
    /* SDRAM controller configuration. */
    /*　Port size: 32 bit, command bit 20. */
    ctlConfig.portSize = kSDRAMC_PortSize32Bit;
    ctlConfig.location = kSDRAMC_Commandbit20;
    ctlConfig.block = kSDRAMC_Block0;
    /* SDRAM with trcd-15ns(min), trp-15ns(min), tras-37ns (min). */
    ctlConfig.latency = kSDRAMC_LatencyOne;
    ctlConfig.address = SDRAM_START_ADDRESS;
    ctlConfig.addressMask = 0x7c0000;

    config.refreshConfig = &refConfig;
    config.blockConfig = &ctlConfig;
    config.numBlockConfig = 1;

    /* Initializes the SDRAM controller. */
    SDRAMC_Init(base, &config);

    /* The external SDRAM initialization sequence. */
    return SDRAM_InitSequence(base, kSDRAMC_Block0, kSDRAM_MrsBurstLenOne, kSDRAM_MrsSequential, kSDRAM_MrsLatencyTwo,
                       kSDRAM_MrsStandOperation, kSDRAM_MrsWriteBurst);
}

/*!
 * @brief The SDRAM Initializes sequence.
 *
 */
static status_t SDRAM_InitSequence(SDRAM_Type *base,
                               sdramc_block_selection_t whichBlock,
                               sdram_burst_len_t burstLen,
                               sdram_burst_type_t burstType,
                               sdram_cas_latency_t casLatency,
                               sdram_operation_mode_t opMode,
                               sdram_write_burst_mode_t writeBurstMode)
{
    uint32_t count = SDRAM_DELAYTIMES_ONEMICROSECOND;
    uint8_t *mrsAddr = 0;
    uint32_t addr = 0;

    /* Issue a PALL command. */
    SDRAMC_SendCommand(base, whichBlock, kSDRAMC_PrechargeCommand);


    /* Accessing a SDRAM location. */
    *(uint8_t *)(SDRAM_START_ADDRESS) = SDRAM_COMMAND_ACCESSVALUE;

    /* Enable the refresh. */
    SDRAMC_SendCommand(base, whichBlock, kSDRAMC_AutoRefreshEnableCommand);

    /* Wait for 8 refresh cycles less than one microsecond. */
    while (count != 0)
    {
        __NOP();
        count--;
    }

    /* Issue MSR command. */
    SDRAMC_SendCommand(base, whichBlock, kSDRAMC_ImrsCommand);

    /* Put the right value on SDRAM address bus for SDRAM mode register,
    *  The address of SDRAM Pins is as below:
    *  A2 ~ A0:   burst length   0
    *     000->1
    *     001->2
    *     010->4
    *     011->8
    *     res
    * A3:   burst type
    *        0 -> seq
    *        1 -> Interleave
    *
    * A6 ~ A4:  CAS latency  (should be set to equal to the tcasl in "sdram_latency_t")
    *       000-> res
    *       001-> 1
    *       010-> 2
    *       011-> 3
    *       res
    * A8 ~ A7:  Operationg Mode
    *       00->Stardard Operation
    *       res
    * A9:    Write Burst Mode
    *       0-> Programmed Burst Length
    *      1-> Single Location Access
    */
    /* A2-A0. */
    if (burstLen & 0x1)
    {
        addr |= 1 << SDRAM_A0;
    }
    if (burstLen & 0x2)
    {
        addr |= 1 << SDRAM_A1;
    }
    if (burstLen & 0x4)
    {
        addr |= 1 << SDRAM_A2;
    }

    /* Sdram address A3. */
    if (burstType & 0x1)
    {
        addr |= 1 << SDRAM_A3;
    }

    /* Sdram address A6-A4. */
    if (casLatency & 0x1)
    {
        addr |= 1 << SDRAM_A4;
    }
    if (casLatency & 0x2)
    {
        addr |= 1 << SDRAM_A5;
    }
    if (casLatency & 0x4)
    {
        addr |= 1 << SDRAM_A6;
    }

    /* Sdram address A8-A7. */
    if (opMode & 0x1)
    {
        addr |= 1 << SDRAM_A7;
    }
    if (opMode & 0x2)
    {
        addr |= 1 << SDRAM_A8;
    }

    /* Sdram address A9. */
    if (writeBurstMode & 0x1)
    {
        addr |= 1 << SDRAM_A9;
    }
    /* Set MRS register. */
    mrsAddr = (uint8_t *)(SDRAM_START_ADDRESS + addr);
    /* Access sdram location. */
    *mrsAddr = SDRAM_COMMAND_ACCESSVALUE;

    return kStatus_Success;
}
