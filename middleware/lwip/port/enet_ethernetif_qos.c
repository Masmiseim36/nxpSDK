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

#include "ethernetif.h"
#include "ethernetif_priv.h"

#ifdef FSL_ETH_ENABLE_CACHE_CONTROL
#include "fsl_cache.h"
#endif /* FSL_ETH_ENABLE_CACHE_CONTROL */

#include "fsl_enet_qos.h"
#include "fsl_phy.h"
#ifdef FSL_ETH_ENABLE_CACHE_CONTROL
#include "fsl_cache.h"
#endif

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
#define ENET_TXBUFF_SIZE (ENET_QOS_FRAME_MAX_FRAMELEN - ENET_QOS_FCS_LEN)
#endif

/* BD address must be 8 bytes alignment (64 bytes alignment for optimal performance) */
#define ENET_BD_ALIGNMENT 64

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

/* BD address alignment should be met */
#if (FSL_ENET_BUFF_ALIGNMENT < ENET_BD_ALIGNMENT)
#undef FSL_ENET_BUFF_ALIGNMENT
#define FSL_ENET_BUFF_ALIGNMENT ENET_BD_ALIGNMENT
#endif

/* The number of RX buffers. ENET_RXBD_NUM is always held by ENET driver,
 * so a couple more are needed to pass zero-copy data into lwIP. */
#ifndef ENET_RXBUFF_NUM
#define ENET_RXBUFF_NUM (ENET_RXBD_NUM * 2)
#endif

/* The number of ENET buffers needed to receive frame of maximum length. */
#define MAX_BUFFERS_PER_FRAME                                                                        \
    ((ENET_QOS_FRAME_MAX_FRAMELEN / CONSTANT_SIZEALIGN(ENET_RXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT)) + \
     ((ENET_QOS_FRAME_MAX_FRAMELEN % CONSTANT_SIZEALIGN(ENET_RXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT) == 0) ? 0 : 1))

/* At least ENET_RXBD_NUM number of buffers is always held by ENET driver for RX.
 * Some additional buffers are needed to pass at least one frame zero-copy data to lwIP. */
#if ENET_RXBUFF_NUM < (ENET_RXBD_NUM + MAX_BUFFERS_PER_FRAME)
#error "ENET_RXBUFF_NUM < (ENET_RXBD_NUM + MAX_BUFFERS_PER_FRAME)"
#endif

#include "enet_configchecks.h"

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
    enet_qos_frame_info_t txDirty[ENET_TXBD_NUM];
    rx_buffer_t *rxDataBuff;
    tx_buffer_t *txDataBuff;
    volatile uint8_t txIdx;
    uint32_t rxBufferStartAddr[ENET_RXBD_NUM];
    rx_pbuf_wrapper_t RxPbufs[ENET_RXBUFF_NUM];
    phy_handle_t *phyHandle;
    phy_speed_t last_speed;
    phy_duplex_t last_duplex;
    bool last_link_up;

    uint32_t intGpioHdl[((HAL_GPIO_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
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
                xEventGroupSetBits(ethernetif->enetTransmitAccessEvent, ethernetif->txFlag);
            }

            break;
        }
        default:
            break;
    }
#endif /* !NO_SYS */
}

#if (LWIP_IPV4 && LWIP_IGMP) || (LWIP_IPV6 && LWIP_IPV6_MLD)
static err_t ethernetif_mcastmacfilter_action(struct netif *netif,
                                              uint8_t *multicastMacAddr,
                                              enum netif_mac_filter_action action)
{
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
                ENET_QOS_AddMulticastGroup(ethernetif->base, multicastMacAddr);
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
                ENET_QOS_LeaveMulticastGroup(ethernetif->base, multicastMacAddr);
            }

            /* Decrease the refcounter */
            ethernetif_mmac_ref_dec(netif, idx);
            return ERR_OK;
        }

        /* No other action is supported. */
        default:
            return ERR_IF;
    }
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

#if ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS
    if (buffer == NULL)
    {
        ENET_QOS_DisableInterrupts(base, (uint32_t)kENET_QOS_DmaRx);
    }
#else
    (void)base;
#endif

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

#if ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS
    ENET_QOS_EnableInterrupts(base, (uint32_t)kENET_QOS_DmaRx);
#else
    (void)base;
#endif

    SYS_ARCH_UNPROTECT(old_level);
}

static enet_qos_mii_speed_t ethernetif_convert_phy_speed(phy_speed_t phy_speed)
{
    enet_qos_mii_speed_t enet_speed = kENET_QOS_MiiSpeed10M;
    switch (phy_speed)
    {
        case kPHY_Speed10M:
            enet_speed = kENET_QOS_MiiSpeed10M;
            break;
        case kPHY_Speed100M:
            enet_speed = kENET_QOS_MiiSpeed100M;
            break;
        case kPHY_Speed1000M:
            enet_speed = kENET_QOS_MiiSpeed1000M;
            break;
        default:
            while (true) // Unknown phy speed
            {
            }
            break;
    }

    return enet_speed;
}

/**
 * Initializes ENET_QOS driver.
 */
void ethernetif_plat_init(struct netif *netif,
                          struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig)
{
    enet_qos_config_t config;
    status_t status;
    enet_qos_buffer_config_t buffCfg[ETHERNETIF_RING_NUM];
    uint32_t i;
#if !NO_SYS
    enet_qos_ptp_config_t ptpConfig = {0};
#endif /* !NO_SYS */

    /* prepare the buffer configuration. */
    buffCfg[0].rxRingLen = ENET_RXBD_NUM;       /* The length of receive buffer descriptor ring. */
    buffCfg[0].txRingLen = ENET_TXBD_NUM;       /* The length of transmit buffer descriptor ring. */
    buffCfg[0].txDescStartAddrAlign =
        ethernetif_get_tx_desc(ethernetif, 0U); /* Aligned transmit descriptor start address. */
    buffCfg[0].txDescTailAddrAlign =
        ethernetif_get_tx_desc(ethernetif, 0U); /* Aligned transmit descriptor tail address. */
    buffCfg[0].txDirtyStartAddr = &ethernetif->txDirty[0];
    buffCfg[0].rxDescStartAddrAlign =
        ethernetif_get_rx_desc(ethernetif, 0U);                   /* Aligned receive descriptor start address. */
    buffCfg[0].rxDescTailAddrAlign =
        ethernetif_get_rx_desc(ethernetif, ENET_RXBD_NUM);        /* Aligned receive descriptor tail address. */
    buffCfg[0].rxBufferStartAddr = ethernetif->rxBufferStartAddr; /* Start addresses of the rx buffers. */
    buffCfg[0].rxBuffSizeAlign   = sizeof(rx_buffer_t);           /* Aligned receive data buffer size. */
#ifdef FSL_ETH_ENABLE_CACHE_CONTROL
    buffCfg[0].rxBuffNeedMaintain = true;                         /* Whether receive data buffer need cache maintain. */
#else
    buffCfg[0].rxBuffNeedMaintain = false; /* Whether receive data buffer need cache maintain. */
#endif

    for (i = 0; i < ENET_RXBUFF_NUM; i++)
    {
        ethernetif->RxPbufs[i].p.custom_free_function = ethernetif_rx_release;
        ethernetif->RxPbufs[i].buffer                 = &(ethernetif->rxDataBuff[i][0]);
        ethernetif->RxPbufs[i].buffer_used            = false;
        ethernetif->RxPbufs[i].netif                  = netif;
    }

    ENET_QOS_GetDefaultConfig(&config);

    /* Used for detection of change.
       Initilize to value different than any possible enum value. */
    ethernetif->last_speed   = (phy_speed_t)0xa5a5;
    ethernetif->last_duplex  = (phy_duplex_t)0x5a5a;
    ethernetif->last_link_up = false;

    ethernetif_phy_init(ethernetif, ethernetifConfig);

    config.specialControl = kENET_QOS_HashMulticastEnable | kENET_QOS_StoreAndForward;
    config.rxBuffAlloc    = ethernetif_rx_alloc;
    config.rxBuffFree     = ethernetif_rx_free;

#if (CHECKSUM_CHECK_IP == 0) || (CHECKSUM_CHECK_TCP == 0) || (CHECKSUM_CHECK_UDP == 0) || \
    (CHECKSUM_CHECK_ICMP == 0) || (CHECKSUM_CHECK_ICMP6 == 0)
    config.specialControl |= kENET_QOS_RxChecksumOffloadEnable;
#endif

#if !NO_SYS
    ptpConfig.tsRollover = kENET_QOS_DigitalRollover;
    config.ptpConfig     = &ptpConfig;

    /* Create the Event for transmit busy release trigger. */
    ethernetif->enetTransmitAccessEvent = xEventGroupCreate();
    ethernetif->txFlag                  = 0x1;

#endif /* !NO_SYS */

    ethernetif->txIdx = 0U;

    status = ENET_QOS_Init(ethernetif->base, &config, netif->hwaddr, 1U, ethernetifConfig->srcClockHz);
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

void **ethernetif_base_ptr(struct ethernetif *ethernetif)
{
    return (void **)&(ethernetif->base);
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
        status = ENET_QOS_SetMII(eif->base, ethernetif_convert_phy_speed(speed), (enet_qos_mii_duplex_t)duplex);
        if (status == kStatus_Success)
        {
            eif->last_speed   = speed;
            eif->last_duplex  = duplex;
            eif->last_link_up = true;
        }
        else
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_on_link_up: Failed to set MII link speed/duplex\n"));
        }
    }
}

void ethernetif_on_link_down(struct netif *netif_)
{
    struct ethernetif *eif = netif_->state;
    eif->last_link_up      = false;
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

        for (counter = ETHERNETIF_TIMEOUT; counter != 0U; counter--)
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

#ifdef FSL_ETH_ENABLE_CACHE_CONTROL
    DCACHE_CleanByRange((uint32_t)data, sizeof(tx_buffer_t));
#endif

#if (CHECKSUM_GEN_IP == 0) && (CHECKSUM_GEN_TCP == 0) && (CHECKSUM_GEN_UDP == 0) && (CHECKSUM_GEN_ICMP == 0) && \
    (CHECKSUM_GEN_ICMP6 == 0)
    enet_qos_tx_offload_t offloadConfig = kENET_QOS_TxOffloadAll;
#elif (CHECKSUM_GEN_IP == 0)
    enet_qos_tx_offload_t offloadConfig = kENET_QOS_TxOffloadIPHeader;
#else
    enet_qos_tx_offload_t offloadConfig = kENET_QOS_TxOffloadDisable;
#endif

    do
    {
        result =
            ENET_QOS_SendFrame(ethernetif->base, &ethernetif->handle, data, length, 0U, false, NULL, offloadConfig);

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
    struct pbuf *p                     = NULL;
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
    unsigned char *pucBuffer;
    u16_t uCopied;

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

    if (p->len > sizeof(tx_buffer_t))
    {
        /* ENET_QOS_SendFrame takes single buffer */
        result = ERR_BUF;
    }
    else
    {
        if (p->len == p->tot_len)
        {
            /* No pbuf chain, still have to copy as pbuf could be reclaimed early. */
            memcpy(pucBuffer, p->payload, p->len);
        }
        else
        {
            /* pbuf chain, copy into contiguous ucBuffer. */
            uCopied = pbuf_copy_partial(p, pucBuffer, p->tot_len, 0);
            LWIP_ASSERT("uCopied != p->tot_len", uCopied == p->tot_len);
        }

        /* Send frame. */
        result = ethernetif_send_frame(ethernetif, pucBuffer, p->tot_len);
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
    if (result != ERR_OK)
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
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_qos_rx_bd_struct_t rxBuffDescrip_0[ENET_RXBD_NUM],
                                  ENET_QOS_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_qos_tx_bd_struct_t txBuffDescrip_0[ENET_TXBD_NUM],
                                  ENET_QOS_BUFF_ALIGNMENT);
    SDK_ALIGN(static rx_buffer_t rxDataBuff_0[ENET_RXBUFF_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_0[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    memset((void *)&ethernetif_0, 0, sizeof(struct ethernetif));

    ethernetif_config_t *cfg = (ethernetif_config_t *)netif->state;

    ethernetif_0.rxBuffDescrip = &(rxBuffDescrip_0[0]);
    ethernetif_0.txBuffDescrip = &(txBuffDescrip_0[0]);
    ethernetif_0.rxDataBuff    = &(rxDataBuff_0[0]);
    ethernetif_0.txDataBuff    = &(txDataBuff_0[0]);

    ethernetif_0.phyHandle = cfg->phyHandle;

    return ethernetif_init(netif, &ethernetif_0, ethernetif_get_enet_qos_base(0U), cfg);
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

    ethernetif_config_t *cfg = (ethernetif_config_t *)netif->state;

    ethernetif_1.rxBuffDescrip = &(rxBuffDescrip_1[0]);
    ethernetif_1.txBuffDescrip = &(txBuffDescrip_1[0]);
    ethernetif_1.rxDataBuff    = &(rxDataBuff_1[0]);
    ethernetif_1.txDataBuff    = &(txDataBuff_1[0]);

    ethernetif_1.phyHandle = cfg->phyHandle;

    return ethernetif_init(netif, &ethernetif_1, ethernetif_get_enet_qos_base(1U), cfg);
}
#endif /* FSL_FEATURE_SOC_ENET_QOS_COUNT */
