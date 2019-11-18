/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define PTP_CLK_FREQ (125000000U)
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
void BOARD_EnableNVIC(void)
{
    /* Enable IRQ STEER */
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_FRAME1_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_FRAME2_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_FRAME0_EVENT_INT_IRQn);
    IRQSTEER_EnableInterrupt(IRQSTEER, CONNECTIVITY_ENET0_TIMER_INT_IRQn);
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
    g_frame[12]    = 0x08U;
    g_frame[13]    = 0x00U;
    g_frame[0x0EU] = 0x40;
    /* Set the UDP PTP event port number. */
    g_frame[0x24U] = (kENET_PtpEventPort >> 8) & 0xFFU;
    g_frame[0x25U] = kENET_PtpEventPort & 0xFFU;
    /* Set the UDP protocol. */
    g_frame[0x17U] = 0x11U;
    /* Add ptp event message type: sync message. */
    g_frame[0x2AU] = ENET_PTP_SYNC_MSG;
    /* Add sequence id. */
    g_frame[0x48U]     = 0;
    g_frame[0x48U + 1] = 0;
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
    bool link      = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    status_t result;
    uint32_t txnumber = 0;
    enet_ptp_time_data_t ptpData;
    enet_ptp_time_t ptpTime;
    uint32_t totalDelay;
    status_t status;
    uint8_t mGAddr[6] = {0x01, 0x00, 0x5e, 0x01, 0x01, 0x1};
    /* Hardware Initialization. */
    sc_ipc_t ipcHandle = BOARD_InitRpc();

    BOARD_InitPins(ipcHandle);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    uint32_t clk = 125000000;
    /* Open power and clock. */
    if (sc_pm_set_resource_power_mode(ipcHandle, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }
    if (sc_pm_set_resource_power_mode(ipcHandle, SC_R_ENET_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on ENET!\r\n");
    }
    if (sc_pm_clock_enable(ipcHandle, SC_R_ENET_0, SC_PM_CLK_PER, false, true) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable ENET clock!\r\n");
    }
    if (sc_pm_set_clock_rate(ipcHandle, SC_R_ENET_0, SC_PM_CLK_PER, &clk) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set clock rate on ENET!\r\n");
    }
    if (sc_misc_set_control(ipcHandle, SC_R_ENET_0, SC_C_DISABLE_50, 1) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set clock control on ENET!\r\n");
    }
    if (sc_misc_set_control(ipcHandle, SC_R_ENET_0, SC_C_TXCLK, 0) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set clock  control on ENET!\r\n");
    }
    if (sc_misc_set_control(ipcHandle, SC_R_ENET_0, SC_C_SEL_125, 1) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set clock control on ENET!\r\n");
    }
    if (sc_misc_set_control(ipcHandle, SC_R_ENET_0, SC_C_CLKDIV, 0) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to set clock control on ENET!\r\n");
    }
    if (sc_pm_clock_enable(ipcHandle, SC_R_ENET_0, SC_PM_CLK_PER, true, true) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to enable peripheral clock on ENET!\r\n");
    }
    BOARD_EnableNVIC();

    PRINTF("\r\n ENET PTP 1588 example start.\r\n");

    /* prepare the buffer configuration. */
    enet_buffer_config_t buffConfig[] = {{
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
        &g_rxBuffDescrip[0],
        &g_txBuffDescrip[0],
        &g_rxDataBuff[0][0],
        &g_txDataBuff[0][0],
    }};

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
    status = PHY_Init(EXAMPLE_ENET, EXAMPLE_PHY, sysClock);
    while (status != kStatus_Success)
    {
        PRINTF("\r\nPHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
        status = PHY_Init(EXAMPLE_ENET, EXAMPLE_PHY, sysClock);
    }

    PHY_GetLinkStatus(EXAMPLE_ENET, EXAMPLE_PHY, &link);
    if (link)
    {
        /* Get the actual PHY link speed. */
        PHY_GetLinkSpeedDuplex(EXAMPLE_ENET, EXAMPLE_PHY, &speed, &duplex);
        /* Change the MII speed and duplex for actual link status. */
        config.miiSpeed  = (enet_mii_speed_t)speed;
        config.miiDuplex = (enet_mii_duplex_t)duplex;
    }

    /* Initialize ENET. */
    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig[0], &g_macAddr[0], sysClock);
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
            result        = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length);
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
