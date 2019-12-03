/*
 * Copyright  2017 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdlib.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
#include "fsl_memory.h"
#endif
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_lpuart.h"
#include "fsl_irqsteer.h"
#include "svc/misc/misc_api.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_ENET CONNECTIVITY__ENET0
#define EXAMPLE_PHY (0x00U)
#define CORE_CLK_FREQ (167000000U)
#define ENET_RXBD_NUM (2)
#define ENET_TXBD_NUM (2)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_BuffSizeAlign(n) ENET_ALIGN(n, ENET_BUFF_ALIGNMENT)
#define ENET_DATA_LENGTH (1000)
#define ENET_HEAD_LENGTH (14)
#define ENET_FRAME_LENGTH ENET_DATA_LENGTH + ENET_HEAD_LENGTH
#define ENET_TRANSMIT_DATA_NUM (30)
#define ENET_ALIGN(x, align) ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)-1)))
#define ENET_HEAD_TYPE_OFFSET 12U               /*!< ENET head type offset. */
#define ENET_VLANTYPE 0x8100U                   /*! @brief VLAN TYPE */
#define ENET_VLANTAGLEN 4U                      /*! @brief VLAN TAG length */
#define ENET_AVBTYPE 0x22F0U                    /*! @brief AVB TYPE */
#define ENET_AVTPDU_IEC61883 0U                 /*! @brief AVTPDU formats to use the IEC 61883 protocol as subtype. */
#define ENET_AVTPDU_IEC61883_SUBTYPE_MASK 0x7FU /*! @brief AVTPDU formats to use the IEC 61883 subtype mask. */
#define ENET_AVTPDU_IEC61883_SPH_OFFSET 26U     /*! @brief AVTPDU IEC61883 format SPH feild byte-offset. */
#define ENET_AVTPDU_IEC61883_SPH_MASK 0x04U     /*! @brief AVTPDU IEC61883 format SPH BIT MASK in the byte. */
#define ENET_AVTPDU_IEC61883_FMT_OFFSET 28U     /*! @brief AVTPDU IEC61883 format FMT feild byte-offset. */
#define ENET_AVTPDU_IEC61883_FMT_MASK 0x3FU     /*! @brief AVTPDU IEC61883 format FMT BIT MASK in the byte. */
#define ENET_AVTPDU_IEC61883_6AUDIOTYPE 0x10U   /*! @brief AVTPDU IEC61883-6 audio&music type. */
#define ENET_AVTPDU_IEC61883_8DVDTYPE 0x00U     /*! @brief AVTPDU IEC61883-8 DV type. */
#define ENET_HTONS(n) __REV16(n)
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Build ENET PTP event frame. */
static void ENET_BuildFrame(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[FSL_FEATURE_ENET_QUEUE][ENET_RXBD_NUM],
                              ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[FSL_FEATURE_ENET_QUEUE][ENET_TXBD_NUM],
                              ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_rxDataBuff1[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff1[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_rxDataBuff2[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff2[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);

enet_handle_t g_handle;

/* The MAC address for ENET device. */
uint8_t g_macAddr[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};
uint8_t g_frame[FSL_FEATURE_ENET_QUEUE][ENET_FRAME_LENGTH];
uint32_t g_rxIndex       = 0;
uint32_t g_rxIndex1      = 0;
uint32_t g_rxIndex2      = 0;
uint32_t g_txIndex       = 0;
uint32_t g_txIndex1      = 0;
uint32_t g_txIndex2      = 0;
uint32_t g_txCurrentIdx  = 0;
uint32_t g_txSuccessFlag = false;
uint32_t g_rxSuccessFlag = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_EnableNVIC(void)
{
    /* Enable IRQ STEER */
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_FRAME1_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_FRAME2_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_FRAME0_EVENT_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_TIMER_INT_IRQn);
    /* Enable the NVIC */
    NVIC_EnableIRQ(IRQSTEER_4_IRQn);
}


/*! @brief Build Frame for transmit. */
static void ENET_BuildFrame(void)
{
    uint32_t count = 0;
    uint8_t index  = 0;

    /* memset */
    for (index = 0; index < FSL_FEATURE_ENET_QUEUE; index++)
    {
        memset(&g_frame[index], 0, ENET_FRAME_LENGTH);
    }

    /* Create the mac addresses. */
    for (index = 0; index < FSL_FEATURE_ENET_QUEUE; index++)
    {
        for (count = 0; count < 6U; count++)
        {
            g_frame[index][count] = 0xFFU;
        }
        memcpy(&g_frame[index][6], &g_macAddr[0], 6U);
    }

    /* Create the different type frame from the ethernet type offset:
     * first: for normal frame
     * second: for Class A data flag specififc AVB frame with VLAN tag.
     * third: for Class B data flag specififc AVB frame with VLAN tag.
     */
    /* First frame. */
    g_frame[0][ENET_HEAD_TYPE_OFFSET] = (ENET_DATA_LENGTH >> 8) & 0xFFU;
    g_frame[0][13]                    = ENET_DATA_LENGTH & 0xFFU;
    for (count = ENET_HEAD_LENGTH; count < ENET_FRAME_LENGTH; count++)
    {
        g_frame[0][count] = count % 0xFFU;
    }

    /* Second frame. */
    *(uint16_t *)&g_frame[1][ENET_HEAD_TYPE_OFFSET] = ENET_HTONS(ENET_VLANTYPE);                    /* VLAN TAG type. */
    *(uint16_t *)&g_frame[1][ENET_HEAD_TYPE_OFFSET + ENET_VLANTAGLEN / 2] = ENET_HTONS((5 << 13U)); /* Prio 5. */
    *(uint16_t *)&g_frame[1][ENET_HEAD_TYPE_OFFSET + ENET_VLANTAGLEN]     = ENET_HTONS(ENET_AVBTYPE); /* AVTP type. */
    uint32_t offset                                                       = ENET_HEAD_TYPE_OFFSET + ENET_VLANTAGLEN + 2;
    g_frame[1][offset]      = 0; /* AVTPDU set subtype with IEC61883 in common header. */
    g_frame[1][26 + offset] = 0; /* AVTPDU set SPH field. */
    g_frame[1][28 + offset] = (1 << 7) | ENET_AVTPDU_IEC61883_6AUDIOTYPE;
    for (count = 50; count < ENET_FRAME_LENGTH; count++)
    {
        g_frame[1][count] = count % 0xFFU;
    }

    /* Third frame. */
    *(uint16_t *)&g_frame[2][ENET_HEAD_TYPE_OFFSET] = ENET_HTONS(ENET_VLANTYPE);                    /* VLAN TAG type. */
    *(uint16_t *)&g_frame[2][ENET_HEAD_TYPE_OFFSET + ENET_VLANTAGLEN / 2] = ENET_HTONS((4 << 13U)); /* Prio 4. */
    *(uint16_t *)&g_frame[2][ENET_HEAD_TYPE_OFFSET + ENET_VLANTAGLEN] =
        ENET_HTONS(ENET_AVBTYPE); /* AVTP type for ether type field. */
    offset                  = ENET_HEAD_TYPE_OFFSET + ENET_VLANTAGLEN + 2;
    g_frame[2][offset]      = 0; /* AVTPDU set subtype with IEC61883 in common header. */
    g_frame[2][26 + offset] = 0; /* AVTPDU set SPH field. */
    g_frame[2][28 + offset] = (1 << 7) | ENET_AVTPDU_IEC61883_8DVDTYPE;
    for (count = 50; count < ENET_FRAME_LENGTH; count++)
    {
        g_frame[2][count] = count % 0xFFU;
    }
}

void ENET_IntCallback(ENET_Type *base, enet_handle_t *handle, uint32_t ringId, enet_event_t event, void *param)
{
    status_t status;
    uint32_t length;

    switch (event)
    {
        case kENET_RxEvent:
            /* Get the Frame size */
            status = ENET_GetRxFrameSizeMultiRing(&g_handle, &length, ringId);
            if ((status == kStatus_Success) && (length != 0))
            {
                /* Received valid frame. Deliver the rx buffer with the size equal to length. */
                uint8_t *data = (uint8_t *)malloc(length);
                status        = ENET_ReadFrameMultiRing(EXAMPLE_ENET, &g_handle, data, length, ringId);
                if (status == kStatus_Success)
                {
                    if (ringId == 0)
                    {
                        g_rxIndex++;
                    }
                    else if (ringId == 1)
                    {
                        g_rxIndex1++;
                    }
                    else if (ringId == 2)
                    {
                        g_rxIndex2++;
                    }
                }
                free(data);
            }
            else if (status == kStatus_ENET_RxFrameError)
            {
                /* update the receive buffer. */
                ENET_ReadFrameMultiRing(EXAMPLE_ENET, &g_handle, NULL, 0, ringId);
            }
            /* Set rx success flag. */
            if (g_rxIndex + g_rxIndex1 + g_rxIndex2 == ENET_TRANSMIT_DATA_NUM)
            {
                g_rxSuccessFlag = true;
            }
            break;
        case kENET_TxEvent:
            switch (ringId)
            {
                case 0:
                    g_txIndex++;
                    g_txCurrentIdx = g_txIndex;
                    break;
                case 1:
                    g_txIndex1++;
                    g_txCurrentIdx = g_txIndex1;
                    break;
                case 2:
                    g_txIndex2++;
                    g_txCurrentIdx = g_txIndex2;
                    break;
                default:
                    break;
            }
            if ((g_txIndex + g_txIndex1 + g_txIndex2) == ENET_TRANSMIT_DATA_NUM)
            {
                g_txSuccessFlag = true;
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
    enet_config_t config;
    uint32_t sysClock;
    bool link = false;
    enet_avb_config_t avbConfig;
    uint32_t ringId    = 0;
    uint32_t testTxNum = 0;
    status_t result;

    /* Hardware Initialization. */
    sc_ipc_t ipc;
    ipc = BOARD_InitRpc();
    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    uint32_t clk = 125000000;
    /* Open power and clock. */
    sc_pm_set_resource_power_mode(ipc, SC_R_ENET_0, SC_PM_PW_MODE_ON);
    if (sc_pm_clock_enable(ipc, SC_R_ENET_0, SC_PM_CLK_PER, false, true) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to disable SC_R_ENET_0 clock\r\n");
    }
    if (sc_pm_set_clock_rate(ipc, SC_R_ENET_0, SC_PM_CLK_PER, &clk) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set SC_R_ENET_0 clock rate\r\n");
    }
    sc_misc_set_control(ipc, SC_R_ENET_0, SC_C_DISABLE_50, 1);
    sc_misc_set_control(ipc, SC_R_ENET_0, SC_C_TXCLK, 0);
    sc_misc_set_control(ipc, SC_R_ENET_0, SC_C_SEL_125, 1);
    sc_misc_set_control(ipc, SC_R_ENET_0, SC_C_CLKDIV, 0);
    if (sc_pm_clock_enable(ipc, SC_R_ENET_0, SC_PM_CLK_PER, true, true) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable SC_R_ENET_0 clock\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTR\r\n");
    }
    BOARD_EnableNVIC();

    memset(&avbConfig, 0, sizeof(enet_avb_config_t));

    /* prepare the buffer configuration. */
    const enet_buffer_config_t buffConfig[FSL_FEATURE_ENET_QUEUE] = {
        {
            ENET_RXBD_NUM,
            ENET_TXBD_NUM,
            SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
            SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
            &g_rxBuffDescrip[0][0],
            &g_txBuffDescrip[0][0],
            &g_rxDataBuff[0][0],
            &g_txDataBuff[0][0],
        },
        {
            ENET_RXBD_NUM,
            ENET_TXBD_NUM,
            SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
            SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
            &g_rxBuffDescrip[1][0],
            &g_txBuffDescrip[1][0],
            &g_rxDataBuff1[0][0],
            &g_txDataBuff1[0][0],
        },
        {
            ENET_RXBD_NUM,
            ENET_TXBD_NUM,
            SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
            SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
            &g_rxBuffDescrip[2][0],
            &g_txBuffDescrip[2][0],
            &g_rxDataBuff2[0][0],
            &g_txDataBuff2[0][0],
        }};

    sysClock = CORE_CLK_FREQ;

    PRINTF("\r\n ENET multi-ring txrx example start.\r\n");

    /* Get default configuration 100M RMII. */
    ENET_GetDefaultConfig(&config);
    /* Set SMI to get PHY link status. */
    sysClock = CORE_CLK_FREQ;
    result   = PHY_Init(EXAMPLE_ENET, EXAMPLE_PHY, sysClock);
    /* PHY will not be succeed to finish auto-negotiation when in loopback mode.
       So if you didn't insert the cable with the valid linker partner or set phy
       with loopback mode. the auto-negotiation will fail. in this demo auto-negotiation
       failure will not be a problem since we use the phy loopback mode.
     */
    if ((result != kStatus_PHY_AutoNegotiateFail) && (result != kStatus_Success))
    {
        PRINTF("PHY initialize failed\r\n");
    }

    /*Set Phy local loop back mode*/
    PHY_EnableLoopback(EXAMPLE_ENET, EXAMPLE_PHY, kPHY_LocalLoop, kPHY_Speed1000M, true);

    /* Initialize ENET. */
    config.miiSpeed = kENET_MiiSpeed1000M;
    config.miiMode  = kENET_RgmiiMode; /* For zebu test mii mode with loop mode*/

    config.interrupt = ENET_TX_INTERRUPT | ENET_RX_INTERRUPT;
    config.ringNum   = 3;
    /* Open promiscuous mode for loopback */
    config.macSpecialConfig = kENET_ControlPromiscuousEnable;
    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig[0], &g_macAddr[0], sysClock);
    /* Ring 1 BW fraction is 0.5 = 1/(1+ 512/512), Ring 2 BW fraction is 0.2 = 1/(1 + 512/128)  */
    avbConfig.idleSlope[0] = kENET_IdleSlope512;
    avbConfig.idleSlope[1] = kENET_IdleSlope128;
    /* Receive classification for ring 1 and ring 2 */
    avbConfig.rxClassifyMatch[0] = ENET_RCMR_CMP0(1) | ENET_RCMR_CMP1(2) | ENET_RCMR_CMP2(3) | ENET_RCMR_CMP3(4);
    avbConfig.rxClassifyMatch[1] = ENET_RCMR_CMP0(5) | ENET_RCMR_CMP1(6) | ENET_RCMR_CMP2(7) | ENET_RCMR_CMP3(7);

    ENET_AVBConfigure(EXAMPLE_ENET, &g_handle, &avbConfig);

    /* Setup callback. */
    ENET_SetCallback(&g_handle, ENET_IntCallback, NULL);

    /* Build broadcast for sending and active for receiving. */
    ENET_BuildFrame();

    ENET_ActiveReadMultiRing(EXAMPLE_ENET);

    while (1)
    {
        if (testTxNum < ENET_TRANSMIT_DATA_NUM)
        {
            /* Send a multicast frame when the PHY is link up. */
            PHY_GetLinkStatus(EXAMPLE_ENET, EXAMPLE_PHY, &link);
            if (link)
            {
                testTxNum++;
                if (kStatus_Success ==
                    ENET_SendFrameMultiRing(EXAMPLE_ENET, &g_handle, &g_frame[ringId][0], ENET_FRAME_LENGTH, ringId))
                {
                    PRINTF("The %d frame transmitted from the ring %d !\r\n", g_txCurrentIdx, ringId);
                    ringId = (ringId + 1) % 3;
                }
                PRINTF("The frame received from the ring 0, 1, 2 is %d, %d, %d now!\r\n", g_rxIndex, g_rxIndex1,
                       g_rxIndex2);
            }
        }
        if (g_txSuccessFlag)
        {
            PRINTF("%d frames transmitted succeed!\r\n", ENET_TRANSMIT_DATA_NUM);
            g_txSuccessFlag = false;
        }
        if (g_rxSuccessFlag)
        {
            PRINTF("%d frames successfully received from the ring 0!\r\n", g_rxIndex);
            PRINTF("%d frames successfully received from the ring 1!\r\n", g_rxIndex1);
            PRINTF("%d frames successfully received from the ring 2!\r\n", g_rxIndex2);
            g_rxSuccessFlag = false;
        }
    }
}
