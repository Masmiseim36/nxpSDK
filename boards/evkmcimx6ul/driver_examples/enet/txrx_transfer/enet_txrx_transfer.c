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
#define EXAMPLE_PHY  0x02U
#define CORE_CLK_FREQ  CLOCK_GetFreq(kCLOCK_AhbClk)
#define APP_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define ENET_RXBD_NUM (4)
#define ENET_TXBD_NUM (4)
#define ENET_RXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH (1000)
#define ENET_TRANSMIT_DATA_NUM (20)
#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Build ENET broadcast frame. */
static void ENET_BuildBroadCastFrame(void);

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

enet_handle_t g_handle;
uint8_t g_frame[ENET_DATA_LENGTH + 14];
uint32_t g_testTxNum = 0;

/*! @brief The MAC address for ENET device. */
uint8_t g_macAddr[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {true, true, false, 1, 1};
    CLOCK_InitEnetPll(&config);
}


/*! @brief Build Frame for transmit. */
static void ENET_BuildBroadCastFrame(void)
{
    uint32_t count = 0;
    uint32_t length = ENET_DATA_LENGTH - 14;

    for (count = 0; count < 6U; count++)
    {
        g_frame[count] = 0xFFU;
    }
    memcpy(&g_frame[6], &g_macAddr[0], 6U);
    g_frame[12] = (length >> 8) & 0xFFU;
    g_frame[13] = length & 0xFFU;

    for (count = 0; count < length; count++)
    {
        g_frame[count + 14] = count % 0xFFU;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    enet_config_t config;
    uint32_t length = 0;
    uint32_t sysClock;
    bool link = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    uint32_t txnumber = 0;
    status_t status;
    enet_data_error_stats_t eErrStatic;

    /* Hardware Initialization. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    PRINTF("\r\n ENET example start.\r\n");

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

    /* Get default configuration. */
    /*
     * config.miiMode = kENET_RmiiMode;
     * config.miiSpeed = kENET_MiiSpeed100M;
     * config.miiDuplex = kENET_MiiFullDuplex;
     * config.rxMaxFrameLen = ENET_FRAME_MAX_FRAMELEN;
     */
    ENET_GetDefaultConfig(&config);

    /* Set SMI to get PHY link status. */
    sysClock = CORE_CLK_FREQ;
    PHY_Init(EXAMPLE_ENET, EXAMPLE_PHY, sysClock);
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

    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig, &g_macAddr[0], sysClock);
    ENET_ActiveRead(EXAMPLE_ENET);

    /* Build broadcast for sending. */
    ENET_BuildBroadCastFrame();

    while (1)
    {
        /* Get the Frame size */
        status = ENET_GetRxFrameSize(&g_handle, &length);
        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            uint8_t *data = (uint8_t *)malloc(length);
            status = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length);
            if (status == kStatus_Success)
            {
                PRINTF(" A frame received. the length %d ", length);
                PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
                       data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
                       data[10], data[11]);
            }
            free(data);
        }
        else if (status == kStatus_ENET_RxFrameError)
        {
            /* Update the received buffer when error happened. */
            /* Get the error information of the received g_frame. */
            ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic);
            /* update the receive buffer. */
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
                    txnumber++;
                    if (kStatus_Success == ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], ENET_DATA_LENGTH))
                    {
                        PRINTF("The %d frame transmitted success!\r\n", txnumber);
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
