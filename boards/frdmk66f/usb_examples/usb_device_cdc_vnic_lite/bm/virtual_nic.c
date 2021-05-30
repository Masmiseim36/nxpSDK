/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
/*${standard_header_anchor}*/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_cdc_acm.h"
#include "usb_device_cdc_rndis.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "virtual_nic_enetif.h"
#include "virtual_nic.h"
#include "virtual_nic_enet_adapter.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#include "usb_phy.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Base unit for ENIT layer is 1Mbps while for RNDIS its 100bps*/
#define ENET_CONVERT_FACTOR (10000)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif
void VNIC_EnetRxBufFree(pbuf_t *pbuf);
void VNIC_EnetTxBufFree(pbuf_t *pbuf);
bool VNIC_EnetGetLinkStatus(void);
uint32_t VNIC_EnetGetSpeed(void);
uint8_t *VNIC_EnetTxBufAlloc(void);
usb_status_t VNIC_EnetSend(uint8_t *buf, uint32_t len);
usb_status_t VNIC_EnetTxDone(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
extern queue_t g_enetRxServiceQueue;
extern queue_t g_enetTxServiceQueue;
extern uint8_t g_hwaddr[ENET_MAC_ADDR_SIZE];
/* Data structure of virtual nic device. */
usb_cdc_vnic_t g_cdcVnic;
/* Buffer to receive data. */
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0))
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[HS_CDC_VNIC_BULK_OUT_PACKET_SIZE];
static uint32_t s_usbBulkMaxPacketSize = HS_CDC_VNIC_BULK_OUT_PACKET_SIZE;
#else
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[FS_CDC_VNIC_BULK_OUT_PACKET_SIZE];
static uint32_t s_usbBulkMaxPacketSize = FS_CDC_VNIC_BULK_OUT_PACKET_SIZE;
#endif

/* Part 1 of usb transmit buffer. */
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0))
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_usbTxPartOneBuffer[HS_CDC_VNIC_BULK_OUT_PACKET_SIZE];
#else
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_usbTxPartOneBuffer[FS_CDC_VNIC_BULK_OUT_PACKET_SIZE];
#endif

/* USB receive buffer to store the rndis packet. size is calculated as
 * ENET_FRAME_MAX_FRAMELEN + sizeof(enet_header_t) + RNDIS_USB_HEADER_SIZE.
 */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_usbRxRndisPacketBuffer[1581];
/* USB receive buffer to store the rndis packet. size is calculated as
 * ENET_FRAME_MAX_FRAMELEN + sizeof(enet_header_t).
 */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_usbTxRndisPacketBuffer[1536];

/* Append byte for zero length packet. */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_zeroSend = 0x00;

/*******************************************************************************
 * Code
 ******************************************************************************/
ENET_Type *BOARD_GetExampleEnetBase(void)
{
    return ENET;
}

uint32_t BOARD_GetPhySysClock(void)
{
    return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}

#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U))
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_cdcVnic.deviceHandle);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}
#endif
#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_cdcVnic.deviceHandle);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}
#endif
void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_UsbPhySrcExt, BOARD_XTAL0_CLK_HZ);
    CLOCK_EnableUsbhs0Clock(kCLOCK_UsbSrcUnused, 0U);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    SystemCoreClockUpdate();
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */
#endif
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber                  = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];
#endif
    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#endif
}
#endif

/*!
 * @brief Set the state of the usb transmit direction
 *
 * @param state The state of the usb transmit direction.
 * @return A USB error code or kStatus_USB_Success.
 */
static inline usb_status_t USB_DeviceVnicTransmitSetState(usb_cdc_vnic_tx_state_t state)
{
    USB_DEVICE_VNIC_CRITICAL_ALLOC();
    USB_DEVICE_VNIC_ENTER_CRITICAL();
    g_cdcVnic.nicTrafficInfo.usbTxState = state;
    USB_DEVICE_VNIC_EXIT_CRITICAL();
    return kStatus_USB_Success;
}

/*!
 * @brief Set the state of the usb receive direction.
 *
 * @param state The state of the usb receive direction.
 * @return A USB error code or kStatus_USB_Success.
 */
static inline usb_status_t USB_DeviceVnicReceiveSetState(usb_cdc_vnic_rx_state_t state)
{
    USB_DEVICE_VNIC_CRITICAL_ALLOC();
    USB_DEVICE_VNIC_ENTER_CRITICAL();
    g_cdcVnic.nicTrafficInfo.usbRxState = state;
    USB_DEVICE_VNIC_EXIT_CRITICAL();
    return kStatus_USB_Success;
}

/*!
 * @brief Interrupt in pipe callback function.
 *
 * This function serves as the callback function for interrupt in pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmInterruptIn(usb_device_handle handle,
                                         usb_device_endpoint_callback_message_struct_t *message,
                                         void *callbackParam)
{
    usb_status_t error = kStatus_USB_Error;
    error              = kStatus_USB_Success;

    return error;
}

/*!
 * @brief Bulk in pipe callback function.
 *
 * This function serves as the callback function for bulk in pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmBulkIn(usb_device_handle handle,
                                    usb_device_endpoint_callback_message_struct_t *message,
                                    void *callbackParam)
{
    usb_status_t error = kStatus_USB_Error;

    if (1 == g_cdcVnic.attach)
    {
        if (NULL == message->buffer)
        {
            return error;
        }

        switch (g_cdcVnic.nicTrafficInfo.usbTxState)
        {
            case TX_PART_ONE_PROCESS:
                USB_DeviceVnicTransmitSetState(TX_PART_ONE_DONE);
                break;
            case TX_PART_TWO_PROCESS:
                USB_DeviceVnicTransmitSetState(TX_PART_TWO_DONE);
                break;
            case TX_ZLP_PROCESS:
                USB_DeviceVnicTransmitSetState(TX_ZLP_DONE);
                break;
            default:
                break;
        }
    }
    return error;
}

/*!
 * @brief Bulk out pipe callback function.
 *
 * This function serves as the callback function for bulk out pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmBulkOut(usb_device_handle handle,
                                     usb_device_endpoint_callback_message_struct_t *message,
                                     void *callbackParam)
{
    usb_status_t error = kStatus_USB_Error;

    if (1 == g_cdcVnic.attach)
    {
        switch (g_cdcVnic.nicTrafficInfo.usbRxState)
        {
            case RX_PART_ONE_PROCESS:
                g_cdcVnic.nicTrafficInfo.usbRxPartOneBuf = message->buffer;
                g_cdcVnic.nicTrafficInfo.usbRxPartOneLen = message->length;
                USB_DeviceVnicReceiveSetState(RX_PART_ONE_DONE);
                break;
            case RX_PART_TWO_PROCESS:
                g_cdcVnic.nicTrafficInfo.usbRxPartTwoBuf = message->buffer;
                g_cdcVnic.nicTrafficInfo.usbRxPartTwoLen = message->length;
                USB_DeviceVnicReceiveSetState(RX_PART_TWO_DONE);
                break;
            default:
                break;
        }
    }
    else
    {
        usb_echo("Discard the received data\r\n");
    }
    return error;
}

/*!
 * @brief Get the setup packet buffer.
 *
 * This function provides the buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setupBuffer The pointer to the address of setup packet buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    static uint32_t cdcVnicSetup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&cdcVnicSetup;
    return kStatus_USB_Success;
}

/*!
 * @brief Get the setup packet data buffer.
 *
 * This function gets the data buffer for setup packet.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    if ((NULL == buffer) || ((*length) > RNDIS_MAX_EXPECTED_COMMAND_SIZE))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = (g_cdcVnic.rndisHandle)->rndisCommand;
    return kStatus_USB_Success;
}

/*!
 * @brief Configure remote wakeup feature.
 *
 * This function configures the remote wakeup feature.
 *
 * @param handle The USB device handle.
 * @param enable 1: enable, 0: disable.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
}

/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_CDC_VNIC_COMM_INTERFACE_INDEX)
    {
        return error;
    }
    switch (setup->bRequest)
    {
        case USB_DEVICE_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND:
            USB_DeviceCdcRndisMessageSet(g_cdcVnic.rndisHandle, buffer, length);
            *length = 0;
            error   = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE:
            *length = setup->wLength;
            USB_DeviceCdcRndisMessageGet(g_cdcVnic.rndisHandle, buffer, length);
            error = kStatus_USB_Success;
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief State process for usb transmit direction.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceVnicTransmit(void)
{
    usb_status_t error = kStatus_USB_Error;
    vnic_enet_transfer_t cdcAcmTransfer;
    pbuf_t *enetPbuf;
    uint32_t usbTxLen;
    uint32_t usbTxPart_1Len;
    uint8_t *nicData;
    uint32_t length;
    USB_DEVICE_VNIC_CRITICAL_ALLOC();
    uint8_t *firstSendBuff = s_usbTxPartOneBuffer;
    switch (g_cdcVnic.nicTrafficInfo.usbTxState)
    {
        case TX_IDLE:
        {
            /* Initialize the tx variables */
            g_cdcVnic.nicTrafficInfo.usbTxNicData         = NULL;
            g_cdcVnic.nicTrafficInfo.usbTxEnetPcb.payload = NULL;
            g_cdcVnic.nicTrafficInfo.usbTxEnetPcb.length  = 0;
            g_cdcVnic.nicTrafficInfo.usbTxRndisPkgLength  = 0;
            g_cdcVnic.nicTrafficInfo.usbTxPart_1Len       = 0;

            /* Get a transfer request from the enet queue */
            error = VNIC_EnetQueueGet(&g_enetRxServiceQueue, &cdcAcmTransfer);
            if (kStatus_USB_Success == error)
            {
                enetPbuf = &(g_cdcVnic.nicTrafficInfo.usbTxEnetPcb);

                enetPbuf->payload = cdcAcmTransfer.buffer;
                enetPbuf->length  = cdcAcmTransfer.length;
                length            = cdcAcmTransfer.length;
                nicData           = cdcAcmTransfer.buffer;

                usbTxLen       = length + RNDIS_USB_OVERHEAD_SIZE;
                usbTxPart_1Len = 0;
                g_cdcVnic.nicTrafficInfo.enetRxUsb2hostSent++;
                g_cdcVnic.nicTrafficInfo.usbTxNicData        = nicData;
                g_cdcVnic.nicTrafficInfo.usbTxRndisPkgLength = usbTxLen;

                /* RNDIS Protocol defines 1 byte call of 0x00, if transfer size is multiple of endpoint packet size */
                g_cdcVnic.nicTrafficInfo.usbTxZeroSendFlag = (uint8_t)((usbTxLen % s_usbBulkMaxPacketSize) ? 0 : 1);

                /* Whichever is smaller but not less than RNDIS_USB_OVERHEAD_SIZE */
                usbTxPart_1Len = usbTxLen > s_usbBulkMaxPacketSize ? s_usbBulkMaxPacketSize : usbTxLen;

                if (usbTxPart_1Len < RNDIS_USB_OVERHEAD_SIZE)
                {
                    /* For s_usbBulkMaxPacketSize as 8, 16 or 32 minimum  usbTxPart_1Len has to be
                     either usbTxLen
                     (which is definitely greater than RNDIS_USB_OVERHEAD_SIZE) or at least 64 which is the next allowed
                     multiple of
                     s_usbBulkMaxPacketSize */
                    usbTxPart_1Len = usbTxLen > 64 ? 64 : usbTxLen;
                }
                g_cdcVnic.nicTrafficInfo.usbTxPart_1Len = usbTxPart_1Len;

                /* Prepare USB Header */
                ((rndis_packet_msg_format_t *)firstSendBuff)->messageType = USB_LONG_TO_LITTLE_ENDIAN(RNDIS_PACKET_MSG);
                ((rndis_packet_msg_format_t *)firstSendBuff)->messageLen  = USB_LONG_TO_LITTLE_ENDIAN(usbTxLen);
                ((rndis_packet_msg_format_t *)firstSendBuff)->dataOffset = USB_LONG_TO_LITTLE_ENDIAN(RNDIS_DATA_OFFSET);
                ((rndis_packet_msg_format_t *)firstSendBuff)->dataLen    = USB_LONG_TO_LITTLE_ENDIAN(length);

                /* Fill rest of firstSendBuff buffers with payload as much as possible */
                memcpy(firstSendBuff + RNDIS_USB_OVERHEAD_SIZE, nicData, usbTxPart_1Len - RNDIS_USB_OVERHEAD_SIZE);

                USB_DEVICE_VNIC_ENTER_CRITICAL();
                error = USB_DeviceSendRequest(g_cdcVnic.deviceHandle, USB_CDC_VNIC_BULK_IN_ENDPOINT, firstSendBuff,
                                              usbTxPart_1Len);
                if (kStatus_USB_Error != error)
                {
                    USB_DeviceVnicTransmitSetState(TX_PART_ONE_PROCESS);
                }
                else
                {
                    usb_echo("Part One of RNDIS packet send failed, 0x%x\n", error);
                    VNIC_EnetRxBufFree(enetPbuf);
                }
                USB_DEVICE_VNIC_EXIT_CRITICAL();
            }
        }
        break;
        case TX_PART_ONE_PROCESS:
            break;
        case TX_PART_ONE_DONE:
        {
            usbTxLen             = g_cdcVnic.nicTrafficInfo.usbTxRndisPkgLength;
            usbTxPart_1Len       = g_cdcVnic.nicTrafficInfo.usbTxPart_1Len;
            nicData              = g_cdcVnic.nicTrafficInfo.usbTxNicData;
            enetPbuf             = &(g_cdcVnic.nicTrafficInfo.usbTxEnetPcb);
            uint8_t returnStatus = kStatus_USB_Success;

            if (usbTxLen > usbTxPart_1Len)
            {
                /* Send the part 2 of the RNDIS packet */
                USB_DEVICE_VNIC_ENTER_CRITICAL();
                memcpy(s_usbTxRndisPacketBuffer, nicData + (usbTxPart_1Len - RNDIS_USB_OVERHEAD_SIZE),
                       usbTxLen - usbTxPart_1Len);
                error = USB_DeviceSendRequest(g_cdcVnic.deviceHandle, USB_CDC_VNIC_BULK_IN_ENDPOINT,
                                              s_usbTxRndisPacketBuffer, usbTxLen - usbTxPart_1Len);
                if (kStatus_USB_Error != error)
                {
                    USB_DeviceVnicTransmitSetState(TX_PART_TWO_PROCESS);
                }
                else
                {
                    usb_echo("Part Two of RNDIS packet send failed, 0x%x\n", returnStatus);
                    VNIC_EnetRxBufFree(enetPbuf);
                }
                USB_DEVICE_VNIC_EXIT_CRITICAL();
            }
            else
            {
                USB_DeviceVnicTransmitSetState(TX_PART_TWO_DONE);
            }
        }
        break;
        case TX_PART_TWO_PROCESS:
            break;
        case TX_PART_TWO_DONE:
        {
            enetPbuf = &(g_cdcVnic.nicTrafficInfo.usbTxEnetPcb);
            if (g_cdcVnic.nicTrafficInfo.usbTxZeroSendFlag == 1)
            {
                /* Send a zero length packet */
                USB_DEVICE_VNIC_ENTER_CRITICAL();
                error = USB_DeviceSendRequest(g_cdcVnic.deviceHandle, USB_CDC_VNIC_BULK_IN_ENDPOINT, &s_zeroSend,
                                              sizeof(uint8_t));
                if (kStatus_USB_Error != error)
                {
                    USB_DeviceVnicTransmitSetState(TX_ZLP_PROCESS);
                }
                else
                {
                    usb_echo("Zero length packet send failed, 0x%x\n", error);
                }
                USB_DEVICE_VNIC_EXIT_CRITICAL();
            }
            else
            {
                VNIC_EnetRxBufFree(enetPbuf);
                g_cdcVnic.nicTrafficInfo.enetRxUsb2host++;
                USB_DeviceVnicTransmitSetState(TX_IDLE);
            }
        }
        break;
        case TX_ZLP_PROCESS:
            break;
        case TX_ZLP_DONE:
        {
            enetPbuf = &(g_cdcVnic.nicTrafficInfo.usbTxEnetPcb);
            VNIC_EnetRxBufFree(enetPbuf);
            g_cdcVnic.nicTrafficInfo.enetRxUsb2host++;
            USB_DeviceVnicTransmitSetState(TX_IDLE);
        }
        break;
        default:
            break;
    }
    return error;
}

/*!
 * @brief State process for usb receive direction.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceVnicReceive(void)
{
    usb_status_t error         = kStatus_USB_Error;
    uint8_t *rndisPktMsgData   = NULL;
    uint32_t frameRemainingLen = 0;
    uint32_t messageLen        = 0;
    uint8_t *buffer;
    uint32_t len;
    USB_DEVICE_VNIC_CRITICAL_ALLOC();
    switch (g_cdcVnic.nicTrafficInfo.usbRxState)
    {
        case RX_IDLE:
        {
            /* Prime for next receive */
            USB_DEVICE_VNIC_ENTER_CRITICAL();
            error = USB_DeviceRecvRequest(g_cdcVnic.deviceHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, s_currRecvBuf,
                                          s_usbBulkMaxPacketSize);
            if (kStatus_USB_Error != error)
            {
                USB_DeviceVnicReceiveSetState(RX_PART_ONE_PROCESS);
            }
            else
            {
                usb_echo("Part One of RNDIS packet recv failed\r\n");
            }
            USB_DEVICE_VNIC_EXIT_CRITICAL();
        }
        break;
        case RX_PART_ONE_PROCESS:
            break;
        case RX_PART_ONE_DONE:
        {
            buffer = (uint8_t *)g_cdcVnic.nicTrafficInfo.usbRxPartOneBuf;
            len    = g_cdcVnic.nicTrafficInfo.usbRxPartOneLen;
            if (USB_CANCELLED_TRANSFER_LENGTH == len)
            {
                USB_DeviceVnicReceiveSetState(RX_IDLE);
                break;
            }
            messageLen = USB_LONG_TO_LITTLE_ENDIAN(*((uint32_t *)(buffer) + 1));
            ;

            if (!(messageLen % s_usbBulkMaxPacketSize))
            {
                /* RNDIS Protocol defines 1 byte call of 0x00, if transfer size is multiple of endpoint packet size */
                messageLen++;
            }
            rndisPktMsgData = s_usbRxRndisPacketBuffer;
            memcpy(rndisPktMsgData, buffer, len);

            frameRemainingLen = messageLen - len;

            /* Receive the second part of RNDIS packet from host */
            if (frameRemainingLen)
            {
                /* Required when ethernet packet + usb header is larger than maxPacketSize */
                USB_DEVICE_VNIC_ENTER_CRITICAL();
                error = USB_DeviceRecvRequest(g_cdcVnic.deviceHandle, USB_CDC_VNIC_BULK_OUT_ENDPOINT,
                                              rndisPktMsgData + s_usbBulkMaxPacketSize, frameRemainingLen);
                if (kStatus_USB_Error != error)
                {
                    USB_DeviceVnicReceiveSetState(RX_PART_TWO_PROCESS);
                }
                else
                {
                    usb_echo("Part Two of RNDIS packet recv failed\r\n");
                }
                USB_DEVICE_VNIC_EXIT_CRITICAL();
            }
            else
            {
                /* Send the ethernet packet */
                USB_DeviceVnicReceiveSetState(RX_USB2ENET_PROCESS);
                error = VNIC_EnetSend((uint8_t *)(rndisPktMsgData + RNDIS_USB_OVERHEAD_SIZE),
                                      messageLen - RNDIS_USB_OVERHEAD_SIZE);

                if (kStatus_USB_Success != error)
                {
                    usb_echo("RX_PART_ONE_DONE, VNIC_EnetSend failed\n");
                }
            }
        }
        break;
        case RX_PART_TWO_PROCESS:
            break;
        case RX_PART_TWO_DONE:
        {
            buffer = (uint8_t *)g_cdcVnic.nicTrafficInfo.usbRxPartTwoBuf;
            len    = g_cdcVnic.nicTrafficInfo.usbRxPartTwoLen;
            if (USB_CANCELLED_TRANSFER_LENGTH == len)
            {
                USB_DeviceVnicReceiveSetState(RX_IDLE);
                break;
            }
            /* Entire ethernet packet with USB header was not received as one transaction */
            rndisPktMsgData = buffer - s_usbBulkMaxPacketSize;

            /* Re-calculate messageLen as it might have changed because of 1 byte of zero recv */
            messageLen = USB_LONG_TO_LITTLE_ENDIAN(*((uint32_t *)rndisPktMsgData + 1));

            /* Send the ethernet packet */
            USB_DeviceVnicReceiveSetState(RX_USB2ENET_PROCESS);
            error = VNIC_EnetSend((uint8_t *)(rndisPktMsgData + RNDIS_USB_OVERHEAD_SIZE),
                                  messageLen - RNDIS_USB_OVERHEAD_SIZE);

            if (kStatus_USB_Success != error)
            {
                usb_echo("RX_PART_TWO_DONE, VNIC_EnetSend failed\n");
            }
        }
        break;
        case RX_USB2ENET_PROCESS:
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)) || \
    (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0))
            VNIC_EnetTxDone();
#endif
            USB_DeviceVnicReceiveSetState(RX_USB2ENET_DONE);
            break;
        case RX_USB2ENET_DONE:
        {
            USB_DeviceVnicReceiveSetState(RX_IDLE);
        }
        break;
        default:
            break;
    }
    return error;
}

/*!
 * @brief Callback for RNDIS specific requests.
 *
 * @param handle The class handle of the CDC ACM class.
 * @param event The event for the RNDIS device.
 * @param param The pointer to parameter of the callback.
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcRndisCallback(usb_device_cdc_rndis_struct_t *handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    usb_device_cdc_rndis_request_param_struct_t *rndisParam;
    rndisParam = (usb_device_cdc_rndis_request_param_struct_t *)param;

    switch (event)
    {
        case kUSB_DeviceCdcEventAppGetLinkSpeed:
            if (1 == g_cdcVnic.attach)
            {
                *((uint32_t *)rndisParam->buffer) = VNIC_EnetGetSpeed();
                *((uint32_t *)rndisParam->buffer) *= ENET_CONVERT_FACTOR;
            }
            break;
        case kUSB_DeviceCdcEventAppGetSendPacketSize:
            *((uint32_t *)rndisParam->buffer) = USB_LONG_TO_LITTLE_ENDIAN(s_usbBulkMaxPacketSize);
            break;
        case kUSB_DeviceCdcEventAppGetRecvPacketSize:
            *((uint32_t *)rndisParam->buffer) = USB_LONG_TO_LITTLE_ENDIAN(s_usbBulkMaxPacketSize);
            break;
        case kUSB_DeviceCdcEventAppGetMacAddress:
            memcpy(rndisParam->buffer, &g_hwaddr[0], ENET_MAC_ADDR_SIZE);
            break;
        case kUSB_DeviceCdcEventAppGetMaxFrameSize:
            *((uint32_t *)rndisParam->buffer) = (ENET_FRAME_MAX_FRAMELEN + RNDIS_USB_HEADER_SIZE);
            break;
        case kUSB_DeviceCdcEventAppGetLinkStatus:
            if (1 == g_cdcVnic.attach)
            {
                *((uint32_t *)rndisParam->buffer) = VNIC_EnetGetLinkStatus();
            }
            break;
        default:
            break;
    }
    return error;
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            uint8_t *message;
            uint32_t len;
            USB_DeviceControlPipeInit(g_cdcVnic.deviceHandle);
            g_cdcVnic.attach               = 0;
            g_cdcVnic.currentConfiguration = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success ==
                USB_DeviceGetStatus(g_cdcVnic.deviceHandle, kUSB_DeviceStatusSpeed, &g_cdcVnic.speed))
            {
                USB_DeviceSetSpeed(handle, g_cdcVnic.speed);
            }
#endif
            USB_DeviceCdcRndisHaltCommand(g_cdcVnic.rndisHandle);
            USB_DeviceCdcRndisResetCommand(g_cdcVnic.rndisHandle, &message, &len);
            VNIC_EnetClearEnetQueue();
            if ((g_cdcVnic.nicTrafficInfo.usbTxEnetPcb.payload != NULL))
            {
                VNIC_EnetRxBufFree(&(g_cdcVnic.nicTrafficInfo.usbTxEnetPcb));
            }
            USB_DeviceVnicTransmitSetState(TX_IDLE);
            USB_DeviceVnicReceiveSetState(RX_PART_ONE_PROCESS);
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                g_cdcVnic.attach               = 0;
                g_cdcVnic.currentConfiguration = 0U;
            }
            else if (USB_CDC_VNIC_CONFIGURE_INDEX == (*temp8))
            {
                usb_device_endpoint_init_struct_t epInitStruct;
                usb_device_endpoint_callback_struct_t epCallback;

                g_cdcVnic.attach               = 1;
                g_cdcVnic.currentConfiguration = *temp8;

                /* Initiailize endpoint for interrupt pipe */
                epCallback.callbackFn    = USB_DeviceCdcAcmInterruptIn;
                epCallback.callbackParam = handle;

                epInitStruct.zlt          = 0;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress =
                    USB_CDC_VNIC_INTERRUPT_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_cdcVnic.speed)
                {
                    epInitStruct.maxPacketSize = HS_CDC_VNIC_INTERRUPT_IN_PACKET_SIZE;
                    epInitStruct.interval      = HS_CDC_VNIC_INTERRUPT_IN_INTERVAL;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_CDC_VNIC_INTERRUPT_IN_PACKET_SIZE;
                    epInitStruct.interval      = FS_CDC_VNIC_INTERRUPT_IN_INTERVAL;
                }

                USB_DeviceInitEndpoint(g_cdcVnic.deviceHandle, &epInitStruct, &epCallback);

                /* Initiailize endpoints for bulk pipe */
                epCallback.callbackFn    = USB_DeviceCdcAcmBulkIn;
                epCallback.callbackParam = handle;

                epInitStruct.zlt          = 0;
                epInitStruct.interval     = 0U;
                epInitStruct.transferType = USB_ENDPOINT_BULK;
                epInitStruct.endpointAddress =
                    USB_CDC_VNIC_BULK_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_cdcVnic.speed)
                {
                    epInitStruct.maxPacketSize = HS_CDC_VNIC_BULK_IN_PACKET_SIZE;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_CDC_VNIC_BULK_IN_PACKET_SIZE;
                }

                USB_DeviceInitEndpoint(g_cdcVnic.deviceHandle, &epInitStruct, &epCallback);

                epCallback.callbackFn    = USB_DeviceCdcAcmBulkOut;
                epCallback.callbackParam = handle;

                epInitStruct.zlt          = 0;
                epInitStruct.interval     = 0U;
                epInitStruct.transferType = USB_ENDPOINT_BULK;
                epInitStruct.endpointAddress =
                    USB_CDC_VNIC_BULK_OUT_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_cdcVnic.speed)
                {
                    epInitStruct.maxPacketSize = HS_CDC_VNIC_BULK_OUT_PACKET_SIZE;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_CDC_VNIC_BULK_OUT_PACKET_SIZE;
                }

                USB_DeviceInitEndpoint(g_cdcVnic.deviceHandle, &epInitStruct, &epCallback);

                if (USB_SPEED_HIGH == g_cdcVnic.speed)
                {
                    s_usbBulkMaxPacketSize = HS_CDC_VNIC_BULK_OUT_PACKET_SIZE;
                }
                else
                {
                    s_usbBulkMaxPacketSize = FS_CDC_VNIC_BULK_OUT_PACKET_SIZE;
                }
                /* Schedule buffer for receive */
                USB_DeviceRecvRequest(handle, USB_CDC_VNIC_BULK_OUT_ENDPOINT, s_currRecvBuf, s_usbBulkMaxPacketSize);
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        return USB_DeviceStallEndpoint(handle, ep);
    }
    else
    {
        return USB_DeviceUnstallEndpoint(handle, ep);
    }
}

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
void APPInit(void)
{
    usb_device_cdc_rndis_config_struct_t rndisConfig;

    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    /* Initialize the FEC interface */
    if (ENET_OK != VNIC_EnetInit())
    {
        usb_echo("VNIC_EnetInit failed\r\n");
    }

    g_cdcVnic.speed        = USB_SPEED_FULL;
    g_cdcVnic.attach       = 0;
    g_cdcVnic.deviceHandle = NULL;

    USB_DeviceVnicReceiveSetState(RX_PART_ONE_PROCESS);

    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_cdcVnic.deviceHandle))
    {
        usb_echo("USB device vnic failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device CDC virtual nic demo\r\n");
    }

    rndisConfig.devMaxTxSize  = ENET_FRAME_MAX_FRAMELEN + RNDIS_USB_HEADER_SIZE;
    rndisConfig.rndisCallback = USB_DeviceCdcRndisCallback;
    if (kStatus_USB_Success != USB_DeviceCdcRndisInit(&rndisConfig, &(g_cdcVnic.rndisHandle)))
    {
        usb_echo("USB_DeviceCdcRndisInit failed\r\n");
    }

    USB_DeviceIsrEnable();

    /*Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection.*/
    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(g_cdcVnic.deviceHandle);
}

/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
void APPTask(void)
{
    if ((1 == g_cdcVnic.attach))
    {
        /* User Code */
        USB_DeviceVnicTransmit();
        USB_DeviceVnicReceive();
    }
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Disable SYSMPU. */
    SYSMPU_Enable(SYSMPU, false);
    /* Set RMII clock src. */
    CLOCK_SetRmii0Clock(1U);

    NVIC_SetPriority((IRQn_Type)ENET_Receive_IRQn, 6U);
#ifdef ENET_ENHANCEDBUFFERDESCRIPTOR_MODE
    NVIC_SetPriority(ENET_Transmit_IRQn, 6U);
    NVIC_SetPriority(ENET_1588_Timer_IRQn, 6U);
#endif

    APPInit();

    while (1)
    {
        APPTask();

#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_cdcVnic.deviceHandle);
#endif
    }
}
