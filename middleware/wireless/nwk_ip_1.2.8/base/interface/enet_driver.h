/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _ENET_DRIVER_H
#define _ENET_DRIVER_H
/*!=================================================================================================
\file       enet_driver.h
\brief      This is a header file for the Ethernet driver adapter.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"
#include "network_utils.h"

/*==================================================================================================
Public macros
==================================================================================================*/
#if FSL_FEATURE_ENET_SUPPORT_PTP
#define ENET_PTP_TXTS_RING_LEN          (25)
#define ENET_PTP_RXTS_RING_LEN          (25)
#define ENET_PTP_L2RING_LEN             (15)
#define ENET_PTP_SYNC_MSG               (0)
#define ENET_PTP_DELAYREQ_MSG           (1)
#define ENET_PTP_ETHERNET_LAYER2_TEST   (1)
#define ENET_IPV4_VERSION               (4)
#endif

#define ENET_RX_RING_LEN                (8)
#define ENET_TX_RING_LEN                (4)
#define ENET_RX_LARGE_BUFFER_NUM        (0)
#define ENET_MII_CLOCK                  (2500000L)
#if FSL_FEATURE_ENET_SUPPORT_PTP
#define ENET_PTP_RING_BUFFER_NUM        (30)
#endif

/*! @brief Define Error codes */
//#define DRIVER_ERROR_BASE             (0xA000)
#define ENET_ERROR_BASE                 (DRIVER_ERROR_BASE | 0x0400)
#define ENET_OK                         (0)
#define ENET_ERROR                      (ENET_ERROR_BASE | 0xff)  /* general ENET error */
#define ENETERR_INVALID_DEVICE          (ENET_ERROR_BASE | 0x00)   /* Device number out of range  */
#define ENETERR_INIT_DEVICE             (ENET_ERROR_BASE | 0x01)   /* Device already initialized  */
#define ENETERR_ALLOC_CFG               (ENET_ERROR_BASE | 0x02)   /* Alloc state failed          */
#define ENETERR_ALLOC_PCB               (ENET_ERROR_BASE | 0x03)   /* Alloc PCBs failed           */
#define ENETERR_ALLOC_BD                (ENET_ERROR_BASE | 0x04)   /* Alloc BDs failed            */
#define ENETERR_INSTALL_ISR             (ENET_ERROR_BASE | 0x05)   /* Install ISR failed          */
#define ENETERR_FREE_PCB                (ENET_ERROR_BASE | 0x06)   /* PCBs in use                 */
#define ENETERR_ALLOC_ECB               (ENET_ERROR_BASE | 0x07)   /* Alloc ECB failed            */
#define ENETERR_OPEN_PROT               (ENET_ERROR_BASE | 0x08)   /* Protocol not open           */
#define ENETERR_CLOSE_PROT              (ENET_ERROR_BASE | 0x09)   /* Protocol already open       */
#define ENETERR_SEND_SHORT              (ENET_ERROR_BASE | 0x0A)   /* Packet too short            */
#define ENETERR_SEND_LONG               (ENET_ERROR_BASE | 0x0B)   /* Packet too long             */
#define ENETERR_JOIN_MULTICAST          (ENET_ERROR_BASE | 0x0C)   /* Not a multicast address     */
#define ENETERR_ALLOC_MCB               (ENET_ERROR_BASE | 0x0D)   /* Alloc MCB failed            */
#define ENETERR_LEAVE_GROUP             (ENET_ERROR_BASE | 0x0E)   /* Not a joined group          */
#define ENETERR_SEND_FULL               (ENET_ERROR_BASE | 0x0F)   /* Transmit ring full          */
#define ENETERR_IP_TABLE_FULL           (ENET_ERROR_BASE | 0x10)   /* IP Table full of IP pairs   */
#define ENETERR_ALLOC                   (ENET_ERROR_BASE | 0x11)   /* Generic alloc failed        */
#define ENETERR_INIT_FAILED             (ENET_ERROR_BASE | 0x12)   /* Device failed to initialize */
#define ENETERR_DEVICE_TIMEOUT          (ENET_ERROR_BASE | 0x13)   /* Device read/write timeout   */
#define ENETERR_ALLOC_BUFFERS           (ENET_ERROR_BASE | 0x14)   /* Buffer alloc failed         */
#define ENETERR_ALLOC_MAC_CONTEXT       (ENET_ERROR_BASE | 0x15)   /* Buffer alloc failed         */
#define ENETERR_NO_TX_BUFFER            (ENET_ERROR_BASE | 0x16)   /* TX Buffer alloc failed      */
#define ENETERR_INVALID_INIT_PARAM      (ENET_ERROR_BASE | 0x17)   /* Invalid init. parameter     */
#define ENETERR_DEVICE_IN_USE           (ENET_ERROR_BASE | 0x18)   /* Shutdown failed, dev. in use*/
#define ENETERR_INITIALIZED_DEVICE      (ENET_ERROR_BASE | 0x19)   /* Device already initialized  */
#define ENETERR_INPROGRESS              (ENET_ERROR_BASE | 0x1A)   /* In Wifi Device Setting of ESSID in progress*/
#define ENETERR_1588_LWEVENT            (ENET_ERROR_BASE | 0x1B)   /* 1588driver lwevent creation failed */
#define ENETERR_INVALID_MODE            (ENET_ERROR_BASE | 0x1C)   /* Invalid mode for this ethernet driver */
#define ENETERR_INVALID_OPTION          (ENET_ERROR_BASE | 0x1D)   /* Invalid option for this ethernet driver */
#define ENETERR_SEND_LARGE              (ENET_ERROR_BASE | 0x1E)   /* Send packet large*/
#define ENETERR_INITIALIZED_MULTICAST   (ENET_ERROR_BASE | 0x1F) /* Multicast group already added*/
#define ENETERR_NULL_MULTICAST          (ENET_ERROR_BASE | 0x20) /* Multicast group NULL*/
#define ENETERR_INVALID_MULTICAST       (ENET_ERROR_BASE | 0x21) /* Invalid Multicast group*/
#define ENETERR_MIN                     (ENETERR_INVALID_DEVICE)
#define ENETERR_MAX                     (ENETERR_INVALID_OPTION)

#define ENET_ALIGN(x, align)            ((unsigned int)((x) + ((align)-1)) & (unsigned int)(~(unsigned int)((align)-1)))

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*!*************************************************************************************************
\fn     ENET_Initialize(uint8_t *pAddress, void **ppEnetHandle)
\brief  Initialize the chip.

\param  [in]    pAddress                Pointer to the Ethernet MAC address
\param  [out]   ppEnetHandle            Ethernet driver handle
\param  [in]    pTaskMsgQueue           Pointer to the task message queue

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Initialize(uint8_t *pAddress, void **ppEnetHandle, taskMsgQueue_t* pTaskMsgQueue);

/*!*************************************************************************************************
\fn     ENET_GetAddress(void *pEnetHandle, uint8_t *pAaddress)
\brief  Retrieves the Ethernet address of a device.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [out]   pAddress                Pointer to the buffer that will store the address
       
\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_GetAddress(void *pEnetHandle, uint8_t *pAddress);

/*!*************************************************************************************************
\fn     ENET_Open(void* enetHandle, int16_t protocol, void (*service)(uint8_t *, uint32_t),
        void* pPrivateData)
\brief  Register a protocol type on an Ethernet channel.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    protocol                The network layer protocol to register
\param  [in]    service                 The callback function
\param  [in]    pPrivateData            Private data for service function

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Open(void *pEnetHandle, uint16_t protocol, void (*service)(void*,uint8_t *,uint32_t),
    void *pPrivateData);
                   
/*!*************************************************************************************************
\fn     ENET_Close(void *pEnetHandle, uint16_t protocol)
\brief  Unregister a protocol type on an Ethernet channel.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    protocol                The network layer protocol to unregister
      
\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Close(void *pEnetHandle,  uint16_t protocol);

/*!*************************************************************************************************
\fn     ENET_Send(void *pEnetHandle, ipPktInfo_t *pPacket, uint16_t protocol, uint8_t *pDest,
        uint32_t flags)
\brief  Send a packet.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    pPacket                 Pointer to the packet that is to be sent
\param  [in]    protocol                The network layer protocol to send this packet with
\param  [in]    pDest                   Pointer to the destination Ethernet address
\param  [in]    flags                   optional flags, zero = default
      
\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Send(void *pEnetHandle, ipPktInfo_t *pPacket, uint16_t protocol, uint8_t *pDest,
    uint32_t flags);
                   
/*!*************************************************************************************************
\fn     ENET_GetMTU(void* enetHandle)
\brief  Get the maximum transmission unit.

\param  [in]    pEnetHandle             Ethernet driver handle
      
\retval         uint32_t                ENET MTU
***************************************************************************************************/
uint32_t ENET_GetMTU(void *pEnetHandle);

/*!*************************************************************************************************
\fn     ENET_Join(void *pEnetHandle, uint8_t *pAddress,uint16_t protocol)
\brief  Join a multicast group on an Ethernet channel.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    pAddress                Pointer to the multicast group
\param  [in]    protocol                The protocol for the multicast group(IPv4 or IPv6)

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Join(void* enetHandle, uint8_t* address, uint16_t protocol);

/*!*************************************************************************************************
\fn     ENET_Leave(void *pEnetHandle, uint8_t *pAddress, uint16_t protocol)
\brief  Leave a multicast group on an Ethernet channel.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    pAddress                Pointer to the multicast group
\param  [in]    protocol                The protocol for the multicast group(IPv4 or IPv6)

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Leave(void *pEnetHandle, uint8_t *pAddress, uint16_t protocol);

#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /*_VIRTUAL_ENET_DRIVER_H */
