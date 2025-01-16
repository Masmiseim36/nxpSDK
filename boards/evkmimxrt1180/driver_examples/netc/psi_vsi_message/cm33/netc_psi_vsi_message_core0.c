/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_msgintr.h"
#include "board.h"
#include "app_core0.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_TX_INTR_MSG_DATA      0U
#define EXAMPLE_RX_INTR_MSG_DATA      1U
#define EXAMPLE_SI_COM_INTR_MSG_DATA  2U
#define EXAMPLE_TX_MSIX_ENTRY_IDX     0U
#define EXAMPLE_RX_MSIX_ENTRY_IDX     1U
#define EXAMPLE_SI_COM_MSIX_ENTRY_IDX 2U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* EP resource. */
static ep_handle_t g_ep_handle;
static swt_transfer_config_t swtTxRxConfig;

/* SWT resource. */
static swt_handle_t g_swt_handle;
static swt_config_t g_swt_config;

/* Buffer descriptor and buffer memeory. */
typedef uint8_t rx_buffer_t[EXAMPLE_EP_RXBUFF_SIZE_ALIGN];

AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EXAMPLE_EP_TEST_FRAME_SIZE], EXAMPLE_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t psiMsgBuff[32], 64);
AT_NONCACHEABLE_SECTION(static uint8_t g_rxFrame[EXAMPLE_EP_RXBUFF_SIZE_ALIGN]);
uint64_t rxBuffAddrArray[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM];

static netc_tx_frame_info_t g_mgmtTxDirty[EXAMPLE_EP_TXBD_NUM];
static netc_tx_frame_info_t mgmtTxFrameInfo;
static volatile bool psiMsgRxReady;
static volatile bool txOver;

/* MAC address. */
uint8_t g_macAddr[6] = {0x54, 0x27, 0x8d, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void APP_BuildBroadCastFrame(void)
{
    uint32_t count  = 0;
    uint32_t length = EXAMPLE_EP_TEST_FRAME_SIZE - 14U;

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
    /* PSI Rx interrupt */
    if ((pendingIntr & (1U << EXAMPLE_SI_COM_INTR_MSG_DATA)) != 0U)
    {
        EP_PsiClearStatus(&g_ep_handle, kNETC_PsiRxMsgFromVsi1Flag);
        psiMsgRxReady = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    status_t result                  = kStatus_Success;
    netc_rx_bdr_config_t rxBdrConfig = {0};
    netc_tx_bdr_config_t txBdrConfig = {0};
    netc_bdr_config_t bdrConfig      = {.rxBdrConfig = &rxBdrConfig, .txBdrConfig = &txBdrConfig};
    netc_buffer_struct_t txBuff      = {.buffer = &g_txFrame, .length = sizeof(g_txFrame)};
    netc_frame_struct_t txFrame      = {.buffArray = &txBuff, .length = 1};
    uint16_t msg                     = 0;
    bool link                        = false;
    netc_msix_entry_t msixEntry[3];
    netc_psi_rx_msg_t msgInfo;
    ep_config_t ep_config;
    uint32_t msgAddr;
    uint32_t length;

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Print the initial banner from Primary core */
    (void)PRINTF("\r\nNETC PSI VSI transfer from the Primary Core!\r\n\n");

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* This section ensures the secondary core image is copied from flash location to the target RAM memory.
       It consists of several steps: image size calculation, image copying and cache invalidation (optional for some
       platforms/cases). These steps are not required on MCUXpresso IDE which copies the secondary core image to the
       target memory during startup automatically. */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
    /* Invalidate cache for memory range the secondary core image has been copied to. */
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        L1CACHE_CleanInvalidateSystemCacheByRange((uint32_t)CORE1_BOOT_ADDRESS, core1_image_size);
    }
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY*/
#endif /* CORE1_IMAGE_COPY_TO_RAM */

    PRINTF("\r\nNETC Switch frame loopback example start.\r\n");

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
        for (uint8_t index = 0U; index < EXAMPLE_EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(MSGINTR1, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(MSGINTR1, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = EXAMPLE_TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = EXAMPLE_RX_INTR_MSG_DATA;
    msixEntry[2].control = kNETC_MsixIntrMaskBit;
    msixEntry[2].msgAddr = msgAddr;
    msixEntry[2].msgData = EXAMPLE_SI_COM_INTR_MSG_DATA;

    /* BD ring configuration. */
    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EXAMPLE_EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EXAMPLE_EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = EXAMPLE_RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    (void)EP_GetDefaultConfig(&ep_config);
    ep_config.si                 = kNETC_ENETC1PSI0;
    ep_config.siConfig.txRingUse = 0;
    ep_config.siConfig.rxRingUse = 1;
    ep_config.siComEntryIdx      = EXAMPLE_SI_COM_MSIX_ENTRY_IDX;
    ep_config.msixEntry          = &msixEntry[0];
    ep_config.entryNum           = 3;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    ep_config.rxCacheMaintain = true;
    ep_config.txCacheMaintain = true;
#endif
    result = EP_Init(&g_ep_handle, &g_macAddr[0], &ep_config, &bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    SWT_GetDefaultConfig(&g_swt_config);
    g_swt_config.ports[1].ethMac.miiMode   = kNETC_RgmiiMode;
    g_swt_config.ports[1].ethMac.miiSpeed  = kNETC_MiiSpeed100M;
    g_swt_config.ports[1].ethMac.miiDuplex = kNETC_MiiFullDuplex;
    /* Only set default forword in switch port1 and internel pseudo MAC port. */
    g_swt_config.bridgeCfg.dVFCfg.portMembership = 0x12U;
    g_swt_config.bridgeCfg.dVFCfg.mlo = kNETC_DisableMACLearn;

    result = SWT_Init(&g_swt_handle, &g_swt_config);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Config switch transfer resource. */
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
        return result;
    }

    APP_BuildBroadCastFrame();

    PRINTF("Wait for PHY link up...\r\n");
    /* Wait link up */
    do
    {
        APP_PHY_GetLinkStatus(&link);
    } while (!link);

    /* Unmask MSIX message interrupt. */
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_TX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_RX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_SI_COM_MSIX_ENTRY_IDX, false);

    EP_PsiClearStatus(&g_ep_handle, kNETC_PsiRxMsgFromVsi1Flag);
    EP_PsiEnableInterrupt(&g_ep_handle, kNETC_PsiRxMsgFromVsi1Flag, true);

    /* Boot Secondary core application */
    (void)PRINTF("Starting Secondary core.\r\n");
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 2, kMCMGR_Start_Synchronous);
    (void)PRINTF("The secondary core application has been started.\r\n");

    result = EP_PsiSetRxBuffer(&g_ep_handle, kNETC_Vsi1, (uintptr_t)&psiMsgBuff[0]);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Wait for the VSI to start. */
    do
    {
        result = EP_PsiGetRxMsg(&g_ep_handle, kNETC_Vsi1, &msgInfo);
    } while (result != kStatus_Success);
    if ((msgInfo.msgBuff[0] == 0U) && (msgInfo.msgBuff[1] == 1U))
    {
        PRINTF("VSI EP initialization succeeded.\r\n");
    }

    while (1)
    {
        do
        {
            result = EP_PsiSendMsg(&g_ep_handle, msg, kNETC_Vsi1);
        } while (result != kStatus_Success);
        PRINTF(" PSI sends message %u to VSI\r\n", msg);
        while (EP_PsiCheckTxBusy(&g_ep_handle, kNETC_Vsi1))
        {
        }
        msg = (msg + 1U) % 10U;

        while (!psiMsgRxReady)
        {
        }
        do
        {
            result = EP_PsiGetRxMsg(&g_ep_handle, kNETC_Vsi1, &msgInfo);
        } while (result != kStatus_Success);
        if ((msgInfo.msgBuff[0] == 0U) && (msgInfo.msgBuff[1] == 0U))
        {
            memcpy(&g_macAddr[0], &msgInfo.msgBuff[4], 6);
            APP_BuildBroadCastFrame();
            assert(EP_SetPrimaryMacAddr(&g_ep_handle, &g_macAddr[0]) == kStatus_Success);
            PRINTF(" PSI receives message from VSI to set PSI MAC address %x:%x:%x:%x:%x:%x.\r\n", g_macAddr[0],
                   g_macAddr[1], g_macAddr[2], g_macAddr[3], g_macAddr[4], g_macAddr[5]);
        }
        psiMsgRxReady = false;

        txOver                  = false;
        swt_mgmt_tx_arg_t txArg = {.ring = 0};
        result                  = SWT_SendFrame(&g_swt_handle, txArg, kNETC_SWITCH0Port1, false, &txFrame, NULL, NULL);
        if (result != kStatus_Success)
        {
            PRINTF("\r\nTransmit frame failed!\r\n");
        }
        while (!txOver)
        {
        }
        SWT_ReclaimTxDescriptor(&g_swt_handle, false, 0);
        if (mgmtTxFrameInfo.status != kNETC_EPTxSuccess)
        {
            PRINTF("\r\nTransmit frame has error!\r\n");
        }

        do
        {
            result = EP_GetRxFrameSize(&g_ep_handle, 0, &length);
            if ((result != kStatus_NETC_RxFrameEmpty) && (result != kStatus_Success))
            {
                result = EP_ReceiveFrameCopy(&g_ep_handle, 0, NULL, 1, NULL);
                assert(result == kStatus_Success);
            }
            else if (length != 0U)
            {
                result = EP_ReceiveFrameCopy(&g_ep_handle, 0, g_rxFrame, length, NULL);
                assert(result == kStatus_Success);
                if ((g_rxFrame[9] == 0xAAU) && (g_rxFrame[10] == 0xBBU) && (g_rxFrame[11] == 0xCCU))
                {
                    PRINTF(" A frame received from VSI.");
                }
                else
                {
                    PRINTF(" A frame received from PSI.");
                }
                PRINTF(" The length is %d.", length);
                PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x.\r\n",
                       g_rxFrame[0], g_rxFrame[1], g_rxFrame[2], g_rxFrame[3], g_rxFrame[4], g_rxFrame[5], g_rxFrame[6],
                       g_rxFrame[7], g_rxFrame[8], g_rxFrame[9], g_rxFrame[10], g_rxFrame[11]);
            }
            else
            {
                break;
            }
        } while (1);

        SDK_DelayAtLeastUs(2000000, CLOCK_GetFreq(kCLOCK_CpuClk));
    }
}
