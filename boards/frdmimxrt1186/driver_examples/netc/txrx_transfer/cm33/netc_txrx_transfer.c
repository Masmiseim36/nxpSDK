/*
 * Copyright 2021-2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_EP_BD_ALIGN       128U
#define EXAMPLE_TX_INTR_MSG_DATA  1U
#define EXAMPLE_RX_INTR_MSG_DATA  2U
#define EXAMPLE_TX_MSIX_ENTRY_IDX 0U
#define EXAMPLE_RX_MSIX_ENTRY_IDX 1U
#define EXAMPLE_FRAME_FID         1U

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (500000U)
#endif

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
/* ENETC pseudo port for management */
#ifndef EXAMPLE_SWT_SI
#define EXAMPLE_SWT_SI kNETC_ENETC1PSI0
#endif
/* Switch pseudo port */
#ifndef EXAMPLE_SWT_PSEUDO_PORT
#define EXAMPLE_SWT_PSEUDO_PORT 0x4U
#endif
#endif
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
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
static netc_hw_si_idx_t g_siIndex[EXAMPLE_EP_NUM] = EXAMPLE_EP_SI;
#endif

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
/* SWT resource. */
static swt_handle_t g_swt_handle;
static swt_config_t g_swt_config;
static swt_transfer_config_t swtTxRxConfig;
#endif

/* Buffer descriptor resource. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EXAMPLE_EP_TEST_FRAME_SIZE], EXAMPLE_EP_BUFF_SIZE_ALIGN);
#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
#endif
AT_NONCACHEABLE_SECTION(static uint8_t g_rxFrame[EXAMPLE_EP_RXBUFF_SIZE_ALIGN]);
static uint64_t rxBuffAddrArray[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM];
#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
static netc_tx_frame_info_t g_mgmtTxDirty[EXAMPLE_EP_TXBD_NUM];
static netc_tx_frame_info_t mgmtTxFrameInfo;
#endif
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_TXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
static netc_tx_frame_info_t g_txDirty[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_TXBD_NUM];
#endif
static netc_tx_frame_info_t txFrameInfo;
static uint8_t txFrameNum, rxFrameNum;
static volatile bool txOver;

/* MAC address. */
static uint8_t g_macAddr[6] = {0x54, 0x27, 0x8d, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief Build Frame for single ring transmit. */
static void APP_BuildBroadCastFrame(void)
{
    uint32_t length = EXAMPLE_EP_TEST_FRAME_SIZE - 14U;
    uint32_t count;

    for (count = 0; count < 6U; count++)
    {
        g_txFrame[count] = 0xFFU;
    }
    memcpy(&g_txFrame[6], &g_macAddr[0], 6U);
    g_txFrame[12] = (length >> 8U) & 0xFFU;
    g_txFrame[13] = length & 0xFFU;

    for (count = 0; count < length; count++)
    {
        g_txFrame[count + 14U] = count % 0xFFU;
    }
}

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
/*! @brief Build Frame for single ring transmit. */
static void APP_BuildBroadCastFrameSwtTag(uint8_t port)
{
    netc_swt_tag_port_no_ts_t tag = {.comTag = {.tpid    = NETC_SWITCH_DEFAULT_ETHER_TYPE,
                                                .subType = kNETC_TagToPortNoTs,
                                                .type    = kNETC_TagToPort,
                                                .qv      = 1,
                                                .ipv     = 0,
                                                .dr      = 0,
                                                .swtId   = 1,
                                                .port    = port}};
    uint32_t headerSize           = 14U + sizeof(tag);
    uint32_t length               = EXAMPLE_EP_TEST_FRAME_SIZE - headerSize;
    uint32_t count;

    for (count = 0; count < 6U; count++)
    {
        g_txFrame[count] = 0xFFU;
    }
    memcpy(&g_txFrame[6], &g_macAddr[0], 6U);
    memcpy(&g_txFrame[12], &tag, sizeof(tag));

    g_txFrame[12 + sizeof(tag)] = (length >> 8U) & 0xFFU;
    g_txFrame[13 + sizeof(tag)] = length & 0xFFU;

    for (count = 0; count < length; count++)
    {
        g_txFrame[count + headerSize] = count % 0xFFU;
    }
}
#endif

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
static status_t APP_SWT_AddTableEntry()
{
    status_t result = kStatus_Success;
    uint32_t entryID;

#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
    netc_tb_ipf_config_t ipfEntryCfg;

    memset(&ipfEntryCfg, 0U, sizeof(netc_tb_ipf_config_t));
    ipfEntryCfg.keye.srcPortMask = EXAMPLE_SWT_USED_PORT_BITMAP;
    ipfEntryCfg.cfge.hr          = kNETC_SoftwareDefHR0;
    ipfEntryCfg.cfge.fltfa       = kNETC_IPFRedirectToMgmtPort;

    /* Frame from used port redirect to management port. */
    for (uint32_t i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        /* Only check the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            continue;
        }

        ipfEntryCfg.keye.srcPort = i;
        result                   = SWT_RxIPFAddTableEntry(&g_swt_handle, &ipfEntryCfg, &entryID);
        if ((kStatus_Success != result) && (entryID != 0xFFFFFFFF))
        {
            PRINTF("\r\n%s: %d, Failed to add IPF table!, result = %u\r\n, entryID = %u", __func__, __LINE__, result,
                   entryID);
            return kStatus_Fail;
        }
    }
#else
    /* Set FDB table, input frame only forwards to pseudo MAC port. */
    netc_tb_fdb_config_t fdbEntryCfg = {
        .keye.fid = EXAMPLE_FRAME_FID, .cfge.portBitmap = (1U << EXAMPLE_SWT_PSEUDO_PORT), .cfge.dynamic = 1};
    memset(&fdbEntryCfg.keye.macAddr[0], 0xFF, 6U);
    result = SWT_BridgeAddFDBTableEntry(&g_swt_handle, &fdbEntryCfg, &entryID);
    if ((kStatus_Success != result) || (0xFFFFFFFFU == entryID))
    {
        PRINTF("\r\n%s: %d, Failed to add FDB table!, result = %d, entryID = %d\r\n", __func__, __LINE__, result,
               entryID);
        return kStatus_Fail;
    }
#endif

    return result;
}
#endif

static status_t APP_ReclaimCallback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
{
    (void)txFrameInfo;

    txFrameInfo = *frameInfo;
    return kStatus_Success;
}

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
static status_t APP_SwtReclaimCallback(swt_handle_t *handle, netc_tx_frame_info_t *frameInfo, void *userData)
{
    mgmtTxFrameInfo = *frameInfo;
    return kStatus_Success;
}
#endif

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
}

#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
status_t APP_EP_XferLoopBack(uint32_t index)
{
    status_t result                  = kStatus_Success;
    netc_rx_bdr_config_t rxBdrConfig = {0};
    netc_tx_bdr_config_t txBdrConfig = {0};
    netc_bdr_config_t bdrConfig      = {.rxBdrConfig = &rxBdrConfig, .txBdrConfig = &txBdrConfig};
    netc_buffer_struct_t txBuff      = {.buffer = &g_txFrame, .length = sizeof(g_txFrame)};
    netc_frame_struct_t txFrame      = {.buffArray = &txBuff, .length = 1};
    bool link                        = false;
    netc_msix_entry_t msixEntry[2];
    netc_hw_mii_mode_t phyMode;
    netc_hw_mii_speed_t phySpeed;
    netc_hw_mii_duplex_t phyDuplex;
    ep_config_t ep_config;
    uint32_t msgAddr;
    uint32_t length;

    PRINTF("\r\nNETC EP%d frame loopback example start.\r\n", index);

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(EXAMPLE_MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(EXAMPLE_MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = EXAMPLE_TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = EXAMPLE_RX_INTR_MSG_DATA;

    /* BD ring configuration. */
    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EXAMPLE_EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EXAMPLE_EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = EXAMPLE_RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    bdrConfig.txBdrConfig[0].bdArray      = &g_txBuffDescrip[0][0];
    bdrConfig.txBdrConfig[0].len          = EXAMPLE_EP_TXBD_NUM;
    bdrConfig.txBdrConfig[0].dirtyArray   = &g_txDirty[0][0];
    bdrConfig.txBdrConfig[0].msixEntryIdx = EXAMPLE_TX_MSIX_ENTRY_IDX;
    bdrConfig.txBdrConfig[0].enIntr       = true;

    /* Wait PHY link up. */
    PRINTF("Wait for PHY link up...\r\n");
    do
    {
        result = APP_PHY_GetLinkStatus(index, &link);
    } while ((result != kStatus_Success) || (!link));
    result = APP_PHY_GetLinkModeSpeedDuplex(index, &phyMode, &phySpeed, &phyDuplex);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Wait a moment for PHY status to be stable. */
    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Endpoint configuration. */
    (void)EP_GetDefaultConfig(&ep_config);
    ep_config.si                    = g_siIndex[index];
    ep_config.siConfig.txRingUse    = 1;
    ep_config.siConfig.rxRingUse    = 1;
    ep_config.reclaimCallback       = APP_ReclaimCallback;
    ep_config.msixEntry             = &msixEntry[0];
    ep_config.entryNum              = 2;
    ep_config.port.ethMac.miiMode   = phyMode;
    ep_config.port.ethMac.miiSpeed  = phySpeed;
    ep_config.port.ethMac.miiDuplex = phyDuplex;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    ep_config.rxCacheMaintain = true;
    ep_config.txCacheMaintain = true;
#endif
#if (defined(FSL_FEATURE_NETC_HAS_ERRATA_052167) && FSL_FEATURE_NETC_HAS_ERRATA_052167)
    /* ERR052167: Actual MAC Tx IPG is longer than configured when transmitting back-to-back packets in MII half duplex
    mode by approximately 15 extra bytes. For example, when configured for IPG=12, the actual IPG will be
    approximately 27. The net result is that maximum throughput will be reduced also in the absence of half-duplex
    collision/retry events. When using MII protocol, using full-duplex mode is recommended instead of half-duplex. If
    using MII half-duplex mode, additional bandwidth loss should be expected and accounted for due to extended IPG. */
    assert(!((phyMode == kNETC_MiiMode) && (phyDuplex == kNETC_MiiHalfDuplex)));
#endif
    result = EP_Init(&g_ep_handle, &g_macAddr[0], &ep_config, &bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Unmask MSIX message interrupt. */
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_TX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_RX_MSIX_ENTRY_IDX, false);

    APP_BuildBroadCastFrame();
    txFrameNum = 0;
    rxFrameNum = 0;

    while (txFrameNum < EXAMPLE_EP_TXFRAME_NUM)
    {
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
        txFrameNum++;
        PRINTF("The %d frame transmitted success!\r\n", txFrameNum);

        do
        {
            result = EP_GetRxFrameSize(&g_ep_handle, 0, &length);
        } while (result == kStatus_NETC_RxFrameEmpty);
        if (result != kStatus_Success)
        {
            return result;
        }

        result = EP_ReceiveFrameCopy(&g_ep_handle, 0, g_rxFrame, length, NULL);
        if (result != kStatus_Success)
        {
            return result;
        }
        rxFrameNum++;
        PRINTF(" A frame received. The length is %d ", length);
        PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
               g_rxFrame[0], g_rxFrame[1], g_rxFrame[2], g_rxFrame[3], g_rxFrame[4], g_rxFrame[5], g_rxFrame[6],
               g_rxFrame[7], g_rxFrame[8], g_rxFrame[9], g_rxFrame[10], g_rxFrame[11]);

        if (memcmp(g_rxFrame, g_txFrame, sizeof(g_txFrame)))
        {
            PRINTF("\r\nTx/Rx frames don't match!\r\n");
            return kStatus_Fail;
        }
    }

    EP_Deinit(&g_ep_handle);

    return result;
}
#endif

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
status_t APP_SWT_XferLoopBack(void)
{
    status_t result                  = kStatus_Success;
    netc_rx_bdr_config_t rxBdrConfig = {0};
    netc_tx_bdr_config_t txBdrConfig = {0};
    netc_bdr_config_t bdrConfig      = {.rxBdrConfig = &rxBdrConfig, .txBdrConfig = &txBdrConfig};
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    uint32_t dataOffset = 12U + sizeof(netc_swt_tag_port_no_ts_t);
#else
    swt_mgmt_tx_arg_t txArg = {0};
#endif
    netc_buffer_struct_t txBuff = {.buffer = &g_txFrame, .length = sizeof(g_txFrame)};
    netc_frame_struct_t txFrame = {.buffArray = &txBuff, .length = 1};
    bool link                   = false;
    netc_msix_entry_t msixEntry[2];
    netc_hw_mii_mode_t phyMode;
    netc_hw_mii_speed_t phySpeed;
    netc_hw_mii_duplex_t phyDuplex;
    ep_config_t g_ep_config;
    uint32_t msgAddr;
    uint32_t length;
    uint32_t i;

    PRINTF("\r\nNETC Switch frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(EXAMPLE_MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(EXAMPLE_MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = EXAMPLE_TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = EXAMPLE_RX_INTR_MSG_DATA;

    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EXAMPLE_EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EXAMPLE_EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = EXAMPLE_RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    (void)EP_GetDefaultConfig(&g_ep_config);
    g_ep_config.si                 = EXAMPLE_SWT_SI;
    g_ep_config.siConfig.txRingUse = 1;
    g_ep_config.siConfig.rxRingUse = 1;
    g_ep_config.reclaimCallback    = APP_ReclaimCallback;
    g_ep_config.msixEntry          = &msixEntry[0];
    g_ep_config.entryNum           = 2;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    g_ep_config.rxCacheMaintain = true;
    g_ep_config.txCacheMaintain = true;
#endif
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
            PRINTF("\r\n%s: %d, Failed to get link status(mode, speed, dumplex)!\r\n", __func__, __LINE__);
            return result;
        }

        g_swt_config.ports[i].ethMac.miiMode   = phyMode;
        g_swt_config.ports[i].ethMac.miiSpeed  = phySpeed;
        g_swt_config.ports[i].ethMac.miiDuplex = phyDuplex;
    }

    /* Wait a moment for PHY status to be stable. */
    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    g_swt_config.bridgeCfg.dVFCfg.portMembership = (1U << EXAMPLE_SWT_PSEUDO_PORT) | EXAMPLE_SWT_USED_PORT_BITMAP;
#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
    g_swt_config.ports[0].commonCfg.ipfCfg.enIPFTable = true;
    g_swt_config.ports[2].commonCfg.ipfCfg.enIPFTable = true;
#else
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID      = EXAMPLE_FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo           = kNETC_FDBLookUpWithDiscard;
    g_swt_config.bridgeCfg.dVFCfg.mlo           = kNETC_DisableMACLearn;
#endif

    g_swt_config.cmdRingUse            = 1U;
    g_swt_config.cmdBdrCfg[0].bdBase   = &g_cmdBuffDescrip[0];
    g_swt_config.cmdBdrCfg[0].bdLength = 8U;

    result = SWT_Init(&g_swt_handle, &g_swt_config);
    if (result != kStatus_Success)
    {
        PRINTF("\r\n%s: %d, Failed to initialize switch!\r\n", __func__, __LINE__);
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
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    swtTxRxConfig.rxCacheMaintain = true;
    swtTxRxConfig.txCacheMaintain = true;
#endif
    result = SWT_ManagementTxRxConfig(&g_swt_handle, &g_ep_handle, &swtTxRxConfig);
    if (kStatus_Success != result)
    {
        PRINTF("\r\n%s: %d, Failed to config TxRx!\r\n", __func__, __LINE__);
        return result;
    }

    /* Unmask MSIX message interrupt. */
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_TX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_RX_MSIX_ENTRY_IDX, false);

#if !(defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG)
    APP_BuildBroadCastFrame();
#endif
    rxFrameNum = 0;
    txFrameNum = 0;

    result = APP_SWT_AddTableEntry();
    if (kStatus_Success != result)
    {
        return kStatus_Fail;
    }

    i = 0;
    while (txFrameNum < EXAMPLE_EP_TXFRAME_NUM)
    {
        /* Only use the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            i++;
            i = ((i >= EXAMPLE_SWT_MAX_PORT_NUM) ? 0 : i);
            continue;
        }

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
        APP_BuildBroadCastFrameSwtTag(i);
#endif

        txOver = false;
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
        result = SWT_SendFrame(&g_swt_handle, &txFrame, NULL, NULL);
#else
        txArg.ring = 0;
        result     = SWT_SendFrame(&g_swt_handle, txArg, (netc_hw_port_idx_t)(kNETC_SWITCH0Port0 + i), false, &txFrame,
                                   NULL, NULL);
#endif
        if (result != kStatus_Success)
        {
            PRINTF("\r\nTransmit frame failed!\r\n");
            return result;
        }
        while (!txOver)
        {
        }
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
        SWT_ReclaimTxDescriptor(&g_swt_handle, 0);
#else
        SWT_ReclaimTxDescriptor(&g_swt_handle, false, 0);
#endif
        if (mgmtTxFrameInfo.status != kNETC_EPTxSuccess)
        {
            PRINTF("\r\nTransmit frame has error!\r\n");
            return kStatus_Fail;
        }
        txFrameNum++;
        PRINTF("The %u frame transmitted success on port %u!\r\n", txFrameNum, (uint8_t)(kNETC_SWITCH0Port0 + i));

        do
        {
#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
            result = SWT_GetRxFrameSize(&g_swt_handle, &length);
#else
            result = EP_GetRxFrameSize(&g_ep_handle, 0, &length);
#endif
        } while (result == kStatus_NETC_RxFrameEmpty);
        if (result != kStatus_Success)
        {
            return result;
        }

#if defined(EXAMPLE_SWT_USE_IPF) && EXAMPLE_SWT_USE_IPF
        result = SWT_ReceiveFrameCopy(&g_swt_handle, g_rxFrame, length, NULL);
#else
        result = EP_ReceiveFrameCopy(&g_ep_handle, 0, g_rxFrame, length, NULL);
#endif
        if (result != kStatus_Success)
        {
            return result;
        }

        rxFrameNum++;
        PRINTF(" A frame received. The length is %d ", length);
        PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
               g_rxFrame[0], g_rxFrame[1], g_rxFrame[2], g_rxFrame[3], g_rxFrame[4], g_rxFrame[5], g_rxFrame[6],
               g_rxFrame[7], g_rxFrame[8], g_rxFrame[9], g_rxFrame[10], g_rxFrame[11]);

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
        if (memcmp(&g_rxFrame[dataOffset], &g_txFrame[dataOffset], sizeof(g_txFrame) - dataOffset))
#else
        if (memcmp(g_rxFrame, g_txFrame, sizeof(g_txFrame)))
#endif
        {
            PRINTF("\r\nTx/Rx frames don't match!\r\n");
            return kStatus_Fail;
        }

        i++;
        i = ((i >= EXAMPLE_SWT_MAX_PORT_NUM) ? 0 : i);
    }

    return result;
}
#endif

int main(void)
{
    status_t result = kStatus_Success;
    uint32_t index;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

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

    for (uint8_t ring = 0U; ring < EXAMPLE_EP_RING_NUM; ring++)
    {
        for (index = 0U; index < EXAMPLE_EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }

#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
    for (index = 0; index < EXAMPLE_EP_NUM; index++)
    {
        APP_EP_XferLoopBack(index);
    }
#endif
#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
    APP_SWT_XferLoopBack();
#endif

    while (1)
    {
    }
}
