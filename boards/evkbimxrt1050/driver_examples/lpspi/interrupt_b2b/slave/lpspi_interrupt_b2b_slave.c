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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "board.h"

#include "fsl_common.h"
#include "pin_mux.h"
#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR (LPSPI3)
#define EXAMPLE_LPSPI_SLAVE_IRQN (LPSPI3_IRQn)
#define EXAMPLE_LPSPI_SLAVE_IRQHandler LPSPI3_IRQHandler

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI_MASTER_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER + 1U))

#define TRANSFER_SIZE 64U /*! Transfer dataSize */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user callback */
void LPSPI_SlaveUserCallback(LPSPI_Type *base, lpspi_slave_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};
uint8_t slaveTxData[TRANSFER_SIZE] = {0U};

volatile uint32_t slaveTxCount;
volatile uint32_t slaveRxCount;
uint8_t g_slaveRxWatermark;
uint8_t g_slaveFifoSize;

volatile bool isSlaveTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_LPSPI_SLAVE_IRQHandler(void)
{
    if (slaveRxCount < TRANSFER_SIZE)
    {
        while (LPSPI_GetRxFifoCount(EXAMPLE_LPSPI_SLAVE_BASEADDR))
        {
            slaveRxData[slaveRxCount] = LPSPI_ReadData(EXAMPLE_LPSPI_SLAVE_BASEADDR);
            slaveRxCount++;

            if (slaveTxCount < TRANSFER_SIZE)
            {
                LPSPI_WriteData(EXAMPLE_LPSPI_SLAVE_BASEADDR, slaveTxData[slaveTxCount]);
                slaveTxCount++;
            }
            if (slaveRxCount == TRANSFER_SIZE)
            {
                break;
            }
        }
    }

    /*Update rxWatermark. There isn't RX interrupt for the last datas if the RX count is not greater than rxWatermark.*/
    if ((TRANSFER_SIZE - slaveRxCount) <= g_slaveRxWatermark)
    {
        EXAMPLE_LPSPI_SLAVE_BASEADDR->FCR =
            (EXAMPLE_LPSPI_SLAVE_BASEADDR->FCR & (~LPSPI_FCR_RXWATER_MASK)) |
            LPSPI_FCR_RXWATER(((TRANSFER_SIZE - slaveRxCount) > 1) ? ((TRANSFER_SIZE - slaveRxCount) - 1U) : (0U));
    }

    /* Check if remaining receive byte count matches user request */
    if ((slaveRxCount == TRANSFER_SIZE) && (slaveTxCount == TRANSFER_SIZE))
    {
        isSlaveTransferCompleted = true;
        /* Disable interrupt requests */
        LPSPI_DisableInterrupts(EXAMPLE_LPSPI_SLAVE_BASEADDR, kLPSPI_RxInterruptEnable);
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER);

    PRINTF("LPSPI board to board functional interrupt example.\r\n");
    PRINTF("  Slave start to receive data...\r\n");

    uint32_t errorCount;
    uint32_t i;
    lpspi_slave_config_t slaveConfig;
    uint32_t whichPcs;
    uint8_t txWatermark;

    /*Slave config*/
    slaveConfig.bitsPerFrame = 8;
    slaveConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    slaveConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    slaveConfig.direction = kLPSPI_MsbFirst;

    slaveConfig.whichPcs = EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT;
    slaveConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    slaveConfig.pinCfg = kLPSPI_SdiInSdoOut;
    slaveConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_SlaveInit(EXAMPLE_LPSPI_SLAVE_BASEADDR, &slaveConfig);

    /*Set up the transfer data*/
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        slaveTxData[i] = i % 256;
        slaveRxData[i] = 0;
    }
    /******************Set up slave first ******************/
    isSlaveTransferCompleted = false;
    slaveTxCount = 0;
    slaveRxCount = 0;
    whichPcs = EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT;

    /*The TX and RX FIFO sizes are always the same*/
    g_slaveFifoSize = LPSPI_GetRxFifoSize(EXAMPLE_LPSPI_SLAVE_BASEADDR);

    /*Set the RX and TX watermarks to reduce the ISR times.*/
    if (g_slaveFifoSize > 1)
    {
        txWatermark = 1;
        g_slaveRxWatermark = g_slaveFifoSize - 2;
    }
    else
    {
        txWatermark = 0;
        g_slaveRxWatermark = 0;
    }

    LPSPI_SetFifoWatermarks(EXAMPLE_LPSPI_SLAVE_BASEADDR, txWatermark, g_slaveRxWatermark);

    LPSPI_Enable(EXAMPLE_LPSPI_SLAVE_BASEADDR, false);
    EXAMPLE_LPSPI_SLAVE_BASEADDR->CFGR1 &= (~LPSPI_CFGR1_NOSTALL_MASK);
    LPSPI_Enable(EXAMPLE_LPSPI_SLAVE_BASEADDR, true);

    /*Flush FIFO , clear status , disable all the interrupts.*/
    LPSPI_FlushFifo(EXAMPLE_LPSPI_SLAVE_BASEADDR, true, true);
    LPSPI_ClearStatusFlags(EXAMPLE_LPSPI_SLAVE_BASEADDR, kLPSPI_AllStatusFlag);
    LPSPI_DisableInterrupts(EXAMPLE_LPSPI_SLAVE_BASEADDR, kLPSPI_AllInterruptEnable);

    EXAMPLE_LPSPI_SLAVE_BASEADDR->TCR =
        (EXAMPLE_LPSPI_SLAVE_BASEADDR->TCR &
         ~(LPSPI_TCR_CONT_MASK | LPSPI_TCR_CONTC_MASK | LPSPI_TCR_RXMSK_MASK | LPSPI_TCR_PCS_MASK)) |
        LPSPI_TCR_CONT(0) | LPSPI_TCR_CONTC(0) | LPSPI_TCR_RXMSK(0) | LPSPI_TCR_TXMSK(0) | LPSPI_TCR_PCS(whichPcs);

    /* Enable the NVIC for LPSPI peripheral. Note that below code is useless if the LPSPI interrupt is in INTMUX ,
    * and you should also enable the INTMUX interrupt in your application.
    */
    EnableIRQ(EXAMPLE_LPSPI_SLAVE_IRQN);

    /*TCR is also shared the FIFO , so wait for TCR written.*/
    while (LPSPI_GetTxFifoCount(EXAMPLE_LPSPI_SLAVE_BASEADDR) != 0)
    {
    }
    /*Fill up the TX data in FIFO */
    while (LPSPI_GetTxFifoCount(EXAMPLE_LPSPI_SLAVE_BASEADDR) < g_slaveFifoSize)
    {
        /*Write the word to TX register*/
        LPSPI_WriteData(EXAMPLE_LPSPI_SLAVE_BASEADDR, slaveTxData[slaveTxCount]);
        ++slaveTxCount;

        if (slaveTxCount == TRANSFER_SIZE)
        {
            break;
        }
    }

    LPSPI_EnableInterrupts(EXAMPLE_LPSPI_SLAVE_BASEADDR, kLPSPI_RxInterruptEnable);

    /******************Wait for master and slave transfer completed.******************/
    while (!isSlaveTransferCompleted)
    {
    }

    errorCount = 0;
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        if (slaveTxData[i] != slaveRxData[i])
        {
            errorCount++;
        }
    }
    if (errorCount == 0)
    {
        PRINTF("\r\nLPSPI transfer all data matched! \r\n");
        /* Print out receive buffer */
        PRINTF("\r\n Slave received:\r\n");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", slaveRxData[i]);
        }
        PRINTF("\r\n");
    }
    else
    {
        PRINTF("\r\nError occurred in LPSPI transfer ! \r\n");
    }

    LPSPI_Deinit(EXAMPLE_LPSPI_SLAVE_BASEADDR);

    PRINTF("End of slave example! \r\n");

    while (1)
    {
    }
}
