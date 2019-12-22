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
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "lwip/igmp.h"
#include "lwip/mld6.h"

#if USE_RTOS && defined(FSL_RTOS_FREE_RTOS)
#include "FreeRTOS.h"
#include "event_groups.h"
#endif

#include "enet_ethernetif.h"
#include "enet_ethernetif_priv.h"

#include "fsl_enet.h"
#include "fsl_phy.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ETH_CRC_SIZE 4

#if (PBUF_POOL_BUFSIZE >= (ENET_FRAME_MAX_FRAMELEN - ETH_CRC_SIZE))
#define RX_FRAME_FITS_INTO_PBUF 1
#else
#define RX_FRAME_FITS_INTO_PBUF 0
#endif /* (PBUF_POOL_BUFSIZE >= (ENET_FRAME_MAX_FRAMELEN - ETH_CRC_SIZE)) */

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 */
struct ethernetif
{
    ENET_Type *base;
#if (defined(FSL_FEATURE_SOC_ENET_COUNT) && (FSL_FEATURE_SOC_ENET_COUNT > 0)) || \
    (USE_RTOS && defined(FSL_RTOS_FREE_RTOS))
    enet_handle_t handle;
#endif
#if USE_RTOS && defined(FSL_RTOS_FREE_RTOS)
    EventGroupHandle_t enetTransmitAccessEvent;
    EventBits_t txFlag;
#endif
    enet_rx_bd_struct_t *RxBuffDescrip;
    enet_tx_bd_struct_t *TxBuffDescrip;
    rx_buffer_t *RxDataBuff;
    tx_buffer_t *TxDataBuff;
};


/*******************************************************************************
 * Code
 ******************************************************************************/
#if USE_RTOS && defined(FSL_RTOS_FREE_RTOS)
#if FSL_FEATURE_ENET_QUEUE > 1
static void ethernet_callback(ENET_Type *base, enet_handle_t *handle, uint32_t ringId, enet_event_t event, void *param)
#else
static void ethernet_callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param)
#endif /* FSL_FEATURE_ENET_QUEUE */
{
    struct netif *netif = (struct netif *)param;
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
                xResult = xEventGroupSetBitsFromISR(ethernetif->enetTransmitAccessEvent, ethernetif->txFlag, &taskToWake);
                if ((pdPASS == xResult) && (pdTRUE == taskToWake))
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

#if LWIP_IPV4 && LWIP_IGMP
err_t ethernetif_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group,
                                 enum netif_mac_filter_action action)
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
            /* Adds the ENET device to a multicast group.*/
            ENET_AddMulticastGroup(ethernetif->base, multicastMacAddr);
            result = ERR_OK;
            break;
        case IGMP_DEL_MAC_FILTER:
            /*
             * Moves the ENET device from a multicast group.
             * Since the ENET_LeaveMulticastGroup() could filter out also other
             * group addresses having the same hash, the call is commented out.
             */
            /* ENET_LeaveMulticastGroup(ethernetif->base, multicastMacAddr); */
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
            /* Adds the ENET device to a multicast group.*/
            ENET_AddMulticastGroup(ethernetif->base, multicastMacAddr);
            result = ERR_OK;
            break;
        case NETIF_DEL_MAC_FILTER:
            /*
             * Moves the ENET device from a multicast group.
             * Since the ENET_LeaveMulticastGroup() could filter out also other
             * group addresses having the same hash, the call is commented out.
             */
            /* ENET_LeaveMulticastGroup(ethernetif->base, multicastMacAddr); */
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
 * Initializes ENET driver.
 */
void ethernetif_enet_init(struct netif *netif, struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig)
{
    enet_config_t config;
    uint32_t sysClock;
    enet_buffer_config_t buffCfg[ENET_RING_NUM];

    /* prepare the buffer configuration. */
    buffCfg[0].rxBdNumber = ENET_RXBD_NUM;                      /* Receive buffer descriptor number. */
    buffCfg[0].txBdNumber = ENET_TXBD_NUM;                      /* Transmit buffer descriptor number. */
    buffCfg[0].rxBuffSizeAlign = sizeof(rx_buffer_t);           /* Aligned receive data buffer size. */
    buffCfg[0].txBuffSizeAlign = sizeof(tx_buffer_t);           /* Aligned transmit data buffer size. */
    buffCfg[0].rxBdStartAddrAlign = &(ethernetif->RxBuffDescrip[0]); /* Aligned receive buffer descriptor start address. */
    buffCfg[0].txBdStartAddrAlign = &(ethernetif->TxBuffDescrip[0]); /* Aligned transmit buffer descriptor start address. */
    buffCfg[0].rxBufferAlign = &(ethernetif->RxDataBuff[0][0]); /* Receive data buffer start address. */
    buffCfg[0].txBufferAlign = &(ethernetif->TxDataBuff[0][0]); /* Transmit data buffer start address. */

    sysClock = CLOCK_GetFreq(ethernetifConfig->clockName);

    ENET_GetDefaultConfig(&config);
    config.ringNum = ENET_RING_NUM;

    ethernetif_phy_init(ethernetif, ethernetifConfig, &config);

#if USE_RTOS && defined(FSL_RTOS_FREE_RTOS)
    uint32_t instance;
    static ENET_Type *const enetBases[] = ENET_BASE_PTRS;
    static const IRQn_Type enetTxIrqId[] = ENET_Transmit_IRQS;
    /*! @brief Pointers to enet receive IRQ number for each instance. */
    static const IRQn_Type enetRxIrqId[] = ENET_Receive_IRQS;
#if defined(ENET_ENHANCEDBUFFERDESCRIPTOR_MODE) && ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    /*! @brief Pointers to enet timestamp IRQ number for each instance. */
    static const IRQn_Type enetTsIrqId[] = ENET_1588_Timer_IRQS;
#endif /* ENET_ENHANCEDBUFFERDESCRIPTOR_MODE */

    /* Create the Event for transmit busy release trigger. */
    ethernetif->enetTransmitAccessEvent = xEventGroupCreate();
    ethernetif->txFlag = 0x1;

    config.interrupt |= kENET_RxFrameInterrupt | kENET_TxFrameInterrupt | kENET_TxBufferInterrupt;

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

    /* Initialize the ENET module.*/
    ENET_Init(ethernetif->base, &ethernetif->handle, &config, &buffCfg[0], netif->hwaddr, sysClock);

#if RX_FRAME_FITS_INTO_PBUF
    /* RX_FRAME_FITS_INTO_PBUF was set on assumption that ENET strips ethernet frame CRC, check it */
    LWIP_ASSERT("ENET set to forward received CRC", (ethernetif->base->RCR & ENET_RCR_CRCFWD_MASK));
#endif /* RX_FRAME_FITS_INTO_PBUF */

#if USE_RTOS && defined(FSL_RTOS_FREE_RTOS)
    ENET_SetCallback(&ethernetif->handle, ethernet_callback, netif);
#endif

    ENET_ActiveRead(ethernetif->base);
}

ENET_Type **ethernetif_enet_ptr(struct ethernetif *ethernetif)
{
    return &(ethernetif->base);
}

/**
 * Returns next buffer for TX.
 * Can wait if no buffer available.
 */
static unsigned char *enet_get_tx_buffer(struct ethernetif *ethernetif)
{
    static unsigned char ucBuffer[ENET_FRAME_MAX_FRAMELEN];
    return ucBuffer;
}

/**
 * Sends frame via ENET.
 */
static err_t enet_send_frame(struct ethernetif *ethernetif, unsigned char *data, const uint32_t length)
{
#if USE_RTOS && defined(FSL_RTOS_FREE_RTOS)
    {
        status_t result;

        do
        {
            result = ENET_SendFrame(ethernetif->base, &ethernetif->handle, data, length);

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

        for (counter = ENET_TIMEOUT; counter != 0U; counter--)
        {
            if (ENET_SendFrame(ethernetif->base, &ethernetif->handle, data, length) != kStatus_ENET_TxFrameBusy)
            {
                return ERR_OK;
            }
        }

        return ERR_TIMEOUT;
    }
#endif
}

struct pbuf *ethernetif_linkinput(struct netif *netif)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *p = NULL;
    uint32_t len;
    status_t status;

    /* Obtain the size of the packet and put it into the "len"
       variable. */
    status = ENET_GetRxFrameSize(&ethernetif->handle, &len);

    if (kStatus_ENET_RxFrameEmpty != status)
    {
        /* Call ENET_ReadFrame when there is a received frame. */
        if (len != 0)
        {
#if ETH_PAD_SIZE
            len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

            /* We allocate a pbuf chain of pbufs from the pool. */
            p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

            if (p != NULL)
            {
#if ETH_PAD_SIZE
                pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

#if RX_FRAME_FITS_INTO_PBUF
                LWIP_ASSERT("Frame does not fit into a single pbuf", p->next == 0);
                ENET_ReadFrame(ethernetif->base, &ethernetif->handle, p->payload, p->len);
#else
                if (p->next == 0) /* One-chain buffer.*/
                {
                    ENET_ReadFrame(ethernetif->base, &ethernetif->handle, p->payload, p->len);
                }
                else    /* Multi-chain buffer.*/
                {
                    uint8_t data_tmp[ENET_FRAME_MAX_FRAMELEN];
                    uint32_t data_tmp_len = 0;
                    struct pbuf *q;

                    ENET_ReadFrame(ethernetif->base, &ethernetif->handle, data_tmp, p->tot_len);

                    /* We iterate over the pbuf chain until we have read the entire
                    * packet into the pbuf. */
                    for (q = p; (q != NULL) && ((data_tmp_len + q->len) <= sizeof(data_tmp)); q = q->next)
                    {
                        /* Read enough bytes to fill this pbuf in the chain. The
                        * available data in the pbuf is given by the q->len
                        * variable. */
                        memcpy(q->payload,  &data_tmp[data_tmp_len], q->len);
                        data_tmp_len += q->len;
                    }
                }
#endif /* RX_FRAME_FITS_INTO_PBUF */

                MIB2_STATS_NETIF_ADD(netif, ifinoctets, p->tot_len);
                if (((u8_t *)p->payload)[0] & 1)
                {
                    /* broadcast or multicast packet*/
                    MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
                }
                else
                {
                    /* unicast packet*/
                    MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
                }
#if ETH_PAD_SIZE
                pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

                LINK_STATS_INC(link.recv);
            }
            else
            {
                /* drop packet*/
                ENET_ReadFrame(ethernetif->base, &ethernetif->handle, NULL, 0U);

                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_linkinput: Fail to allocate new memory space\n"));

                LINK_STATS_INC(link.memerr);
                LINK_STATS_INC(link.drop);
                MIB2_STATS_NETIF_INC(netif, ifindiscards);
            }
        }
        else
        {
            /* Update the received buffer when error happened. */
            if (status == kStatus_ENET_RxFrameError)
            {
#if 0 && defined(FSL_FEATURE_SOC_ENET_COUNT) && (FSL_FEATURE_SOC_ENET_COUNT > 0) /* Error statisctics */
        enet_data_error_stats_t eErrStatic;
        /* Get the error information of the received g_frame. */
        ENET_GetRxErrBeforeReadFrame(&ethernetif->handle, &eErrStatic);
#endif

                /* Update the receive buffer. */
                ENET_ReadFrame(ethernetif->base, &ethernetif->handle, NULL, 0U);

                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_linkinput: RxFrameError\n"));

                LINK_STATS_INC(link.drop);
                MIB2_STATS_NETIF_INC(netif, ifindiscards);
            }
        }
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

    pucBuffer = enet_get_tx_buffer(ethernetif);
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
        /* No pbuf chain, don't have to copy -> faster. */
        pucBuffer = (unsigned char *)p->payload;
    }
    else
    {
        /* pbuf chain, copy into contiguous ucBuffer. */
        if (p->tot_len > ENET_FRAME_MAX_FRAMELEN)
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
    result = enet_send_frame(ethernetif, pucBuffer, p->tot_len);

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
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t rxBuffDescrip_0[ENET_RXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t txBuffDescrip_0[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static rx_buffer_t rxDataBuff_0[ENET_RXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_0[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    ethernetif_0.RxBuffDescrip = &(rxBuffDescrip_0[0]);
    ethernetif_0.TxBuffDescrip = &(txBuffDescrip_0[0]);
    ethernetif_0.RxDataBuff = &(rxDataBuff_0[0]);
    ethernetif_0.TxDataBuff = &(txDataBuff_0[0]);

    return ethernetif_init(netif, &ethernetif_0, 0U, (ethernetif_config_t *)netif->state);
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
    SDK_ALIGN(static rx_buffer_t rxDataBuff_1[ENET_RXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);
    SDK_ALIGN(static tx_buffer_t txDataBuff_1[ENET_TXBD_NUM], FSL_ENET_BUFF_ALIGNMENT);

    ethernetif_1.RxBuffDescrip = &(rxBuffDescrip_1[0]);
    ethernetif_1.TxBuffDescrip = &(txBuffDescrip_1[0]);
    ethernetif_1.RxDataBuff = &(rxDataBuff_1[0]);
    ethernetif_1.TxDataBuff = &(txDataBuff_1[0]);

    return ethernetif_init(netif, &ethernetif_1, 1U, (ethernetif_config_t *)netif->state);
}
#endif /* FSL_FEATURE_SOC_*_ENET_COUNT */
