/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_irqsteer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN DMA__CAN0
#define EXAMPLE_CAN_CLKSRC kCLOCK_DMA_Can0
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_DMA_Can0)
#define RX_MESSAGE_BUFFER_NUM (9)
#define TX_MESSAGE_BUFFER_NUM (8)

#define DLC (8)

/* To get most precise baud rate under some circumstances, users need to set
   quantum which is composed of PSEG1/PSEG2/PROPSEG. Because CAN clock prescaler
   = source clock/(baud rate * quantum), for e.g. 84M clock and 1M baud rate, the
   quantum should be .e.g 14=(6+3+1)+4, so prescaler is 6. By default, quantum
   is set to 10=(3+2+1)+4, because for most platforms e.g. 120M source clock/(1M
   baud rate * 10) is an integer. Remember users must ensure the calculated
   prescaler an integer thus to get precise baud rate. */
#define SET_CAN_QUANTUM 0
#define PSEG1 3
#define PSEG2 2
#define PROPSEG 1
#define FPSEG1 3
#define FPSEG2 3
#define FPROPSEG 1
#if (defined(FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829) && FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829)
/* To consider the First valid MB must be used as Reserved TX MB for ERR005829
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set default zero, the first valid TX MB Number is 8
   If RX FIFO enable(RFEN bit in MCE set as 1) and RFFN in CTRL2 is set by other value(0x1~0xF), User should consider
   detail first valid MB number
   If RX FIFO disable(RFEN bit in MCE set as 0) , the first valid MB number is zero */
#ifdef RX_MESSAGE_BUFFER_NUM
#undef RX_MESSAGE_BUFFER_NUM
#define RX_MESSAGE_BUFFER_NUM (10)
#endif
#ifdef TX_MESSAGE_BUFFER_NUM
#undef TX_MESSAGE_BUFFER_NUM
#define TX_MESSAGE_BUFFER_NUM (9)
#endif
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_mb_transfer_t txXfer, rxXfer;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t txFrame, rxFrame;
#else
flexcan_frame_t txFrame, rxFrame;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
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
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;

    /* Initialize board hardware. */
    sc_ipc_t ipc;

    ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Power on Peripherals. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipc, SC_R_CAN_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on FLEXCAN\r\n");
    }

    /* Set Peripheral clock frequency. */
    if (CLOCK_SetIpFreq(kCLOCK_DMA_Can0, SC_24MHZ) == 0)
    {
        PRINTF("Error: Failed to set FLEXCAN frequency\r\n");
    }

    /* Enable interrupt in irqsteer */
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA_FLEXCAN0_INT_IRQn);

    PRINTF("\r\n==FlexCAN loopback example -- Start.==\r\n\r\n");

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 1000000U;
     * flexcanConfig.baudRateFD = 2000000U;
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
#if (defined(USE_CANFD) && USE_CANFD)
    if (FLEXCAN_FDCalculateImprovedTimingValues(flexcanConfig.baudRate, flexcanConfig.baudRateFD, EXAMPLE_CAN_CLK_FREQ,
                                                &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        PRINTF("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#else
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
#endif

#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, false);
#else
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
#endif

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(0x123);
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#else
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#endif

/* Setup Tx Message Buffer. */
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#else
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#endif

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Start receive data through Rx Message Buffer. */
    rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    rxXfer.framefd = &rxFrame;
    FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#else
    rxXfer.frame = &rxFrame;
    FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#endif

    /* Prepare Tx Frame for sending. */
    txFrame.format = kFLEXCAN_FrameFormatStandard;
    txFrame.type   = kFLEXCAN_FrameTypeData;
    txFrame.id     = FLEXCAN_ID_STD(0x123);
    txFrame.length = DLC;
#if (defined(USE_CANFD) && USE_CANFD)
    txFrame.brs = 1;
#endif
#if (defined(USE_CANFD) && USE_CANFD)
    uint8_t i = 0;
    for (i = 0; i < DWORD_IN_MB; i++)
    {
        txFrame.dataWord[i] = i;
    }
#else
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x33) |
                        CAN_WORD0_DATA_BYTE_3(0x44);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x55) | CAN_WORD1_DATA_BYTE_5(0x66) | CAN_WORD1_DATA_BYTE_6(0x77) |
                        CAN_WORD1_DATA_BYTE_7(0x88);
#endif

    PRINTF("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
#if (defined(USE_CANFD) && USE_CANFD)
    for (i = 0; i < DWORD_IN_MB; i++)
    {
        PRINTF("tx word%d = 0x%x\r\n", i, txFrame.dataWord[i]);
    }
#else
    PRINTF("tx word0 = 0x%x\r\n", txFrame.dataWord0);
    PRINTF("tx word1 = 0x%x\r\n", txFrame.dataWord1);
#endif

    /* Send data through Tx Message Buffer. */
    txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    txXfer.framefd = &txFrame;
    FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#else
    txXfer.frame = &txFrame;
    FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#endif

    /* Waiting for Rx Message finish. */
    while ((!rxComplete) || (!txComplete))
    {
    };

    PRINTF("\r\nReceived message from MB%d\r\n", RX_MESSAGE_BUFFER_NUM);
#if (defined(USE_CANFD) && USE_CANFD)
    for (i = 0; i < DWORD_IN_MB; i++)
    {
        PRINTF("rx word%d = 0x%x\r\n", i, rxFrame.dataWord[i]);
    }
#else
    PRINTF("rx word0 = 0x%x\r\n", rxFrame.dataWord0);
    PRINTF("rx word1 = 0x%x\r\n", rxFrame.dataWord1);
#endif

    PRINTF("\r\n==FlexCAN loopback example -- Finish.==\r\n");

    while (1)
    {
    }
}
