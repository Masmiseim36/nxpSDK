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
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"

#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

#include "ethernetif.h"
#include "ethernetif_priv.h"

#include "fsl_enet.h"
#include "fsl_phy.h"

/*
 * Padding of ethernet frames has to be disabled for zero-copy functionality
 * since ENET driver requires the starting buffer addresses to be aligned.
 */
#if ETH_PAD_SIZE != 0
#error "ETH_PAD_SIZE != 0"
#endif /* ETH_PAD_SIZE != 0 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL
#if defined(FSL_FEATURE_L2CACHE_LINESIZE_BYTE) && \
    ((!defined(FSL_SDK_DISBLE_L2CACHE_PRESENT)) || (FSL_SDK_DISBLE_L2CACHE_PRESENT == 0))
#if defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define FSL_CACHE_LINESIZE_MAX  MAX(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE, FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
#define FSL_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_CACHE_LINESIZE_MAX)
#else
#define FSL_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L2CACHE_LINESIZE_BYTE)
#endif
#elif defined(FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#define FSL_ENET_BUFF_ALIGNMENT MAX(ENET_BUFF_ALIGNMENT, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)
#else
#define FSL_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
#else
#define FSL_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif

/* The maximum length of lwIP frame to transmit. */
#define MAX_TX_FRAMELEN (ENET_FRAME_MAX_FRAMELEN - ENET_FRAME_CRC_LEN)

/* The maximum length of frame to receive from ENET. */
#define MAX_RX_FRAMELEN (ENET_FRAME_MAX_FRAMELEN)

/* The length of RX buffer. */
#ifndef ENET_RXBUFF_SIZE
#define ENET_RXBUFF_SIZE (MAX_RX_FRAMELEN)
#endif

/* The number of ENET buffers needed to receive frame of maximum length. */
#define MAX_BUFFERS_PER_FRAME                                                           \
    (MAX_RX_FRAMELEN / CONSTANT_SIZEALIGN(ENET_RXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT)) + \
        ((MAX_RX_FRAMELEN % CONSTANT_SIZEALIGN(ENET_RXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT) == 0) ? 0 : 1)

/* The length of TX buffer. */
#ifndef ENET_TXBUFF_SIZE
#define ENET_TXBUFF_SIZE (MAX_TX_FRAMELEN)
#endif

/* The number of buffer descriptors in ENET RX ring. */
#ifndef ENET_RXBD_NUM
#define ENET_RXBD_NUM (5)
#endif

/* Ring should be able to receive at least 1 frame with maximum length. */
#if ENET_RXBD_NUM < MAX_BUFFERS_PER_FRAME
#error "ENET_RXBD_NUM < MAX_BUFFERS_PER_FRAME"
#endif

/* The number of RX buffers. ENET_RXBD_NUM is always held by ENET driver,
 * so a couple more are needed to pass zero-copy data into lwIP. */
#ifndef ENET_RXBUFF_NUM
#define ENET_RXBUFF_NUM (ENET_RXBD_NUM * 2)
#endif

/* At least ENET_RXBD_NUM number of buffers is always held by ENET driver for RX.
 * Some additional buffers are needed to pass at least one frame zero-copy data to lwIP. */
#if ENET_RXBUFF_NUM < (ENET_RXBD_NUM + MAX_BUFFERS_PER_FRAME)
#error "ENET_RXBUFF_NUM < (ENET_RXBD_NUM + MAX_BUFFERS_PER_FRAME)"
#endif

/* The number of buffer descriptors in ENET TX ring. */
#ifndef ENET_TXBD_NUM
#define ENET_TXBD_NUM (3)
#endif

#if (CONSTANT_SIZEALIGN(ENET_TXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT) * ENET_TXBD_NUM) < (MAX_TX_FRAMELEN)
#warning \
    "The combined size of TX buffers is not enough to hold a frame of maximum length. \
It may be or may not be possible to transmit such a frame, depending if the ENET DMA is faster \
than submitting of subsequent buffers (with chunks of the frame data) or not."
#endif

typedef uint8_t rx_buffer_t[SDK_SIZEALIGN(ENET_RXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT)];
typedef uint8_t tx_buffer_t[SDK_SIZEALIGN(ENET_TXBUFF_SIZE, FSL_ENET_BUFF_ALIGNMENT)];

#include "kinetis_configchecks.h"

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

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 */
struct ethernetif
{
    ENET_Type *base;
#if (defined(FSL_FEATURE_SOC_ENET_COUNT) && (FSL_FEATURE_SOC_ENET_COUNT > 0)) || (USE_RTOS && defined(SDK_OS_FREE_RTOS))
    enet_handle_t handle;
#endif
#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
    EventGroupHandle_t enetTransmitAccessEvent;
    EventBits_t txFlag;
#endif
    enet_rx_bd_struct_t *RxBuffDescrip;
    enet_tx_bd_struct_t *TxBuffDescrip;
    rx_buffer_t *RxDataBuff;
    tx_buffer_t *TxDataBuff;
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
void *ethernetif_get_enet_base(const uint8_t enetIdx);

/*******************************************************************************
 * Code
 ******************************************************************************/
#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
static void ethernet_callback(ENET_Type *base,
                              enet_handle_t *handle,
#if FSL_FEATURE_ENET_QUEUE > 1
                              uint32_t ringId,
#endif /* FSL_FEATURE_ENET_QUEUE */
                              enet_event_t event,
                              enet_frame_info_t *frameInfo,
                              void *userData)
{
    struct netif *netif           = (struct netif *)userData;
    struct ethernetif *ethernetif = netif->state;
    BaseType_t xResult;

    switch (event)
    {
        case kENET_RxEvent:
            ethernetif_input(netif);
            break;
        case kENET_TxEvent:
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
        }
        break;
        default:
            break;
    }
}
#endif

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
                ENET_AddMulticastGroup(ethernetif->base, multicastMacAddr);
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
                ENET_LeaveMulticastGroup(ethernetif->base, multicastMacAddr);
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

/*! @brief Defines the ENET Rx memory buffer free function pointer. */
static void *ethernetif_rx_alloc(ENET_Type *base, void *userData, uint8_t ringId)
{
    struct netif *netif           = (struct netif *)userData;
    struct ethernetif *ethernetif = netif->state;
    void *buffer                  = NULL;
    int i;
    (void)ringId;

    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

    for (i = 0; i < ENET_RXBUFF_NUM; i++)
    {
        if (!ethernetif->RxPbufs[i].buffer_used)
        {
            ethernetif->RxPbufs[i].buffer_used = true;
            buffer                             = &ethernetif->RxDataBuff[i];
            break;
        }
    }

#if ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS
    if (buffer == NULL)
    {
        ENET_DisableInterrupts(base, (uint32_t)kENET_RxFrameInterrupt);
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
static void ethernetif_rx_free(ENET_Type *base, void *buffer, void *userData, uint8_t ringId)
{
    struct netif *netif           = (struct netif *)userData;
    struct ethernetif *ethernetif = netif->state;
    int idx                       = ((rx_buffer_t *)buffer) - ethernetif->RxDataBuff;
    (void)ringId;
    LWIP_ASSERT("Freed buffer out of range", ((idx >= 0) && (idx < ENET_RXBUFF_NUM)));

    SYS_ARCH_DECL_PROTECT(old_level);
    SYS_ARCH_PROTECT(old_level);

    LWIP_ASSERT("ethernetif_rx_free: freeing unallocated buffer", ethernetif->RxPbufs[idx].buffer_used);
    ethernetif->RxPbufs[idx].buffer_used = false;

#if ETH_DISABLE_RX_INT_WHEN_OUT_OF_BUFFERS
    ENET_EnableInterrupts(base, (uint32_t)kENET_RxFrameInterrupt);
#else
    (void)base;
#endif

    SYS_ARCH_UNPROTECT(old_level);
}

/**
 * Initializes ENET driver.
 */
void ethernetif_plat_init(struct netif *netif,
                          struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig)
{
    enet_config_t config;
    enet_buffer_config_t buffCfg[ETHERNETIF_RING_NUM];
    int i;

    /* prepare the buffer configuration. */
    buffCfg[0].rxBdNumber      = ENET_RXBD_NUM;       /* Receive buffer descriptor number. */
    buffCfg[0].txBdNumber      = ENET_TXBD_NUM;       /* Transmit buffer descriptor number. */
    buffCfg[0].rxBuffSizeAlign = sizeof(rx_buffer_t); /* Aligned receive data buffer size. */
    buffCfg[0].txBuffSizeAlign = sizeof(tx_buffer_t); /* Aligned transmit data buffer size. */
    buffCfg[0].rxBdStartAddrAlign =
        &(ethernetif->RxBuffDescrip[0]);              /* Aligned receive buffer descriptor start address. */
    buffCfg[0].txBdStartAddrAlign =
        &(ethernetif->TxBuffDescrip[0]);              /* Aligned transmit buffer descriptor start address. */
    buffCfg[0].rxBufferAlign =
        NULL; /* Receive data buffer start address. NULL when buffers are allocated by callback for RX zero-copy. */
    buffCfg[0].txBufferAlign = &(ethernetif->TxDataBuff[0][0]); /* Transmit data buffer start address. */
    buffCfg[0].txFrameInfo = NULL; /* Transmit frame information start address. Set only if using zero-copy transmit. */
    buffCfg[0].rxMaintainEnable = true; /* Receive buffer cache maintain. */
    buffCfg[0].txMaintainEnable = true; /* Transmit buffer cache maintain. */

    ENET_GetDefaultConfig(&config);
    config.ringNum     = ETHERNETIF_RING_NUM;
    config.rxBuffAlloc = ethernetif_rx_alloc;
    config.rxBuffFree  = ethernetif_rx_free;
    config.userData    = netif;

    /* Used for detection of change.
       Initilize to value different than any possible enum value. */
    ethernetif->last_speed   = (phy_speed_t)0xa5a5;
    ethernetif->last_duplex  = (phy_duplex_t)0x5a5a;
    ethernetif->last_link_up = false;

    ethernetif_phy_init(ethernetif, ethernetifConfig);

#ifdef LWIP_ENET_FLEXIBLE_CONFIGURATION
    extern void BOARD_ENETFlexibleConfigure(enet_config_t * config);
    BOARD_ENETFlexibleConfigure(&config);
#endif

#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
    uint32_t instance;
    static ENET_Type *const enetBases[]  = ENET_BASE_PTRS;
    static const IRQn_Type enetTxIrqId[] = ENET_Transmit_IRQS;
    /*! @brief Pointers to enet receive IRQ number for each instance. */
    static const IRQn_Type enetRxIrqId[] = ENET_Receive_IRQS;
#if defined(ENET_ENHANCEDBUFFERDESCRIPTOR_MODE) && ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    /*! @brief Pointers to enet timestamp IRQ number for each instance. */
    static const IRQn_Type enetTsIrqId[] = ENET_1588_Timer_IRQS;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

    /* Create the Event for transmit busy release trigger. */
    ethernetif->enetTransmitAccessEvent = xEventGroupCreate();
    ethernetif->txFlag                  = 0x1;

    config.interrupt |=
        kENET_RxFrameInterrupt | kENET_TxFrameInterrupt | kENET_TxBufferInterrupt | kENET_LateCollisionInterrupt;
    config.callback = ethernet_callback;

    for (instance = 0; instance < ARRAY_SIZE(enetBases); instance++)
    {
        if (enetBases[instance] == ethernetif->base)
        {
#ifdef __CA7_REV
            GIC_SetPriority(enetRxIrqId[instance], ENET_PRIORITY);
            GIC_SetPriority(enetTxIrqId[instance], ENET_PRIORITY);
#if defined(ENET_ENHANCEDBUFFERDESCRIPTOR_MODE) && ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
            GIC_SetPriority(enetTsIrqId[instance], ENET_1588_PRIORITY);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
#else
            NVIC_SetPriority(enetRxIrqId[instance], ENET_PRIORITY);
            NVIC_SetPriority(enetTxIrqId[instance], ENET_PRIORITY);
#if defined(ENET_ENHANCEDBUFFERDESCRIPTOR_MODE) && ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
            NVIC_SetPriority(enetTsIrqId[instance], ENET_1588_PRIORITY);
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */
#endif /* __CA7_REV */
            break;
        }
    }

    LWIP_ASSERT("Input Ethernet base error!", (instance != ARRAY_SIZE(enetBases)));
#endif /* USE_RTOS */

    for (i = 0; i < ENET_RXBUFF_NUM; i++)
    {
        ethernetif->RxPbufs[i].p.custom_free_function = ethernetif_rx_release;
        ethernetif->RxPbufs[i].buffer                 = &(ethernetif->RxDataBuff[i][0]);
        ethernetif->RxPbufs[i].buffer_used            = false;
        ethernetif->RxPbufs[i].netif                  = netif;
    }

    config.txAccelerConfig = 0;
    config.rxAccelerConfig = kENET_RxAccelMacCheckEnabled;

#if (CHECKSUM_GEN_IP == 0)
    config.txAccelerConfig |= kENET_TxAccelIpCheckEnabled;
#endif
#if (CHECKSUM_GEN_TCP == 0) && (CHECKSUM_GEN_UDP == 0) && (CHECKSUM_GEN_ICMP == 0)
    config.txAccelerConfig |= kENET_TxAccelProtoCheckEnabled;
#endif
#if (CHECKSUM_CHECK_IP == 0)
    config.rxAccelerConfig |= kENET_RxAccelIpCheckEnabled;
    config.macSpecialConfig &= ~(kENET_ControlStoreAndFwdDisable);
#endif
#if (CHECKSUM_CHECK_TCP == 0) || (CHECKSUM_CHECK_UDP == 0) || (CHECKSUM_CHECK_ICMP == 0)
    config.rxAccelerConfig |= kENET_RxAccelProtoCheckEnabled;
    config.macSpecialConfig &= ~(kENET_ControlStoreAndFwdDisable);
#endif

    /* Initialize the ENET module. */
    ENET_Init(ethernetif->base, &ethernetif->handle, &config, &buffCfg[0], netif->hwaddr, ethernetifConfig->srcClockHz);

    ENET_ActiveRead(ethernetif->base);
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

    if (!eif->last_link_up || (speed != eif->last_speed) || (duplex != eif->last_duplex))
    {
        ENET_SetMII(eif->base, (enet_mii_speed_t)speed, (enet_mii_duplex_t)duplex);
        eif->last_speed   = speed;
        eif->last_duplex  = duplex;
        eif->last_link_up = true;
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
static unsigned char *enet_get_tx_buffer(struct ethernetif *ethernetif)
{
    static unsigned char ucBuffer[MAX_TX_FRAMELEN];
    return ucBuffer;
}

/**
 * Sends frame via ENET.
 */
static err_t enet_send_frame(struct ethernetif *ethernetif, unsigned char *data, const uint32_t length)
{
#if USE_RTOS && defined(SDK_OS_FREE_RTOS)
    {
        status_t result;

        do
        {
            result = ENET_SendFrame(ethernetif->base, &ethernetif->handle, data, length, 0, false, NULL);

            if (result == kStatus_ENET_TxFrameBusy)
            {
                xEventGroupWaitBits(ethernetif->enetTransmitAccessEvent, ethernetif->txFlag, pdTRUE, (BaseType_t) false,
                                    portMAX_DELAY);
            }

        } while (result == kStatus_ENET_TxFrameBusy);
        return ERR_OK;
    }
#else
    {
        uint32_t counter;

        for (counter = ETHERNETIF_TIMEOUT; counter != 0U; counter--)
        {
            if (ENET_SendFrame(ethernetif->base, &ethernetif->handle, data, length, 0, false, NULL) !=
                kStatus_ENET_TxFrameBusy)
            {
                return ERR_OK;
            }
        }

        return ERR_TIMEOUT;
    }
#endif
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
static struct pbuf *ethernetif_rx_frame_to_pbufs(struct ethernetif *ethernetif, enet_rx_frame_struct_t *rxFrame)
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
        idx = ((rx_buffer_t *)buffer) - ethernetif->RxDataBuff;
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
    struct ethernetif *ethernetif                       = netif->state;
    enet_buffer_struct_t buffers[MAX_BUFFERS_PER_FRAME] = {{0}};
    enet_rx_frame_struct_t rxFrame                      = {.rxBuffArray = &buffers[0]};
    struct pbuf *p                                      = NULL;
    status_t status;

    /* Read frame. */
    status = ENET_GetRxFrame(ethernetif->base, &ethernetif->handle, &rxFrame, 0);

    switch (status)
    {
        case kStatus_Success:
            /* Frame read, process it into pbufs. */
            p = ethernetif_rx_frame_to_pbufs(ethernetif, &rxFrame);
            break;

        case kStatus_ENET_RxFrameEmpty:
            /* Frame not available. */
            break;

        case kStatus_ENET_RxFrameError:
            /* Error receiving frame */
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_linkinput: RxFrameError\n"));
            LINK_STATS_INC(link.drop);
            MIB2_STATS_NETIF_INC(netif, ifindiscards);
            break;

        case kStatus_ENET_RxFrameDrop:
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

    pucBuffer = enet_get_tx_buffer(ethernetif);
    if (pucBuffer == NULL)
    {
        return ERR_BUF;
    }

    /* Initiate transfer. */

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    if (p->tot_len > MAX_TX_FRAMELEN)
    {
        result = ERR_BUF;
    }
    else
    {
        if (p->len == p->tot_len)
        {
            /* No pbuf chain, don't have to copy -> faster. */
            pucBuffer = (unsigned char *)p->payload;
        }
        else
        {
            /* pbuf chain, copy into contiguous ucBuffer. */
            uCopied = pbuf_copy_partial(p, pucBuffer, p->tot_len, 0);
            LWIP_ASSERT("uCopied != p->tot_len", uCopied == p->tot_len);
        }

        /* Send frame. */
        result = enet_send_frame(ethernetif, pucBuffer, p->tot_len);
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
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t rxBuffDescrip_0[ENET_RXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t txBuffDescrip_0[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static rx_buffer_t rxDataBuff_0[ENET_RXBUFF_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_0[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    ethernetif_config_t *cfg = (ethernetif_config_t *)netif->state;

    ethernetif_0.RxBuffDescrip = &(rxBuffDescrip_0[0]);
    ethernetif_0.TxBuffDescrip = &(txBuffDescrip_0[0]);
    ethernetif_0.RxDataBuff    = &(rxDataBuff_0[0]);
    ethernetif_0.TxDataBuff    = &(txDataBuff_0[0]);

    ethernetif_0.phyHandle = cfg->phyHandle;

    return ethernetif_init(netif, &ethernetif_0, ethernetif_get_enet_base(0U), cfg);
}

#if defined(FSL_FEATURE_SOC_ENET_COUNT) && (FSL_FEATURE_SOC_ENET_COUNT > 1)
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
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t rxBuffDescrip_1[ENET_RXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t txBuffDescrip_1[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static rx_buffer_t rxDataBuff_1[ENET_RXBUFF_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_1[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    ethernetif_config_t *cfg = (ethernetif_config_t *)netif->state;

    ethernetif_1.RxBuffDescrip = &(rxBuffDescrip_1[0]);
    ethernetif_1.TxBuffDescrip = &(txBuffDescrip_1[0]);
    ethernetif_1.RxDataBuff    = &(rxDataBuff_1[0]);
    ethernetif_1.TxDataBuff    = &(txDataBuff_1[0]);

    ethernetif_1.phyHandle = cfg->phyHandle;

    return ethernetif_init(netif, &ethernetif_1, ethernetif_get_enet_base(1U), cfg);
}
#endif /* FSL_FEATURE_SOC_*_ENET_COUNT */
