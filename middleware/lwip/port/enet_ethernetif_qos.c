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
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/sys.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "lwip/igmp.h"
#include "lwip/mld6.h"

#if !NO_SYS
#include "FreeRTOS.h"
#include "event_groups.h"
#include "lwip/tcpip.h"
#endif /* !NO_SYS */

#include "enet_ethernetif.h"
#include "enet_ethernetif_priv.h"

#include "fsl_enet_qos.h"
#include "fsl_phy.h"

/*
 * Padding of ethernet frames has to be disabled for zero-copy functionality
 * since ENET_QOS driver requires the starting buffer addresses to be aligned.
 */
#if ETH_PAD_SIZE != 0
#error "ETH_PAD_SIZE != 0"
#endif /* ETH_PAD_SIZE != 0 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef ENET_RXBD_NUM
#define ENET_RXBD_NUM (5)
#endif

#ifndef ENET_TXBD_NUM
#define ENET_TXBD_NUM (4)
#endif

#ifndef ENET_RXBUFF_SIZE
#define ENET_RXBUFF_SIZE (ENET_QOS_FRAME_MAX_FRAMELEN)
#endif

#ifndef ENET_TXBUFF_SIZE
#define ENET_TXBUFF_SIZE (ENET_QOS_FRAME_MAX_FRAMELEN)
#endif

#ifndef FSL_FEATURE_L2CACHE_LINESIZE_BYTE
#define FSL_FEATURE_L2CACHE_LINESIZE_BYTE 0
#endif

#ifndef FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
#define FSL_FEATURE_L1DCACHE_LINESIZE_BYTE 0
#endif

#if (FSL_FEATURE_L2CACHE_LINESIZE_BYTE > FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define FSL_ENET_BUFF_ALIGNMENT FSL_FEATURE_L2CACHE_LINESIZE_BYTE
#else
#define FSL_ENET_BUFF_ALIGNMENT FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
#endif

/* The number of RX buffers. ENET_RXBD_NUM is always held by ENET driver,
 * so a couple more are needed to pass zero-copy data into lwIP. */
#ifndef ENET_RXBUFF_NUM
#define ENET_RXBUFF_NUM (ENET_RXBD_NUM * 2)
#endif

/* The number of ENET buffers needed to receive frame of maximum length. */
#define MAX_BUFFERS_PER_FRAME \
    ((ENET_QOS_FRAME_MAX_FRAMELEN / ENET_RXBUFF_SIZE) + ((ENET_QOS_FRAME_MAX_FRAMELEN % ENET_RXBUFF_SIZE == 0) ? 0 : 1))

/* At least ENET_RXBD_NUM number of buffers is always held by ENET driver for RX.
 * Some additional buffers are needed to pass at least one frame zero-copy data to lwIP. */
#if ENET_RXBUFF_NUM < (ENET_RXBD_NUM + MAX_BUFFERS_PER_FRAME)
#error "ENET_RXBUFF_NUM < (ENET_RXBD_NUM + MAX_BUFFERS_PER_FRAME)"
#endif

typedef uint8_t rx_buffer_t[SDK_SIZEALIGN(ENET_RXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT)];
typedef uint8_t tx_buffer_t[SDK_SIZEALIGN(ENET_TXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT)];

/*!
 * @brief Used to wrap received data in a pbuf to be passed into lwIP
 *        without copying.
 * Once last reference is released, buffer can be used by ENET RX DMA again.
 */
typedef struct rx_pbuf_wrapper
{
    struct pbuf_custom p;      /*!< Pbuf wrapper. Has to be first. */
    void *buffer;              /*!< Original buffer wrapped by p. */
    volatile bool buffer_used; /*!< Wrapped buffer is used by ENET or lwIP. */
    struct netif *netif;       /*!< Network interface context data. */
} rx_pbuf_wrapper_t;

/*!
 * @brief Helper struct to hold private data used to operate the ethernet interface.
 */
struct ethernetif
{
    ENET_QOS_Type *base;
    enet_qos_handle_t handle;
#if !NO_SYS
    EventGroupHandle_t enetTransmitAccessEvent;
    EventBits_t txFlag;
#endif /* !NO_SYS */
    enet_qos_rx_bd_struct_t *rxBuffDescrip;
    enet_qos_tx_bd_struct_t *txBuffDescrip;
    enet_qos_frame_info_t txDirty[ENET_RXBD_NUM];
    rx_buffer_t *rxDataBuff;
    tx_buffer_t *txDataBuff;
    volatile uint8_t txIdx;
    uint32_t rxBufferStartAddr[ENET_RXBD_NUM];
    rx_pbuf_wrapper_t RxPbufs[ENET_RXBUFF_NUM];
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void ethernetif_rx_release(struct pbuf *p);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * Called from ENET ISR.
 */
static void ethernet_callback(
    ENET_QOS_Type *base, enet_qos_handle_t *handle, enet_qos_event_t event, uint8_t channel, void *param)
{
#if !NO_SYS
    struct netif *netif           = (struct netif *)param;
    struct ethernetif *ethernetif = netif->state;
    BaseType_t xResult;

    switch (event)
    {
        case kENET_QOS_RxIntEvent:
            ethernetif_input(netif);
            break;
        case kENET_QOS_TxIntEvent:
        {
            portBASE_TYPE taskToWake = pdFALSE;

#ifdef __CA7_REV
            if (SystemGetIRQNestingLevel())
#else
            if (__get_IPSR())
#endif
            {
                xResult =
                    xEventGroupSetBitsFromISR(ethernetif->enetTransmitAccessEvent, ethernetif->txFlag, &taskToWake);
                if ((pdPASS == xResult) && (pdTRUE == taskToWake))
                {
                    portYIELD_FROM_ISR(taskToWake);
                }
            }
            else
            {
                xEventGroupSetBits(ethernetif->enetTransmitAccessEvent, ethernetif->txFlag);
            }

            break;
        }
        default:
            break;
    }
#endif /* !NO_SYS */
}

#if LWIP_IPV4 && LWIP_IGMP
err_t ethernetif_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
    struct ethernetif *ethernetif = netif->state;
    uint8_t multicastMacAddr[6];
    err_t result;

    multicastMacAddr[0] = 0x01U;
    multicastMacAddr[1] = 0x00U;
    multicastMacAddr[2] = 0x5EU;
    multicastMacAddr[3] = (group->addr >> 8) & 0x7FU;
    multicastMacAddr[4] = (group->addr >> 16) & 0xFFU;
    multicastMacAddr[5] = (group->addr >> 24) & 0xFFU;

    switch (action)
    {
        case IGMP_ADD_MAC_FILTER:
            /* Adds the ENET device to a multicast group. */
            ENET_QOS_AddMulticastGroup(ethernetif->base, multicastMacAddr);
            result = ERR_OK;
            break;
        case IGMP_DEL_MAC_FILTER:
            /*
             * Moves the ENET device from a multicast group.
             * Since the ENET_LeaveMulticastGroup() could filter out also other
             * group addresses having the same hash, the call is commented out.
             */
            /* ENET_QOS_LeaveMulticastGroup(ethernetif->base, multicastMacAddr); */
            result = ERR_OK;
            break;
        default:
            result = ERR_IF;
            break;
    }

    return result;
}
#endif

#if LWIP_IPV6 && LWIP_IPV6_MLD
err_t ethernetif_mld_mac_filter(struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action)
{
    struct ethernetif *ethernetif = netif->state;
    uint8_t multicastMacAddr[6];
    err_t result;

    multicastMacAddr[0] = 0x33U;
    multicastMacAddr[1] = 0x33U;
    multicastMacAddr[2] = (group->addr[3]) & 0xFFU;
    multicastMacAddr[3] = (group->addr[3] >> 8) & 0xFFU;
    multicastMacAddr[4] = (group->addr[3] >> 16) & 0xFFU;
    multicastMacAddr[5] = (group->addr[3] >> 24) & 0xFFU;

    switch (action)
    {
        case NETIF_ADD_MAC_FILTER:
            /* Adds the ENET device to a multicast group. */
            ENET_QOS_AddMulticastGroup(ethernetif->base, multicastMacAddr);
            result = ERR_OK;
            break;
        case NETIF_DEL_MAC_FILTER:
            /*
             * Moves the ENET device from a multicast group.
             * Since the ENET_LeaveMulticastGroup() could filter out also other
             * group addresses having the same hash, the call is commented out.
             */
            /* ENET_QOS_LeaveMulticastGroup(ethernetif->base, multicastMacAddr); */
            result = ERR_OK;
            break;
        default:
            result = ERR_IF;
            break;
    }

    return result;
}
#endif

/**
 * Gets the RX descriptor by its index.
 */
static inline enet_qos_rx_bd_struct_t *ethernetif_get_rx_desc(struct ethernetif *ethernetif, uint32_t index)
{
    return &(ethernetif->rxBuffDescrip[index]);
}

/**
 * Gets the TX descriptor by its index.
 */
static inline enet_qos_tx_bd_struct_t *ethernetif_get_tx_desc(struct ethernetif *ethernetif, uint32_t index)
{
    return &(ethernetif->txBuffDescrip[index]);
}

/*! @brief Defines the ENET Rx memory buffer free function pointer. */
static void *ethernetif_rx_alloc(ENET_QOS_Type *base, void *userData, uint8_t channel)
{
    struct netif *netif           = (struct netif *)userData;
    struct ethernetif *ethernetif = netif->state;
    void *buffer                  = NULL;
    int i;

    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

    for (i = 0; i < ENET_RXBUFF_NUM; i++)
    {
        if (!ethernetif->RxPbufs[i].buffer_used)
        {
            ethernetif->RxPbufs[i].buffer_used = true;
            buffer                             = &ethernetif->rxDataBuff[i];
            break;
        }
    }

    SYS_ARCH_UNPROTECT(old_level);

    return buffer;
}

/**
 * Callback for release of RX zero-copy buffer from ENET driver.
 */
static void ethernetif_rx_free(ENET_QOS_Type *base, void *buffer, void *userData, uint8_t channel)
{
    struct netif *netif           = (struct netif *)userData;
    struct ethernetif *ethernetif = netif->state;
    int idx                       = ((rx_buffer_t *)buffer) - ethernetif->rxDataBuff;
    LWIP_ASSERT("Freed buffer out of range", ((idx >= 0) && (idx < ENET_RXBUFF_NUM)));

    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

    LWIP_ASSERT("ethernetif_rx_free: freeing unallocated buffer", ethernetif->RxPbufs[idx].buffer_used);
    ethernetif->RxPbufs[idx].buffer_used = false;

    SYS_ARCH_UNPROTECT(old_level);
}

/**
 * Initializes ENET_QOS driver.
 */
void ethernetif_enet_init(struct netif *netif,
                          struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig)
{
    enet_qos_config_t config;
    status_t status;
    uint32_t sysClock;
    enet_qos_buffer_config_t buffCfg[ENET_RING_NUM];
    phy_speed_t speed;
    phy_duplex_t duplex;
    uint32_t i;
#if !NO_SYS
    enet_qos_ptp_config_t ptpConfig = {0};
#endif /* !NO_SYS */

    /* prepare the buffer configuration. */
    buffCfg[0].rxRingLen = ENET_RXBD_NUM; /* The length of receive buffer descriptor ring. */
    buffCfg[0].txRingLen = ENET_TXBD_NUM; /* The length of transmit buffer descriptor ring. */
    buffCfg[0].txDescStartAddrAlign =
        ethernetif_get_tx_desc(ethernetif, 0U); /* Aligned transmit descriptor start address. */
    buffCfg[0].txDescTailAddrAlign =
        ethernetif_get_tx_desc(ethernetif, 0U); /* Aligned transmit descriptor tail address. */
    buffCfg[0].txDirtyStartAddr = &ethernetif->txDirty[0];
    buffCfg[0].rxDescStartAddrAlign =
        ethernetif_get_rx_desc(ethernetif, 0U); /* Aligned receive descriptor start address. */
    buffCfg[0].rxDescTailAddrAlign =
        ethernetif_get_rx_desc(ethernetif, ENET_RXBD_NUM);         /* Aligned receive descriptor tail address. */
    buffCfg[0].rxBufferStartAddr  = ethernetif->rxBufferStartAddr; /* Start addresses of the rx buffers. */
    buffCfg[0].rxBuffSizeAlign    = sizeof(rx_buffer_t);           /* Aligned receive data buffer size. */
    buffCfg[0].rxBuffNeedMaintain = true; /* Whether receive data buffer need cache maintain. */

    sysClock = ethernetifConfig->phyHandle->mdioHandle->resource.csrClock_Hz;
    
    for (i = 0; i < ENET_RXBUFF_NUM; i++)
    {
        ethernetif->RxPbufs[i].p.custom_free_function = ethernetif_rx_release;
        ethernetif->RxPbufs[i].buffer                 = &(ethernetif->rxDataBuff[i][0]);
        ethernetif->RxPbufs[i].buffer_used            = false;
        ethernetif->RxPbufs[i].netif                  = netif;
    }

    ENET_QOS_GetDefaultConfig(&config);

    ethernetif_phy_init(ethernetif, ethernetifConfig, &speed, &duplex);
    switch (speed)
    {
        case kPHY_Speed10M:
            config.miiSpeed = kENET_QOS_MiiSpeed10M;
            break;
        case kPHY_Speed100M:
            config.miiSpeed = kENET_QOS_MiiSpeed100M;
            break;
        case kPHY_Speed1000M:
            config.miiSpeed = kENET_QOS_MiiSpeed1000M;
            break;
        default:
            break;
    }
    config.miiDuplex = (enet_qos_mii_duplex_t)duplex;

    config.specialControl = kENET_QOS_HashMulticastEnable | kENET_QOS_StoreAndForward;
    config.rxBuffAlloc = ethernetif_rx_alloc;
    config.rxBuffFree = ethernetif_rx_free;

#if !NO_SYS
    ptpConfig.tsRollover = kENET_QOS_DigitalRollover;
    config.ptpConfig     = &ptpConfig;

    /* Create the Event for transmit busy release trigger. */
    ethernetif->enetTransmitAccessEvent = xEventGroupCreate();
    ethernetif->txFlag                  = 0x1;

#endif /* !NO_SYS */

    ethernetif->txIdx = 0U;

    status = ENET_QOS_Init(ethernetif->base, &config, netif->hwaddr, 1U, sysClock);
    LWIP_ASSERT("ENET_QOS_Init failed\r\n", status == kStatus_Success);

    /* Create the handler. */
#if !NO_SYS
    ENET_QOS_EnableInterrupts(ethernetif->base, kENET_QOS_DmaTx | kENET_QOS_DmaRx);
#endif /* !NO_SYS */
    ENET_QOS_CreateHandler(ethernetif->base, &ethernetif->handle, &config, &buffCfg[0], ethernet_callback, netif);

    status = ENET_QOS_DescriptorInit(ethernetif->base, &config, &buffCfg[0]);
    LWIP_ASSERT("ENET_QOS_DescriptorInit failed\r\n", status == kStatus_Success);

    status = ENET_QOS_RxBufferAllocAll(ethernetif->base, &ethernetif->handle);
    LWIP_ASSERT("ENET_QOS_RxBufferAllocAll failed\r\n", status == kStatus_Success);

    /* Active TX/RX. */
    ENET_QOS_StartRxTx(ethernetif->base, 1, 1);
}

void **ethernetif_enet_ptr(struct ethernetif *ethernetif)
{
    return (void **)&(ethernetif->base);
}

/**
 * Returns next buffer for TX.
 * Can wait if no buffer available.
 */
static unsigned char *ethernetif_get_tx_buffer(struct ethernetif *ethernetif)
{
    enet_qos_tx_bd_struct_t *txBuffDesc = ethernetif_get_tx_desc(ethernetif, ethernetif->txIdx);
#if !NO_SYS
    while (1)
    {
        if (ENET_QOS_IsTxDescriptorDmaOwn(txBuffDesc))
        {
            xEventGroupWaitBits(ethernetif->enetTransmitAccessEvent, ethernetif->txFlag, pdTRUE, (BaseType_t) false,
                                portMAX_DELAY);
        }
        else
        {
            break;
        }
    }
#else
    {
        uint32_t counter;

        for (counter = ENET_TIMEOUT; counter != 0U; counter--)
        {
            if (!ENET_QOS_IsTxDescriptorDmaOwn(txBuffDesc))
            {
                break;
            }
        }

        if (counter == 0U)
        {
            return (unsigned char *)NULL;
        }
    }
#endif /* !NO_SYS */

    return (unsigned char *)&(ethernetif->txDataBuff[ethernetif->txIdx][0]);
}

/**
 * Sends frame via ENET_QOS.
 */
static err_t ethernetif_send_frame(struct ethernetif *ethernetif, unsigned char *data, const uint32_t length)
{
    status_t result;

    DCACHE_CleanByRange((uint32_t)data, sizeof(tx_buffer_t));

    do
    {
        result = ENET_QOS_SendFrame(ethernetif->base, &ethernetif->handle, data, length, 0U, false, NULL);

#if !NO_SYS
        if (result == kStatus_ENET_QOS_TxFrameBusy)
        {
            xEventGroupWaitBits(ethernetif->enetTransmitAccessEvent, ethernetif->txFlag, pdTRUE, (BaseType_t) false,
                                portMAX_DELAY);
        }
#endif /* !NO_SYS */
    } while (result == kStatus_ENET_QOS_TxFrameBusy);

    if (result == kStatus_Success)
    {
        ethernetif->txIdx = (ethernetif->txIdx + 1U) % ENET_TXBD_NUM;
        return ERR_OK;
    }
    else
    {
        return ERR_IF;
    }
}

/**
 * Reclaims RX buffer held by the p after p is no longer used
 * by the application / lwIP.
 */
static void ethernetif_rx_release(struct pbuf *p)
{
    rx_pbuf_wrapper_t *wrapper    = (rx_pbuf_wrapper_t *)p;
    struct netif *netif           = wrapper->netif;
    struct ethernetif *ethernetif = netif->state;

    ethernetif_rx_free(ethernetif->base, wrapper->buffer, netif, 0);
}

/** Wraps received buffer(s) into a pbuf or a pbuf chain and returns it. */
static struct pbuf *ethernetif_rx_frame_to_pbufs(struct ethernetif *ethernetif, enet_qos_rx_frame_struct_t *rxFrame)
{
    void *buffer;
    uint16_t bufferLength;
    rx_pbuf_wrapper_t *wrapper;
    uint16_t len   = 0U;
    struct pbuf *p = NULL;
    struct pbuf *q = NULL;
    int idx;
    int i;

    for (i = 0; ((i < MAX_BUFFERS_PER_FRAME) && (len < rxFrame->totLen)); i++)
    {
        buffer       = rxFrame->rxBuffArray[i].buffer;
        bufferLength = rxFrame->rxBuffArray[i].length;
        len += bufferLength;

        /* Find pbuf wrapper for the actually read byte buffer */
        idx = ((rx_buffer_t *)buffer) - ethernetif->rxDataBuff;
        LWIP_ASSERT("Buffer returned by ENET_GetRxFrame() doesn't match any RX buffer descriptor",
                    ((idx >= 0) && (idx < ENET_RXBUFF_NUM)));
        wrapper = &ethernetif->RxPbufs[idx];
        LWIP_ASSERT("Buffer returned by ENET_GetRxFrame() doesn't match wrapper buffer", wrapper->buffer == buffer);

        /* Wrap the received buffer in pbuf. */
        if (p == NULL)
        {
            p = pbuf_alloced_custom(PBUF_RAW, bufferLength, PBUF_REF, &wrapper->p, buffer, bufferLength);
            LWIP_ASSERT("pbuf_alloced_custom() failed", p);
        }
        else
        {
            q = pbuf_alloced_custom(PBUF_RAW, bufferLength, PBUF_REF, &wrapper->p, buffer, bufferLength);
            LWIP_ASSERT("pbuf_alloced_custom() failed", q);

            pbuf_cat(p, q);
        }
    }

    LWIP_ASSERT("p->tot_len != rxFrame->totLen", p->tot_len == rxFrame->totLen);

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
    enet_qos_buffer_struct_t buffers[MAX_BUFFERS_PER_FRAME];
    enet_qos_rx_frame_struct_t rxFrame = {.rxBuffArray = &buffers[0]};
    struct pbuf *p                 = NULL;
    status_t status;

    status = ENET_QOS_GetRxFrame(ethernetif->base, &ethernetif->handle, &rxFrame, 0);
    switch (status)
    {
        case kStatus_Success:
            /* Frame read, process it into pbufs. */
            p = ethernetif_rx_frame_to_pbufs(ethernetif, &rxFrame);
            break;

        case kStatus_ENET_QOS_RxFrameEmpty:
            /* Frame not available. */
            break;

        case kStatus_ENET_QOS_RxFrameError:
            /* Error receiving frame */
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_linkinput: RxFrameError\n"));
            LINK_STATS_INC(link.drop);
            MIB2_STATS_NETIF_INC(netif, ifindiscards);
            break;

        case kStatus_ENET_QOS_RxFrameDrop:
            /* Frame received, but it had to be dropped
             * because new buffer(s) allocation failed in the ENET driver. */
            LINK_STATS_INC(link.drop);
            MIB2_STATS_NETIF_INC(netif, ifindiscards);
            break;

        default:
            LWIP_ASSERT("Unhandled return value.", 0);
            break;
    }

    return p;
}

err_t ethernetif_linkoutput(struct netif *netif, struct pbuf *p)
{
    err_t result;
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *q;
    unsigned char *pucBuffer;
    unsigned char *pucChar;

    LWIP_ASSERT("Output packet buffer empty", p);

    pucBuffer = ethernetif_get_tx_buffer(ethernetif);
    if (pucBuffer == NULL)
    {
        return ERR_BUF;
    }

    /* Initiate transfer. */

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    if (p->len == p->tot_len)
    {
        /* No pbuf chain, still have to copy as pbuf could be reclaimed early. */
        memcpy(pucBuffer, p->payload, p->len);
    }
    else
    {
        /* pbuf chain, copy into contiguous ucBuffer. */
        if (p->tot_len >= ENET_QOS_FRAME_MAX_FRAMELEN)
        {
            return ERR_BUF;
        }
        else
        {
            pucChar = pucBuffer;

            for (q = p; q != NULL; q = q->next)
            {
                /* Send the data from the pbuf to the interface, one pbuf at a
                time. The size of the data in each pbuf is kept in the ->len
                variable. */
                /* send data from(q->payload, q->len); */
                memcpy(pucChar, q->payload, q->len);
                pucChar += q->len;
            }
        }
    }

    /* Send frame. */
    result = ethernetif_send_frame(ethernetif, pucBuffer, p->tot_len);

    MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
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

    LINK_STATS_INC(link.xmit);

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
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_qos_rx_bd_struct_t rxBuffDescrip_0[ENET_RXBD_NUM],
                                  ENET_QOS_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_qos_tx_bd_struct_t txBuffDescrip_0[ENET_TXBD_NUM],
                                  ENET_QOS_BUFF_ALIGNMENT);
    SDK_ALIGN(static rx_buffer_t rxDataBuff_0[ENET_RXBUFF_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_0[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    memset((void *)&ethernetif_0, 0, sizeof(struct ethernetif));

    ethernetif_0.rxBuffDescrip = &(rxBuffDescrip_0[0]);
    ethernetif_0.txBuffDescrip = &(txBuffDescrip_0[0]);
    ethernetif_0.rxDataBuff    = &(rxDataBuff_0[0]);
    ethernetif_0.txDataBuff    = &(txDataBuff_0[0]);

    return ethernetif_init(netif, &ethernetif_0, ethernetif_get_enet_qos_base(0U), (ethernetif_config_t *)netif->state);
}

#if defined(FSL_FEATURE_SOC_ENET_QOS_COUNT) && (FSL_FEATURE_SOC_ENET_QOS_COUNT > 1)
/**
 * Should be called at the beginning of the program to set up the
 * second network interface. It calls the function ethernetif_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif1_init(struct netif *netif)
{
    static struct ethernetif ethernetif_1;
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_qos_rx_bd_struct_t rxBuffDescrip_1[ENET_RXBD_NUM],
                                  ENET_QOS_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_qos_tx_bd_struct_t txBuffDescrip_1[ENET_TXBD_NUM],
                                  ENET_QOS_BUFF_ALIGNMENT);
    SDK_ALIGN(static rx_buffer_t rxDataBuff_1[ENET_RXBUFF_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_1[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    memset((void *)&ethernetif_1, 0, sizeof(struct ethernetif));

    ethernetif_1.rxBuffDescrip = &(rxBuffDescrip_1[0]);
    ethernetif_1.txBuffDescrip = &(txBuffDescrip_1[0]);
    ethernetif_1.rxDataBuff    = &(rxDataBuff_1[0]);
    ethernetif_1.txDataBuff    = &(txDataBuff_1[0]);

    return ethernetif_init(netif, &ethernetif_1, ethernetif_get_enet_qos_base(1U), (ethernetif_config_t *)netif->state);
}
#endif /* FSL_FEATURE_SOC_ENET_QOS_COUNT */
