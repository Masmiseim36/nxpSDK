/*
* Copyright (c) 2016, Freescale Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_cdc_rndis.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "virtual_nic_enetif.h"
#include "virtual_nic_enet_adapter.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if (!FSL_FEATURE_SOC_ENET_COUNT)
#error This application requires FSL_FEATURE_SOC_ENET_COUNT defined non-zero.
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ENET_RXRTCSBUFF_NUM (6)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
enet_err_t ENETIF_Init(void);
enet_err_t ENETIF_Output(pbuf_t *packetBuffer);
uint32_t ENETIF_GetSpeed(void);
bool ENETIF_GetLinkStatus(void);
void VNIC_EnetPBufFree(pbuf_t *pbuf);

/*******************************************************************************
* Variables
******************************************************************************/
extern usb_cdc_vnic_t g_cdcVnic;
/* Ethernet queue for nic device. */
queue_t g_enetServiceQueue;
/* Buffer queue of the ethernet queue. */
static vnic_enet_transfer_t s_enetQueueBuf[ENET_QUEUE_MAX];
/* Enet data bufffer queue. */
static uint8_t *s_dataBuffQue;
/* Enet data bufffer. */
static uint8_t s_dataBuffer[ENET_RXRTCSBUFF_NUM][ENET_FRAME_MAX_FRAMELEN] = {{0}};
/* Enet available data buffer count . */
static uint32_t s_dataBufferFreeCnt = 0;
/* Mac address of device. */
uint8_t g_hwaddr[ENET_MAC_ADDR_SIZE];

/*******************************************************************************
* Code
******************************************************************************/
/*!
 * @brief Enqueue the packet buffer.
 *
 * @return none.
 */
static inline void VNIC_EnetEnqueueBuffer(void **queue, void *buffer)
{
    bool isFree = false;
    void *temp;

    USB_DEVICE_VNIC_ENTER_CRITICAL();
    temp = *queue;
    /* If the packet buffer already in free list, then ignore. */
    while (NULL != temp)
    {
        if (temp == buffer)
        {
            isFree = true;
            break;
        }
        temp = *(void **)temp;
    }
    if (false == isFree)
    {
        *((void **)buffer) = *queue;
        *queue = buffer;
        s_dataBufferFreeCnt++;
    }
    USB_DEVICE_VNIC_EXIT_CRITICAL();
}

/*!
 * @brief Dequeue the packet buffer.
 *
 * @return The address of the available packet buffer.
 */
static inline void *VNIC_EnetDequeueBuffer(void **queue)
{
    void *buffer;

    USB_DEVICE_VNIC_ENTER_CRITICAL();
    buffer = *queue;

    if (buffer)
    {
        *queue = *((void **)buffer);
    }
    if (NULL != buffer)
    {
        s_dataBufferFreeCnt--;
    }
    USB_DEVICE_VNIC_EXIT_CRITICAL();
    return buffer;
}

/*!
 * @brief Allocate the packet buffer.
 *
 * @return The address of the available packet buffer.
 */
uint8_t *VNIC_EnetPBufAlloc(void)
{
    uint8_t *ret = NULL;
    ret = VNIC_EnetDequeueBuffer((void **)&s_dataBuffQue);
    return (uint8_t *)ret;
}

/*!
 * @brief Recycle the packet buffer.
 *
 * @return none.
 */
void VNIC_EnetPBufFree(pbuf_t *pbuf)
{
    VNIC_EnetEnqueueBuffer((void **)&s_dataBuffQue, pbuf->payload);
}

/*!
 * @brief Services an IP packet.
 *
 * @return ENET or error code.
 */
void VNIC_EnetCallback(pbuf_t *pbuffer)
{
    usb_status_t error = kStatus_USB_Error;
    vnic_enet_transfer_t cdcAcmTransfer;
    cdcAcmTransfer.buffer = pbuffer->payload;
    cdcAcmTransfer.length = pbuffer->length;

    error = VNIC_EnetQueuePut(&g_enetServiceQueue, &cdcAcmTransfer);
    if (kStatus_USB_Success != error)
    {
        error = kStatus_USB_Busy;
        VNIC_EnetPBufFree(pbuffer);
    }
    else
    {
        g_cdcVnic.nicTrafficInfo.enetRxEnet2usb++;
    }
    return;
}

/*!
 * @brief Initialize the ethernet module.
 *
 * @return Error code.
 */
uint32_t VNIC_EnetInit(void)
{
    uint32_t error;
    uint32_t count;
    g_enetServiceQueue.qArray = s_enetQueueBuf;

    VNIC_EnetQueueInit(&g_enetServiceQueue, ENET_QUEUE_MAX);

    /* Initialize the packet buffer */
    for (count = 0; count < ENET_RXRTCSBUFF_NUM; count++)
    {
        VNIC_EnetEnqueueBuffer((void **)&s_dataBuffQue, &s_dataBuffer[count]);
    }
#if ((defined FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

    error = ENETIF_Init();
    if (error != ENET_OK)
    {
        usb_echo("ENET_Initialization Failed\n");
        return ENET_ERROR;
    }
    return error;
}

/*!
 * @brief Send packets to Ethernet module.
 *
 * @return Error code.
 */
enet_err_t VNIC_EnetSend(uint8_t *buf, uint32_t len)
{
    enet_err_t error;
    pbuf_t packetBuffer;
    packetBuffer.length = len;
    packetBuffer.payload = buf;
    error = ENETIF_Output(&packetBuffer);
    return (enet_err_t)error;
}

/*!
 * @brief Get the ethernet speed.
 *
 * @return Value of ethernet speed.
 */
uint32_t VNIC_EnetGetSpeed(void)
{
    return ENETIF_GetSpeed();
}

/*!
 * @brief Get the ethernet link status.
 *
 * @return Ture if linked, otherwise false.
 */
bool VNIC_EnetGetLinkStatus(void)
{
    return ENETIF_GetLinkStatus();
}
/*!
 * @brief Clear the transfer requests in enet queue.
 *
 * @return Error code.
 */
usb_status_t VNIC_EnetClearEnetQueue(void)
{
    usb_status_t error = kStatus_USB_Success;
    vnic_enet_transfer_t cdcAcmTransfer;
    pbuf_t enetPbuf;
    while (kStatus_USB_Success == error)
    {
        error = VNIC_EnetQueueGet(&g_enetServiceQueue, &cdcAcmTransfer);
        if (kStatus_USB_Success == error)
        {
            enetPbuf.payload = cdcAcmTransfer.buffer;
            enetPbuf.length = cdcAcmTransfer.length;
            VNIC_EnetPBufFree(&enetPbuf);
            g_cdcVnic.nicTrafficInfo.enetRxUsb2hostCleared++;
        }
    }
    return error;
}
