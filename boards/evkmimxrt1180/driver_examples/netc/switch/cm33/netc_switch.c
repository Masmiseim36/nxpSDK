/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_TX_INTR_MSG_DATA     1U
#define EXAMPLE_RX_INTR_MSG_DATA     2U
#define EXAMPLE_EP_TX_INTR_MSG_DATA  3U
#define EXAMPLE_TX_MSIX_ENTRY_IDX    0U
#define EXAMPLE_RX_MSIX_ENTRY_IDX    1U
#define EXAMPLE_EP_TX_MSIX_ENTRY_IDX 2U
#define EXAMPLE_FRAME_FID            1U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Rx buffer memeory type. */
typedef uint8_t rx_buffer_t[EXAMPLE_EP_RXBUFF_SIZE_ALIGN];

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* EP resource. */
static ep_handle_t g_ep_handle;

/* SWT resource. */
static swt_handle_t g_swt_handle;
static swt_config_t g_swt_config;
static swt_transfer_config_t swtTxRxConfig;

/* Buffer descriptor resource. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_TXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EXAMPLE_EP_TEST_FRAME_SIZE], EXAMPLE_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);

static uint64_t rxBuffAddrArray[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM];
static netc_tx_frame_info_t g_mgmtTxDirty[EXAMPLE_EP_TXBD_NUM];
static netc_tx_frame_info_t mgmtTxFrameInfo;
static netc_tx_frame_info_t g_txDirty[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_TXBD_NUM];
static netc_tx_frame_info_t txFrameInfo;
static volatile bool txOver;

static uint8_t g_macAddr[6] = {0x54, 0x27, 0x8d, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief Build Frame for single ring transmit. */
static void APP_BuildBroadCastFrame(uint32_t dstPort, uint32_t srcPort)
{
    uint32_t length = EXAMPLE_EP_TEST_FRAME_SIZE - 14U;
    uint32_t count;

    memcpy(&g_txFrame[0], &g_macAddr[0], 5U);
    g_txFrame[5] = dstPort;
    memcpy(&g_txFrame[6], &g_macAddr[0], 5U);
    g_txFrame[11] = srcPort;
    g_txFrame[12] = (length >> 8U) & 0xFFU;
    g_txFrame[13] = length & 0xFFU;

    for (count = 0; count < length; count++)
    {
        g_txFrame[count + 14U] = count % 0xFFU;
    }
}

static status_t APP_ReclaimCallback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
{
    txFrameInfo = *frameInfo;
    return kStatus_Success;
}

static status_t APP_SwtReclaimCallback(swt_handle_t *handle, netc_tx_frame_info_t *frameInfo, void *userData)
{
    mgmtTxFrameInfo = *frameInfo;
    return kStatus_Success;
}

void msgintrCallback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
    /* Transmit interrupt */
    if ((pendingIntr & (1U << EXAMPLE_TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&g_ep_handle, 1, 0);
        txOver = true;
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << EXAMPLE_RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(&g_ep_handle, 1);
    }
    /* EP Transmit interrupt */
    if ((pendingIntr & (1U << EXAMPLE_EP_TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&g_ep_handle, 2, 0);
        txOver = true;
    }
}

status_t APP_SWT_MacLearning_Forwarding(void)
{
    status_t result                  = kStatus_Success;
    netc_rx_bdr_config_t rxBdrConfig = {0};
    netc_tx_bdr_config_t txBdrConfig = {0};
    netc_bdr_config_t bdrConfig      = {.rxBdrConfig = &rxBdrConfig, .txBdrConfig = &txBdrConfig};
    netc_buffer_struct_t txBuff      = {.buffer = &g_txFrame, .length = sizeof(g_txFrame)};
    netc_frame_struct_t txFrame      = {.buffArray = &txBuff, .length = 1};
    swt_mgmt_tx_arg_t txArg          = {0};
    bool link                        = false;
    netc_port_phy_mac_traffic_statistic_t swtTxStatis;
    netc_tb_fdb_search_criteria_t sCriteria;
    netc_tb_fdb_rsp_data_t resp;
    netc_msix_entry_t msixEntry[3];
    netc_hw_mii_mode_t phyMode;
    netc_hw_mii_speed_t phySpeed;
    netc_hw_mii_duplex_t phyDuplex;
    ep_config_t g_ep_config;
    uint32_t msgAddr;
    uint32_t txCount;
    uint32_t i;

    PRINTF("\r\nNETC Switch example start.\r\n");

    for (uint8_t ring = 0U; ring < EXAMPLE_EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EXAMPLE_EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(EXAMPLE_MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(EXAMPLE_MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = EXAMPLE_TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = EXAMPLE_RX_INTR_MSG_DATA;
    msixEntry[2].control = kNETC_MsixIntrMaskBit;
    msixEntry[2].msgAddr = msgAddr;
    msixEntry[2].msgData = EXAMPLE_EP_TX_INTR_MSG_DATA;

    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EXAMPLE_EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EXAMPLE_EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = EXAMPLE_RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    bdrConfig.txBdrConfig[0].bdArray      = &g_txBuffDescrip[0][0];
    bdrConfig.txBdrConfig[0].len          = EXAMPLE_EP_TXBD_NUM;
    bdrConfig.txBdrConfig[0].dirtyArray   = &g_txDirty[0][0];
    bdrConfig.txBdrConfig[0].msixEntryIdx = EXAMPLE_EP_TX_MSIX_ENTRY_IDX;
    bdrConfig.txBdrConfig[0].enIntr       = true;

    (void)EP_GetDefaultConfig(&g_ep_config);
    g_ep_config.si                 = kNETC_ENETC1PSI0;
    g_ep_config.siConfig.txRingUse = 1;
    g_ep_config.siConfig.rxRingUse = 1;
    g_ep_config.reclaimCallback    = APP_ReclaimCallback;
    g_ep_config.msixEntry          = &msixEntry[0];
    g_ep_config.entryNum           = 3;

    result = EP_Init(&g_ep_handle, &g_macAddr[0], &g_ep_config, &bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    SWT_GetDefaultConfig(&g_swt_config);

    /* Wait PHY link up. */
    PRINTF("Wait for PHY link up...\r\n");
    for (i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        /* Only check the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            continue;
        }

        do
        {
            result = APP_PHY_GetLinkStatus(EXAMPLE_SWT_PORT0 + i, &link);
        } while ((result != kStatus_Success) || (!link));
        result = APP_PHY_GetLinkModeSpeedDuplex(EXAMPLE_SWT_PORT0 + i, &phyMode, &phySpeed, &phyDuplex);
        if (result != kStatus_Success)
        {
            return result;
        }
        g_swt_config.ports[i].ethMac.miiMode   = phyMode;
        g_swt_config.ports[i].ethMac.miiSpeed  = phySpeed;
        g_swt_config.ports[i].ethMac.miiDuplex = phyDuplex;
        g_swt_config.ports[i].bridgeCfg.isRxVlanAware = false;
    }

    g_swt_config.bridgeCfg.dVFCfg.portMembership = 0x1FU;
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID = EXAMPLE_FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo = kNETC_FDBLookUpWithDiscard;
    g_swt_config.bridgeCfg.dVFCfg.mlo = kNETC_HardwareMACLearn;

    g_swt_config.cmdRingUse            = 1U;
    g_swt_config.cmdBdrCfg[0].bdBase   = &g_cmdBuffDescrip[0];
    g_swt_config.cmdBdrCfg[0].bdLength = 8U;

    result = SWT_Init(&g_swt_handle, &g_swt_config);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Configure switch transfer resource. */
    swtTxRxConfig.enUseMgmtRxBdRing            = false;
    swtTxRxConfig.enUseMgmtTxBdRing            = true;
    swtTxRxConfig.mgmtTxBdrConfig.bdArray      = &g_mgmtTxBuffDescrip[0];
    swtTxRxConfig.mgmtTxBdrConfig.len          = EXAMPLE_EP_TXBD_NUM;
    swtTxRxConfig.mgmtTxBdrConfig.dirtyArray   = &g_mgmtTxDirty[0];
    swtTxRxConfig.mgmtTxBdrConfig.msixEntryIdx = EXAMPLE_TX_MSIX_ENTRY_IDX;
    swtTxRxConfig.mgmtTxBdrConfig.enIntr       = true;
    swtTxRxConfig.reclaimCallback              = APP_SwtReclaimCallback;

    result = SWT_ManagementTxRxConfig(&g_swt_handle, &g_ep_handle, &swtTxRxConfig);
    if (kStatus_Success != result)
    {
        return result;
    }

    /* Unmask MSIX message interrupt. */
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_TX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_RX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_EP_TX_MSIX_ENTRY_IDX, false);

    PRINTF("\r\nMAC learning.\r\n");
    i = 0;
    while (1)
    {
        /* Only use the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            i++;
            i = ((i >= EXAMPLE_SWT_MAX_PORT_NUM) ? 0 : i);
            if (i == 0U)
            {
                break;
            }
            continue;
        }

        /* Send the frame on port and loopback, let MAC learning work. */
        APP_BuildBroadCastFrame(0xF0, i);
        txOver     = false;
        txArg.ring = 0;
        result     = SWT_SendFrame(&g_swt_handle, txArg, (netc_hw_port_idx_t)(kNETC_SWITCH0Port0 + i), false, &txFrame, NULL, NULL);
        if (result != kStatus_Success)
        {
            PRINTF("\r\nTransmit frame failed!\r\n");
            return result;
        }
        while (!txOver)
        {
        }
        SWT_ReclaimTxDescriptor(&g_swt_handle, false, 0);
        if (mgmtTxFrameInfo.status != kNETC_EPTxSuccess)
        {
            PRINTF("\r\nTransmit frame has error!\r\n");
            return kStatus_Fail;
        }
        PRINTF("The frame received from port %u.", i);
        PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x.",
               g_txFrame[0], g_txFrame[1], g_txFrame[2], g_txFrame[3], g_txFrame[4], g_txFrame[5], g_txFrame[6],
               g_txFrame[7], g_txFrame[8], g_txFrame[9], g_txFrame[10], g_txFrame[11]);

        memset(&sCriteria, 0, sizeof(sCriteria));
        sCriteria.resumeEntryId   = 0xFFFFFFFF;
        sCriteria.cfgeMc          = kNETC_FDBCfgeMacthPortBitmap;
        sCriteria.cfge.portBitmap = 1U << i;
        if (SWT_BridgeSearchFDBTableEntry(&g_swt_handle, &sCriteria, &resp) != kStatus_Success)
        {
            PRINTF("\r\nGet FDB entry error!\r\n");
            return kStatus_Fail;
        }
        PRINTF(" Port %u bounds to MAC %02x:%02x:%02x:%02x:%02x:%02x.\r\n", i, resp.keye.macAddr[0], 
               resp.keye.macAddr[1], resp.keye.macAddr[2], resp.keye.macAddr[3], resp.keye.macAddr[4], resp.keye.macAddr[5]);

        i++;
        i = ((i >= EXAMPLE_SWT_MAX_PORT_NUM) ? 0 : i);
        if (i == 0U)
        {
            break;
        }
    }

    PRINTF("\r\nFrame forwarding.\r\n");
    while (1)
    {
        /* Only use the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            i++;
            i = ((i >= EXAMPLE_SWT_MAX_PORT_NUM) ? 0 : i);
            if (i == 0U)
            {
                break;
            }
            continue;
        }

        /* Disable the PHY loopback on this port */
        result = APP_PHY_EnableLoopback(i+1U, false);
        if (kStatus_Success != result)
        {
            return result;
        }

        NETC_PortGetPhyMacTxStatistic(g_swt_handle.hw.ports[i].eth, kNETC_ExpressMAC, &swtTxStatis);
        txCount = swtTxStatis.total511To1023BPacket;

        APP_BuildBroadCastFrame(i, 0xF0);
        txOver = false;
        result = EP_SendFrame(&g_ep_handle, 0, &txFrame, NULL, NULL);
        if (result != kStatus_Success)
        {
            PRINTF("\r\nTransmit frame failed!\r\n");
            return result;
        }
        while (!txOver)
        {
        }
        EP_ReclaimTxDescriptor(&g_ep_handle, 0);
        if (txFrameInfo.status != kNETC_EPTxSuccess)
        {
            PRINTF("\r\nTransmit frame failed!\r\n");
            return kStatus_Fail;
        }

        do
        {
            NETC_PortGetPhyMacTxStatistic(g_swt_handle.hw.ports[i].eth, kNETC_ExpressMAC, &swtTxStatis);
        } while (swtTxStatis.total511To1023BPacket != txCount + 1U);

        PRINTF("The frame received from pseudo port. Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x.",
               g_txFrame[0], g_txFrame[1], g_txFrame[2], g_txFrame[3], g_txFrame[4], g_txFrame[5], g_txFrame[6],
               g_txFrame[7], g_txFrame[8], g_txFrame[9], g_txFrame[10], g_txFrame[11]);
        PRINTF(" Frame forwarding to port %u.\r\n", i);

        i++;
        i = ((i >= EXAMPLE_SWT_MAX_PORT_NUM) ? 0 : i);
        if (i == 0U)
        {
            break;
        }
    }
    
    return result;
}

int main(void)
{
    status_t result = kStatus_Success;

    BOARD_InitHardware();

    result = APP_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = APP_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    APP_SWT_MacLearning_Forwarding();

    while (1)
    {
    }
}
