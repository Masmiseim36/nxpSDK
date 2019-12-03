/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "fsl_flexcan_edma.h"
#include "board.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_irqsteer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN ADMA__CAN0
#define EXAMPLE_CAN_DMA ADMA__EDMA3
#define EXAMPLE_CAN_DMA_CHANNEL (12)
#define EXAMPLE_CAN_CLKSRC kCLOCK_DMA_Can0
/*
 * When CLK_SRC=1, the protocol engine works at fixed frequency of 160M.
 * If other frequency wanted, please use CLK_SRC=0 and set the working frequency for SC_R_CAN_0.
 */
#define EXAMPLE_CAN_CLK_FREQ (SC_160MHZ)
#define EXAMPLE_CAN_DMAMUX DMAMUX0
#define TX_MESSAGE_BUFFER_NUM (9)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_edma_handle_t flexcanEdmaHandle;
edma_handle_t flexcanRxFifoEdmaHandle;
flexcan_mb_transfer_t txXfer;
flexcan_fifo_transfer_t rxFifoXfer;
flexcan_frame_t txFrame;
AT_NONCACHEABLE_SECTION(flexcan_frame_t rxFrame);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    /* Process FlexCAN Tx event. */
    if ((kStatus_FLEXCAN_TxIdle == status) && (TX_MESSAGE_BUFFER_NUM == result))
    {
        txComplete = true;
    }
}

/*!
 * @brief FlexCAN DMA Call Back function
 */
static void flexcan_dma_callback(CAN_Type *base, flexcan_edma_handle_t *handle, status_t status, void *userData)
{
    /* Process FlexCAN Rx event. */
    if (kStatus_FLEXCAN_RxFifoIdle == status)
    {
        rxComplete = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t edmaConfig;
    flexcan_config_t flexcanConfig;
    flexcan_rx_fifo_config_t rxFifoConfig;
    uint32_t rxFifoFilter[] = {
        FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x321, 0, 0), FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x321, 1, 0),
        FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x123, 0, 0), FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x123, 1, 0)};
    uint8_t i;

    /* Initialize board hardware. */
    sc_ipc_t ipc;

    ipc = BOARD_InitRpc();
    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Power on Peripherals. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_CAN_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on FLEXCAN\r\n");
    }

    /* Power on EDMA . */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_DMA_3_CH12, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on EMDA3 Channel\r\n");
    }

    /* Power on IRQSteer . */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTR\r\n");
    }

    /* Enable interrupt in irqsteer */
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, ADMA_FLEXCAN0_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, ADMA_FLEXCAN0_DMA_INT_IRQn);

    PRINTF("\r\n==FlexCAN loopback edma example -- Start.==\r\n\r\n");

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 1000000U;
     * flexcanConfig.maxMbNum = 16;
     * flexcanConfig.enableLoopBack = false;
     * flexcanConfig.enableSelfWakeup = false;
     * flexcanConfig.enableIndividMask = false;
     * flexcanConfig.enableDoze = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
#if (!defined(FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE)) || !FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE
    flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
#else
#if defined(CAN_CTRL1_CLKSRC_MASK)
    if (!FSL_FEATURE_FLEXCAN_INSTANCE_SUPPORT_ENGINE_CLK_SEL_REMOVEn(EXAMPLE_CAN))
    {
        flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
    }
#endif
#endif /* FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE */
    flexcanConfig.enableLoopBack = true;

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    if (FLEXCAN_CalculateImprovedTimingValues(flexcanConfig.baudRate, EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        PRINTF("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    /* Configure DMA. */
    DMAMUX_Init(EXAMPLE_CAN_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_CAN_DMAMUX, EXAMPLE_CAN_DMA_CHANNEL, EXAMPLE_CAN_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_CAN_DMAMUX, EXAMPLE_CAN_DMA_CHANNEL);
#endif

    /*
     * edmaConfig.enableRoundRobinArbitration = false;
     * edmaConfig.enableHaltOnError = true;
     * edmaConfig.enableContinuousLinkMode = false;
     * edmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_CAN_DMA, &edmaConfig);

    /* Create EDMA handle. */
    EDMA_CreateHandle(&flexcanRxFifoEdmaHandle, EXAMPLE_CAN_DMA, EXAMPLE_CAN_DMA_CHANNEL);

    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    /* Setup Rx FIFO. */
    rxFifoConfig.idFilterTable = rxFifoFilter;
    rxFifoConfig.idFilterType  = kFLEXCAN_RxFifoFilterTypeA;
    rxFifoConfig.idFilterNum   = sizeof(rxFifoFilter) / sizeof(rxFifoFilter[0]);
    rxFifoConfig.priority      = kFLEXCAN_RxFifoPrioHigh;
    FLEXCAN_SetRxFifoConfig(EXAMPLE_CAN, &rxFifoConfig, true);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Create FlexCAN EDMA handle structure and set call back function. */
    FLEXCAN_TransferCreateHandleEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, flexcan_dma_callback, NULL,
                                     &flexcanRxFifoEdmaHandle);

    /* Send first message through Tx Message Buffer. */
    txFrame.format    = kFLEXCAN_FrameFormatStandard;
    txFrame.type      = kFLEXCAN_FrameTypeData;
    txFrame.id        = FLEXCAN_ID_STD(0x123);
    txFrame.length    = 8;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x11) | CAN_WORD0_DATA_BYTE_2(0x11) |
                        CAN_WORD0_DATA_BYTE_3(0x11);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x11) | CAN_WORD1_DATA_BYTE_5(0x11) | CAN_WORD1_DATA_BYTE_6(0x11) |
                        CAN_WORD1_DATA_BYTE_7(0x11);

    txXfer.frame = &txFrame;
    txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
    FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg1 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Send second message through Tx Message Buffer. */
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x22) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x22) |
                        CAN_WORD0_DATA_BYTE_3(0x22);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x22) | CAN_WORD1_DATA_BYTE_5(0x22) | CAN_WORD1_DATA_BYTE_6(0x22) |
                        CAN_WORD1_DATA_BYTE_7(0x22);
    FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg2 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Send third message through Tx Message Buffer. */
    txXfer.frame      = &txFrame;
    txXfer.mbIdx      = TX_MESSAGE_BUFFER_NUM;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x33) | CAN_WORD0_DATA_BYTE_1(0x33) | CAN_WORD0_DATA_BYTE_2(0x33) |
                        CAN_WORD0_DATA_BYTE_3(0x33);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x33) | CAN_WORD1_DATA_BYTE_5(0x33) | CAN_WORD1_DATA_BYTE_6(0x33) |
                        CAN_WORD1_DATA_BYTE_7(0x33);
    FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg3 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Send fourth message through Tx Message Buffer. */
    txXfer.frame      = &txFrame;
    txXfer.mbIdx      = TX_MESSAGE_BUFFER_NUM;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x44) | CAN_WORD0_DATA_BYTE_1(0x44) | CAN_WORD0_DATA_BYTE_2(0x44) |
                        CAN_WORD0_DATA_BYTE_3(0x44);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x44) | CAN_WORD1_DATA_BYTE_5(0x44) | CAN_WORD1_DATA_BYTE_6(0x44) |
                        CAN_WORD1_DATA_BYTE_7(0x44);
    FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg4 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Receive data through Rx FIFO. */
    rxFifoXfer.frame = &rxFrame;
    for (i = 0; i < 4; i++)
    {
        FLEXCAN_TransferReceiveFifoEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, &rxFifoXfer);
        while (!rxComplete)
        {
        }
        rxComplete = false;

        PRINTF("Receive Msg%d from FIFO: word0 = 0x%x, word1 = 0x%x, ID Filter Hit%d.\r\n", i + 1, rxFrame.dataWord0,
               rxFrame.dataWord1, rxFrame.idhit);
    }

    PRINTF("\r\n==FlexCAN loopback EDMA example -- Finish.==\r\n");

    while (1)
    {
        __WFI();
    }
}
