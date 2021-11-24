/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_mcan.h"
#include "pin_mux.h"
#include "board.h"
#include "stdlib.h"

#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USE_CANFD (1U)
/*
 *    CAN_DATASIZE   DLC    BYTES_IN_MB
 *    8              8      kMCAN_8ByteDatafield
 *    12             9      kMCAN_12ByteDatafield
 *    16             10     kMCAN_16ByteDatafield
 *    20             11     kMCAN_20ByteDatafield
 *    24             12     kMCAN_24ByteDatafield
 *    32             13     kMCAN_32ByteDatafield
 *    48             14     kMCAN_48ByteDatafield
 *    64             15     kMCAN_64ByteDatafield
 *
 *  CAN data size (pay load size), DLC and Bytes in Message buffer must align.
 *
 */
#define DLC         (15)
#define BYTES_IN_MB kMCAN_64ByteDatafield
/* If not define USE_CANFD or define it 0, CAN_DATASIZE should be 8. */
#define CAN_DATASIZE (64U)
/* If user need to auto execute the improved timming configuration. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
#define EXAMPLE_MCAN_IRQHandler    CAN0_IRQ0_IRQHandler
#define EXAMPLE_MCAN_IRQn          CAN0_IRQ0_IRQn
#define EXAMPLE_MCAN               CAN0
#define MCAN_CLK_FREQ              CLOCK_GetMCanClkFreq()
#define STDID_OFFSET               (18U)
#define MSG_RAM_BASE               0x04000000U
#define STD_FILTER_OFS 0x0
#define RX_FIFO0_OFS   0x10U
#define TX_BUFFER_OFS  0x20U
#define MSG_RAM_SIZE   (TX_BUFFER_OFS + 8 + CAN_DATASIZE)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
mcan_tx_buffer_frame_t txFrame;
mcan_rx_buffer_frame_t rxFrame;
uint8_t tx_data[CAN_DATASIZE];
uint8_t rx_data[CAN_DATASIZE];
mcan_handle_t mcanHandle;
mcan_buffer_transfer_t txXfer;
mcan_fifo_transfer_t rxXfer;
uint32_t txIdentifier;
uint32_t rxIdentifier;
#ifndef MSG_RAM_BASE
SDK_ALIGN(uint8_t msgRam[MSG_RAM_SIZE], 1U << CAN_MRBA_BA_SHIFT);
#else
#define msgRam MSG_RAM_BASE
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief MCAN Call Back function
 */
static void mcan_callback(CAN_Type *base, mcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        case kStatus_MCAN_RxFifo0Idle:
        {
            rxComplete = true;
        }
        break;

        case kStatus_MCAN_TxIdle:
        {
            txComplete = true;
        }
        break;

        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    mcan_config_t mcanConfig;
    mcan_frame_filter_config_t rxFilter;
    mcan_std_filter_element_config_t stdFilter;
    mcan_rx_fifo_config_t rxFifo0;
    mcan_tx_buffer_config_t txBuffer;
    uint8_t node_type;
    uint8_t numMessage = 0;
    uint8_t cnt        = 0;

    /* Initialize board hardware. */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* Set MCAN clock 96Mhz/4=24MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivCanClk, 4U, true);
    CLOCK_AttachClk(kMCAN_DIV_to_MCAN);

    BOARD_InitPins();
    BOARD_BootClockFROHF96M();
    BOARD_InitDebugConsole();

    do
    {
        PRINTF("Please select local node as A or B:\r\n");
        PRINTF("Note: Node B should start first.\r\n");
        PRINTF("Node:");
        node_type = GETCHAR();
        PRINTF("%c", node_type);
        PRINTF("\r\n");
    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));

    /* Select mailbox ID. */
    if ((node_type == 'A') || (node_type == 'a'))
    {
        txIdentifier = 0x321U;
        rxIdentifier = 0x123U;
    }
    else
    {
        txIdentifier = 0x123U;
        rxIdentifier = 0x321U;
    }

    MCAN_GetDefaultConfig(&mcanConfig);
#if (defined(USE_CANFD) && USE_CANFD)
    mcanConfig.enableCanfdNormal = true;
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    mcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(timing_config));
#if (defined(USE_CANFD) && USE_CANFD)
    if (MCAN_FDCalculateImprovedTimingValues(mcanConfig.baudRateA, mcanConfig.baudRateD, MCAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(mcanConfig.timingConfig), &timing_config, sizeof(mcan_timing_config_t));
    }
    else
    {
        PRINTF("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#else
    if (MCAN_CalculateImprovedTimingValues(mcanConfig.baudRateA, MCAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(mcanConfig.timingConfig), &timing_config, sizeof(mcan_timing_config_t));
    }
    else
    {
        PRINTF("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif
#endif

    MCAN_Init(EXAMPLE_MCAN, &mcanConfig, MCAN_CLK_FREQ);

    /* Create MCAN handle structure and set call back function. */
    MCAN_TransferCreateHandle(EXAMPLE_MCAN, &mcanHandle, mcan_callback, NULL);

    /* Set Message RAM base address and clear to avoid BEU/BEC error. */
    MCAN_SetMsgRAMBase(EXAMPLE_MCAN, (uint32_t)msgRam);
    memset((void *)msgRam, 0, MSG_RAM_SIZE * sizeof(uint8_t));

    /* STD filter config. */
    rxFilter.address  = STD_FILTER_OFS;
    rxFilter.idFormat = kMCAN_FrameIDStandard;
    rxFilter.listSize = 1U;
    rxFilter.nmFrame  = kMCAN_reject0;
    rxFilter.remFrame = kMCAN_rejectFrame;
    MCAN_SetFilterConfig(EXAMPLE_MCAN, &rxFilter);

    stdFilter.sfec = kMCAN_storeinFifo0;
    /* Classic filter mode, only filter matching ID. */
    stdFilter.sft   = kMCAN_classic;
    stdFilter.sfid1 = rxIdentifier;
    stdFilter.sfid2 = 0x7FFU;
    MCAN_SetSTDFilterElement(EXAMPLE_MCAN, &rxFilter, &stdFilter, 0);

    /* RX fifo0 config. */
    rxFifo0.address       = RX_FIFO0_OFS;
    rxFifo0.elementSize   = 1U;
    rxFifo0.watermark     = 0;
    rxFifo0.opmode        = kMCAN_FifoBlocking;
    rxFifo0.datafieldSize = kMCAN_8ByteDatafield;
#if (defined(USE_CANFD) && USE_CANFD)
    rxFifo0.datafieldSize = BYTES_IN_MB;
#endif
    MCAN_SetRxFifo0Config(EXAMPLE_MCAN, &rxFifo0);

    /* TX buffer config. */
    memset(&txBuffer, 0, sizeof(txBuffer));
    txBuffer.address       = TX_BUFFER_OFS;
    txBuffer.dedicatedSize = 1U;
    txBuffer.fqSize        = 0;
    txBuffer.datafieldSize = kMCAN_8ByteDatafield;
#if (defined(USE_CANFD) && USE_CANFD)
    txBuffer.datafieldSize = BYTES_IN_MB;
#endif
    MCAN_SetTxBufferConfig(EXAMPLE_MCAN, &txBuffer);

    /* Finish software initialization and enter normal mode, synchronizes to
       CAN bus, ready for communication */
    MCAN_EnterNormalMode(EXAMPLE_MCAN);

    if ((node_type == 'A') || (node_type == 'a'))
    {
        PRINTF("Press any key to trigger one-shot transmission\r\n\r\n");
    }
    else
    {
        PRINTF("Start to Wait data from Node A\r\n\r\n");
    }

    while (1)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            GETCHAR();
            /* Config TX frame data. */
            memset(tx_data, 0, sizeof(uint8_t) * CAN_DATASIZE);
            for (cnt = 0; cnt < CAN_DATASIZE; cnt++)
            {
                tx_data[cnt] = cnt;
            }
            tx_data[0] += numMessage++;
            txFrame.xtd  = kMCAN_FrameIDStandard;
            txFrame.rtr  = kMCAN_FrameTypeData;
            txFrame.fdf  = 0;
            txFrame.brs  = 0;
            txFrame.dlc  = 8U;
            txFrame.id   = txIdentifier << STDID_OFFSET;
            txFrame.data = tx_data;
            txFrame.size = CAN_DATASIZE;
#if (defined(USE_CANFD) && USE_CANFD)
            txFrame.fdf = 1;
            txFrame.brs = 1;
            txFrame.dlc = DLC;
#endif
            txXfer.frame     = &txFrame;
            txXfer.bufferIdx = 0;
            MCAN_TransferSendNonBlocking(EXAMPLE_MCAN, &mcanHandle, &txXfer);

            while (!txComplete)
            {
            }
            txComplete = false;

            /* Start receive data through Rx FIFO 0. */
            memset(rx_data, 0, sizeof(uint8_t) * CAN_DATASIZE);
            /* the MCAN engine can't auto to get rx payload size, we need set it. */
            rxFrame.size = CAN_DATASIZE;
            rxXfer.frame = &rxFrame;
            MCAN_TransferReceiveFifoNonBlocking(EXAMPLE_MCAN, 0, &mcanHandle, &rxXfer);

            /* Wait until message received. */
            while (!rxComplete)
            {
            }
            rxComplete = false;

            /* After call the API of rMCAN_TransferReceiveFifoNonBlocking success, we can
             * only get a point (rxFrame.data) to the fifo reading entrance.
             * Copy the received frame data from the FIFO by the pointer(rxFrame.data). */
            memcpy(rx_data, rxFrame.data, rxFrame.size);

            PRINTF("Received Frame ID: 0x%x\r\n", rxFrame.id >> STDID_OFFSET);
            PRINTF("Received Frame DATA: ");
            cnt = 0;
            while (cnt < rxFrame.size)
            {
                PRINTF("0x%x ", rx_data[cnt++]);
            }
            PRINTF("\r\n");
            PRINTF("Press any key to trigger the next transmission!\r\n\r\n");
        }
        else
        {
            memset(rx_data, 0, sizeof(uint8_t) * CAN_DATASIZE);
            /* the MCAN engine can't auto to get rx payload size, we need set it. */
            rxFrame.size = CAN_DATASIZE;
            rxXfer.frame = &rxFrame;
            MCAN_TransferReceiveFifoNonBlocking(EXAMPLE_MCAN, 0, &mcanHandle, &rxXfer);

            while (!rxComplete)
            {
            }
            rxComplete = false;

            /* After call the API of rMCAN_TransferReceiveFifoNonBlocking success, we can
             * only get a point (rxFrame.data) to the fifo reading entrance.
             * Copy the received frame data from the FIFO by the pointer(rxFrame.data). */
            memcpy(rx_data, rxFrame.data, rxFrame.size);

            PRINTF("Received Frame ID: 0x%x\r\n", rxFrame.id >> STDID_OFFSET);
            PRINTF("Received Frame DATA: ");

            cnt = 0;
            while (cnt < rxFrame.size)
            {
                PRINTF("0x%x ", rx_data[cnt++]);
            }
            PRINTF("\r\n");

            /* Copy received frame data to tx frame. */
            memcpy(tx_data, rx_data, CAN_DATASIZE);

            txFrame.xtd      = rxFrame.xtd;
            txFrame.rtr      = rxFrame.rtr;
            txFrame.fdf      = rxFrame.fdf;
            txFrame.brs      = rxFrame.brs;
            txFrame.dlc      = rxFrame.dlc;
            txFrame.id       = txIdentifier << STDID_OFFSET;
            txFrame.data     = tx_data;
            txFrame.size     = rxFrame.size;
            txXfer.frame     = &txFrame;
            txXfer.bufferIdx = 0;

            MCAN_TransferSendNonBlocking(EXAMPLE_MCAN, &mcanHandle, &txXfer);

            while (!txComplete)
            {
            }
            txComplete = false;
            PRINTF("Wait Node A to trigger the next transmission!\r\n\r\n");
        }
    }
}
