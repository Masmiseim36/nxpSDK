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

#include <stdlib.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_cache.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_ENET ENET1
#define EXAMPLE_PHY 0x02U
#define CORE_CLK_FREQ  CLOCK_GetFreq(kCLOCK_AhbClk)
#define PTP_CLK_FREQ   25000000U
#define EXAMPLE_ENET_TIMERSYNC_IRQ ENET1_1588_IRQn
#define APP_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)

#define ENET_RXBD_NUM (4)
#define ENET_TXBD_NUM (4)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH (1000)
#define ENET_TRANSMIT_DATA_NUM (20)
#define ENET_PTP_SYNC_MSG 0x00U
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Build ENET PTP event frame. */
static void ENET_BuildPtpEventFrame(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);

/* Buffers for store receive and transmit timestamp. */
enet_ptp_time_data_t g_rxPtpTsBuff[ENET_RXBD_NUM];
enet_ptp_time_data_t g_txPtpTsBuff[ENET_TXBD_NUM];

enet_handle_t g_handle;
uint8_t g_frame[ENET_DATA_LENGTH + 14];
uint32_t g_testTxNum = 0;

/* The MAC address for ENET device. */
uint8_t g_macAddr[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, true, true, 1, 1};
    CLOCK_InitEnetPll(&config);
}


/*! @brief Build Frame for transmit. */
static void ENET_BuildPtpEventFrame(void)
{
    uint8_t mGAddr[6] = {0x01, 0x00, 0x5e, 0x01, 0x01, 0x1};
    /* Build for PTP event message frame. */
    memcpy(&g_frame[0], &mGAddr[0], 6);
    /* The six-byte source MAC address. */
    memcpy(&g_frame[6], &g_macAddr[0], 6);
    /* The type/length: if data length is used make sure it's smaller than 1500 */
    g_frame[12] = 0x08U;
    g_frame[13] = 0x00U;
    g_frame[0x0EU] = 0x40;
    g_frame[0x24U] = (kENET_PtpEventPort >> 8) & 0xFFU;
    g_frame[0x25U] = kENET_PtpEventPort & 0xFFU;
    g_frame[0x17U] = 0x11U;
    /* Add ptp event message type: sync message. */
    g_frame[0x2AU - 12U] = ENET_PTP_SYNC_MSG;
    /* Add sequence id. */
    g_frame[0x48U - 12U] = 0;
    g_frame[0x48U - 11U] = 0;
}

/*!
 * @brief Main function
 */
int main(void)
{
    enet_config_t config;
    uint32_t length = 0;
    uint32_t sysClock;
    uint32_t ptpClock;
    uint32_t count = 0;
    bool link = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    status_t result;
    uint32_t txnumber = 0;
    enet_ptp_time_data_t ptpData;
    enet_ptp_time_t ptpTime;
    uint32_t totalDelay;
    uint8_t mGAddr[6] = {0x01, 0x00, 0x5e, 0x01, 0x01, 0x1};
    /* Hardware Initialization. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();
    /* Install IRQ Handler */
    SystemInitIrqTable();

    PRINTF("\r\n ENET PTP 1588 example start.\r\n");

    /* prepare the buffer configuration. */
    enet_buffer_config_t buffConfig = {
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        &g_rxBuffDescrip[0],
        &g_txBuffDescrip[0],
        &g_rxDataBuff[0][0],
        &g_txDataBuff[0][0],
    };

    sysClock = CORE_CLK_FREQ;
    ptpClock = PTP_CLK_FREQ;
    /* Prepare the PTP configure */
    enet_ptp_config_t ptpConfig = {
        ENET_RXBD_NUM, ENET_TXBD_NUM, &g_rxPtpTsBuff[0], &g_txPtpTsBuff[0], kENET_PtpTimerChannel1, ptpClock,
    };

    /*
     * config.miiMode = kENET_RmiiMode;
     * config.miiSpeed = kENET_MiiSpeed100M;
     * config.miiDuplex = kENET_MiiFullDuplex;
     * config.rxMaxFrameLen = ENET_FRAME_MAX_FRAMELEN;
     */
    ENET_GetDefaultConfig(&config);
    PHY_Init(EXAMPLE_ENET, EXAMPLE_PHY, sysClock);
    /* Try to get PHY right link status. */
    PHY_GetLinkStatus(EXAMPLE_ENET, EXAMPLE_PHY, &link);
    if (link)
    {
        /* Get the actual PHY link speed. */
        PHY_GetLinkSpeedDuplex(EXAMPLE_ENET, EXAMPLE_PHY, &speed, &duplex);

        /* Change the MII speed and duplex for actual link status. */
        config.miiSpeed = (enet_mii_speed_t)speed;
        config.miiDuplex = (enet_mii_duplex_t)duplex;
    }
    else
    {
        PRINTF("\r\nPHY Link down, please check the cable connection and link partner setting.\r\n");
    }

    /* Initialize ENET. */
    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig, &g_macAddr[0], sysClock);
    /* Configure PTP */
    ENET_Ptp1588Configure(EXAMPLE_ENET, &g_handle, &ptpConfig);
    /* Add to multicast group to receive ptp multicast frame. */
    ENET_AddMulticastGroup(EXAMPLE_ENET, &mGAddr[0]);
    /* Active ENET read. */
    ENET_ActiveRead(EXAMPLE_ENET);

    ENET_BuildPtpEventFrame();

    for (count = 1; count <= 10; count++)
    {
        ENET_Ptp1588GetTimer(EXAMPLE_ENET, &g_handle, &ptpTime);
        PRINTF(" Get the %d-th time", count);
        PRINTF(" %d second,", (uint32_t)ptpTime.second);
        PRINTF(" %d nanosecond  \r\n", ptpTime.nanosecond);
        for (totalDelay = 0; totalDelay < 0xffffffU; totalDelay++)
        {
        }
    }

    while (1)
    {
        /* Get the Frame size */
        result = ENET_GetRxFrameSize(&g_handle, &length);
        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            uint8_t *data = (uint8_t *)malloc(length);
            result = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length);
            if (result == kStatus_Success)
            {
                PRINTF(" A frame received. the length %d ", length);
                memset((void *)&ptpData, 0, sizeof(enet_ptp_time_data_t));
                ptpData.messageType = ENET_PTP_SYNC_MSG;
                if (ENET_GetRxFrameTime(&g_handle, &ptpData) == kStatus_Success)
                {
                    PRINTF(" the timestamp is %d second,", (uint32_t)ptpData.timeStamp.second);
                    PRINTF(" %d nanosecond  \r\n", ptpData.timeStamp.nanosecond);
                }
                PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
                       data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
                       data[10], data[11]);
            }
            free(data);
        }
        else if (result == kStatus_ENET_RxFrameError)
        {
            /* Update the received buffer when error happened. */
            ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0);
        }

        if (g_testTxNum < ENET_TRANSMIT_DATA_NUM)
        {
            /* Send a multicast frame when the PHY is link up. */
            if (kStatus_Success == PHY_GetLinkStatus(EXAMPLE_ENET, EXAMPLE_PHY, &link))
            {
                if (link)
                {
                    g_testTxNum++;
                    if (kStatus_Success == ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], ENET_DATA_LENGTH))
                    {
                        txnumber++;
                        PRINTF("The %d frame transmitted success!", txnumber);
                        memset((void *)&ptpData, 0, sizeof(enet_ptp_time_data_t));
                        ptpData.messageType = ENET_PTP_SYNC_MSG;
                        if (ENET_GetTxFrameTime(&g_handle, &ptpData) == kStatus_Success)
                        {
                            PRINTF(" the timestamp is %d second,", (uint32_t)ptpData.timeStamp.second);
                            PRINTF(" %d nanosecond  \r\n", ptpData.timeStamp.nanosecond);
                        }
                        else
                        {
                            PRINTF(" Get transmit timestamp failed! \r\n");
                        }
                    }
                    else
                    {
                        PRINTF(" \r\nTransmit frame failed!\r\n");
                    }
                }
            }
        }
    }
}
