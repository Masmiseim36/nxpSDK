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
#include "virtual_nic_enetif.h"

#include "fsl_enet.h"
#include "fsl_phy.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_cdc_rndis.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "virtual_nic_enet_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void ENETIF_Input(enet_handle_t *handle, void *param);
void VNIC_EnetCallback(pbuf_t *pbuffer);
void VNIC_EnetPBufFree(pbuf_t *pbuf);
uint8_t *VNIC_EnetPBufAlloc(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
enet_handle_t g_handle;
extern uint8_t g_hwaddr[ENET_MAC_ADDR_SIZE];
#if defined(__ICCARM__)
#pragma data_alignment = ENET_BUFF_ALIGNMENT
#endif
__ALIGN_BEGIN enet_rx_bd_struct_t RxBuffDescrip[ENET_RXBD_NUM] __ALIGN_END;
#if defined(__ICCARM__)
#pragma data_alignment = ENET_BUFF_ALIGNMENT
#endif
__ALIGN_BEGIN enet_tx_bd_struct_t TxBuffDescrip[ENET_TXBD_NUM] __ALIGN_END;

#if defined(__ICCARM__)
#pragma data_alignment = ENET_BUFF_ALIGNMENT
#endif
__ALIGN_BEGIN uint8_t RxDataBuff[ENET_RXBD_NUM][ENET_BuffSizeAlign(ENET_RXBUFF_SIZE)] __ALIGN_END;

#if defined(__ICCARM__)
#pragma data_alignment = ENET_BUFF_ALIGNMENT
#endif
__ALIGN_BEGIN uint8_t TxDataBuff[ENET_TXBD_NUM][ENET_BuffSizeAlign(ENET_TXBUFF_SIZE)] __ALIGN_END;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Services an ethernet interrupt.
 *
 * @return none.
 */
void ENETIF_Callback(ENET_Type *base, enet_handle_t *handle, enet_event_t event, void *param)
{
    switch (event)
    {
        case kENET_RxEvent:
            ENETIF_Input(handle, param);
            break;
        default:
            break;
    }
}

/*!
 * @brief Get the ethernet speed.
 *
 * @return Value of ethernet speed in Mbps.
 */
uint32_t ENETIF_GetSpeed(void)
{
    uint32_t phyAddr = 0;
    bool link = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    uint32_t count = 0;
    while ((count < ENET_PHY_TIMEOUT) && (!link))
    {
        PHY_GetLinkStatus(ENET, phyAddr, &link);
        if (link)
        {
            /* Get the actual PHY link speed. */
            PHY_GetLinkSpeedDuplex(ENET, phyAddr, &speed, &duplex);
        }

        count++;
    }

    if (count == ENET_PHY_TIMEOUT)
    {
        usb_echo("\r\nPHY Link down, please check the cable connection.\r\n");
    }
    return (kPHY_Speed100M == speed ? 100 : 10);
}

/*!
 * @brief Get the ethernet link status.
 *
 * @return Ture if linked, otherwise false.
 */
bool ENETIF_GetLinkStatus(void)
{
    uint32_t phyAddr = 0;
    bool link = false;

    uint32_t count = 0;
    while ((count < ENET_PHY_TIMEOUT) && (!link))
    {
        PHY_GetLinkStatus(ENET, phyAddr, &link);

        count++;
    }

    if (count == ENET_PHY_TIMEOUT)
    {
        usb_echo("\r\nPHY Link down, please check the cable connection.\r\n");
    }
    return link;
}

/*!
 * @brief Transmit the packet.
 *
 * @return Error code.
 */

enet_err_t ENETIF_Output(pbuf_t *packetBuffer)
{
    if (packetBuffer->length >= ENET_FRAME_MAX_FRAMELEN)
    {
        return ENET_ERROR;
    }
    /* Send a frame out. */
    if (kStatus_Success == ENET_SendFrame(ENET, &g_handle, packetBuffer->payload, packetBuffer->length))
    {
        return ENET_OK;
    }
    return ENET_ERROR;
}

/*!
 * @brief Handle the receptions of packets from the network interface.
 *
 * @return none.
 */
void ENETIF_Input(enet_handle_t *handle, void *param)
{
    enet_header_t *ethhdr;
    status_t status;
    uint32_t length = 0;
    enet_data_error_stats_t eErrStatic;

    /* Read all data from ring buffer and send to uper layer */
    do
    {
        /* Get the Frame size */
        status = ENET_GetRxFrameSize(handle, &length);

        /* Call ENET_ReadFrame when there is a received frame. */
        if (length != 0)
        {
            pbuf_t packetBuffer;
            packetBuffer.payload = VNIC_EnetPBufAlloc();
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            if (packetBuffer.payload == NULL)
            {
                /* No packet buffer available, ignore this packet. */
                ENET_ReadFrame(ENET, handle, NULL, length);
                return;
            }
            packetBuffer.length = (uint16_t)length;
            ENET_ReadFrame(ENET, handle, packetBuffer.payload, packetBuffer.length);

            /* points to packet payload, which starts with an Ethernet header */
            ethhdr = (enet_header_t *)packetBuffer.payload;
            switch (USB_SHORT_FROM_BIG_ENDIAN(ethhdr->type))
            {
                /* IP or ARP packet? */
                case ETHTYPE_IP:
                case ETHTYPE_ARP:
                    /* send packet to application to process */
                    VNIC_EnetCallback(&packetBuffer);
                    break;

                default:
                    VNIC_EnetPBufFree(&packetBuffer);
                    break;
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

/*!
 * @brief Initialize the ethernet.
 *
 * @return Error code.
 */
enet_err_t ENETIF_Init(void)
{
    enet_err_t result = ENET_OK;
    enet_config_t config;
    uint32_t count = 0;
    uint32_t sysClock;
    uint32_t phyAddr = 0;
    phy_speed_t speed;
    phy_duplex_t duplex;
    bool link = false;

    /* initialize the hardware */
    /* set MAC hardware address */
    g_hwaddr[0] = configMAC_ADDR0;
    g_hwaddr[1] = configMAC_ADDR1;
    g_hwaddr[2] = configMAC_ADDR2;
    g_hwaddr[3] = configMAC_ADDR3;
    g_hwaddr[4] = configMAC_ADDR4;
    g_hwaddr[5] = configMAC_ADDR5;

    /* prepare the buffer configuration. */
    enet_buffer_config_t buffCfg = {
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        ENET_BuffSizeAlign(ENET_RXBUFF_SIZE),
        ENET_BuffSizeAlign(ENET_TXBUFF_SIZE),
        &RxBuffDescrip[0],
        &TxBuffDescrip[0],
        &RxDataBuff[0][0],
        &TxDataBuff[0][0],
    };

    sysClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    ENET_GetDefaultConfig(&config);
    PHY_Init(ENET, 0, sysClock);

    while ((count < ENET_PHY_TIMEOUT) && (!link))
    {
        PHY_GetLinkStatus(ENET, phyAddr, &link);
        if (link)
        {
            /* Get the actual PHY link speed. */
            PHY_GetLinkSpeedDuplex(ENET, phyAddr, &speed, &duplex);
            /* Change the MII speed and duplex for actual link status. */
            config.miiSpeed = (enet_mii_speed_t)speed;
            config.miiDuplex = (enet_mii_duplex_t)duplex;
            config.interrupt = kENET_RxFrameInterrupt;
        }

        count++;
    }

    if (count == ENET_PHY_TIMEOUT)
    {
        usb_echo("\r\nPHY Link down, please check the cable connection.\r\n");
    }

    ENET_Init(ENET, &g_handle, &config, &buffCfg, g_hwaddr, sysClock);
    ENET_SetCallback(&g_handle, ENETIF_Callback, NULL);

    ENET_ActiveRead(ENET);

    NVIC_SetPriority(ENET_Receive_IRQn, 6U);
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    NVIC_SetPriority(ENET_Transmit_IRQn, 6U);
    NVIC_SetPriority(ENET_1588_Timer_IRQn, 6U);
#endif
    return result;
}
