/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
\file       enet_driver.c
\brief      This is a private source file for the Ethernet driver adapter.

==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "fsl_enet.h"
#include "fsl_phy.h"
#include "fsl_os_abstraction.h"
#include "pin_mux.h"

#include "enet_driver.h"

#if FSL_RTOS_MQX
#include "bsp_config.h"
#endif

#include "MemManager.h"
#include "FunctionLib.h"

#define ENET_INSTANCE_COUNT         (1)

#ifndef DRIVER_ERROR_BASE
    #define DRIVER_ERROR_BASE       (0x00010000ul)
#endif

#define ENET_RXBD_NUM               (8)
#define ENET_TXBD_NUM               (4)
#define ENET_EXTRXBD_NUM            (4)
#define ENET_RXBUFF_SIZE            (kEnetMaxFrameSize)
#define ENET_TXBUFF_SIZE            (kEnetMaxFrameSize)
#define ENET_RXLARGEBUFF_NUM        (4)
#define ENET_RX_BUFFER_ALIGNMENT    (16)
#define ENET_TX_BUFFER_ALIGNMENT    (16)
#define ENET_BD_ALIGNMENT           (16)
#define ENET_RXBuffSizeAlign(n)     ENET_ALIGN(n, ENET_RX_BUFFER_ALIGNMENT)
#define ENET_TXBuffSizeAlign(n)     ENET_ALIGN(n, ENET_TX_BUFFER_ALIGNMENT)

#ifndef kEnetMaxFrameSize
    #define kEnetMaxFrameSize       ENET_FRAME_MAX_FRAMELEN
#endif

#define ENET_MAX_MTU                1500U

#define ENET_MAC_ADDR_SIZE          (6U)

extern uint32_t gaUniqueId[4];
/*==================================================================================================
Private macros
==================================================================================================*/
#define ENET_DEVICE_NB              (0)
#define DEVICE_MAC_ADDRESS          0x00, 0x60, 0x37, 0x00, 0xFA, 0x5D
#define ENET_PHY_TIMEOUT            (0xFFFFU)

/*==================================================================================================
Private type definitions
==================================================================================================*/

/* Ethernet header. */
typedef struct _enet_header
{
    uint8_t dstAddr[ENET_MAC_ADDR_SIZE];
    uint8_t srcAddr[ENET_MAC_ADDR_SIZE];
    uint8_t type[2];
} enet_header_t;

/*! @brief Define ECB structure contains protocol type and it's related service function*/
typedef struct enetServiceStruct_tag
{
    uint16_t  protocol;
    void (*service)(void*,uint8_t *, uint32_t);
    void *privateData;
    struct enetServiceStruct_tag *next;
} enetServiceStruct_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/
static uint32_t ENET_buffer_init(uint8_t devNumber, enet_buffer_config_t *pBuffCfg);
static void ENETIF_ISR(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param);
static void ENETIF_Input(void *pParam);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
enet_handle_t m_Enethandle;
uint8_t gEnetMacHwAddr[] = {DEVICE_MAC_ADDRESS};
enet_config_t gEnetMacCfg[ENET_INSTANCE_COUNT] =
{{
    .macSpecialConfig = 0,
    .interrupt = 0,
    .rxMaxFrameLen = ENET_FRAME_MAX_FRAMELEN,
    .miiMode = kENET_RmiiMode,
    .miiSpeed = kENET_MiiSpeed100M,
    .miiDuplex = kENET_MiiFullDuplex,
    .rxAccelerConfig = 0,
    .txAccelerConfig = 0,
    .pauseDuration = 0,
    .rxFifoEmptyThreshold = 0,
#if FSL_FEATURE_ENET_HAS_RECEIVE_STATUS_THRESHOLD
    .rxFifoStatEmptyThreshold = 0,
#endif
    .rxFifoFullThreshold = 0,
    .txFifoWatermark = 0
}};
osaMutexId_t enetContextSync;     /*!< Sync signal*/

uint8_t *txBdPtr[ENET_INSTANCE_COUNT], *rxBdPtr[ENET_INSTANCE_COUNT];
uint8_t *txBuffer[ENET_INSTANCE_COUNT], *rxBuffer[ENET_INSTANCE_COUNT];
uint8_t *rxExtBuffer[ENET_INSTANCE_COUNT];
taskMsgQueue_t *mpTaskMsgQueue;

#if FSL_FEATURE_ENET_SUPPORT_PTP && 0
enet_mac_ptp_ts_data_t ptpTsRxData[ENET_PTP_RXTS_RING_LEN];
enet_mac_ptp_ts_data_t ptpTsTxData[ENET_PTP_TXTS_RING_LEN];
#endif

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     ENET_GetAddress(void* enetHandle, uint8_t* address)
\brief  Initialize ENET buffers.

\param  [in]    devNumber               handle to Ethernet driver
\param  [out]   pBuffCfg                pointer to buffer configuration

\retval         uint32_t                ENET_OK
                                        error code
***************************************************************************************************/
static uint32_t ENET_buffer_init
(
    uint8_t devNumber,
    enet_buffer_config_t *pBuffCfg
)
{
    uint32_t rxBufferSizeAlign, txBufferSizeAlign;
    uint8_t  *txBufferAlign, *rxBufferAlign;
    volatile enet_tx_bd_struct_t *txBdPtrAlign;
    volatile enet_rx_bd_struct_t *rxBdPtrAlign;

    /* Check input parameter*/
    if(!pBuffCfg)
    {
        return kStatus_InvalidArgument;
    }

    /* Allocate ENET receive buffer descriptors*/
    txBdPtr[devNumber] = (uint8_t *)MEM_BufferAllocForever(ENET_TXBD_NUM * sizeof(enet_tx_bd_struct_t) + ENET_BD_ALIGNMENT, 0);
    if (!txBdPtr[devNumber])
    {
        return kStatus_Fail;
    }
    FLib_MemSet(txBdPtr[devNumber], 0, ENET_TXBD_NUM * sizeof(enet_tx_bd_struct_t) + ENET_BD_ALIGNMENT);
    txBdPtrAlign = (volatile enet_tx_bd_struct_t *)ENET_ALIGN((uint32_t)txBdPtr[devNumber], ENET_BD_ALIGNMENT);

    rxBdPtr[devNumber] = (uint8_t *)MEM_BufferAllocForever(ENET_RXBD_NUM * sizeof(enet_rx_bd_struct_t) + ENET_BD_ALIGNMENT, 0);
    if(!rxBdPtr[devNumber])
    {
        MEM_BufferFree(txBdPtr[devNumber]);
        return kStatus_Fail;
    }
    FLib_MemSet(rxBdPtr[devNumber], 0, ENET_RXBD_NUM * sizeof(enet_rx_bd_struct_t) + ENET_BD_ALIGNMENT);
    rxBdPtrAlign = (volatile enet_rx_bd_struct_t *)ENET_ALIGN((uint32_t)rxBdPtr[devNumber], ENET_BD_ALIGNMENT);

    /* Allocate the transmit and receive date buffers*/
    rxBufferSizeAlign = ENET_RXBuffSizeAlign(ENET_RXBUFF_SIZE);
    rxBuffer[devNumber] = (uint8_t *)MEM_BufferAllocForever(ENET_RXBD_NUM * rxBufferSizeAlign  + ENET_RX_BUFFER_ALIGNMENT, 0);
    if (!rxBuffer[devNumber])
    {
        MEM_BufferFree(txBdPtr[devNumber]);
        MEM_BufferFree(rxBdPtr[devNumber]);
        return kStatus_Fail;
    }
    FLib_MemSet(rxBuffer[devNumber], 0, ENET_RXBD_NUM * rxBufferSizeAlign  + ENET_RX_BUFFER_ALIGNMENT);
    rxBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)rxBuffer[devNumber], ENET_RX_BUFFER_ALIGNMENT);

    txBufferSizeAlign = ENET_TXBuffSizeAlign(ENET_TXBUFF_SIZE);
    txBuffer[devNumber] = (uint8_t *)MEM_BufferAllocForever(ENET_TXBD_NUM * txBufferSizeAlign + ENET_TX_BUFFER_ALIGNMENT, 0);
    if (!txBuffer[devNumber])
    {
        MEM_BufferFree(txBdPtr[devNumber]);
        MEM_BufferFree(rxBdPtr[devNumber]);
        MEM_BufferFree(rxBuffer[devNumber]);
        return kStatus_Fail;
    }
    FLib_MemSet(txBuffer[devNumber], 0, ENET_TXBD_NUM * txBufferSizeAlign + ENET_TX_BUFFER_ALIGNMENT);
    txBufferAlign = (uint8_t *)ENET_ALIGN((uint32_t)txBuffer[devNumber], ENET_TX_BUFFER_ALIGNMENT);

#if FSL_FEATURE_ENET_SUPPORT_PTP && 0
    buffCfgPtr->ptpTsRxDataPtr = &ptpTsRxData[0];
    buffCfgPtr->ptpTsRxBuffNum = ENET_PTP_RXTS_RING_LEN;
    buffCfgPtr->ptpTsTxDataPtr = &ptpTsTxData[0];
    buffCfgPtr->ptpTsTxBuffNum = ENET_PTP_TXTS_RING_LEN;
#endif

    pBuffCfg->rxBdNumber = ENET_RXBD_NUM;
    pBuffCfg->txBdNumber = ENET_TXBD_NUM;

    pBuffCfg->rxBuffSizeAlign = rxBufferSizeAlign;
    pBuffCfg->txBuffSizeAlign = txBufferSizeAlign;

    pBuffCfg->rxBdStartAddrAlign = rxBdPtrAlign;
    pBuffCfg->txBdStartAddrAlign = txBdPtrAlign;

    pBuffCfg->rxBufferAlign = rxBufferAlign;
    pBuffCfg->txBufferAlign = txBufferAlign;

    return kStatus_Success;
}

/*!*************************************************************************************************
\private
\fn     ENETIF_ISR(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param)
\brief  Driver callback.

\param  [in]    base                    handle to Ethernet driver
\param  [in]    handle                  mac address
\param  [in]    event                   enet event type
\param  [in]    pParam                  pointer to parameters

\return         void
***************************************************************************************************/
static void ENETIF_ISR
(
    ENET_Type *base,
    enet_handle_t *handle,
    enet_event_t event,
    void *param
)
{
    switch (event)
    {
        case kENET_RxEvent:
            (void)NWKU_SendMsg(ENETIF_Input, (void*)handle, mpTaskMsgQueue);
            break;
        default:
            break;
    }
}


/*!*************************************************************************************************
\private
\fn     ENETIF_Input(void *param)
\brief  RX handler.

\param  [in]    pParam                  pointer to parameters

\return         void
***************************************************************************************************/
static void ENETIF_Input
(
    void *pParam
)
{
    enet_header_t *pEthHdr;
    status_t status;
    uint32_t length = 0;
    enet_data_error_stats_t eErrStatic;
    enetServiceStruct_t* serviceStructPtr;
    uint16_t type;
    enet_handle_t *handle = (enet_handle_t *)pParam;

    /* Read all data from ring buffer and send to uper layer */
    do
    {
        /* Get the Frame size */
        status = ENET_GetRxFrameSize(handle, &length);

        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
            uint8_t *pPayload = MEM_BufferAlloc(length);
            bool_t found = FALSE;

            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            if (pPayload == NULL)
            {
                /* No packet buffer available, ignore this packet. */
                ENET_ReadFrame(ENET, handle, NULL, length);
                return;
            }

            ENET_ReadFrame(ENET, handle, pPayload, length);

            pEthHdr = (enet_header_t*)pPayload;
            type = ntohas(pEthHdr->type);
            for(serviceStructPtr = (enetServiceStruct_t*)handle->userData; serviceStructPtr;
                serviceStructPtr = serviceStructPtr->next)
            {
                if(serviceStructPtr->protocol == type)
                {
                    serviceStructPtr->service(serviceStructPtr->privateData, pPayload, length);
                    found = TRUE;
                    break;
                }
            }

            if (found == FALSE)
            {
                MEM_BufferFree(pPayload);
            }
        }
        else
        {
            /* Update the received buffer when error happened. */
            if (status != kStatus_Success)
            {
                /* Get the error information of the received g_frame. */
                ENET_GetRxErrBeforeReadFrame(handle, &eErrStatic);
                /* update the receive buffer. */
                ENET_ReadFrame(ENET, handle, NULL, 0);
            }
        }
    } while (kStatus_ENET_RxFrameEmpty != status);
}

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     ENET_Initialize(uint8_t *pAddress, void **ppEnetHandle)
\brief  Initialize the chip.

\param  [in]    pAddress                Pointer to the Ethernet MAC address
\param  [out]   ppEnetHandle            Ethernet driver handle
\param  [in]    pTaskMsgQueue           Pointer to the task message queue

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Initialize
(
    uint8_t *pAddress,
    void **pEnetHandle,
    taskMsgQueue_t* pTaskMsgQueue
)
{
    uint32_t result = ENET_OK;
    uint32_t count = 0;
    uint32_t sysClock;
    uint32_t phyAddr = 0;
    phy_speed_t speed;
    phy_duplex_t duplex;
    bool link = false;
    enet_buffer_config_t buffCfg;
    uint8_t *uniqueIdPtr = (uint8_t *)gaUniqueId;
        
    *pEnetHandle = &m_Enethandle;
    mpTaskMsgQueue = pTaskMsgQueue;

    if(NULL != pAddress)
    {
        FLib_MemCpy(gEnetMacHwAddr, pAddress, sizeof(gEnetMacHwAddr));
    }

    if(gEnetMacHwAddr[3] == 0xFF && 
       gEnetMacHwAddr[4] == 0xFF && 
       gEnetMacHwAddr[5] == 0xFF)
    {
        gEnetMacHwAddr[3] = uniqueIdPtr[8];
        gEnetMacHwAddr[4] = uniqueIdPtr[12];
        gEnetMacHwAddr[5] = uniqueIdPtr[15];       
    }

    BOARD_InitENET();

    /* prepare the buffer configuration. */
    (void)ENET_buffer_init(0, &buffCfg);

    /* initialize the hardware */
    sysClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    PHY_Init(ENET, 0, sysClock);

    /* Create sync signal*/
    enetContextSync = OSA_MutexCreate();
    while ((count < ENET_PHY_TIMEOUT) && (!link))
    {
        PHY_GetLinkStatus(ENET, phyAddr, &link);
        if (link)
        {
            /* Get the actual PHY link speed. */
            PHY_GetLinkSpeedDuplex(ENET, phyAddr, &speed, &duplex);

            /* Change the MII speed and duplex for actual link status. */
            gEnetMacCfg[0].miiSpeed = (enet_mii_speed_t)speed;
            gEnetMacCfg[0].miiDuplex = (enet_mii_duplex_t)duplex;
            gEnetMacCfg[0].interrupt = kENET_RxFrameInterrupt;
        }

        count++;
    }

    ENET_Init(ENET, &m_Enethandle, &gEnetMacCfg[0], &buffCfg, gEnetMacHwAddr, sysClock);
    ENET_SetCallback(&m_Enethandle, ENETIF_ISR, NULL);
    ENET_ActiveRead(ENET);
    NVIC_SetPriority(ENET_Receive_IRQn, 6U);

    return result;
}

/*!*************************************************************************************************
\fn     ENET_GetAddress(void *pEnetHandle, uint8_t *pAaddress)
\brief  Retrieves the Ethernet address of a device.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [out]   pAddress                Pointer to the buffer that will store the address

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_GetAddress
(
      void *pEnetHandle,
      uint8_t *pAddress
)
{
    FLib_MemCpy(pAddress, gEnetMacHwAddr, 6);
    return ENET_OK;
}

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
uint32_t ENET_Open
(
    void *pEnetHandle,
    uint16_t protocol,
    void (*service)(void*, uint8_t *, uint32_t),
    void *pPrivateData
)
{
    enet_handle_t *pHandle = (enet_handle_t*)pEnetHandle;
    enetServiceStruct_t* serviceStructPtr;
    enetServiceStruct_t** searchPtr;
    uint32_t returnStatus = ENET_OK;

    /* Check input parameter*/
    if (pEnetHandle != NULL)
    {
        for(searchPtr=(enetServiceStruct_t **)(&pHandle->userData); *searchPtr; searchPtr=&(*searchPtr)->next)
        {
            if ((*searchPtr)->protocol == protocol)
            {
                returnStatus = ENETERR_OPEN_PROT;
                break;
            }
        }
        if(ENET_OK == returnStatus)
        {
            serviceStructPtr = (enetServiceStruct_t*)MEM_BufferAllocForever(sizeof(enetServiceStruct_t), 0);
            if (NULL != serviceStructPtr)
            {
                serviceStructPtr->protocol = protocol;
                serviceStructPtr->service = service;
                serviceStructPtr->privateData = pPrivateData;
                serviceStructPtr->next = NULL;
                *searchPtr = serviceStructPtr;
            }
            else
            {
                returnStatus = ENETERR_ALLOC_ECB;
            }
        }
    }
    else
    {
        returnStatus = ENETERR_INVALID_DEVICE;
    }

    return returnStatus;
}

/*!*************************************************************************************************
\fn     ENET_Close(void *pEnetHandle, uint16_t protocol)
\brief  Unregister a protocol type on an Ethernet channel.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    protocol                The network layer protocol to unregister

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Close
(
    void* enetHandle,
    uint16_t protocol
)
{
    return ENET_OK;
}

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
uint32_t ENET_Send
(
    void *pEnetHandle,
    ipPktInfo_t *pPacket,
    uint16_t protocol,
    uint8_t *pDest,
    uint32_t flags
)
{
    uint16_t frameLen;
    enet_header_t *pFrame;
    uint32_t retVal = ENET_ERROR;

    frameLen = NWKU_NwkBufferTotalSize(pPacket->pNwkBuff) + sizeof(enet_header_t);
    if (frameLen < ENET_FRAME_MAX_FRAMELEN)
    {
        pFrame = NWKU_MEM_BufferAlloc(frameLen);
        if (pFrame != NULL)
        {
            FLib_MemCpy(pFrame->dstAddr, pDest, ENET_MAC_ADDR_SIZE);
            FLib_MemCpy(pFrame->srcAddr, gEnetMacHwAddr, ENET_MAC_ADDR_SIZE);
            htonas(pFrame->type, protocol);
            NWKU_NwkBufferToRegularBuffer(pPacket->pNwkBuff, (uint8_t*)(pFrame + 1));

            /* Send a frame out. */
            if (kStatus_Success == ENET_SendFrame(ENET, pEnetHandle, (uint8_t*)pFrame, frameLen))
            {
                retVal = ENET_OK;
            }

            MEM_BufferFree(pFrame);
        }
    }

    NWKU_FreeIpPktInfo(&pPacket);
    return retVal;
}


/*!*************************************************************************************************
\fn     ENET_GetMTU(void* enetHandle)
\brief  Get the maximum transmission unit.

\param  [in]    pEnetHandle             Ethernet driver handle

\retval         uint32_t                ENET MTU
***************************************************************************************************/
uint32_t ENET_GetMTU
(
    void* enetHandle
)
{
    enet_handle_t *pEnetHandle = (enet_handle_t*)enetHandle;

    /* Check input parameter*/
    if (pEnetHandle == NULL)
    {
        return ENETERR_INVALID_DEVICE;
    }
    
    return ENET_MAX_MTU;

}

/*!*************************************************************************************************
\fn     ENET_Join(void *pEnetHandle, uint8_t *pAddress,uint16_t protocol)
\brief  Join a multicast group on an Ethernet channel.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    pAddress                Pointer to the multicast group
\param  [in]    protocol                The protocol for the multicast group(IPv4 or IPv6)

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Join
(
    void *pEnetHandle,
    uint8_t *pAddress,
    uint16_t protocol
)
{
    /* Make sure it's a multicast group*/
    if (!(pAddress[0] & 1U))
    {
        return ENETERR_INVALID_MULTICAST;
    }

    OSA_MutexLock(enetContextSync, osaWaitForever_c);
    ENET_AddMulticastGroup(ENET, pAddress);
    OSA_MutexUnlock(enetContextSync);

    return ENET_OK;
}

/*!*************************************************************************************************
\fn     ENET_Leave(void *pEnetHandle, uint8_t *pAddress, uint16_t protocol)
\brief  Leave a multicast group on an Ethernet channel.

\param  [in]    pEnetHandle             Ethernet driver handle
\param  [in]    pAddress                Pointer to the multicast group
\param  [in]    protocol                The protocol for the multicast group(IPv4 or IPv6)

\return         uint32_t                ENET_OK or error code
***************************************************************************************************/
uint32_t ENET_Leave
(
      void *pEnetHandle,
      uint8_t *pAddress,
      uint16_t protocol
)
{
    /* Make sure it's a multicast group*/
    if (!(pAddress[0] & 1U))
    {
        return ENETERR_INVALID_MULTICAST;
    }

    OSA_MutexLock(enetContextSync, osaWaitForever_c);
    ENET_LeaveMulticastGroup(ENET, pAddress);
    OSA_MutexUnlock(enetContextSync);

    return ENET_OK;
}

/*================================================================================================*/

/*==================================================================================================
Private debug functions
==================================================================================================*/
