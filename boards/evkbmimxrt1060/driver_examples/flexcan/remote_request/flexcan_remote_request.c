/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
#define DLC (8)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexcan_handle_t flexcanHandle;
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_mb_transfer_t txXfer, rxXfer;
flexcan_frame_t requestFrame, responseFrame, receiveFrame;
uint8_t node_type;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static FLEXCAN_CALLBACK(flexcan_callback)
{
    if ((node_type == 'A') || (node_type == 'a'))
    {
        switch (status)
        {
            case kStatus_FLEXCAN_RxRemote:
                if (REQUEST_MB_NUM == result)
                {
                    rxComplete = true;
                }
                break;
            case kStatus_FLEXCAN_TxSwitchToRx:
                if (REQUEST_MB_NUM == result)
                {
                    txComplete = true;
                }
                break;
            default:
                break;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    
    /* Initialize board hardware. */
    BOARD_InitHardware();

    LOG_INFO("******* FLEXCAN Remote Request EXAMPLE *******\r\n");
    LOG_INFO("    Message format: Standard (11 bit id)\r\n");
    LOG_INFO("    Message buffer %d used for Request.\r\n", REQUEST_MB_NUM);
    LOG_INFO("    Message buffer %d used for Response.\r\n", RESPONSE_MB_NUM);
    LOG_INFO("    Interrupt Mode: Enabled\r\n");
    LOG_INFO("**********************************************\r\n\r\n");

    do
    {
        LOG_INFO("Please select local node as A or B:\r\n");
        LOG_INFO("Note: Node B should start first.\r\n");
        LOG_INFO("Node:");
        node_type = GETCHAR();
        LOG_INFO("%c", node_type);
        LOG_INFO("\r\n");
    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));

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
    /* 
     * flexcanConfig.enableRemoteRequestFrameStored = false:
     * Frame's ID is compared to the IDs of the receive mailboxes with the CODE field configured as
     * 0b1010(kFLEXCAN_RxMbRanswer). If there is a matching ID, then this mailbox content will be
     * transmitted as response automatically. The received remote request frame is not stored in
     * receive buffer. It is only used to trigger a transmission of a frame in response.
     * 
     * flexcanConfig.enableRemoteRequestFrameStored = true:
     * Frame's ID is compared to the IDs of the receive mailboxes with the CODE field configured as
     * 0b0100, 0b0010, 0b0110. Message buffer will store the remote frame in the same fashion of a
     * data frame. No automatic remote response frame will be generated. User need to setup another
     * message buffer to respond remote request.
     */
    flexcanConfig.enableRemoteRequestFrameStored = false;

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

#if defined(EXAMPLE_CAN_BIT_RATE)
    flexcanConfig.bitRate = EXAMPLE_CAN_BIT_RATE;
#endif

    /* If special quantum setting is needed, set the timing parameters. */
#if (defined(SET_CAN_QUANTUM) && SET_CAN_QUANTUM)
    flexcanConfig.timingConfig.phaseSeg1 = PSEG1;
    flexcanConfig.timingConfig.phaseSeg2 = PSEG2;
    flexcanConfig.timingConfig.propSeg   = PROPSEG;
#if (defined(FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE) && FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE)
    flexcanConfig.timingConfig.fphaseSeg1 = FPSEG1;
    flexcanConfig.timingConfig.fphaseSeg2 = FPSEG2;
    flexcanConfig.timingConfig.fpropSeg   = FPROPSEG;
#endif
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

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    if ((node_type == 'A') || (node_type == 'a'))
    {
        /* Setup Remote Request Message Buffer. */
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, REQUEST_MB_NUM, true);

        LOG_INFO("Press any key to trigger one-shot remote request\r\n\r\n");
    }
    else
    {
        /* Setup Remote Response Message Buffer. */
        responseFrame.id        = FLEXCAN_ID_STD(0x321);
        responseFrame.type      = (uint8_t)kFLEXCAN_FrameTypeData;  /* Respond Data Frame. */
        responseFrame.format    = (uint8_t)kFLEXCAN_FrameFormatStandard;
        responseFrame.length    = (uint8_t)DLC;
        responseFrame.dataWord0 = 0;
        responseFrame.dataWord1 = 0x55;
        FLEXCAN_SetRemoteResponseMbConfig(EXAMPLE_CAN, RESPONSE_MB_NUM, &responseFrame);

        LOG_INFO("Start to wait request from Node A\r\n\r\n");
    }

    while (true)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            GETCHAR();

            /* 
             * Transmit Remote Request Frame.
             * Not recommend to setup other mailbox with same ID as Remote Request mailbox.
             * If so, Remote Response Frame maybe stored in other mailbox, instead of Remote
             * Request mailbox.
             */
            requestFrame.id     = FLEXCAN_ID_STD(0x321);
            requestFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
            requestFrame.type   = (uint8_t)kFLEXCAN_FrameTypeRemote;
            requestFrame.length = (uint8_t)DLC;
            txXfer.mbIdx        = (uint8_t)REQUEST_MB_NUM;
            txXfer.frame        = &requestFrame;
            (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);

            while (!txComplete)
            {
            };
            txComplete = false;

            /* 
             * After the remote request frame is transmitted successfully, the Remote Request
             * mailbox becomes a Receive mailbox, with the same ID as before.
             */
            LOG_INFO("Remote request complete!\r\n");

            /* Wait Remote Response Frame. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            /* Read Remote Response Frame from request mailbox. */
            FLEXCAN_ReadRxMb(EXAMPLE_CAN, (uint8_t)REQUEST_MB_NUM, &receiveFrame);

            LOG_INFO("Response message word0: 0x%x, word1: 0x%x\r\n", receiveFrame.dataWord0, receiveFrame.dataWord1);
            LOG_INFO("Response message Timestamp: %d\r\n\r\n", receiveFrame.timestamp);
        }
        else
        {
            /* Wait until respond remote frame complete. */
            while (0U == FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN, (uint64_t)1U << RESPONSE_MB_NUM))
            {
            }
            FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN, (uint64_t)1U << RESPONSE_MB_NUM);

            responseFrame.dataWord0++;
            FLEXCAN_SetRemoteResponseMbConfig(EXAMPLE_CAN, RESPONSE_MB_NUM, &responseFrame);

            LOG_INFO("Remote response complete!\r\n");
            LOG_INFO("Update next response message.\r\n\r\n");
        }
    }
}
