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
 * Copyright 2016-2019 NXP
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

#include "fsl_enet.h"
#include "fsl_phy.h"

#if MEM_ALIGNMENT != FSL_ENET_BUFF_ALIGNMENT
/* These two has to match for zero-copy functionality */
#error "MEM_ALIGNMENT != FSL_ENET_BUFF_ALIGNMENT"
#endif /* MEM_ALIGNMENT != FSL_ENET_BUFF_ALIGNMENT */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Used to wrap received data in a pbuf to be passed into lwIP
 *        without copying.
 * Once last reference is released, RX descriptor will be returned to DMA.
 */
typedef struct rx_pbuf_wrapper
{
    struct pbuf_custom p;           /*!< Pbuf wrapper. Has to be first. */
    enet_rx_bd_struct_t* rxDesc;    /*!< Descriptor holding the data. */
    struct ethernetif *ethernetif;  /*!< Ethernet interface context data. */
    volatile bool ownedByLwip;      /*!< If true, descriptor cannot be reused by DMA yet. */
} rx_pbuf_wrapper_t;

/*!
 * @brief Helper struct to hold private data used to operate
 *        your ethernet interface.
 */
struct ethernetif
{
    ENET_Type *base;
    enet_handle_t handle;
#if !NO_SYS
    EventGroupHandle_t enetTransmitAccessEvent;
    EventBits_t txFlag;
#endif /* !NO_SYS */
    enet_rx_bd_struct_t *RxBuffDescrip;
    enet_tx_bd_struct_t *TxBuffDescrip;
    rx_buffer_t *RxDataBuff;
    volatile struct pbuf *txPbufs[ENET_TXBD_NUM];
    volatile uint8_t txIdx;
    volatile uint8_t txReleaseIdx;
    rx_pbuf_wrapper_t rxPbufs[ENET_RXBD_NUM];
    uint8_t rxIdx;
    const mem_range_t *non_dma_memory;
};

static void ethernetif_tx_release(struct ethernetif *ethernetif);
static void ethernetif_rx_release(struct pbuf *p);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * Called from ENET ISR.
 */
static void ethernet_callback(ENET_Type *base, enet_handle_t *handle,
                              enet_event_t event, uint8_t channel, void *param)
#if NO_SYS
{
    struct netif *netif = (struct netif *)param;
    struct ethernetif *ethernetif = netif->state;

    if (event == kENET_TxIntEvent)
    {
        ethernetif_tx_release(ethernetif);
    }
}
#else
{
    struct netif *netif = (struct netif *)param;
    struct ethernetif *ethernetif = netif->state;
    BaseType_t xResult;    

    switch (event)
    {
        case kENET_RxIntEvent:
            ethernetif_input(netif);
            break;
        case kENET_TxIntEvent:
        {
            portBASE_TYPE taskToWake = pdFALSE;

            ethernetif_tx_release(ethernetif);

#ifdef __CA7_REV
            if (SystemGetIRQNestingLevel())
#else
            if (__get_IPSR())
#endif 
            {
                xResult = xEventGroupSetBitsFromISR(
                                            ethernetif->enetTransmitAccessEvent,
                                            ethernetif->txFlag, &taskToWake);
                if ((pdPASS == xResult) && (pdTRUE == taskToWake))
                {
                    portYIELD_FROM_ISR(taskToWake);
                }
            }
            else
            {
                xEventGroupSetBits(ethernetif->enetTransmitAccessEvent,
                                   ethernetif->txFlag);
            }

            break;
        }
        default:
            break;
    }
}
#endif /* NO_SYS */

#if LWIP_IPV4 && LWIP_IGMP
err_t ethernetif_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group,
                                 enum netif_mac_filter_action action)
{
    struct ethernetif *ethernetif = netif->state;
    err_t result;

    switch (action)
    {
        case IGMP_ADD_MAC_FILTER:
            /* LPC ENET does not accept multicast selectively,
             * so all multicast has to be passed through. */
            ENET_AcceptAllMulticast(ethernetif->base);
            result = ERR_OK;
            break;
        case IGMP_DEL_MAC_FILTER:
            /*
             * Moves the ENET device from a multicast group.
             * Since we don't keep track of which multicast groups
             * are still to enabled, the call is commented out.
             */
            /* ENET_RejectAllMulticast(ethernetif->base); */
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
err_t ethernetif_mld_mac_filter(struct netif *netif, const ip6_addr_t *group,
                                enum netif_mac_filter_action action)
{
    struct ethernetif *ethernetif = netif->state;
    err_t result;

    switch (action)
    {
        case NETIF_ADD_MAC_FILTER:
            /* LPC ENET does not accept multicast selectively,
             * so all multicast has to be passed through. */
            ENET_AcceptAllMulticast(ethernetif->base);
            result = ERR_OK;
            break;
        case NETIF_DEL_MAC_FILTER:
            /*
             * Moves the ENET device from a multicast group.
             * Since we don't keep track of which multicast groups
             * are still to enabled, the call is commented out.
             */
            /* ENET_RejectAllMulticast(ethernetif->base); */
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
static inline enet_rx_bd_struct_t *ethernetif_get_rx_desc(
                                                  struct ethernetif *ethernetif,
                                                  uint32_t index)
{
    return &(ethernetif->RxBuffDescrip[index]);
}

/**
 * Gets the TX descriptor by its index.
 */
static inline enet_tx_bd_struct_t *ethernetif_get_tx_desc(
                                                  struct ethernetif *ethernetif,
                                                  uint32_t index)
{
    return &(ethernetif->TxBuffDescrip[index]);
}

/**
 * Initializes ENET driver.
 */
void ethernetif_enet_init(struct netif *netif, struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig)
{
    enet_config_t config;
    uint32_t sysClock;
    enet_buffer_config_t buffCfg[ENET_RING_NUM];
    uint32_t rxBufferStartAddr[ENET_RXBD_NUM];
    uint32_t i;

    /* calculate start addresses of all rx buffers */
    for (i = 0; i < ENET_RXBD_NUM; i++)
    {
        rxBufferStartAddr[i] = (uint32_t)&(ethernetif->RxDataBuff[i][ETH_PAD_SIZE]);
    }

    /* prepare the buffer configuration. */
    buffCfg[0].rxRingLen = ENET_RXBD_NUM;                          /* The length of receive buffer descriptor ring. */
    buffCfg[0].txRingLen = ENET_TXBD_NUM;                          /* The length of transmit buffer descriptor ring. */
    buffCfg[0].txDescStartAddrAlign = ethernetif_get_tx_desc(ethernetif, 0U); /* Aligned transmit descriptor start address. */
    buffCfg[0].txDescTailAddrAlign = ethernetif_get_tx_desc(ethernetif, 0U);  /* Aligned transmit descriptor tail address. */
    buffCfg[0].rxDescStartAddrAlign = ethernetif_get_rx_desc(ethernetif, 0U); /* Aligned receive descriptor start address. */
    buffCfg[0].rxDescTailAddrAlign = ethernetif_get_rx_desc(ethernetif, ENET_RXBD_NUM); /* Aligned receive descriptor tail address. */
    buffCfg[0].rxBufferStartAddr = rxBufferStartAddr;              /* Start addresses of the rx buffers. */
    buffCfg[0].rxBuffSizeAlign = sizeof(rx_buffer_t);              /* Aligned receive data buffer size. */

    sysClock = CLOCK_GetFreq(ethernetifConfig->clockName);

    LWIP_ASSERT("ethernetifConfig->non_dma_memory == NULL", (ethernetifConfig->non_dma_memory != NULL));
    ethernetif->non_dma_memory = ethernetifConfig->non_dma_memory;

    ENET_GetDefaultConfig(&config);
    config.multiqueueCfg = NULL;

    ethernetif_phy_init(ethernetif, ethernetifConfig, &config);

#if !NO_SYS
    /* Create the Event for transmit busy release trigger. */
    ethernetif->enetTransmitAccessEvent = xEventGroupCreate();
    ethernetif->txFlag = 0x1;
#endif /* !NO_SYS */
    NVIC_SetPriority(ETHERNET_IRQn, ENET_PRIORITY);

    ethernetif->txIdx = 0U;
    ethernetif->rxIdx = 0U;
    ethernetif->txReleaseIdx = 0U;

    for (i = 0; i < ENET_RXBD_NUM; i++)
    {
        ethernetif->rxPbufs[i].p.custom_free_function = ethernetif_rx_release;
        ethernetif->rxPbufs[i].rxDesc = &ethernetif->RxBuffDescrip[i];
        ethernetif->rxPbufs[i].ethernetif = ethernetif;
        ethernetif->rxPbufs[i].ownedByLwip = false;
    }

    ENET_Init(ethernetif->base, &config, netif->hwaddr, sysClock);

#if defined(LPC54018_SERIES)
    /* Workaround for receive issue on lpc54018 */
    ethernetif->base->MAC_FRAME_FILTER |= ENET_MAC_FRAME_FILTER_RA_MASK;
#endif

    /* Create the handler. */
#if NO_SYS
    ENET_EnableInterrupts(ethernetif->base, kENET_DmaTx);
#else
    ENET_EnableInterrupts(ethernetif->base, kENET_DmaTx | kENET_DmaRx);
#endif /* NO_SYS */
    ENET_CreateHandler(ethernetif->base, &ethernetif->handle, &config,
                       &buffCfg[0], ethernet_callback, netif);

    ENET_DescriptorInit(ethernetif->base, &config, &buffCfg[0]);

    /* Active TX/RX. */
    ENET_StartRxTx(ethernetif->base, 1, 1);
}

ENET_Type **ethernetif_enet_ptr(struct ethernetif *ethernetif)
{
    return &(ethernetif->base);
}

/**
 * Find the ENET instance index from its base address.
 */
static uint32_t ethernetif_get_enet_idx(ENET_Type *base)
{
    static ENET_Type *const s_enetBases[] = ENET_BASE_PTRS;
    uint32_t instance;

    for (instance = 0; instance < FSL_FEATURE_SOC_LPC_ENET_COUNT; instance++)
    {
        if (s_enetBases[instance] == base)
        {
            break;
        }
    }

    LWIP_ASSERT("Cannot find ENET instance index from its base address.",
                instance < FSL_FEATURE_SOC_LPC_ENET_COUNT);

    return instance;
}

/**
 * Sends (part of) a frame via ENET.
 * TODO: Since ENET_SendFrame() could not be used, some functionality it does
 * is missing here for now (channel selection depending on AVB content,
 * timestamping.
 */
static void ethernetif_send_buffer(struct ethernetif *ethernetif,
                                   unsigned char *data,
                                   const uint32_t length,
                                   struct pbuf *p_to_release,
                                   enet_desc_flag flag)
{
    static const IRQn_Type s_enetIrqId[] = ENET_IRQS;
    enet_tx_bd_struct_t *txDesc = ethernetif_get_tx_desc(ethernetif,
                                                         ethernetif->txIdx);
    ethernetif->txPbufs[ethernetif->txIdx] = p_to_release;
    ethernetif->txIdx = (ethernetif->txIdx + 1) % ENET_TXBD_NUM;

    /* Prepare the descriptor for transmit. */
    txDesc->buff1Addr = (uint32_t)data;
    txDesc->buff2Addr = (uint32_t)NULL;
    txDesc->buffLen =
                     ENET_TXDESCRIP_RD_BL1(length) | ENET_TXDESCRIP_RD_IOC_MASK;

    txDesc->controlStat =
                    ENET_TXDESCRIP_RD_FL(length) | ENET_TXDESCRIP_RD_LDFD(flag);
    if ((flag & kENET_FirstFlagOnly) == 0)
    {
        /*
         * Submit to DMA if not the first descriptor in chain.
         * All the descriptors have to be prepared before the first one
         * is flagged for DMA and transfer starts. ENET could output invalid
         * frames otherwise (the exception is Store and Forward mode, where
         * delays between preparing of descriptors does not matter).
         */
        txDesc->controlStat |= ENET_TXDESCRIP_RD_OWN_MASK;
    }

    enet_tx_bd_ring_t *txBdRing = (enet_tx_bd_ring_t *)
                                                &ethernetif->handle.txBdRing[0];

    /*
     * Increment txDescUsed.
     * Without this, callback would not fire from ENET ISR on finished TX.
     * This is kind of a hack. Alternative could be to define
     * void ETHERNET_DriverIRQHandler(void) and handle IRQs completely
     * in this file.
     */
    DisableIRQ(s_enetIrqId[ethernetif_get_enet_idx(ethernetif->base)]);
    txBdRing->txDescUsed++;
    EnableIRQ(s_enetIrqId[ethernetif_get_enet_idx(ethernetif->base)]);
}

/**
 * Reclaims exactly one TX descriptor after its data has been sent out.
 * Then the descriptor can be used by application to prepare next data to send.
 */
static void ethernetif_tx_release(struct ethernetif *ethernetif)
{
    LWIP_ASSERT("Attempt to release more TX buffers than acquired.",
                ethernetif->txIdx != ethernetif->txReleaseIdx);
    enet_tx_bd_struct_t *txDesc
                         = &ethernetif->TxBuffDescrip[ethernetif->txReleaseIdx];
    LWIP_ASSERT("TX buffer still owned by DMA.",
                !ENET_IsTxDescriptorDmaOwn(txDesc));

    struct pbuf *p = (struct pbuf *)
                                  ethernetif->txPbufs[ethernetif->txReleaseIdx];
    if (p != NULL)
    {
#if ETH_PAD_SIZE
        /* Reclaim the padding, force because it may be REF pbuf. */
        pbuf_header_force(p, ETH_PAD_SIZE);
#endif

#if NO_SYS
#if defined(LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT) && LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT
        pbuf_free(p);
#else
        #error "Bare metal requires LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT=1 because pbuf_free() is being called from an ISR"
#endif /* LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT */
#else
        if (pbuf_free_callback(p) != ERR_OK)
        {
#if defined(LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT) && LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT
            pbuf_free(p);
#else
            LWIP_ASSERT("Failed to enqueue pbuf deallocation on tcpip_thread",
                        0);
#endif /* LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT */
        }
#endif /* NO_SYS */

        ethernetif->txPbufs[ethernetif->txReleaseIdx] = NULL;
    }

    ethernetif->txReleaseIdx = (ethernetif->txReleaseIdx + 1) % ENET_TXBD_NUM;
}

/**
 * Reclaims RX descriptor which holds the p's buffer after p is no longer used
 * by the application / lwIP. The DMA can receive new data into
 * the descriptor's buffer then.
 * Note that RX buffers may be freed by lwIP out of the order in which they were
 * passed to lwIP. Therefore there may be spaces between the RX descriptors
 * flagged as owned by DMA and DMA could still wait until it's actual position
 * is released.
 */
static void ethernetif_rx_release(struct pbuf *p)
{
    SYS_ARCH_DECL_PROTECT(old_level);
    rx_pbuf_wrapper_t *wrapper = (rx_pbuf_wrapper_t *)p;
#if NO_SYS
    bool intEnable = false;
#else
    bool intEnable = true;
#endif /* NO_SYS */

    SYS_ARCH_PROTECT(old_level);

    wrapper->ownedByLwip = false;

    /* Update the receive buffer descriptor. */
    ENET_UpdateRxDescriptor(wrapper->rxDesc, NULL, NULL, intEnable, false);
    ENET_UpdateRxDescriptorTail(wrapper->ethernetif->base, 0U,
          (uint32_t)ethernetif_get_rx_desc(wrapper->ethernetif, ENET_RXBD_NUM));

    SYS_ARCH_UNPROTECT(old_level);
}

/**
 * Gets the length of a received frame (if there is some).
 */
static status_t ethernetif_get_rx_frame_size(struct ethernetif *ethernetif,
                                             uint32_t *length)
{
    uint8_t index = ethernetif->rxIdx;
    enet_rx_bd_struct_t *rxDesc;
    uint32_t rxControl;

    /* Reset the length to zero. */
    *length = 0;

    do
    {
        rxDesc = ethernetif_get_rx_desc(ethernetif, index);
        rxControl = ENET_GetRxDescriptor(rxDesc);

        if ((rxControl & ENET_RXDESCRIP_WR_OWN_MASK)
            || (ethernetif->rxPbufs[index].ownedByLwip))
        {
            /*
             * Buffer descriptor is owned by DMA or lwIP.
             * We haven't received any complete frame yet.
             */
            return kStatus_ENET_RxFrameEmpty;
        }

        /* Application owns the buffer descriptor. */
        if (rxControl & ENET_RXDESCRIP_WR_LD_MASK)
        {
            /* It's last descriptor of a frame, get its status or length. */
            if (rxControl & ENET_RXDESCRIP_WR_ERRSUM_MASK)
            {
                return kStatus_ENET_RxFrameError;
            }
            else
            {
                *length = rxControl & ENET_RXDESCRIP_WR_PACKETLEN_MASK;
                return kStatus_Success;
            }
        }

        index = (index + 1U) % ENET_RXBD_NUM;
    } while (index != ethernetif->rxIdx);

    /*
     * All descriptors have data but the end of the frame not detected.
     */
    return kStatus_ENET_RxFrameError;
}

/**
 * Drops (releases) receive descriptors until the last one of a frame is reached
 * or drops entire descriptor ring when all descriptors have data but end
 * of the frame not detected among them.
 * Function can be called only after ethernetif_get_rx_frame_size() indicates
 * that there actually is a frame error or a received frame.
 */
static void ethernetif_drop_frame(struct ethernetif *ethernetif)
{
#if NO_SYS
    bool intEnable = false;
#else
    bool intEnable = true;
#endif /* NO_SYS */

    enet_rx_bd_struct_t *rxDesc;
    uint8_t index = ethernetif->rxIdx;
    uint32_t rxControl;

    do
    {
        rxDesc = ethernetif_get_rx_desc(ethernetif, ethernetif->rxIdx);
        ethernetif->rxIdx = (ethernetif->rxIdx + 1U) % ENET_RXBD_NUM;
        rxControl = ENET_GetRxDescriptor(rxDesc);

        /* Update the receive buffer descriptor. */
        ENET_UpdateRxDescriptor(rxDesc, NULL, NULL, intEnable, false);

        /* Find the last buffer descriptor for the frame. */
        if (rxControl & ENET_RXDESCRIP_WR_LD_MASK)
        {
            break;
        }
    } while (ethernetif->rxIdx != index);

    ENET_UpdateRxDescriptorTail(ethernetif->base, 0U,
                   (uint32_t)ethernetif_get_rx_desc(ethernetif, ENET_RXBD_NUM));
}

/**
 * Reads a received frame - wraps its descriptor buffer(s) into a pbuf
 * or a pbuf chain, flag descriptors as owned by lwIP and returns the pbuf.
 * The descriptors are returned to DMA only after the returned pbuf is released.
 * Function can be called only after ethernetif_get_rx_frame_size() indicates
 * that there actually is a received frame.
 */
static struct pbuf *ethernetif_read_frame(struct ethernetif *ethernetif,
                                          uint32_t length)
{
    rx_pbuf_wrapper_t *wrapper;
    enet_rx_bd_struct_t *rxDesc;
    uint32_t rxControl;
    uint32_t len = 0;
    struct pbuf *p = NULL;
    struct pbuf *q = NULL;

    do
    {
        wrapper = &ethernetif->rxPbufs[ethernetif->rxIdx];
        wrapper->ownedByLwip = true;
        ethernetif->rxIdx = (ethernetif->rxIdx + 1U) % ENET_RXBD_NUM;

        rxDesc = wrapper->rxDesc;
        rxControl = ENET_GetRxDescriptor(rxDesc);

        len = (rxControl & ENET_RXDESCRIP_WR_PACKETLEN_MASK);

        /* Wrap the receive buffer in pbuf. */
        if (p == NULL)
        {
            p = pbuf_alloced_custom(PBUF_RAW, len, PBUF_REF, &wrapper->p,
                                    (void *)rxDesc->buff1Addr, len);
            LWIP_ASSERT("pbuf_alloced_custom() failed", p);

#if ETH_PAD_SIZE
            /* Add the padding header, force because it is a REF type buffer. */
            pbuf_header_force(p, ETH_PAD_SIZE);
#endif
        }
        else
        {
            q = pbuf_alloced_custom(PBUF_RAW, len, PBUF_REF, &wrapper->p,
                                    (void *)rxDesc->buff1Addr, len);
            LWIP_ASSERT("pbuf_alloced_custom() failed", q);

            pbuf_cat(p, q);
        }
    } while (((rxControl & ENET_RXDESCRIP_WR_LD_MASK) == 0U)
             && (p->tot_len < length));

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

/**
 * Attempts to read a frame from ENET and returns it wrapped in a pbuf
 * or returns NULL when no frame is received. Discards invalid frames.
 */
struct pbuf *ethernetif_linkinput(struct netif *netif)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *p = NULL;
    uint32_t len;
    status_t status;

    /* Obtain the size of the packet and put it into the "len" variable. */
    status = ethernetif_get_rx_frame_size(ethernetif, &len);

    if (status == kStatus_Success)
    {
        p = ethernetif_read_frame(ethernetif, len);

        if (p == NULL)
        {
            /* Could not initialise wrapper pbuf(s) - drop the frame. */
            ethernetif_drop_frame(ethernetif);

            LWIP_DEBUGF(NETIF_DEBUG,
                 ("ethernetif_linkinput: Fail to allocate new memory space\n"));

            LINK_STATS_INC(link.memerr);
            LINK_STATS_INC(link.drop);
            MIB2_STATS_NETIF_INC(netif, ifindiscards);
        }
    }
    else if (status == kStatus_ENET_RxFrameError)
    {
        /* Update the received buffer when error happened. */
        ethernetif_drop_frame(ethernetif);

        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_linkinput: RxFrameError\n"));

        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif, ifindiscards);
    }

    return p;
}

/**
 * Returns the number of TX descriptors which could be used by lwIP/application
 * to put new TX data into.
 *
 * The max number of free descriptors is (ENET_TXBD_NUM - 1), that is when
 * ethernetif->txReleaseIdx == ethernetif->txIdx. Having the capacity decreased
 * by one allows to avoid locking: txReleaseIdx is advanced only from ISR
 * and txIdx from tcpip_thread/main loop. Should we use full capacity and have
 * some variable to indicate between the "all buffers are free" vs. "all buffers
 * are used" situation, it would be manipulated from two contexts hence locking
 * would be needed.
 */
static inline int ethernetif_avail_tx_descs(struct ethernetif *ethernetif)
{
    return (ethernetif->txReleaseIdx + ENET_TXBD_NUM - 1 - ethernetif->txIdx)
                                                                % ENET_TXBD_NUM;
}

/**
 * Attempts to output a frame from ENET. The function avoids copying of
 * p's payload when possible. In such situation it increases p's reference count
 * and decreases it (and possibly releases p) after the payload is sent.
 */
err_t ethernetif_linkoutput(struct netif *netif, struct pbuf *p)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *q;
    struct pbuf *pbuf_to_free = NULL;
    struct pbuf *p_copy;
    uint16_t clen;
    bool copy = false;
    const mem_range_t *non_dma_memory;
    uint8_t *dst;
    uint32_t cnt = 0;
    uint8_t first_idx;
    uint32_t tail_address;

    LWIP_ASSERT("Output packet buffer empty", p);

    if ((p->tot_len - ETH_PAD_SIZE) > ENET_FRAME_MAX_FRAMELEN)
    {
        return ERR_BUF;
    }

    clen = pbuf_clen(p);

    /* Check if relocation is needed */

    if (clen > (ENET_TXBD_NUM - 1))
    {
        /* Pbuf chain is too long to be prepared for DMA at once. */
        copy = true;
    }

    for (q = p; (q != NULL) && !copy; q = q->next)
    {
        /*
         * Check if payload is aligned is not desired: lwIP creates RAM pbufs
         * in a way that the data coming after the headers are aligned, but not
         * the beginning of the ethernet header. LPC ENET DMA will read from
         * the aligned address, which is ok, because there is additional space
         * before the headers to make up for alignment - so DMA will not read
         * from invalid address or unrelated data.
         */

        /* Check payload address is usable by ENET DMA */
        for (non_dma_memory = ethernetif->non_dma_memory;
             (non_dma_memory->start != 0U)
             || (non_dma_memory->end != 0U); non_dma_memory++)
        {
            if ((q->payload >= (void *) non_dma_memory->start)
                && (q->payload < (void *) non_dma_memory->end))
            {
                copy = true;
                break;
            }
        }
    }

	if (copy)
    {
        /* Pbuf needs to be copied. */

        p_copy = pbuf_alloc(PBUF_RAW, (uint16_t) p->tot_len, PBUF_POOL);
		if (p_copy == NULL)
        {
			return ERR_MEM;
		}

		dst = (uint8_t *) p_copy->payload;
		for (q = p; q != NULL; q = q->next)
        {
            LWIP_ASSERT("Copied bytes would exceed p->tot_len",
                    (q->len + dst - (uint8_t *) p_copy->payload) <= p->tot_len);
            memcpy(dst, (uint8_t *)q->payload, q->len);
			dst += q->len;
		}
        LWIP_ASSERT("Copied bytes != p->tot_len",
                    (dst - (uint8_t *) p_copy->payload) == p->tot_len);
		p_copy->len = p_copy->tot_len = p->tot_len;

		p = p_copy;
	}
    else
    {
        /*
         * Increase reference count so p is released only after it is sent.
         * For copied pbuf, ref is already 1 after pbuf_alloc().
         */
        pbuf_ref(p);
    }

    /*
     * Wait until the sufficient number of descriptors are available,
     * as we have to start the transfer of the first buffer only
     * after all buffers in chain are prepared.
     */
    while (ethernetif_avail_tx_descs(ethernetif) < clen)
    {
#if !NO_SYS
        xEventGroupWaitBits(ethernetif->enetTransmitAccessEvent,
                            ethernetif->txFlag, pdTRUE, (BaseType_t) false,
                            portMAX_DELAY);
#endif /* !NO_SYS */
        cnt++;
        if (cnt >= ENET_TIMEOUT)
        {
            return ERR_TIMEOUT;
        }
    }

#if ETH_PAD_SIZE
    /* Drop the padding. */
    pbuf_header(p, -ETH_PAD_SIZE);
#endif

    /* Initiate transfer. */

    first_idx = ethernetif->txIdx;

    for (q = p; q != NULL; q = q->next)
    {
        enet_desc_flag flag = kENET_MiddleFlag;
        pbuf_to_free = NULL;

        if (q == p)
        {
            flag |= kENET_FirstFlagOnly;
        }

        if (q->next == NULL)
        {
            flag |= kENET_LastFlagOnly;

            /* On last TX interrupt, free pbuf chain. */
            pbuf_to_free = p;
        }

        ethernetif_send_buffer(ethernetif, q->payload, q->len, pbuf_to_free,
                               flag);
    }

    /* All pbufs from chain are prepared, allow DMA to access the first one. */
    ethernetif_get_tx_desc(ethernetif, first_idx)->controlStat |=
                                                     ENET_TXDESCRIP_RD_OWN_MASK;

    /* Update the transmit tail address. */
    if (ethernetif->txIdx == 0U)
    {
        tail_address = (uint32_t)ethernetif_get_tx_desc(ethernetif,
                                                        ENET_TXBD_NUM);
    }
    else
    {
        tail_address = (uint32_t)ethernetif_get_tx_desc(ethernetif,
                                                        ethernetif->txIdx);
    }
    ENET_UpdateTxDescriptorTail(ethernetif->base, 0, tail_address);

    MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
    if (((uint8_t *)p->payload)[0] & 1)
    {
        /* broadcast or multicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
    }
    else
    {
        /* unicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
    }
    LINK_STATS_INC(link.xmit);

    return ERR_OK;
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
    SDK_ALIGN(static rx_buffer_t rxDataBuff_0[ENET_RXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    ethernetif_0.RxBuffDescrip = &(rxBuffDescrip_0[0]);
    ethernetif_0.TxBuffDescrip = &(txBuffDescrip_0[0]);
    ethernetif_0.RxDataBuff = &(rxDataBuff_0[0]);

    return ethernetif_init(netif, &ethernetif_0, 0U, (ethernetif_config_t *)netif->state);
}

#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 1)
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
    SDK_ALIGN(static rx_buffer_t rxDataBuff_1[ENET_RXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    ethernetif_1.RxBuffDescrip = &(rxBuffDescrip_1[0]);
    ethernetif_1.TxBuffDescrip = &(txBuffDescrip_1[0]);
    ethernetif_1.RxDataBuff = &(rxDataBuff_1[0]);

    return ethernetif_init(netif, &ethernetif_1, 1U, (ethernetif_config_t *)netif->state);
}
#endif /* FSL_FEATURE_SOC_*_ENET_COUNT */
