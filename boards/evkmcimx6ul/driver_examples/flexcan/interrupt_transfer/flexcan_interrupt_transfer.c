/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_clock.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN CAN1
#define EXAMPLE_CAN_CLK_FREQ CLOCK_GetOscFreq()
#define RX_MESSAGE_BUFFER_NUM (9)
#define TX_MESSAGE_BUFFER_NUM (8)

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
flexcan_frame_t frame;
uint32_t txIdentifier;
uint32_t rxIdentifier;

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
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

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
    uint8_t node_type;

    /* Initialize board hardware. */
    BOARD_InitPins();    
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    
    /* Set CAN source to OSC 24M */
    CLOCK_SetMux(kCLOCK_CanMux, 1U); 
    /* Set CAN divider to 1 */
    CLOCK_SetDiv(kCLOCK_CanDiv, 0);  
    /*Enable CAN transceiver*/
    BOARD_NXP74LV595_SetValue(kNXP74LV595_CAN1_2_STBY , kSignal_NXP74LV595_Low);

    /* Install IRQ Handler */
    SystemInitIrqTable();

    PRINTF("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    PRINTF("    Message format: Standard (11 bit id)\r\n");
    PRINTF("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    PRINTF("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    PRINTF("    Interrupt Mode: Enabled\r\n");
    PRINTF("    Operation Mode: TX and RX --> Normal\r\n");
    PRINTF("*********************************************\r\n\r\n");

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
        txIdentifier = 0x321;
        rxIdentifier = 0x123;
    }
    else
    {
        txIdentifier = 0x123;
        rxIdentifier = 0x321;
    }

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 125000U;
     * flexcanConfig.maxMbNum = 16;
     * flexcanConfig.enableLoopBack = false;
     * flexcanConfig.enableSelfWakeup = false;
     * flexcanConfig.enableIndividMask = false;
     * flexcanConfig.enableDoze = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    /* Init FlexCAN module. */
#if (!defined(FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE)) || !FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE
    flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
#endif /* FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE */
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type = kFLEXCAN_FrameTypeData;
    mbConfig.id = FLEXCAN_ID_STD(rxIdentifier);
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);

    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    if ((node_type == 'A') || (node_type == 'a'))
    {
        PRINTF("Press any key to trigger one-shot transmission\r\n\r\n");
        frame.dataByte0 = 0;
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

            frame.id = FLEXCAN_ID_STD(txIdentifier);
            frame.format = kFLEXCAN_FrameFormatStandard;
            frame.type = kFLEXCAN_FrameTypeData;
            frame.length = 1;
            txXfer.frame = &frame;
            txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
            FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);

            while (!txComplete)
            {
            };
            txComplete = false;

            /* Start receive data through Rx Message Buffer. */
            rxXfer.frame = &frame;
            rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
            FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);

            /* Wait until Rx MB full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            PRINTF("Rx MB ID: 0x%3x, Rx MB data: 0x%x\r\n", frame.id >> CAN_ID_STD_SHIFT, frame.dataByte0);
            PRINTF("Press any key to trigger the next transmission!\r\n\r\n");
            frame.dataByte0++;
        }
        else
        {
            /* Start receive data through Rx Message Buffer. */
            rxXfer.frame = &frame;
            rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
            FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);

            /* Wait until Rx receive full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            PRINTF("Rx MB ID: 0x%3x, Rx MB data: 0x%x\r\n", frame.id >> CAN_ID_STD_SHIFT, frame.dataByte0);

            frame.id = FLEXCAN_ID_STD(txIdentifier);
            txXfer.frame = &frame;
            txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
            FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);

            while (!txComplete)
            {
            };
            txComplete = false;
            PRINTF("Wait Node A to trigger the next transmission!\r\n\r\n");
        }
    }
}
