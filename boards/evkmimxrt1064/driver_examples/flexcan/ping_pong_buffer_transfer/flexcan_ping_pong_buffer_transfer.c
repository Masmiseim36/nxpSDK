/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN                   CAN2
#define EXAMPLE_FLEXCAN_RxWarningIRQn CAN2_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    CAN2_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     CAN2_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        CAN2_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    CAN2_IRQHandler

/* Considering that the first valid MB must be used as Reserved TX MB for ERR005829. */
#define RX_QUEUE_BUFFER_BASE  (1U)
#define RX_QUEUE_BUFFER_SIZE  (4U)
#define TX_MESSAGE_BUFFER_NUM (8U)

/* Select 60M clock divided by USB1 PLL (480 MHz) as master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (2U)
/* Get frequency of flexcan clock */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8U) / (FLEXCAN_CLOCK_SOURCE_DIVIDER + 1U))
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
/* Rx queue end Message Buffer index. */
#define RX_QUEUE_BUFFER_END_1 (RX_QUEUE_BUFFER_BASE + RX_QUEUE_BUFFER_SIZE - 1U)
#define RX_QUEUE_BUFFER_END_2 (RX_QUEUE_BUFFER_BASE + RX_QUEUE_BUFFER_SIZE * 2U - 1U)
/* Tx MB ID. */
#define TX_MB_ID 0x321UL
/* RX MB individual mask, which will make FLEXCAN to check only 1~8 bits of the ID of the received message. */
#define RX_MB_ID_MASK 0xFFUL
/* RX MB ID after mask. */
#define RX_MB_ID_AFTER_MASK (RX_MB_ID_MASK & TX_MB_ID)

#if (defined(USE_CANFD) && USE_CANFD)
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
#define DLC         (15)
#define BYTES_IN_MB kFLEXCAN_64BperMB
#else
#define DLC (8)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t rxQueue1Flag = 0;
volatile uint32_t rxQueue2Flag = 0;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t rxFrame[RX_QUEUE_BUFFER_SIZE * 2];
flexcan_fd_frame_t txFrame;
#else
flexcan_frame_t rxFrame[RX_QUEUE_BUFFER_SIZE * 2];
flexcan_frame_t txFrame;
#endif
volatile status_t rxStatus = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief User read message buffer function
 */
#if (defined(USE_CANFD) && USE_CANFD)
static status_t User_ReadRxMb(CAN_Type *base, uint8_t mbIdx, flexcan_fd_frame_t *pRxFrame)
#else
static status_t User_ReadRxMb(CAN_Type *base, uint8_t mbIdx, flexcan_frame_t *pRxFrame)
#endif
{
    /* Assertion. */
    assert(mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(NULL != pRxFrame);

    status_t status;
    uint32_t cs_temp;
    uint8_t rx_code;
    uint32_t can_id = 0;
#if (defined(USE_CANFD) && USE_CANFD)
    uint8_t cnt = 0;
    uint32_t dataSize;
    dataSize                  = (base->FDCTRL & CAN_FDCTRL_MBDSR0_MASK) >> CAN_FDCTRL_MBDSR0_SHIFT;
    uint8_t payload_dword     = 1;
    volatile uint32_t *mbAddr = &(base->MB[0].CS);
    uint32_t offset;

    switch (dataSize)
    {
        case (uint32_t)kFLEXCAN_8BperMB:
            offset = (((uint32_t)mbIdx / 32U) * 512U + ((uint32_t)mbIdx % 32U) * 16U);
            break;
        case (uint32_t)kFLEXCAN_16BperMB:
            offset = (((uint32_t)mbIdx / 21U) * 512U + ((uint32_t)mbIdx % 21U) * 24U);
            break;
        case (uint32_t)kFLEXCAN_32BperMB:
            offset = (((uint32_t)mbIdx / 12U) * 512U + ((uint32_t)mbIdx % 12U) * 40U);
            break;
        case (uint32_t)kFLEXCAN_64BperMB:
            offset = (((uint32_t)mbIdx / 7U) * 512U + ((uint32_t)mbIdx % 7U) * 72U);
            break;
        default:
            /* All the cases have been listed above, the default clause should not be reached. */
            assert(false);
            break;
    }
    /* To get the dword aligned offset, need to divide by 4. */
    offset = offset / 4U;

    /* Read CS field of Rx Message Buffer to lock Message Buffer. */
    cs_temp = mbAddr[offset];
    can_id  = mbAddr[offset + 1U];
#else
    /* Read CS field of Rx Message Buffer to lock Message Buffer. */
    cs_temp = base->MB[mbIdx].CS;
    can_id  = base->MB[mbIdx].ID;
#endif
    /* Get Rx Message Buffer Code field. */
    rx_code = (uint8_t)((cs_temp & CAN_CS_CODE_MASK) >> CAN_CS_CODE_SHIFT);

    /* Check to see if Rx Message Buffer is full or overrun. */
    if ((0x2 == rx_code) || (0x6 == rx_code))
    {
        /* Store Message ID. */
        pRxFrame->id = can_id & (CAN_ID_EXT_MASK | CAN_ID_STD_MASK);

        /* Get the message ID and format. */
        pRxFrame->format = (cs_temp & CAN_CS_IDE_MASK) != 0U ? (uint8_t)kFLEXCAN_FrameFormatExtend :
                                                               (uint8_t)kFLEXCAN_FrameFormatStandard;

        /* Get the message type. */
        pRxFrame->type =
            (cs_temp & CAN_CS_RTR_MASK) != 0U ? (uint8_t)kFLEXCAN_FrameTypeRemote : (uint8_t)kFLEXCAN_FrameTypeData;

        /* Get the message length. */
        pRxFrame->length = (uint8_t)((cs_temp & CAN_CS_DLC_MASK) >> CAN_CS_DLC_SHIFT);

        /* Get the time stamp. */
        pRxFrame->timestamp = (uint16_t)((cs_temp & CAN_CS_TIME_STAMP_MASK) >> CAN_CS_TIME_STAMP_SHIFT);

#if (defined(USE_CANFD) && USE_CANFD)
        /* Calculate the DWORD number, dataSize 0/1/2/3 corresponds to 8/16/32/64
           Bytes payload. */
        for (cnt = 0; cnt < (dataSize + 1U); cnt++)
        {
            payload_dword *= 2U;
        }

        /* Store Message Payload. */
        for (cnt = 0; cnt < payload_dword; cnt++)
        {
            pRxFrame->dataWord[cnt] = mbAddr[offset + 2U + cnt];
        }

        /* Restore original Rx ID value*/
        mbAddr[offset + 1U] = FLEXCAN_ID_STD(RX_MB_ID_AFTER_MASK);
#else
        /* Store Message Payload. */
        pRxFrame->dataWord0 = base->MB[mbIdx].WORD0;
        pRxFrame->dataWord1 = base->MB[mbIdx].WORD1;
        /* Restore original Rx value*/
        base->MB[mbIdx].ID = FLEXCAN_ID_STD(RX_MB_ID_AFTER_MASK);
#endif
        /* Read free-running timer to unlock Rx Message Buffer. */
        (void)base->TIMER;

        if (0x2 == rx_code)
        {
            status = kStatus_Success;
        }
        else
        {
            status = kStatus_FLEXCAN_RxOverflow;
        }
    }
    else
    {
        /* Read free-running timer to unlock Rx Message Buffer. */
        (void)base->TIMER;

        status = kStatus_Fail;
    }

    return status;
}

/*!
 * @brief User FlexCAN IRQ handler function
 */
static void User_TransferHandleIRQ(CAN_Type *base)
{
    status_t status;
    uint32_t EsrStatus;
    uint32_t i;

    do
    {
        /* Get Current FlexCAN Module Error and Status. */
        EsrStatus = FLEXCAN_GetStatusFlags(base);

        /* To handle FlexCAN Error and Status Interrupt first. */
        if (0U != (EsrStatus & ((uint32_t)kFLEXCAN_TxWarningIntFlag | (uint32_t)kFLEXCAN_RxWarningIntFlag |
                                (uint32_t)kFLEXCAN_BusOffIntFlag | (uint32_t)kFLEXCAN_ErrorIntFlag)))
        {
            rxStatus = kStatus_FLEXCAN_ErrorStatus;
            /* Clear FlexCAN Error and Status Interrupt. */
            FLEXCAN_ClearStatusFlags(base, (uint32_t)kFLEXCAN_TxWarningIntFlag | (uint32_t)kFLEXCAN_RxWarningIntFlag |
                                               (uint32_t)kFLEXCAN_BusOffIntFlag | (uint32_t)kFLEXCAN_ErrorIntFlag);
        }
        else if (0U != (EsrStatus & (uint32_t)kFLEXCAN_WakeUpIntFlag))
        {
            rxStatus = kStatus_FLEXCAN_WakeUp;
            /* Clear FlexCAN Wake Up Interrupt. */
            FLEXCAN_ClearStatusFlags(base, (uint32_t)kFLEXCAN_WakeUpIntFlag);
        }
        else
        {
            /* Handle real data transfer. */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
#if (RX_QUEUE_BUFFER_END_1 >= 64U)
            if (0U != FLEXCAN_GetHigh64MbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_END_1 - 64U)))
#else
            if (0U != FLEXCAN_GetMbStatusFlags(base, (uint64_t)1U << RX_QUEUE_BUFFER_END_1))
#endif
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
            if (0U != FLEXCAN_GetMbStatusFlags(base, (uint64_t)1U << RX_QUEUE_BUFFER_END_1))
#else
            if (0U != FLEXCAN_GetMbStatusFlags(base, (uint32_t)1U << RX_QUEUE_BUFFER_END_1))
#endif
            {
                /* Queue 1 end Message Buffer interrupt. */
                rxQueue1Flag = 1U;
                for (i = 0; i < RX_QUEUE_BUFFER_SIZE; i++)
                {
                    /* Default receive message ID is 0x321, and after individual mask 0xFF, can match the Rx queue MB ID
                       0x21. Change queue 1 MBs individual ID mask to 0x3FF, will make it can't match the Rx queue MB
                       ID, so queue 1 will ignore the next CAN/CANFD messages and the queue 2 will receive the messages.
                     */
                    FLEXCAN_SetRxIndividualMask(EXAMPLE_CAN, RX_QUEUE_BUFFER_BASE + i,
                                                FLEXCAN_RX_MB_STD_MASK(RX_MB_ID_MASK | TX_MB_ID, 0, 0));
                    (void)User_ReadRxMb(EXAMPLE_CAN, RX_QUEUE_BUFFER_BASE + i, &rxFrame[i]);
                    /* Clear queue 1 Message Buffer receive status. */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
                    if ((RX_QUEUE_BUFFER_BASE + i) >= 64U)
                    {
                        FLEXCAN_ClearHigh64MbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_BASE + i - 64U));
                    }
                    else
                    {
                        FLEXCAN_ClearMbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_BASE + i));
                    }
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
                    FLEXCAN_ClearMbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_BASE + i));
#else
                    FLEXCAN_ClearMbStatusFlags(base, (uint32_t)1U << (RX_QUEUE_BUFFER_BASE + i));
#endif
                }
            }
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
#if (RX_QUEUE_BUFFER_END_2 >= 64U)
            else if (0U != FLEXCAN_GetHigh64MbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_END_2 - 64U)))
#else
            else if (0U != FLEXCAN_GetMbStatusFlags(base, (uint64_t)1U << RX_QUEUE_BUFFER_END_2))
#endif
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
            else if (0U != FLEXCAN_GetMbStatusFlags(base, (uint64_t)1U << RX_QUEUE_BUFFER_END_2))
#else
            else if (0U != FLEXCAN_GetMbStatusFlags(base, (uint32_t)1U << RX_QUEUE_BUFFER_END_2))
#endif
            {
                /* Queue 2 end Message Buffer interrupt. */
                rxQueue2Flag = 1U;
                /* Restore queue 1 ID mask to make it can receive the next CAN/CANFD messages again. */
                for (i = 0; i < RX_QUEUE_BUFFER_SIZE; i++)
                    FLEXCAN_SetRxIndividualMask(EXAMPLE_CAN, RX_QUEUE_BUFFER_BASE + i,
                                                FLEXCAN_RX_MB_STD_MASK(RX_MB_ID_MASK, 0, 0));
                for (; i < (RX_QUEUE_BUFFER_SIZE * 2U); i++)
                {
                    status = User_ReadRxMb(EXAMPLE_CAN, RX_QUEUE_BUFFER_BASE + i, &rxFrame[i]);

                    /* Clear queue 2 Message Buffer receive status. */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
                    if ((RX_QUEUE_BUFFER_BASE + i) >= 64U)
                    {
                        FLEXCAN_ClearHigh64MbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_BASE + i - 64U));
                    }
                    else
                    {
                        FLEXCAN_ClearMbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_BASE + i));
                    }
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
                    FLEXCAN_ClearMbStatusFlags(base, (uint64_t)1U << (RX_QUEUE_BUFFER_BASE + i));
#else
                    FLEXCAN_ClearMbStatusFlags(base, (uint32_t)1U << (RX_QUEUE_BUFFER_BASE + i));
#endif
                    /* Due to enable the queue feature, the rx overflow may only occur in the last matched MB
                       (RX_QUEUE_BUFFER_END_2). */
                    if (((RX_QUEUE_BUFFER_BASE + i) == RX_QUEUE_BUFFER_END_2) && (status == kStatus_FLEXCAN_RxOverflow))
                        rxStatus = status;
                }
            }
        }
    }
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
#if (RX_QUEUE_BUFFER_END_1 >= 64U)
    while (
        (0U != FLEXCAN_GetHigh64MbStatusFlags(base, ((uint64_t)1U << (RX_QUEUE_BUFFER_END_1 - 64U)) |
                                                        ((uint64_t)1U << (RX_QUEUE_BUFFER_END_2 - 64U)))) ||
        (0U != (FLEXCAN_GetStatusFlags(base) & ((uint32_t)kFLEXCAN_TxWarningIntFlag |
                                                (uint32_t)kFLEXCAN_RxWarningIntFlag | (uint32_t)kFLEXCAN_BusOffIntFlag |
                                                (uint32_t)kFLEXCAN_ErrorIntFlag | (uint32_t)kFLEXCAN_WakeUpIntFlag))));
#elif (RX_QUEUE_BUFFER_END_2 >= 64U)
    while (
        (0U != FLEXCAN_GetMbStatusFlags(base, ((uint64_t)1U << RX_QUEUE_BUFFER_END_1))) ||
        (0U != FLEXCAN_GetHigh64MbStatusFlags(base, ((uint64_t)1U << (RX_QUEUE_BUFFER_END_2 - 64U)))) ||
        (0U != (FLEXCAN_GetStatusFlags(base) & ((uint32_t)kFLEXCAN_TxWarningIntFlag |
                                                (uint32_t)kFLEXCAN_RxWarningIntFlag | (uint32_t)kFLEXCAN_BusOffIntFlag |
                                                (uint32_t)kFLEXCAN_ErrorIntFlag | (uint32_t)kFLEXCAN_WakeUpIntFlag))));
#else
    while (
        (0U != FLEXCAN_GetMbStatusFlags(
                   base, ((uint64_t)1U << RX_QUEUE_BUFFER_END_1) | ((uint64_t)1U << RX_QUEUE_BUFFER_END_2))) ||
        (0U != (FLEXCAN_GetStatusFlags(base) & ((uint32_t)kFLEXCAN_TxWarningIntFlag |
                                                (uint32_t)kFLEXCAN_RxWarningIntFlag | (uint32_t)kFLEXCAN_BusOffIntFlag |
                                                (uint32_t)kFLEXCAN_ErrorIntFlag | (uint32_t)kFLEXCAN_WakeUpIntFlag))));
#endif
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    while (
        (0U != FLEXCAN_GetMbStatusFlags(
                   base, ((uint64_t)1U << RX_QUEUE_BUFFER_END_1) | ((uint64_t)1U << RX_QUEUE_BUFFER_END_2))) ||
        (0U != (FLEXCAN_GetStatusFlags(base) & ((uint32_t)kFLEXCAN_TxWarningIntFlag |
                                                (uint32_t)kFLEXCAN_RxWarningIntFlag | (uint32_t)kFLEXCAN_BusOffIntFlag |
                                                (uint32_t)kFLEXCAN_ErrorIntFlag | (uint32_t)kFLEXCAN_WakeUpIntFlag))));
#else
    while (
        (0U != FLEXCAN_GetMbStatusFlags(
                   base, ((uint32_t)1U << RX_QUEUE_BUFFER_END_1) | ((uint32_t)1U << RX_QUEUE_BUFFER_END_2))) ||
        (0U != (FLEXCAN_GetStatusFlags(base) & ((uint32_t)kFLEXCAN_TxWarningIntFlag |
                                                (uint32_t)kFLEXCAN_RxWarningIntFlag | (uint32_t)kFLEXCAN_BusOffIntFlag |
                                                (uint32_t)kFLEXCAN_ErrorIntFlag | (uint32_t)kFLEXCAN_WakeUpIntFlag))));
#endif
}

void EXAMPLE_FLEXCAN_IRQHandler(void)
{
    User_TransferHandleIRQ(EXAMPLE_CAN);
    SDK_ISR_EXIT_BARRIER;
}
/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
    uint8_t node_type;
    uint32_t i;
    static uint32_t TxCount = 1;

    /* Initialize board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Clock setting for FLEXCAN*/
    CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);

    LOG_INFO("********* FLEXCAN PingPong Buffer Example *********\r\n");
    LOG_INFO("    Message format: Standard (11 bit id)\r\n");
    LOG_INFO("    Node B Message buffer %d to %d used as Rx queue 1.\r\n", RX_QUEUE_BUFFER_BASE, RX_QUEUE_BUFFER_END_1);
    LOG_INFO("    Node B Message buffer %d to %d used as Rx queue 2.\r\n", RX_QUEUE_BUFFER_END_1 + 1U,
             RX_QUEUE_BUFFER_END_2);
    LOG_INFO("    Node A Message buffer %d used as Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    LOG_INFO("*********************************************\r\n\r\n");

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

    /* Enable Rx Individual Mask and Queue feature. */
    flexcanConfig.enableIndividMask = true;

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

/* Use the FLEXCAN API to automatically get the ideal bit timing configuration. */
#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
#if (defined(USE_CANFD) && USE_CANFD)
    if (FLEXCAN_FDCalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.bitRate, flexcanConfig.bitRateFD,
                                                EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#else
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
#endif

#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, true);
#else
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
#endif

#if (defined(USE_PHY_TJA1152) && USE_PHY_TJA1152)
    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    /* Initialize TJA1152. */
    /* STB=H, configuration CAN messages are expected from the local host via TXD pin. */
    RGPIO_PortSet(EXAMPLE_STB_RGPIO, 1u << EXAMPLE_STB_RGPIO_PIN);

    /* Classical CAN messages with standard identifier 0x555 must be transmitted 
     * by the local host controller until acknowledged by the TJA1152 for
     * automatic bit rate detection. Do not set frame.brs = 1U to keep nominal
     * bit rate in CANFD frame data phase. */
    txFrame.id     = FLEXCAN_ID_STD(0x555);
    txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    txFrame.length = 0U;
#if (defined(USE_CANFD) && USE_CANFD)
    (void)FLEXCAN_TransferFDSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#else
    (void)FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#endif
    SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Configuration of spoofing protection. */
    /* Add 0x321 and 0x123 to Transmission Whitelist. */
    txFrame.id     = FLEXCAN_ID_EXT(0x18DA00F1);
    txFrame.format = (uint8_t)kFLEXCAN_FrameFormatExtend;
    txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    txFrame.length = 6U;
#if (defined(USE_CANFD) && USE_CANFD)
    txFrame.dataWord[0] = CAN_WORD_DATA_BYTE_0(0x10) | CAN_WORD_DATA_BYTE_1(0x00) | CAN_WORD_DATA_BYTE_2(0x33) |
                          CAN_WORD_DATA_BYTE_3(0x21);
    txFrame.dataWord[1] = CAN_WORD_DATA_BYTE_4(0x11) | CAN_WORD_DATA_BYTE_5(0x23);
    (void)FLEXCAN_TransferFDSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#else
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x10) | CAN_WORD0_DATA_BYTE_1(0x00) | CAN_WORD0_DATA_BYTE_2(0x33) |
                        CAN_WORD0_DATA_BYTE_3(0x21);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x11) | CAN_WORD1_DATA_BYTE_5(0x23);
    (void)FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#endif
    SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Configuration of command message ID. */
    /* Reconfiguration is only accepted locally. Keep CONFIG_ID as default value 0x18DA00F1. */
    txFrame.length = 5U;
#if (defined(USE_CANFD) && USE_CANFD)
    txFrame.dataWord[0] = CAN_WORD_DATA_BYTE_0(0x60) | CAN_WORD_DATA_BYTE_1(0x98) | CAN_WORD_DATA_BYTE_2(0xDA) |
                          CAN_WORD_DATA_BYTE_3(0x00);
    txFrame.dataWord[1] = CAN_WORD_DATA_BYTE_4(0xF1);
    (void)FLEXCAN_TransferFDSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#else
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x60) | CAN_WORD0_DATA_BYTE_1(0x98) | CAN_WORD0_DATA_BYTE_2(0xDA) |
                        CAN_WORD0_DATA_BYTE_3(0x00);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0xF1);
    (void)FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#endif
    SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Leaving configuration mode. */
    /* Configuration into volatile memory only. */
    txFrame.length = 8U;
#if (defined(USE_CANFD) && USE_CANFD)
    txFrame.dataWord[0] = CAN_WORD_DATA_BYTE_0(0x71) | CAN_WORD_DATA_BYTE_1(0x02) | CAN_WORD_DATA_BYTE_2(0x03) |
                          CAN_WORD_DATA_BYTE_3(0x04);
    txFrame.dataWord[1] = CAN_WORD_DATA_BYTE_4(0x05) | CAN_WORD_DATA_BYTE_5(0x06) | CAN_WORD_DATA_BYTE_6(0x07) |
                          CAN_WORD_DATA_BYTE_7(0x08);
    (void)FLEXCAN_TransferFDSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#else
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x71) | CAN_WORD0_DATA_BYTE_1(0x02) | CAN_WORD0_DATA_BYTE_2(0x03) |
                        CAN_WORD0_DATA_BYTE_3(0x04);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x05) | CAN_WORD1_DATA_BYTE_5(0x06) | CAN_WORD1_DATA_BYTE_6(0x07) |
                        CAN_WORD1_DATA_BYTE_7(0x08);              
    (void)FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#endif
    SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    LOG_INFO("Initialize TJA1152 successfully!\r\n\r\n");

    /* STB=L, TJA1152 switch from secure standby mode to normal mode. */
    RGPIO_PortClear(EXAMPLE_STB_RGPIO, 1u << EXAMPLE_STB_RGPIO_PIN);
    /* Initialize TJA1152 end. */    
#endif

    if ((node_type == 'A') || (node_type == 'a'))
    {
        /* Setup Tx Message Buffer. */
#if (defined(USE_CANFD) && USE_CANFD)
        FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#else
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#endif
        txFrame.dataByte0 = 0;
        txFrame.dataByte1 = 0x55;
    }
    else
    {
        FLEXCAN_EnableInterrupts(EXAMPLE_CAN, (uint32_t)kFLEXCAN_BusOffInterruptEnable |
                                                  (uint32_t)kFLEXCAN_ErrorInterruptEnable |
                                                  (uint32_t)kFLEXCAN_RxWarningInterruptEnable);
        /* Enable interrupts in NVIC. */
        (void)EnableIRQ(EXAMPLE_FLEXCAN_RxWarningIRQn);
        (void)EnableIRQ(EXAMPLE_FLEXCAN_BusOffIRQn);
        (void)EnableIRQ(EXAMPLE_FLEXCAN_ErrorIRQn);
        (void)EnableIRQ(EXAMPLE_FLEXCAN_MBIRQn);
        /* Setup Rx Message Buffer. */
        /* Suppose to receive message ID 0x21. */
        mbConfig.format = kFLEXCAN_FrameFormatStandard;
        mbConfig.type   = kFLEXCAN_FrameTypeData;
        mbConfig.id     = FLEXCAN_ID_STD(RX_MB_ID_AFTER_MASK);

        for (i = 0U; i < RX_QUEUE_BUFFER_SIZE * 2U; i++)
        {
            /* Setup Rx individual ID mask 0xff. */
            /* Rx will receive message ID 0x321 and match with supposed ID 0x21. */
            FLEXCAN_SetRxIndividualMask(EXAMPLE_CAN, RX_QUEUE_BUFFER_BASE + i,
                                        FLEXCAN_RX_MB_STD_MASK(RX_MB_ID_MASK, 0, 0));
#if (defined(USE_CANFD) && USE_CANFD)
            FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, RX_QUEUE_BUFFER_BASE + i, &mbConfig, true);
#else
            FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_QUEUE_BUFFER_BASE + i, &mbConfig, true);
#endif
        }
        /* Enable receive interrupt for Rx queue 1 & 2 end Message Buffer. */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
#if (RX_QUEUE_BUFFER_END_1 >= 64U)
        FLEXCAN_EnableHigh64MbInterrupts(EXAMPLE_CAN, (uint64_t)1U << (RX_QUEUE_BUFFER_END_1 - 64U));
#else
        FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, (uint64_t)1U << RX_QUEUE_BUFFER_END_1);
#endif
#if (RX_QUEUE_BUFFER_END_2 >= 64U)
        FLEXCAN_EnableHigh64MbInterrupts(EXAMPLE_CAN, (uint64_t)1U << (RX_QUEUE_BUFFER_END_2 - 64U));
#else
        FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, (uint64_t)1U << RX_QUEUE_BUFFER_END_2);
#endif
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
        FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, (uint64_t)1U << RX_QUEUE_BUFFER_END_1);
        FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, (uint64_t)1U << RX_QUEUE_BUFFER_END_2);
#else
        FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, (uint32_t)1U << RX_QUEUE_BUFFER_END_1);
        FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN, (uint32_t)1U << RX_QUEUE_BUFFER_END_2);
#endif
        LOG_INFO("Start to Wait data from Node A\r\n\r\n");
    }

    while (true)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            uint8_t index  = 0;
            uint32_t times = 0;
            LOG_INFO("Please input the number of CAN/CANFD messages to be send and end with enter.\r\n");
            while (index != 0x0D)
            {
                index = GETCHAR();
                if ((index >= '0') && (index <= '9'))
                {
                    (void)PUTCHAR(index);
                    times = times * 10 + (index - 0x30U);
                }
            }
            LOG_INFO("\r\n");
            txFrame.id     = FLEXCAN_ID_STD(TX_MB_ID);
            txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
            txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
            txFrame.length = (uint8_t)DLC;
#if (defined(USE_CANFD) && USE_CANFD)
            txFrame.brs = 1U;
            txFrame.edl = 1U;
#endif

            for (i = 1; i <= times; i++)
            {
#if (defined(USE_CANFD) && USE_CANFD)
                (void)FLEXCAN_TransferFDSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#else
                (void)FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
#endif
                /* Wait for 200ms after every 2 RX_QUEUE_BUFFER_SIZE transmissions. */
                if ((TxCount % (RX_QUEUE_BUFFER_SIZE * 2U)) == 0U)
                    SDK_DelayAtLeastUs(200000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

                txFrame.dataByte0++;
                TxCount++;
            }
            LOG_INFO("Transmission done.\r\n\r\n");
        }
        else
        {
            /* Wait until Rx queue 1 full. */
            while (rxQueue1Flag != 1U)
            {
            };
            rxQueue1Flag = 0;
            LOG_INFO("Read Rx MB from Queue 1.\r\n");
            for (i = 0; i < RX_QUEUE_BUFFER_SIZE; i++)
            {
                LOG_INFO("Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", rxFrame[i].id >> CAN_ID_STD_SHIFT,
                         rxFrame[i].dataByte0, rxFrame[i].timestamp);
            }
            /* Wait until Rx queue 2 full. */
            while (rxQueue2Flag != 1U)
            {
            };
            rxQueue2Flag = 0;
            LOG_INFO("Read Rx MB from Queue 2.\r\n");
            for (; i < (RX_QUEUE_BUFFER_SIZE * 2U); i++)
            {
                LOG_INFO("Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", rxFrame[i].id >> CAN_ID_STD_SHIFT,
                         rxFrame[i].dataByte0, rxFrame[i].timestamp);
            }
            if (rxStatus == kStatus_FLEXCAN_RxOverflow)
            {
                rxStatus = 0;
                LOG_INFO("The data in the last MB %d in the queue 2 is overwritten\r\n", RX_QUEUE_BUFFER_END_2);
            }
            LOG_INFO("Wait Node A to trigger the next 8 messages!\r\n\r\n");
        }
    }
}
