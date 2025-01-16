/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "fsl_flexcan_edma.h"
#include "board.h"
#include "app.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
#ifndef RX_MESSAGE_COUNT
#define RX_MESSAGE_COUNT (6U)
#endif
/*
 *    DWORD_IN_MB    DLC    BYTES_IN_MB             Maximum MBs
 *    2              8      kFLEXCAN_8BperMB        64
 *    4              10     kFLEXCAN_16BperMB       42
 *    8              13     kFLEXCAN_32BperMB       25
 *    16             15     kFLEXCAN_64BperMB       14
 *
 * Dword in each message buffer, Length of data in bytes, Payload size must align,
 * and the Message Buffers are limited corresponding to each payload configuration:
 */
#define DLC         (8)
#define BYTES_IN_MB kFLEXCAN_8BperMB
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
flexcan_frame_t txFrame = {0};
AT_NONCACHEABLE_SECTION(flexcan_frame_t rxFrame[RX_MESSAGE_COUNT]);
/* Config fifo filters to make it accept std frame with ID 0x123 ~ 0x12A. */
uint32_t rxFifoFilter[] = {FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x123, 0, 0), 
                           FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x124, 0, 0), 
                           FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x125, 0, 0), 
                           FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x126, 0, 0), 
                           FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x127, 0, 0), 
                           FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x128, 0, 0), 
                           FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x129, 0, 0), 
                           FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x12A, 0, 0)};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static FLEXCAN_CALLBACK(flexcan_callback)
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

#if !(defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
    uint8_t node_type;
#endif
    uint32_t i;

    /* Initialize board hardware. */
    BOARD_InitHardware();

    LOG_INFO("FlexCAN Legacy Rx FIFO edma example.\r\n");
#if (defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
    LOG_INFO("Loopback mode, Message buffer %d used for Tx, Legacy Rx FIFO used for Rx.\r\n", TX_MESSAGE_BUFFER_NUM);
#else
    LOG_INFO("Board to board mode.\r\n");
    LOG_INFO("Node B Legacy Rx FIFO used for Rx.\r\n");
    LOG_INFO("Node A Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);

    do
    {
        LOG_INFO("Please select local node as A or B:\r\n");
        LOG_INFO("Note: Node B should start first.\r\n");
        LOG_INFO("Node:");
        node_type = GETCHAR();
        LOG_INFO("%c", node_type);
        LOG_INFO("\r\n");
    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));
#endif

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.bitRate                = 1000000U;
     * flexcanConfig.bitRateFD              = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    flexcanConfig.bitRate = 500000U;

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

#if (defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
    flexcanConfig.enableLoopBack = true;
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
    if (FLEXCAN_CalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.bitRate, EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

#if !(defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
    if ((node_type == 'A') || (node_type == 'a'))
    {
#endif
        /* Setup Tx Message Buffer. */
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
        /* Send messages through Tx Message Buffer. */
        txFrame.dataWord0 = 0;
        txFrame.dataWord1 = 0x55;
        txFrame.format    = (uint8_t)kFLEXCAN_FrameFormatStandard;
        txFrame.type      = (uint8_t)kFLEXCAN_FrameTypeData;
        txFrame.id        = FLEXCAN_ID_STD(0x123);
        txFrame.length    = (uint8_t)DLC;
        txXfer.frame      = &txFrame;
        txXfer.mbIdx      = (uint8_t)TX_MESSAGE_BUFFER_NUM;
#if !(defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
    }
    else
    {
#endif
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
#if defined(BOARD_GetEDMAConfig)
        BOARD_GetEDMAConfig(edmaConfig);
#endif
        EDMA_Init(EXAMPLE_CAN_DMA, &edmaConfig);

        /* Create EDMA handle. */
        EDMA_CreateHandle(&flexcanRxFifoEdmaHandle, EXAMPLE_CAN_DMA, EXAMPLE_CAN_DMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
        EDMA_SetChannelMux(EXAMPLE_CAN_DMA, EXAMPLE_CAN_DMA_CHANNEL, FLEXCAN_DMA_REQUEST_SOURCE);
#endif

        rxFifoConfig.idFilterTable = rxFifoFilter;
        rxFifoConfig.idFilterNum   = 8U;
        rxFifoConfig.idFilterType  = kFLEXCAN_RxFifoFilterTypeA;
        rxFifoConfig.priority      = kFLEXCAN_RxFifoPrioHigh;
        FLEXCAN_SetRxFifoConfig(EXAMPLE_CAN, &rxFifoConfig, true);

        rxFifoXfer.frame    = &rxFrame[0];
        rxFifoXfer.frameNum = RX_MESSAGE_COUNT;
        /* Create FlexCAN EDMA handle structure and set call back function. */
        FLEXCAN_TransferCreateHandleEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, flexcan_dma_callback, NULL,
                                         &flexcanRxFifoEdmaHandle);
#if !(defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
        LOG_INFO("Start to Wait data from Node A.\r\n\r\n");
    }
#endif

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    while (true)
    {
#if !(defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
        if ((node_type == 'A') || (node_type == 'a'))
        {
#endif
            LOG_INFO("Press any key to trigger %d transmission.\r\n\r\n", RX_MESSAGE_COUNT);
            GETCHAR();
            for (i = 0; i < RX_MESSAGE_COUNT; i++)
            {
                txFrame.id = FLEXCAN_ID_STD(0x123U + i);
                (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);

                while (!txComplete)
                {
                };
                txComplete = false;
                LOG_INFO("Send Msg%d: word0 = 0x%x, word1 = 0x%x, id = 0x%x.\r\n", i, txFrame.dataWord0,
                            txFrame.dataWord1, 0x123U + i);
                txFrame.dataWord0++;
            }
            LOG_INFO("\r\n");
#if !(defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
        }
        else
        {
#endif
            /* Receive data through Legacy Rx FIFO. */
            if (FLEXCAN_TransferReceiveFifoEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, &rxFifoXfer) != kStatus_Success)
            {
                LOG_INFO("Receive CAN message from Legacy Rx FIFO failed!\r\n");
                return -1;
            }
            else
            {
                while (!rxComplete)
                {
                }
                rxComplete = false;
                for (i = 0; i < RX_MESSAGE_COUNT; i++)
                {
                    LOG_INFO("Receive Msg%d: word0 = 0x%x, word1 = 0x%x, ID Filter Hit: %d, Time stamp: %d.\r\n",
                        i, rxFrame[i].dataWord0, rxFrame[i].dataWord1, rxFrame[i].idhit, rxFrame[i].timestamp);
                }
                LOG_INFO("\r\n");
            }
#if !(defined(ENABLE_LOOPBACK) && ENABLE_LOOPBACK)
            LOG_INFO("Wait for the next %d messages!\r\n\r\n", RX_MESSAGE_COUNT);
        }
#endif
    }
}
