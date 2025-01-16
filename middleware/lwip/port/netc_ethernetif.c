/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/ethip6.h"
#include "lwip/igmp.h"
#include "lwip/mem.h"
#include "lwip/mld6.h"
#include "lwip/pbuf.h"
#include "lwip/snmp.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/debug.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"

#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

#include "ethernetif.h"
#include "ethernetif_priv.h"

#if defined(NETC_PORT_USE_INCLUDES) && (NETC_PORT_USE_INCLUDES != 0)
#include "lwip_netc_port.h"
#endif

// #include "fsl_netc.h"
#include "fsl_phy.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#if defined(NETC_VSI_NUM_USED) && (NETC_VSI_NUM_USED > 0)
#include "fsl_netc_msg.h"
#endif
#include "fsl_msgintr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define NETC_TIMEOUT (0xFFFU)

#ifndef NETC_FRAME_MAX_FRAMELEN
#define NETC_FRAME_MAX_FRAMELEN (1518U)
#endif
#define NETC_FCS_LEN (4U)

/* The length of RX buffer. */
#ifndef NETC_RXBUFF_SIZE
#define NETC_RXBUFF_SIZE (NETC_FRAME_MAX_FRAMELEN)
#endif

#define NETC_BUFF_ALIGNMENT (64U)
#define NETC_BD_ALIGNMENT   (128U)

/* The number of NETC buffers needed to receive frame of maximum length. */
#define MAX_BUFFERS_PER_FRAME                                                                \
                                                                                             \
    ((NETC_FRAME_MAX_FRAMELEN / CONSTANT_SIZEALIGN(NETC_RXBUFF_SIZE, NETC_BUFF_ALIGNMENT)) + \
     ((NETC_FRAME_MAX_FRAMELEN % CONSTANT_SIZEALIGN(NETC_RXBUFF_SIZE, NETC_BUFF_ALIGNMENT) == 0) ? 0 : 1))

/* The length of TX buffer. */
#ifndef NETC_TXBUFF_SIZE
#define NETC_TXBUFF_SIZE (NETC_FRAME_MAX_FRAMELEN - NETC_FCS_LEN)
#endif

/* The number of buffer descriptors in NETC RX ring. */
#ifndef NETC_RXBD_NUM
#define NETC_RXBD_NUM (8)
#endif

/* Ring should be able to receive at least 1 frame with maximum length. */
#if NETC_RXBD_NUM < MAX_BUFFERS_PER_FRAME
#error "NETC_RXBD_NUM < MAX_BUFFERS_PER_FRAME"
#endif

/* The number of RX buffers. NETC_RXBD_NUM is always held by NETC driver,
 * so a couple more are needed to pass zero-copy data into lwIP. */
#ifndef NETC_RXBUFF_NUM
#define NETC_RXBUFF_NUM (NETC_RXBD_NUM + 5)
#endif

/* At least NETC_RXBD_NUM number of buffers is always held by NETC driver for RX.
 * Some additional buffers are needed to pass at least one frame zero-copy data to lwIP. */
#if NETC_RXBUFF_NUM < (NETC_RXBD_NUM + MAX_BUFFERS_PER_FRAME)
#error "NETC_RXBUFF_NUM < (NETC_RXBD_NUM + MAX_BUFFERS_PER_FRAME)"
#endif

/* The number of buffer descriptors in NETC TX ring. */
#ifndef NETC_TXBD_NUM
#define NETC_TXBD_NUM (8)
#endif

/* The number of buffer descriptors in the NETC command BD ring. */
#ifndef NETC_CMDBD_NUM
#define NETC_CMDBD_NUM (8)
#endif

/* The number of TX buffers. For now, we are just copying into a single buffer
 * and wait until it is sent out before sending other. */
#define NETC_TXBUFF_NUM (1)

/* Promiscuous mode - receive everything. Disables hardware-assisted MAC address filtering. */
#ifndef NETC_PROMISCUOUS
#define NETC_PROMISCUOUS (0)
#endif

#if defined(FSL_ETH_ENABLE_CACHE_CONTROL)
#if defined(FSL_FEATURE_L2CACHE_LINESIZE_BYTE) && \
    ((!defined(FSL_SDK_DISBLE_L2CACHE_PRESENT)) || (FSL_SDK_DISBLE_L2CACHE_PRESENT == 0))
#if defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define FSL_CACHE_LINESIZE_MAX  MAX(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE, FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
#define FSL_NETC_BUFF_ALIGNMENT MAX(NETC_BUFF_ALIGNMENT, FSL_CACHE_LINESIZE_MAX)
#else
#define FSL_NETC_BUFF_ALIGNMENT MAX(NETC_BUFF_ALIGNMENT, FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
#endif
#elif defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define FSL_NETC_BUFF_ALIGNMENT MAX(NETC_BUFF_ALIGNMENT, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#else
#define FSL_NETC_BUFF_ALIGNMENT NETC_BUFF_ALIGNMENT
#endif
#else
#define FSL_NETC_BUFF_ALIGNMENT NETC_BUFF_ALIGNMENT
#endif

#define TX_MSIX_ENTRY_IDX     0U
#define RX_MSIX_ENTRY_IDX     1U
#define SI_COM_MSIX_ENTRY_IDX 2U
#define TX_INTR_MSG_DATA      1U
#define RX_INTR_MSG_DATA      2U
#define SI_COM_INTR_MSG_DATA  3U

#define TX_RINGS 1U
#define RX_RINGS 1U

#ifndef NETC_MSGINTR
#define NETC_MSGINTR MSGINTR1
#endif

#ifndef NETC_MSGINTR_PRIORITY
#define NETC_MSGINTR_PRIORITY 6U
#endif

#ifndef NETC_MSGINTR_IRQ
#define NETC_MSGINTR_IRQ MSGINTR1_IRQn
#endif

#ifndef NETC_PSI
#define NETC_PSI kNETC_ENETC0PSI0
#endif

#ifndef NETC_VSI_NUM_USED
#define NETC_VSI_NUM_USED 0
#endif

#if (NETC_VSI_NUM_USED > 0) && (NO_SYS != 0)
#error "NETC VSI requires RTOS support"
#endif

#ifndef NETC_MII_MODE
#define NETC_MII_MODE kNETC_RmiiMode
#endif

#ifndef NETC_MII_SPEED
#define NETC_MII_SPEED kNETC_MiiSpeed100M
#endif

#ifndef SI_MSG_THREAD_STACKSIZE
#define SI_MSG_THREAD_STACKSIZE 1500
#endif

#ifndef SI_MSG_THREAD_PRIO
#define SI_MSG_THREAD_PRIO 3
#endif

#include "netc_configchecks.h"

typedef uint8_t rx_buffer_t[SDK_SIZEALIGN(NETC_RXBUFF_SIZE, FSL_NETC_BUFF_ALIGNMENT)];
typedef uint8_t tx_buffer_t[SDK_SIZEALIGN(NETC_TXBUFF_SIZE, FSL_NETC_BUFF_ALIGNMENT)];

#if (NETC_VSI_NUM_USED > 0)
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t psiMsgBuff1[1024], 32);
#endif
#if (NETC_VSI_NUM_USED > 1)
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t psiMsgBuff2[1024], 32);
#endif
/*!
 * @brief Used to wrap received data in a pbuf to be passed into lwIP
 *        without copying.
 * Once last reference is released, buffer can be used by NETC RX DMA again.
 */
typedef struct rx_pbuf_wrapper
{
    struct pbuf_custom p;      /*!< Pbuf wrapper. Has to be first. */
    void *buffer;              /*!< Original buffer wrapped by p. */
    volatile bool buffer_used; /*!< Wrapped buffer is used by NETC or lwIP. */
    ep_handle_t *handle;
    uint8_t ring;
} rx_pbuf_wrapper_t;

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 */
struct ethernetif
{
    ep_handle_t *ep_handle;
    ep_config_t *ep_config;
#if (NETC_VSI_NUM_USED > 0)
    netc_msix_entry_t msixEntry[3];
#else
    netc_msix_entry_t msixEntry[2];
#endif
#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
    EventGroupHandle_t transmitAccessEvent;
    EventBits_t txFlag;
#endif
    netc_rx_bdr_config_t rxBdrConfig[RX_RINGS];
    netc_tx_bdr_config_t txBdrConfig[TX_RINGS];
    netc_bdr_config_t bdrConfig;

    netc_cmd_bd_t *cmdBuffDescrip;

    netc_rx_bd_t *rxBuffDescrip;
    netc_tx_bd_t *txBuffDescrip;
    netc_tx_frame_info_t *txDirty;
    netc_tx_frame_info_t txFrameInfo;

    rx_buffer_t *rxDataBuff;
    tx_buffer_t *txDataBuff;

    uint64_t *rxBuffAddrArray;

    rx_pbuf_wrapper_t rxPbufs[NETC_RXBUFF_NUM];

    phy_handle_t *phyHandle;
    phy_speed_t last_speed;
    phy_duplex_t last_duplex;
    bool last_link_up;

    uint32_t intGpioHdl[((HAL_GPIO_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];

    struct netif *netif;
};

static struct ethernetif *s_ethernetif = NULL;

static rx_pbuf_wrapper_t *get_rx_pbufs_from_handle(ep_handle_t *handle, uint8_t ring)
{
    (void)ring;
    void *ep_user_data = handle->cfg.userData;

    return (rx_pbuf_wrapper_t *)ep_user_data;
}

/*! @brief Defines the NETC Rx memory buffer free function pointer. */
static void *rx_buff_alloc(ep_handle_t *handle, uint8_t ring, uint32_t length, void *userData)
{
    (void)userData;

    void *buffer             = NULL;
    rx_pbuf_wrapper_t *p_wrp = get_rx_pbufs_from_handle(handle, ring);
    int i;

    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

    for (i = 0; i < NETC_RXBUFF_NUM; i++)
    {
        if (!p_wrp[i].buffer_used)
        {
            p_wrp[i].buffer_used = true;
            buffer               = p_wrp[i].buffer;
            break;
        }
    }

#if ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS
    if (buffer == NULL)
    {
        /* Mask (disable) RX interrupt */
        EP_MsixSetEntryMask(handle, RX_MSIX_ENTRY_IDX, true);
    }
#else
    (void)handle;
#endif

    SYS_ARCH_UNPROTECT(old_level);

    return buffer;
}

/**
 * Callback for release of RX zero-copy buffer from NETC driver.
 */
static void rx_buf_free(ep_handle_t *handle, uint8_t ring, void *address, void *userData)
{
    (void)userData;

    rx_pbuf_wrapper_t *p_wrp = get_rx_pbufs_from_handle(handle, ring);
    int i;
    bool found = false;

    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

    for (i = 0; i < NETC_RXBUFF_NUM; i++)
    {
        if (p_wrp[i].buffer == address)
        {
            LWIP_ASSERT("Requested free of rx buffer marked as not used.", p_wrp[i].buffer_used);
            p_wrp[i].buffer_used = false;
            found                = true;
            break;
        }
    }

    LWIP_ASSERT("Requested free of unknown rx buffer.", found);

#if ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS
    /* Unmask (enable) RX interrupt */
    EP_MsixSetEntryMask(handle, RX_MSIX_ENTRY_IDX, false);
#else
    (void)handle;
#endif

    SYS_ARCH_UNPROTECT(old_level);
}

static void rx_pbuf_free_from_lwip(struct pbuf *p)
{
    rx_pbuf_wrapper_t *p_wrp = (rx_pbuf_wrapper_t *)p;

    rx_buf_free(p_wrp->handle, p_wrp->ring, p_wrp->buffer, NULL);
}

void msgintrCallback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
    struct ethernetif *ethernetif = s_ethernetif;
#if (NETC_VSI_NUM_USED > 0)
    uint32_t msg_recv_flags = kNETC_PsiRxMsgFromVsi1Flag;
#endif

#if (NETC_VSI_NUM_USED > 1)
    msg_recv_flags |= kNETC_PsiRxMsgFromVsi2Flag;
#endif

    if (NULL == ethernetif)
    {
        return;
    }

    /* Transmit interrupt */
    if ((pendingIntr & (1U << TX_INTR_MSG_DATA)) != 0U)
    {
#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
#ifdef __CA7_REV
        if (SystemGetIRQNestingLevel())
#else
        if (__get_IPSR())
#endif
        {
            portBASE_TYPE taskToWake = pdFALSE;

            BaseType_t xResult =
                xEventGroupSetBitsFromISR(ethernetif->transmitAccessEvent, ethernetif->txFlag, &taskToWake);
            LWIP_ASSERT(
                "xEventGroupSetBitsFromISR failed, increase configTIMER_QUEUE_LENGTH or configTIMER_TASK_PRIORITY",
                pdPASS == xResult);
            if (pdPASS == xResult)
            {
                portYIELD_FROM_ISR(taskToWake);
            }
        }
        else
        {
            xEventGroupSetBits(ethernetif->transmitAccessEvent, ethernetif->txFlag);
        }
#endif

        EP_CleanTxIntrFlags(ethernetif->ep_handle, 1, 0);
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(ethernetif->ep_handle, 1);
#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
        ethernetif_input(ethernetif->netif);
#endif
    }
#if (NETC_VSI_NUM_USED > 0)
    /* PSI Rx interrupt */
    if ((pendingIntr & (1U << SI_COM_INTR_MSG_DATA)) != 0U)
    {
        EP_PsiClearStatus(ethernetif->ep_handle, msg_recv_flags);
    }
#endif
}

static status_t ethernetif_tx_reclaim_callback(ep_handle_t *handle,
                                               uint8_t ring,
                                               netc_tx_frame_info_t *frameInfo,
                                               void *userData)
{
    struct ethernetif *ethernetif = s_ethernetif;
    LWIP_ASSERT("ethernetif is NULL", ethernetif != NULL);

    ethernetif->txFrameInfo = *frameInfo;

    return kStatus_Success;
}

#if (NETC_VSI_NUM_USED > 0)
static void netc_si_msg_thread(void *arg)
{
    struct ethernetif *ethernetif = s_ethernetif;
    status_t result               = kStatus_Success;
    netc_psi_rx_msg_t msgInfo;

    while (1)
    {
        result = EP_PsiRxMsg(ethernetif->ep_handle, kNETC_Vsi1, &msgInfo);
        if (result == kStatus_Success)
        {
            EP_PsiHandleRxMsg(ethernetif->ep_handle, 1, &msgInfo);
        }

#if (NETC_VSI_NUM_USED > 1)
        result = EP_PsiRxMsg(ethernetif->ep_handle, kNETC_Vsi2, &msgInfo);
        if (result == kStatus_Success)
        {
            EP_PsiHandleRxMsg(ethernetif->ep_handle, 2, &msgInfo);
        }
#endif
        sys_msleep(1U);
    }
}
#endif

static status_t ethernetif_getlinkstatus(ep_handle_t *handle, uint8_t *link)
{
    struct ethernetif *eif = s_ethernetif;

    *link = (eif->last_link_up ? 1U : 0U);

    return kStatus_Success;
}

static status_t ethernetif_getlinkspeed(ep_handle_t *handle, netc_hw_mii_speed_t *speed, netc_hw_mii_duplex_t *duplex)
{
    struct ethernetif *eif = s_ethernetif;

    *speed  = (netc_hw_mii_speed_t)eif->last_speed;
    *duplex = (netc_hw_mii_duplex_t)eif->last_duplex;

    return kStatus_Success;
}

/**
 * Initializes NETC driver.
 */
void ethernetif_plat_init(struct netif *netif,
                          struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig)
{
    status_t result = kStatus_Success;
#if (NETC_VSI_NUM_USED > 0)
    uint32_t msg_recv_flags = kNETC_PsiRxMsgFromVsi1Flag;
#endif
    int i;

#if (NETC_VSI_NUM_USED > 1)
    msg_recv_flags |= kNETC_PsiRxMsgFromVsi2Flag;
#endif

    ethernetif->netif = netif;

    /* Used for detection of change.
       Initilize to value different than any possible enum value. */
    ethernetif->last_speed   = (phy_speed_t)0xa5a5;
    ethernetif->last_duplex  = (phy_duplex_t)0x5a5a;
    ethernetif->last_link_up = false;

    ethernetif->bdrConfig.rxBdrConfig = &ethernetif->rxBdrConfig[0];
    ethernetif->bdrConfig.txBdrConfig = &ethernetif->txBdrConfig[0];

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(NETC_MSGINTR, &msgintrCallback);
    uint32_t msgAddr                 = MSGINTR_GetIntrSelectAddr(NETC_MSGINTR, 0);
    ethernetif->msixEntry[0].control = kNETC_MsixIntrMaskBit;
    ethernetif->msixEntry[0].msgAddr = msgAddr;
    ethernetif->msixEntry[0].msgData = TX_INTR_MSG_DATA;
    ethernetif->msixEntry[1].control = kNETC_MsixIntrMaskBit;
    ethernetif->msixEntry[1].msgAddr = msgAddr;
    ethernetif->msixEntry[1].msgData = RX_INTR_MSG_DATA;
#if (NETC_VSI_NUM_USED > 0)
    ethernetif->msixEntry[2].control = kNETC_MsixIntrMaskBit;
    ethernetif->msixEntry[2].msgAddr = msgAddr;
    ethernetif->msixEntry[2].msgData = SI_COM_INTR_MSG_DATA;
#endif
    /* BD ring configuration. */
    ethernetif->bdrConfig.rxBdrConfig[0].msixEntryIdx  = RX_MSIX_ENTRY_IDX;
    ethernetif->bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    ethernetif->bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    ethernetif->bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    ethernetif->bdrConfig.rxBdrConfig[0].intrThreshold = 1;
    ethernetif->bdrConfig.rxBdrConfig[0].bdArray       = &ethernetif->rxBuffDescrip[0];
    ethernetif->bdrConfig.rxBdrConfig[0].len           = NETC_RXBD_NUM;
    ethernetif->bdrConfig.rxBdrConfig[0].buffAddrArray = &ethernetif->rxBuffAddrArray[0];
    ethernetif->bdrConfig.rxBdrConfig[0].buffSize      = sizeof(rx_buffer_t);

    ethernetif->bdrConfig.txBdrConfig[0].msixEntryIdx = TX_MSIX_ENTRY_IDX;
    ethernetif->bdrConfig.txBdrConfig[0].enIntr       = true;
    ethernetif->bdrConfig.txBdrConfig[0].bdArray      = &ethernetif->txBuffDescrip[0];
    ethernetif->bdrConfig.txBdrConfig[0].len          = NETC_TXBD_NUM;
    ethernetif->bdrConfig.txBdrConfig[0].dirtyArray   = &ethernetif->txDirty[0];

    for (i = 0; i < NETC_RXBD_NUM; i++)
    {
        ethernetif->bdrConfig.rxBdrConfig[0].buffAddrArray[i] = (uintptr_t) & (ethernetif->rxDataBuff[i]);
    }

    /* Endpoint configuration. */
    (void)EP_GetDefaultConfig(ethernetif->ep_config);
    ethernetif->ep_config->si                 = NETC_PSI;
    ethernetif->ep_config->siConfig.txRingUse = TX_RINGS;
    ethernetif->ep_config->siConfig.rxRingUse = RX_RINGS;
    ethernetif->ep_config->reclaimCallback    = ethernetif_tx_reclaim_callback;
#if (NETC_VSI_NUM_USED > 0)
    ethernetif->ep_config->siComEntryIdx = SI_COM_MSIX_ENTRY_IDX;
#endif
    ethernetif->ep_config->msixEntry = &ethernetif->msixEntry[0];
#if (NETC_VSI_NUM_USED > 0)
    ethernetif->ep_config->entryNum = 3;
#else
    ethernetif->ep_config->entryNum = 2;
#endif

    for (i = 0; i < NETC_RXBUFF_NUM; i++)
    {
        ethernetif->rxPbufs[i].p.custom_free_function = rx_pbuf_free_from_lwip;
        ethernetif->rxPbufs[i].buffer                 = &(ethernetif->rxDataBuff[i]);
        ethernetif->rxPbufs[i].buffer_used            = false;
        ethernetif->rxPbufs[i].handle                 = ethernetif->ep_handle;
        ethernetif->rxPbufs[i].ring                   = 0;
    }

    ethernetif->ep_config->rxCacheMaintain = true;
    ethernetif->ep_config->txCacheMaintain = true;
    ethernetif->ep_config->rxZeroCopy      = true;
    ethernetif->ep_config->rxBuffAlloc     = rx_buff_alloc;
    ethernetif->ep_config->rxBuffFree      = rx_buf_free;
    ethernetif->ep_config->userData        = &ethernetif->rxPbufs[0];

    ethernetif->ep_config->port.ethMac.miiMode        = NETC_MII_MODE;
    ethernetif->ep_config->port.ethMac.miiSpeed       = NETC_MII_SPEED;
    ethernetif->ep_config->port.ethMac.miiDuplex      = kNETC_MiiFullDuplex;
    ethernetif->ep_config->port.ethMac.rxMaxFrameSize = NETC_FRAME_MAX_FRAMELEN;

#if (CHECKSUM_CHECK_IP == 1)
    ethernetif->ep_config->parserCfg.disL3Checksum = true;
#else
    ethernetif->ep_config->parserCfg.disL3Checksum = false;
#endif
#if (CHECKSUM_CHECK_TCP == 1) && (CHECKSUM_CHECK_UDP == 1)
    ethernetif->ep_config->parserCfg.disL4Checksum = true;
#else
    ethernetif->ep_config->parserCfg.disL4Checksum = false;
#endif

    uint8_t mac_addr[6];
    memcpy(mac_addr, &ethernetifConfig->macAddress, sizeof(mac_addr));

#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
    ethernetif->txFlag              = 1;
    ethernetif->transmitAccessEvent = xEventGroupCreate();
    xEventGroupSetBits(ethernetif->transmitAccessEvent, ethernetif->txFlag);

    NVIC_SetPriority(NETC_MSGINTR_IRQ, NETC_MSGINTR_PRIORITY);
#endif

    ethernetif->ep_config->cmdBdrConfig.bdBase   = ethernetif->cmdBuffDescrip;
    ethernetif->ep_config->cmdBdrConfig.bdLength = NETC_CMDBD_NUM;

    // Allow receive of packets.
    result = EP_Init(ethernetif->ep_handle, mac_addr, ethernetif->ep_config, &ethernetif->bdrConfig);
    LWIP_ASSERT("Netc ep init failed", (result == kStatus_Success));

    ethernetif->ep_handle->getLinkStatus = ethernetif_getlinkstatus;
    ethernetif->ep_handle->getLinkSpeed  = ethernetif_getlinkspeed;

    ethernetif_phy_init(ethernetif, ethernetifConfig);

    netc_si_l2mf_config_t l2mfConfig = {
#if NETC_PROMISCUOUS == 1
        .macUCPromis = true,
        .macMCPromis = true,
#else
        .macUCPromis = false,
        .macMCPromis = false,
#endif
        .rejectUC = false,
        .rejectMC = false,
        .rejectBC = false
    };
    EP_RxL2MFInit(ethernetif->ep_handle, &l2mfConfig);

    /* Unmask MSIX message interrupt. */
    EP_MsixSetEntryMask(ethernetif->ep_handle, TX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(ethernetif->ep_handle, RX_MSIX_ENTRY_IDX, false);
#if (NETC_VSI_NUM_USED > 0)
    EP_MsixSetEntryMask(ethernetif->ep_handle, SI_COM_MSIX_ENTRY_IDX, false);

    EP_PsiClearStatus(ethernetif->ep_handle, msg_recv_flags);
    EP_PsiEnableInterrupt(ethernetif->ep_handle, msg_recv_flags, true);

    EP_PsiSetRxBuffer(ethernetif->ep_handle, kNETC_Vsi1, (uintptr_t)&psiMsgBuff1[0]);
#endif
#if (NETC_VSI_NUM_USED > 1)
    EP_PsiSetRxBuffer(ethernetif->ep_handle, kNETC_Vsi2, (uintptr_t)&psiMsgBuff2[0]);
#endif

#if (NETC_VSI_NUM_USED > 0)
    /* Create VSI-PSI messaging thread */
    sys_thread_new("netc_si_msg_thread", netc_si_msg_thread, NULL, SI_MSG_THREAD_STACKSIZE, SI_MSG_THREAD_PRIO);
#endif
}

/** Wraps received buffer(s) into a pbuf or a pbuf chain and returns it. */
static struct pbuf *ethernetif_rx_frame_to_pbufs(struct ethernetif *ethernetif, netc_frame_struct_t *frame)
{
    struct pbuf *p_root = NULL;
    struct pbuf *p      = NULL;
    int i               = 0;
    int buf_n;

    for (buf_n = 0; buf_n < frame->length; buf_n++)
    {
        netc_buffer_struct_t *bs    = &frame->buffArray[buf_n];
        rx_pbuf_wrapper_t *pw_found = NULL;
        int n                       = 0;

        // seek pbuf
        while ((n < NETC_RXBUFF_NUM) && (pw_found == NULL))
        {
            if (ethernetif->rxPbufs[i].buffer == bs->buffer)
            {
                pw_found = &ethernetif->rxPbufs[i];

                p = pbuf_alloced_custom(PBUF_RAW, bs->length, PBUF_REF, &pw_found->p, bs->buffer, NETC_RXBUFF_SIZE);
                LWIP_ASSERT("pbuf_alloced_custom() failed", p);

                if (p_root == NULL)
                {
                    p_root = p;
                }
                else
                {
                    pbuf_cat(p_root, p);
                }
            }

            n++;
            i++;
            i %= NETC_RXBUFF_NUM; // wrap
        }

        LWIP_ASSERT("Rx buffer not found in pbuf array", pw_found != NULL);
    }

    MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
    if (((u8_t *)p->payload)[0] & 1)
    {
        /* broadcast or multicast packet */
        MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
    }
    else
    {
        /* unicast packet */
        MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
    }

    LINK_STATS_INC(link.recv);

    return p;
}

struct pbuf *ethernetif_linkinput(struct netif *netif)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *p                = NULL;

    netc_buffer_struct_t buffers[MAX_BUFFERS_PER_FRAME];
    netc_frame_struct_t frame = {.length = (MAX_BUFFERS_PER_FRAME), .buffArray = buffers};

    status_t result = EP_ReceiveFrame(ethernetif->ep_handle, 0 /*ring*/, &frame, NULL);

    if (result == kStatus_Success)
    {
        /* Frame read, process it into pbufs. */
        p = ethernetif_rx_frame_to_pbufs(ethernetif, &frame);
    }
    else
    {
        // TODO
    }

    return p;
}

err_t ethernetif_linkoutput(struct netif *netif, struct pbuf *p)
{
    err_t result;
    struct ethernetif *ethernetif = netif->state;
    u16_t uCopied;

    LWIP_ASSERT("Output packet buffer empty", p);

    // TODO Check if we can use more TX DATA buffers without waiting and reclaim TX descriptor(s) asynchronously later.
    netc_buffer_struct_t buff = {.buffer = &ethernetif->txDataBuff[0][0], .length = 0};
    netc_frame_struct_t frame = {.buffArray = &buff, .length = 1};

    /* pbuf chain, copy into single frame buf. */
    if (p->tot_len > (NETC_FRAME_MAX_FRAMELEN - NETC_FCS_LEN))
    {
        return ERR_BUF;
    }
    else
    {
        uCopied = pbuf_copy_partial(p, buff.buffer, p->tot_len, 0);
        LWIP_ASSERT("uCopied != p->tot_len", uCopied == p->tot_len);
    }
    /* Send frame. */

    buff.length = p->tot_len;

    status_t status = EP_SendFrame(ethernetif->ep_handle, 0 /*ring*/, &frame, NULL, NULL);

    if (status == kStatus_Busy)
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_linkoutput: EP_SendFrame busy\n"));
        result = ERR_TIMEOUT;
    }
    else if (status != kStatus_Success)
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_linkoutput: EP_SendFrame failed: 0x%x\n", status));
        result = ERR_IF;
    }
    else
    {
        result = ERR_OK;
#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
        xEventGroupWaitBits(ethernetif->transmitAccessEvent, ethernetif->txFlag, pdTRUE, (BaseType_t) false,
                            portMAX_DELAY);
#else
        EP_WaitUnitilTxComplete(ethernetif->ep_handle, 0);
#endif
        EP_ReclaimTxDescriptor(ethernetif->ep_handle, 0);
        if (ethernetif->txFrameInfo.status != kNETC_EPTxSuccess)
        {
            LWIP_DEBUGF(NETIF_DEBUG,
                        ("ethernetif_linkoutput: Transmit frame failed: 0x%x\n", ethernetif->txFrameInfo.status));
            result = ERR_IF;
        }
    }

    if (((u8_t *)p->payload)[0] & 1)
    {
        /* broadcast or multicast packet*/
        MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
    }
    else
    {
        /* unicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
    }
    /* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    /* Increment statistics counters accordingly. */
    if (status != kStatus_Success)
    {
        LINK_STATS_INC(link.err);
        MIB2_STATS_NETIF_INC(netif, ifouterrors);
    }
    else
    {
        LINK_STATS_INC(link.xmit);
        MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
    }

    return result;
}

/**
 * Should be called at the beginning of the program to set up the
 * first network interface. It calls the function ethernetif_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif0_init(struct netif *netif)
{
    static struct ethernetif ethernetif_0;

    /* EP resource. */
    static ep_handle_t ep_handle;
    static ep_config_t ep_config;

    /* Buffer descriptor and buffer memory. */

    AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t rxBuffDescrip_0[NETC_RXBD_NUM], NETC_BD_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t txBuffDescrip_0[NETC_TXBD_NUM], NETC_BD_ALIGNMENT);
    SDK_ALIGN(static rx_buffer_t rxDataBuff_0[NETC_RXBUFF_NUM], NETC_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_0[NETC_TXBUFF_NUM], NETC_BUFF_ALIGNMENT);

    AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t cmdBuffDescrip_0[NETC_CMDBD_NUM], NETC_BD_ALIGNMENT);

    static uint64_t rxBuffAddrArray_0[NETC_RXBD_NUM];

    static netc_tx_frame_info_t txDirty_0[NETC_TXBD_NUM];

    ethernetif_config_t *cfg = (ethernetif_config_t *)netif->state;

    ethernetif_0.cmdBuffDescrip  = &cmdBuffDescrip_0[0];
    ethernetif_0.rxBuffDescrip   = &rxBuffDescrip_0[0];
    ethernetif_0.txBuffDescrip   = &txBuffDescrip_0[0];
    ethernetif_0.rxDataBuff      = &rxDataBuff_0[0];
    ethernetif_0.txDataBuff      = &txDataBuff_0[0];
    ethernetif_0.txDirty         = &txDirty_0[0];
    ethernetif_0.rxBuffAddrArray = &rxBuffAddrArray_0[0];

    ethernetif_0.ep_handle = &ep_handle;
    ethernetif_0.ep_config = &ep_config;

    ethernetif_0.phyHandle = cfg->phyHandle;

    s_ethernetif = &ethernetif_0; // TODO do not use static/global variable

    return ethernetif_init(netif, &ethernetif_0, &ethernetif_0 /*ethernetif_get_enet_base(0U)*/, cfg);
}

void *ethernetif_get_enet_base(const uint8_t enetIdx)
{
    (void)enetIdx;

    return NULL; // TODO implement
}

phy_handle_t *ethernetif_get_phy(struct netif *netif_)
{
    struct ethernetif *eif = netif_->state;
    return eif->phyHandle;
}

hal_gpio_handle_t ethernetif_get_int_gpio_hdl(struct netif *netif_)
{
    struct ethernetif *eif = netif_->state;
    return (hal_gpio_handle_t)eif->intGpioHdl;
}

phy_speed_t ethernetif_get_link_speed(struct netif *netif_)
{
    struct ethernetif *eif = netif_->state;
    return eif->last_speed;
}

phy_duplex_t ethernetif_get_link_duplex(struct netif *netif_)
{
    struct ethernetif *eif = netif_->state;
    return eif->last_duplex;
}

void ethernetif_on_link_up(struct netif *netif_, phy_speed_t speed, phy_duplex_t duplex)
{
    struct ethernetif *eif = netif_->state;
    status_t status;

    if (!eif->last_link_up || (speed != eif->last_speed) || (duplex != eif->last_duplex))
    {
        status = EP_Up(eif->ep_handle, (netc_hw_mii_speed_t)speed, (netc_hw_mii_duplex_t)duplex);
        LWIP_ASSERT("EP_Up failed", status == kStatus_Success);
        eif->last_speed   = speed;
        eif->last_duplex  = duplex;
        eif->last_link_up = true;
#if (NETC_VSI_NUM_USED > 0)
        EP_PsiNotifyLink(eif->ep_handle);
#endif
    }
}

void ethernetif_on_link_down(struct netif *netif_)
{
    struct ethernetif *eif = netif_->state;
#if (NETC_VSI_NUM_USED == 0)
    status_t status;
#endif

    if (eif->last_link_up)
    {
#if (NETC_VSI_NUM_USED == 0)
        status = EP_Down(eif->ep_handle);
        LWIP_ASSERT("EP_Down failed", status == kStatus_Success);
#endif
        eif->last_link_up = false;

#if (NETC_VSI_NUM_USED > 0)
        EP_PsiNotifyLink(eif->ep_handle);
#endif
    }
}

#if (LWIP_IPV4 && LWIP_IGMP) || (LWIP_IPV6 && LWIP_IPV6_MLD)
static err_t ethernetif_mcastmacfilter_action(struct netif *netif,
                                              uint8_t *multicastMacAddr,
                                              enum netif_mac_filter_action action)
{
#if NETC_PROMISCUOUS == 0
    struct ethernetif *ethernetif = netif->state;

    /* Get item's index. */
    int idx = ethernetif_mmac_get_idx(netif, multicastMacAddr);
    if (idx == -1)
        return ERR_MEM;

    switch (action)
    {
        /* Action - add an address to the filter. */
        case NETIF_ADD_MAC_FILTER:
        {
            if (ethernetif_mmac_is_new(idx))
            {
                /* New item - update the hardware filter */
                if (EP_RxL2MFAddEMTableEntry(ethernetif->ep_handle, idx, multicastMacAddr) != kStatus_Success)
                {
                    return ERR_IF;
                }
            }

            /* Increase the refcounter */
            ethernetif_mmac_ref_inc(netif, multicastMacAddr, idx);
            return ERR_OK;
        }

        /* Action - remove an address from the filter. */
        case NETIF_DEL_MAC_FILTER:
        {
            /* Shouldn't happen - yet nonexistent MAC address */
            if (ethernetif_mmac_is_new(idx))
                return ERR_IF;

            if (ethernetif_mmac_is_pending(idx))
            {
                /* Address pending removal - update the hardware filter. */
                if (EP_RxL2MFDelEMTableEntry(ethernetif->ep_handle, idx) != kStatus_Success)
                {
                    return ERR_IF;
                }
            }

            /* Decrease the refcounter */
            ethernetif_mmac_ref_dec(netif, idx);
            return ERR_OK;
        }

        /* No other action is supported. */
        default:
            return ERR_IF;
    }
#else
    return ERR_OK;
#endif
}
#endif

#if LWIP_IPV4 && LWIP_IGMP
err_t ethernetif_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
    uint8_t multicastMacAddr[6];

    multicastMacAddr[0] = 0x01U;
    multicastMacAddr[1] = 0x00U;
    multicastMacAddr[2] = 0x5EU;
    multicastMacAddr[3] = (group->addr >> 8) & 0x7FU;
    multicastMacAddr[4] = (group->addr >> 16) & 0xFFU;
    multicastMacAddr[5] = (group->addr >> 24) & 0xFFU;

    return ethernetif_mcastmacfilter_action(netif, multicastMacAddr, action);
}
#endif

#if LWIP_IPV6 && LWIP_IPV6_MLD
err_t ethernetif_mld_mac_filter(struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action)
{
    uint8_t multicastMacAddr[6];

    multicastMacAddr[0] = 0x33U;
    multicastMacAddr[1] = 0x33U;
    multicastMacAddr[2] = (group->addr[3]) & 0xFFU;
    multicastMacAddr[3] = (group->addr[3] >> 8) & 0xFFU;
    multicastMacAddr[4] = (group->addr[3] >> 16) & 0xFFU;
    multicastMacAddr[5] = (group->addr[3] >> 24) & 0xFFU;

    return ethernetif_mcastmacfilter_action(netif, multicastMacAddr, action);
}
#endif

void **ethernetif_base_ptr(struct ethernetif *ethernetif)
{
    (void)ethernetif;

    static void *fake_base;
    return &fake_base;
}
