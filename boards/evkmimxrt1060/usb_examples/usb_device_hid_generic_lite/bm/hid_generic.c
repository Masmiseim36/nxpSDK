/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_hid.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "hid_generic.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#include "pin_mux.h"
#include "usb_phy.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

static usb_status_t USB_DeviceHidGenericInterruptIn(usb_device_handle handle,
                                                    usb_device_endpoint_callback_message_struct_t *message,
                                                    void *callbackParam);
static usb_status_t USB_DeviceHidGenericInterruptOut(usb_device_handle handle,
                                                     usb_device_endpoint_callback_message_struct_t *message,
                                                     void *callbackParam);
static void USB_DeviceApplicationInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint32_t s_GenericBuffer0[USB_HID_GENERIC_OUT_BUFFER_LENGTH >> 2];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint32_t s_GenericBuffer1[USB_HID_GENERIC_OUT_BUFFER_LENGTH >> 2];
usb_hid_generic_struct_t g_UsbDeviceHidGeneric;

extern uint8_t g_UsbDeviceCurrentConfigure;
extern uint8_t g_UsbDeviceInterface[USB_HID_GENERIC_INTERFACE_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}

void USB_OTG2_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL, BOARD_USB_PHY_TXCAL45DP, BOARD_USB_PHY_TXCAL45DM,
    };

    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 480000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

/* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceEhciTaskFunction(deviceHandle);
}
#endif

/* The hid generic interrupt IN endpoint callback */
static usb_status_t USB_DeviceHidGenericInterruptIn(usb_device_handle handle,
                                                    usb_device_endpoint_callback_message_struct_t *message,
                                                    void *callbackParam)
{
    if (g_UsbDeviceHidGeneric.attach)
    {
    }

    return kStatus_USB_Error;
}

/* The hid generic interrupt OUT endpoint callback */
static usb_status_t USB_DeviceHidGenericInterruptOut(usb_device_handle handle,
                                                     usb_device_endpoint_callback_message_struct_t *message,
                                                     void *callbackParam)
{
    if (g_UsbDeviceHidGeneric.attach)
    {
        USB_DeviceSendRequest(g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_ENDPOINT_IN,
                              (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                              USB_HID_GENERIC_OUT_BUFFER_LENGTH);
        g_UsbDeviceHidGeneric.bufferIndex ^= 1U;
        return USB_DeviceRecvRequest(g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                                     (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                                     USB_HID_GENERIC_OUT_BUFFER_LENGTH);
    }

    return kStatus_USB_Error;
}

/* The device callback */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* USB bus reset signal detected */
            /* Initialize the control pipes */
            USB_DeviceControlPipeInit(g_UsbDeviceHidGeneric.deviceHandle);
            g_UsbDeviceHidGeneric.attach = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceHidGeneric.deviceHandle, kUSB_DeviceStatusSpeed,
                                                           &g_UsbDeviceHidGeneric.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceHidGeneric.speed);
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (USB_HID_GENERIC_CONFIGURE_INDEX == (*temp8))
            {
                /* If the confguration is valid, initliaze the HID generic interrupt IN pipe */
                usb_device_endpoint_init_struct_t epInitStruct;
                usb_device_endpoint_callback_struct_t epCallback;

                epCallback.callbackFn = USB_DeviceHidGenericInterruptIn;
                epCallback.callbackParam = handle;

                epInitStruct.zlt = 0U;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress =
                    USB_HID_GENERIC_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_UsbDeviceHidGeneric.speed)
                {
                    epInitStruct.maxPacketSize = HS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE;
                }

                USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

                epCallback.callbackFn = USB_DeviceHidGenericInterruptOut;
                epCallback.callbackParam = handle;

                epInitStruct.zlt = 0U;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress =
                    USB_HID_GENERIC_ENDPOINT_OUT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_UsbDeviceHidGeneric.speed)
                {
                    epInitStruct.maxPacketSize = HS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE;
                }

                USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

                g_UsbDeviceHidGeneric.attach = 1U;

                error = USB_DeviceRecvRequest(
                    g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                    (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                    USB_HID_GENERIC_OUT_BUFFER_LENGTH);
            }
            break;
        default:
            break;
    }

    return error;
}

/* Get setup buffer */
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    /* Keep the setup is 4-byte aligned */
    static uint32_t hid_generic_setup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&hid_generic_setup;
    return kStatus_USB_Success;
}

/* Configure device remote wakeup */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
}

/* Configure the endpoint status (idle or stall) */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if (((USB_HID_GENERIC_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)) ||
            ((USB_HID_GENERIC_ENDPOINT_OUT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if (((USB_HID_GENERIC_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)) ||
            ((USB_HID_GENERIC_ENDPOINT_OUT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return kStatus_USB_InvalidRequest;
}

/* Get class-specific request buffer */
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    static uint8_t setupOut[8];
    if ((NULL == buffer) || ((*length) > sizeof(setupOut)))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = setupOut;
    return kStatus_USB_Success;
}

/* Handle class-specific request */
usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_HID_GENERIC_INTERFACE_INDEX)
    {
        return error;
    }

    switch (setup->bRequest)
    {
        case USB_DEVICE_HID_REQUEST_GET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_GET_IDLE:
            break;
        case USB_DEVICE_HID_REQUEST_GET_PROTOCOL:
            break;
        case USB_DEVICE_HID_REQUEST_SET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_SET_IDLE:
            break;
        case USB_DEVICE_HID_REQUEST_SET_PROTOCOL:
            break;
        default:
            break;
    }

    return error;
}

static void USB_DeviceApplicationInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    /* Set HID generic default state */
    g_UsbDeviceHidGeneric.speed = USB_SPEED_FULL;
    g_UsbDeviceHidGeneric.attach = 0U;
    g_UsbDeviceHidGeneric.deviceHandle = NULL;
    g_UsbDeviceHidGeneric.buffer[0] = (uint8_t *)&s_GenericBuffer0[0];
    g_UsbDeviceHidGeneric.buffer[1] = (uint8_t *)&s_GenericBuffer1[0];

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_UsbDeviceHidGeneric.deviceHandle))
    {
        usb_echo("USB device generic failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device HID generic demo\r\n");
    }

    USB_DeviceIsrEnable();

    /* Start USB device HID generic */
    USB_DeviceRun(g_UsbDeviceHidGeneric.deviceHandle);
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_ConfigMPU();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    USB_DeviceApplicationInit();

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_UsbDeviceHidGeneric.deviceHandle);
#endif
    }
}
