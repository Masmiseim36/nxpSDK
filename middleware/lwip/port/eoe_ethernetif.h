/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EOE_ETHERNETIF_H___
#define _EOE_ETHERNETIF_H___

#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "lwipopts.h"
#include "lwip/err.h"

#define IFNAME0 'e'
#define IFNAME1 'c'
#ifndef FRAME_MAX_FRAMELEN
#define FRAME_MAX_FRAMELEN (1536U)
#endif
#ifndef STACK_SIZE
#define STACK_SIZE 256
#endif
#ifndef EOE_RECEIVE_TASK_PRIORITY
#define EOE_RECEIVE_TASK_PRIORITY 3
#endif
#ifndef RX_QUEUE_SIZE
#define RX_QUEUE_SIZE 30
#endif
#ifndef TX_QUEUE_SIZE
#define TX_QUEUE_SIZE 30
#endif
#ifndef FRAME_BUFF_NUM
#define FRAME_BUFF_NUM 40
#endif

/**
 * This function is to apply for an EoE buffer to transfer data
 *
 * @param size Size of buffer
 * 
 * @return Pointer to buffer if success
 *         NULL if failure
 */
void *EoE_FrameBuffAlloc(uint32_t size);

/**
 * This function is to free EoE buffer
 *
 * @param pointer Pointer to buffer
 */
void EoE_FrameBuffFree(void *buf);

/**
 * EoE network port initialization function
 *
 * @return ERR_OK if the EoE network port is initialized
 *         ERR_MEM if EoE receive task couldn't be created
 *         ERR_BUF if frame buffer queue couldn't be created
 */
err_t EoE_EthernetInit(void);

/**
 * This function is called by EtherCAT slave stack if a new Ethernet frame is received
 *
 * @param pFrame to the received Ethernet frame
 * @param length length of the received frame
 * 
 * @return ERR_OK if rx frame send success
 *         ERR_MEM if rx frame couldn't be sent
 */
err_t EoE_PutFrameToRxQueue(uint16_t *pFrame, uint16_t frameSize);

/**
 * This function is called by EtherCAT slave stack to get Ethernet frame from EoE driver
 * 
 * @param pFrame send the buf of the Ethernet frame
 * @param length length of the send frame
 * 
 * @return ERR_OK if tx frame receive success
 *         ERR_MEM if tx frame couldn't be received
 */
err_t EoE_GetFrameFromTxQueue(uint16_t **pFrame, uint16_t *frameSize);

/**
 * EoE get global netif variable function
 *
 * @return netif if the global netif geted
 */
struct netif *EoE_GetNetif(void);

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
err_t net_add_interface(struct netif *netif);
#endif
/** @}*/
