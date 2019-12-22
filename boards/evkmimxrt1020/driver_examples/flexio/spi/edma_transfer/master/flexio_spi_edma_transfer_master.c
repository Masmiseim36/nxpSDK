/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexio_spi_edma.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_dmamux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*Master related*/
#define BOARD_FLEXIO_BASE FLEXIO1
#define FLEXIO_SPI_MOSI_PIN 1U
#define FLEXIO_SPI_MISO_PIN 0U
#define FLEXIO_SPI_SCK_PIN 3U
#define FLEXIO_SPI_CSn_PIN 2U

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define FLEXIO_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define FLEXIO_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER (7U)
#define FLEXIO_CLOCK_FREQUENCY (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (FLEXIO_CLOCK_PRE_DIVIDER + 1U))

#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_FLEXIO_SPI_DMA_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_CHANNEL (0U)
#define FLEXIO_SPI_RX_DMA_CHANNEL (1U)
#define FLEXIO_TX_SHIFTER_INDEX 0U
#define FLEXIO_RX_SHIFTER_INDEX 2U
#define EXAMPLE_TX_DMA_SOURCE kDmaRequestMuxFlexIO1Request0Request1
#define EXAMPLE_RX_DMA_SOURCE kDmaRequestMuxFlexIO1Request2Request3
#define BUFFER_SIZE (64)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static flexio_spi_master_edma_handle_t g_spiHandle;
static edma_handle_t txHandle;
static edma_handle_t rxHandle;
FLEXIO_SPI_Type spiDev;
AT_NONCACHEABLE_SECTION_INIT(static uint8_t sendBuff[BUFFER_SIZE]) = {0U};
AT_NONCACHEABLE_SECTION_INIT(static uint8_t recvBuff[BUFFER_SIZE]) = {0U};
volatile bool completeFlag                                         = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void spi_master_completionCallback(FLEXIO_SPI_Type *base,
                                          flexio_spi_master_edma_handle_t *handle,
                                          status_t status,
                                          void *userData)
{
    if (status == kStatus_Success)
    {
        completeFlag = true;
    }
}

int main(void)
{
    uint8_t i                  = 0;
    uint8_t err                = 0;
    flexio_spi_transfer_t xfer = {0};
    flexio_spi_master_config_t userConfig;
    dma_request_source_t dma_request_source_tx;
    dma_request_source_t dma_request_source_rx;
    edma_config_t config;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Clock setting for Flexio */
    CLOCK_SetMux(kCLOCK_Flexio1Mux, FLEXIO_CLOCK_SELECT);
    CLOCK_SetDiv(kCLOCK_Flexio1PreDiv, FLEXIO_CLOCK_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Flexio1Div, FLEXIO_CLOCK_DIVIDER);
    PRINTF("\r\nFlexIO SPI edma example\r\n");
    PRINTF("Master Start...\r\n");

    /* Init FlexIO SPI. */
    /*
     * userConfig.enableMaster = true;
     * userConfig.enableInDoze = false;
     * userConfig.enableInDebug = true;
     * userConfig.enableFastAccess = false;
     * userConfig.baudRate_Bps = 500000U;
     * userConfig.phase = kFLEXIO_SPI_ClockPhaseFirstEdge;
     * userConfig.dataMode = kFLEXIO_SPI_8BitMode;
     */
    FLEXIO_SPI_MasterGetDefaultConfig(&userConfig);
    userConfig.baudRate_Bps = 4000000U;

    spiDev.flexioBase      = BOARD_FLEXIO_BASE;
    spiDev.SDOPinIndex     = FLEXIO_SPI_MOSI_PIN;
    spiDev.SDIPinIndex     = FLEXIO_SPI_MISO_PIN;
    spiDev.SCKPinIndex     = FLEXIO_SPI_SCK_PIN;
    spiDev.CSnPinIndex     = FLEXIO_SPI_CSn_PIN;
    spiDev.shifterIndex[0] = FLEXIO_TX_SHIFTER_INDEX;
    spiDev.shifterIndex[1] = FLEXIO_RX_SHIFTER_INDEX;
    spiDev.timerIndex[0]   = 0U;
    spiDev.timerIndex[1]   = 1U;

    /*Init EDMA for example.*/
    DMAMUX_Init(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR);
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(EXAMPLE_FLEXIO_SPI_DMA_BASEADDR, &config);

    dma_request_source_tx = (dma_request_source_t)EXAMPLE_TX_DMA_SOURCE;
    dma_request_source_rx = (dma_request_source_t)EXAMPLE_RX_DMA_SOURCE;

    /* Request DMA channels for TX & RX. */
    DMAMUX_SetSource(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_TX_DMA_CHANNEL, dma_request_source_tx);
    DMAMUX_SetSource(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_RX_DMA_CHANNEL, dma_request_source_rx);
    DMAMUX_EnableChannel(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_TX_DMA_CHANNEL);
    DMAMUX_EnableChannel(EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR, FLEXIO_SPI_RX_DMA_CHANNEL);
    EDMA_CreateHandle(&txHandle, EXAMPLE_FLEXIO_SPI_DMA_BASEADDR, FLEXIO_SPI_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&rxHandle, EXAMPLE_FLEXIO_SPI_DMA_BASEADDR, FLEXIO_SPI_RX_DMA_CHANNEL);

    FLEXIO_SPI_MasterInit(&spiDev, &userConfig, FLEXIO_CLOCK_FREQUENCY);

    /* Init Buffer. */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        sendBuff[i] = i;
    }

    /* Send to slave. */
    xfer.txData   = sendBuff;
    xfer.rxData   = recvBuff;
    xfer.dataSize = BUFFER_SIZE;
    xfer.flags    = kFLEXIO_SPI_8bitMsb;
    FLEXIO_SPI_MasterTransferCreateHandleEDMA(&spiDev, &g_spiHandle, spi_master_completionCallback, NULL, &txHandle,
                                              &rxHandle);
    FLEXIO_SPI_MasterTransferEDMA(&spiDev, &g_spiHandle, &xfer);
    while (!completeFlag)
    {
    }
    completeFlag = false;

    /* Check if the data is right. */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (sendBuff[i] != recvBuff[i])
        {
            PRINTF("The %d is wrong! data is %d\r\n", i, recvBuff[i]);
            err++;
        }
    }
    if (err == 0)
    {
        PRINTF("\r\nMaster runs successfully!\r\n");
    }

    while (1)
    {
    }
}
