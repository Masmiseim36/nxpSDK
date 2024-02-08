/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "queue.h"
#include "eoe_ethernetif.h"
#include "netif/etharp.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
struct rxEntryDes
{
    /* Pointer to frame buffer type used by a TCP/IP stack */
    struct pbuf *pbuf;
    /* length of this buffer */
    uint16_t len;
};

struct txEntryDes
{
    /* Pointer to frame buffer type */
    void *buf;
    /* length of this buffer */
    uint16_t len;
};

static uint8_t mac_address[6] = {0x02, 0x01, 0x05, 0x40, 0x03, 0xe9};

static StaticQueue_t rxFrameQueue;

static StaticQueue_t txFrameQueue;

static StaticQueue_t frameBuffPool;

static struct netif *EoE_netif;

static uint8_t FrameBuffPoolMemory[FRAME_BUFF_NUM][FRAME_MAX_FRAMELEN];

static QueueHandle_t frameBuffPoolHandle = NULL;

static QueueHandle_t rxFrameQueueHandle = NULL;

QueueHandle_t txFrameQueueHandle = NULL;

uint8_t rxQueueStorageArea[RX_QUEUE_SIZE * sizeof(struct rxEntryDes)];

uint8_t txQueueStorageArea[TX_QUEUE_SIZE * sizeof(struct txEntryDes)];

uint8_t sendBuffPoolStorageArea[FRAME_BUFF_NUM * sizeof(struct txEntryDes)];

static TaskHandle_t EoE_ReceiveTaskHandle = NULL;

StaticTask_t EoE_ReceiveTaskBuffer;

StackType_t EoE_ReceiveTaskStack[STACK_SIZE];

uint8_t netif_macaddr[6];

ip_addr_t netif_ipaddr, netif_netmask, netif_gw;

void *EoE_FrameBuffAlloc(uint32_t size)
{
    struct txEntryDes buffEntry;
    if (size > FRAME_MAX_FRAMELEN) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to alloc frame buffer: size > FRAME_MAX_FRAMELEN\n"));
        return NULL;
    }
    if (xQueueReceive(frameBuffPoolHandle, &buffEntry, (TickType_t) 0) == pdPASS) {
        return buffEntry.buf;
    } else {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to alloc frame buffer!\n"));
        return NULL;
    }
}

void EoE_FrameBuffFree(void *buf)
{
    struct txEntryDes buffEntry;
    buffEntry.buf = buf;
    buffEntry.len = 0;
    if (buf != NULL) {
        if (xQueueSend(frameBuffPoolHandle, &buffEntry, (TickType_t) 0) != pdPASS) {
            LWIP_DEBUGF(NETIF_DEBUG, ("Failed to free frame buffer!\n"));
        }
    } else {
        LWIP_DEBUGF(NETIF_DEBUG, ("EoE_FrameBuffFree: buf is NULL!\n"));
    }
}

/**
 * EoE rxEntry initialization function
 *
 * @return ERR_OK if the rxEntry init success
 *         ERR_MEM if rxEntry init failed
 */
err_t EoE_RxEntryInit(struct rxEntryDes *rxEntry)
{
    struct pbuf *p = pbuf_alloc(PBUF_RAW, PBUF_POOL_BUFSIZE, PBUF_POOL);
    if (p != NULL) {
        rxEntry->pbuf = p;
        rxEntry->len = p->len;
        return ERR_OK;
    } else {
        rxEntry->pbuf = NULL;
        rxEntry->len = 0;
        return ERR_MEM;
    }
}

err_t EoE_PutFrameToRxQueue(uint16_t *pFrame, uint16_t frameSize)
{
    struct rxEntryDes rxEntry;
    if (EoE_RxEntryInit(&rxEntry)) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to init EoE rx entry!\n"));
        return ERR_MEM;
    }
    MEMCPY(rxEntry.pbuf->payload, pFrame, frameSize);
    rxEntry.len = frameSize;

    if (xQueueSend(rxFrameQueueHandle, (void *)&rxEntry, (TickType_t) 0) != pdPASS) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to send to rxFrameQueue!\n"));
        return ERR_MEM;
    }
    vTaskResume(EoE_ReceiveTaskHandle);
    return ERR_OK;
}

err_t EoE_GetFrameFromTxQueue(uint16_t **pFrame, uint16_t *frameSize)
{
    struct txEntryDes txEntry;
    if ((xQueueReceive(txFrameQueueHandle, (void *)&txEntry, (TickType_t) 0) != pdPASS))
    {
        return ERR_MEM;
    }
    *pFrame = txEntry.buf;
    *frameSize = txEntry.len;
    return ERR_OK;
}

struct netif *EoE_GetNetif(void)
{
    return EoE_netif;
}

err_t EoE_Output(struct netif *netif, struct pbuf *p)
{
    struct txEntryDes txEntry;
    int len = 0;
    int offset = 0;

    while (p != NULL) {
        txEntry.buf = EoE_FrameBuffAlloc(FRAME_MAX_FRAMELEN);
        if (txEntry.buf == NULL) {
            LWIP_DEBUGF(NETIF_DEBUG, ("Failed to alloc tx buffer!\n"));
            return ERR_BUF;
        }
        len = p->len - offset;
        if (len > FRAME_MAX_FRAMELEN) {
            len = FRAME_MAX_FRAMELEN;
        }
        MEMCPY(txEntry.buf, (int *)p->payload + offset, len);
        txEntry.len = len;
        if (xQueueSend(txFrameQueueHandle, (void *)&txEntry, (TickType_t) 0) != pdPASS) {
            LWIP_DEBUGF(NETIF_DEBUG, ("Failed to send to txFrameQueue!\n"));
            return ERR_TIMEOUT;
        }
        offset += len;
        if (p->len - offset <= 0) {
            if (p->next !=  NULL) {
                p = p->next;
                len = 0;
                offset =0;
            } else {
                break;
            }
        } 
    }
    return ERR_OK;
}

/**
 * EoE receive task
 * When the EoE_RecvFrame function sends the frame to rxFrameQueue, this 
 * method gets frame from rxFrameQueue and sends it to ethernet_input
 */
void EoE_ReceiveTask(void *arg) 
{
    while (1) {
        struct rxEntryDes rxEntry;
        if (xQueueReceive(rxFrameQueueHandle, &rxEntry, (TickType_t) 0) == pdPASS) {
            struct pbuf *p = rxEntry.pbuf;
            p->len = p->tot_len = rxEntry.len;
            if (rxEntry.pbuf != NULL) {
                /* pass all packets to ethernet_input, which decides what packets it supports */
                if (EoE_netif->input(p, EoE_netif) != (err_t)ERR_OK) {
                    LWIP_DEBUGF(NETIF_DEBUG, ("Failed to send frame to ethernet_input\n"));
                    pbuf_free(p);
                }
            }
        } else {
            vTaskSuspend(NULL);
        }
    }
}

/**
 * EoE frame buffer pool initialization function
 *
 * @return ERR_OK if the frame buffer pool is initialized
 *         ERR_MEM if txEntry couldn't be sent
 */
static err_t FrameBuffPoolInit(void)
{
    int i;
    struct txEntryDes txEntry;
    
    for ( i = 0; i < FRAME_BUFF_NUM; i++) {
        txEntry.len = 0;
        txEntry.buf = &FrameBuffPoolMemory[i];
        if (xQueueSend(frameBuffPoolHandle, &txEntry, (TickType_t) 0) != pdPASS) {
            LWIP_DEBUGF(NETIF_DEBUG, ("Failed to init frame buffer pool!\n"));
            return ERR_MEM;
        }
    }
    return ERR_OK;
}

err_t EoE_EthernetInit(void)
{
    rxFrameQueueHandle = xQueueCreateStatic(RX_QUEUE_SIZE, sizeof(struct rxEntryDes), rxQueueStorageArea, &rxFrameQueue);
    if (rxFrameQueueHandle == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to create rx frame queue!\n"));
        return ERR_BUF;
    }

    txFrameQueueHandle = xQueueCreateStatic(TX_QUEUE_SIZE, sizeof(struct txEntryDes), txQueueStorageArea, &txFrameQueue);
    if (txFrameQueueHandle == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to create tx frame queue!\n"));
        return ERR_BUF;
    }

    frameBuffPoolHandle = xQueueCreateStatic(FRAME_BUFF_NUM, sizeof(struct txEntryDes), sendBuffPoolStorageArea, &frameBuffPool);
    if (frameBuffPoolHandle == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to create frame buffer pool!\n"));
        return ERR_BUF;
    }

    if (FrameBuffPoolInit()) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to init frame buffer pool!\n"));
        return ERR_BUF;
    }
    
    /* create EoE receive task in RTOS */
    EoE_ReceiveTaskHandle = xTaskCreateStatic(EoE_ReceiveTask, "EoE_ReceiveTask", STACK_SIZE, NULL, EOE_RECEIVE_TASK_PRIORITY, 
                                                EoE_ReceiveTaskStack, &EoE_ReceiveTaskBuffer);

    if (EoE_ReceiveTaskHandle == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("Failed to create EoE receive task!\n"));
        return ERR_MEM;
    }
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
err_t EoE_netif_init(struct netif *netif)
{
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */
	
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    netif->output = etharp_output;
    netif->linkoutput = EoE_Output;
    netif->mtu = 1500;   /* maximum transfer unit */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;
    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;
    /* set MAC hardware address */
    memcpy(netif->hwaddr, mac_address, sizeof(netif->hwaddr));

    return ERR_OK;
}

err_t net_add_interface(struct netif *netif)
{
    EoE_netif = netif;
    /* Set default (zero) values */
    ip4_addr_set_zero(&netif_ipaddr);
    ip4_addr_set_zero(&netif_netmask);
    ip4_addr_set_zero(&netif_gw);

   /** 
    * Let lwIP TCP/IP thread initialise and add the interface.
    * The interface will be down until netif_set_up() is called.
    */
    netifapi_netif_add(EoE_netif, &netif_ipaddr, &netif_netmask, &netif_gw, NULL, EoE_netif_init, tcpip_input);

    return ERR_OK;
}
