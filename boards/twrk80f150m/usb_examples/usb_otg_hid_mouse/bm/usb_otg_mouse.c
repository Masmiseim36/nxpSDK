/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_otg_config.h"
#include "usb_host_config.h"
#include "usb_device_config.h"
#include "usb_otg.h"
#include "usb_host.h"
#include "usb_device.h"
#include "usb_otg_mouse.h"
#include "board.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#if ((defined USB_OTG_KHCI_PERIPHERAL_ENABLE) && (USB_OTG_KHCI_PERIPHERAL_ENABLE))
#include "usb_otg_max3353.h"
#endif
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define GPIO_IRQHandler PORTA_IRQHandler
#define BOARD_MAX3353_GPIO_PIN (13)
#define BOARD_MAX3353_GPIO_PORT (PORTA)
#define BOARD_MAX3353_GPIO (GPIOA)
#define BOARD_MAX3353_GPIO_IRQ (PORTA_IRQn)

#define BOARD_MAX3353_I2C (I2C0)
#define BOARD_MAX3353_I2C_CLK_SRC (kCLOCK_BusClk)
#define BOARD_MAX3353_I2C_SLAVE_ADDR_7BIT (0x2CU)
#define BOARD_MAX3353_I2C_BAUDRATE (100U)


#define USB_HOST_INTERRUPT_PRIORITY (3U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern usb_status_t USB_OtgHostMouseInit(usb_host_handle *hostHandle);
extern usb_status_t USB_OtgHostMouseDeinit(usb_host_handle hostHandle);
extern usb_status_t USB_OtgDeviceMouseInit(usb_device_handle *deviceHandle);
extern usb_status_t USB_OtgDeviceMouseDeinit(usb_device_handle deviceHandle);
extern void USB_HostHidMouseTask(void);
extern void USB_OtgClockInit(void);
extern void USB_OtgIsrEnable(void);
void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

usb_otg_handle g_OtgHandle;
usb_otg_mouse_instance_t g_OtgMouseInstance;

/*******************************************************************************
 * Code
 ******************************************************************************/

void USB0_IRQHandler(void)
{
    USB_OtgKhciIsrFunction(g_OtgHandle);

    if (g_OtgMouseInstance.otgMouseState == kState_Host)
    {
        USB_HostKhciIsrFunction(g_OtgMouseInstance.hostHandle);
    }
    else if (g_OtgMouseInstance.otgMouseState == kState_Device)
    {
        USB_DeviceKhciIsrFunction(g_OtgMouseInstance.deviceHandle);
    }
    else
    {
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

void USB_OtgClockInit(void)
{
    /* initialize khci clock */
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
}

void USB_OtgIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTKhciIrq[] = USB_IRQS;
    irqNumber                = usbHOSTKhciIrq[0];

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

static void USB_OtgAppPrintState(uint32_t state)
{
    switch (state)
    {
        case kOtg_State_AIdle:
            usb_echo("enter a_idle\r\n");
            break;

        case kOtg_State_AWaitVrise:
            usb_echo("enter a_wait_vrise\r\n");
            break;

        case kOtg_State_AWaitBcon:
            usb_echo("enter a_wait_bcon\r\n");
            break;

        case kOtg_State_AWaitVfall:
            usb_echo("enter a_wait_vfall\r\n");
            break;

        case kOtg_State_AHost:
            g_OtgMouseInstance.aSetBHNPEnable = 0U;
            usb_echo("enter a_host\r\n");
            break;

        case kOtg_State_APeripheral:
            usb_echo("enter a_peripheral\r\n");
            break;

        case kOtg_State_ASuspend:
            usb_echo("enter a_suspend\r\n");
            break;

        case kOtg_State_AVbusErr:
            usb_echo("enter a_vbus_err\r\n");
            break;

        case kOtg_State_BIdle:
            usb_echo("enter b_idle\r\n");
            break;

        case kOtg_State_BSrpInit:
            usb_echo("enter b_srp_init\r\n");
            break;

        case kOtg_State_BPeripheral:
            usb_echo("enter b_peripheral\r\n");
            break;

        case kOtg_State_BWaitAcon:
            usb_echo("enter b_wait_acon\r\n");
            break;

        case kOtg_State_BHost:
            usb_echo("enter b_host\r\n");
            break;

        default:
            break;
    }
}

static void USB_OtgMousePrintMenu(usb_otg_mouse_instance_t *otgMouseInstance)
{
    switch (otgMouseInstance->otgStateMachine)
    {
        case kOtg_State_AIdle:
            usb_echo("The menu:\r\n");
            usb_echo("1. bus request\r\n");
            usb_echo("2. bus release (set bus request false)\r\n");
            usb_echo("3. set bus drop false\r\n");
            usb_echo("4. set bus drop true\r\n");
            break;

        case kOtg_State_AHost:
            usb_echo("The menu:\r\n");
            if (otgMouseInstance->aSetBHNPEnable == 1U)
            {
                usb_echo("2. bus release\r\n");
            }
            else
            {
                usb_echo("warning: this demo don't let release bus before a_set_b_hnp_en is set.\r\n");
            }
            usb_echo("4. set bus drop true\r\n");
            break;

        case kOtg_State_APeripheral:
            usb_echo("The menu:\r\n");
            usb_echo("1. bus request\r\n");
            usb_echo("4. set bus drop true\r\n");
            break;

        case kOtg_State_AVbusErr:
            usb_echo("The menu:\r\n");
            usb_echo("5. clear error\r\n");
            break;

        case kOtg_State_BIdle:
            usb_echo("The menu:\r\n");
            usb_echo("1. bus request (SRP)\r\n");
            break;

        case kOtg_State_BPeripheral:
            usb_echo("The menu:\r\n");
            usb_echo("1. bus request (HNP)\r\n");
            break;

        case kOtg_State_BHost:
            usb_echo("The menu:\r\n");
            usb_echo("2. bus release\r\n");
            break;

        default:
            break;
    }
}

static void USB_OtgMouseProcessMenu(usb_otg_mouse_instance_t *otgMouseInstance, char ch)
{
    uint8_t data;

    if ((ch >= '1') && (ch <= '9'))
    {
        switch (otgMouseInstance->otgStateMachine)
        {
            case kOtg_State_AIdle:
                if (ch == '1')
                {
                    usb_echo("1. bus request\r\n");
                    USB_OtgBusRequest(g_OtgHandle);
                }
                else if (ch == '2')
                {
                    usb_echo("2. bus release (set bus request false)\r\n");
                    USB_OtgBusRelease(g_OtgHandle);
                }
                else if (ch == '3')
                {
                    usb_echo("3. set bus drop false\r\n");
                    USB_OtgBusDrop(g_OtgHandle, 0);
                }
                else if (ch == '4')
                {
                    usb_echo("4. set bus drop true\r\n");
                    USB_OtgBusDrop(g_OtgHandle, 1);
                }
                else
                {
                }
                break;

            case kOtg_State_AHost:
                if (ch == '2')
                {
                    if (!otgMouseInstance->aSetBHNPEnable)
                    {
                        usb_echo("warning: a_set_b_hnp_en is not set, this demo don't let release bus\r\n");
                    }
                    else
                    {
                        usb_echo("2. bus release\r\n");
                        USB_OtgBusRelease(g_OtgHandle);
                    }
                }
                else if (ch == '4')
                {
                    usb_echo("4. set bus drop true\r\n");
                    USB_OtgBusDrop(g_OtgHandle, 1);
                }
                else
                {
                }
                break;

            case kOtg_State_APeripheral:
                if (ch == '1')
                {
                    usb_echo("1. bus request\r\n");
                    data = 1;
                    USB_OtgBusRequest(g_OtgHandle);
                    USB_DeviceSetStatus(otgMouseInstance->deviceHandle, kUSB_DeviceStatusOtg, &data);
                }
                else if (ch == '4')
                {
                    usb_echo("4. set bus drop true\r\n");
                    USB_OtgBusDrop(g_OtgHandle, 1);
                }
                else
                {
                }
                break;

            case kOtg_State_AVbusErr:
                if (ch == '5')
                {
                    usb_echo("5. clear error\r\n");
                    USB_OtgClearError(g_OtgHandle);
                }
                break;

            case kOtg_State_BIdle:
                if (ch == '1')
                {
                    usb_echo("1. bus request (SRP)\r\n");
                    USB_OtgBusRequest(g_OtgHandle);
                }
                break;

            case kOtg_State_BPeripheral:
                if (ch == '1')
                {
                    usb_echo("1. bus request (HNP)\r\n");
                    data = 1;
                    USB_OtgBusRequest(g_OtgHandle);
                    USB_DeviceSetStatus(otgMouseInstance->deviceHandle, kUSB_DeviceStatusOtg, &data);
                }
                break;

            case kOtg_State_BHost:
                if (ch == '2')
                {
                    usb_echo("2. bus release\r\n");
                    USB_OtgBusRelease(g_OtgHandle);
                }
                break;

            default:
                break;
        }
    }
}

static void USB_OtgMouseProcessStack(usb_otg_mouse_instance_t *otgMouseInstance, uint8_t stack)
{
    usb_status_t status;
    IRQn_Type usbFsIrqs[] = USB_IRQS;
    IRQn_Type usbIrq = usbFsIrqs[kUSB_ControllerKhci0 - kUSB_ControllerKhci0];

    switch (stack)
    {
        case kOtg_StackHostInit:
            if (otgMouseInstance->otgMouseState == kState_Device)
            {
                DisableIRQ(usbIrq);
                status = USB_OtgDeviceMouseDeinit(otgMouseInstance->deviceHandle);
                otgMouseInstance->otgMouseState = kState_None;
                otgMouseInstance->deviceHandle = NULL;
                EnableIRQ(usbIrq);
                if (status == kStatus_USB_Success)
                {
                    usb_echo("device deinit success\r\n");
                }
                else
                {
                    usb_echo("device deinit fail\r\n");
                }
            }
            otgMouseInstance->hostHandle = NULL;
            DisableIRQ(usbIrq);
            otgMouseInstance->otgMouseState = kState_Host;
            otgMouseInstance->aSetBHNPEnable = 0U;
            status = USB_OtgHostMouseInit(&otgMouseInstance->hostHandle);
            EnableIRQ(usbIrq);
            if (status == kStatus_USB_Success)
            {
                usb_echo("host init success\r\n");
            }
            else
            {
                usb_echo("host init fail\r\n");
            }
            break;

        case kOtg_StackDeviceInit:
            if (otgMouseInstance->otgMouseState == kState_Host)
            {
                DisableIRQ(usbIrq);
                status = USB_OtgHostMouseDeinit(otgMouseInstance->hostHandle);
                otgMouseInstance->otgMouseState = kState_None;
                otgMouseInstance->hostHandle = NULL;
                EnableIRQ(usbIrq);
                if (status == kStatus_USB_Success)
                {
                    usb_echo("host deinit success\r\n");
                }
                else
                {
                    usb_echo("host deinit fail\r\n");
                }
            }
            otgMouseInstance->deviceHandle = NULL;
            DisableIRQ(usbIrq);
            otgMouseInstance->otgMouseState = kState_Device;
            status = USB_OtgDeviceMouseInit(&otgMouseInstance->deviceHandle);
            EnableIRQ(usbIrq);
            if (status == kStatus_USB_Success)
            {
                usb_echo("device init success\r\n");
            }
            else
            {
                usb_echo("device init fail\r\n");
            }
            break;

        case kOtg_StackHostDeinit:
            DisableIRQ(usbIrq);
            status = USB_OtgHostMouseDeinit(otgMouseInstance->hostHandle);
            otgMouseInstance->otgMouseState = kState_None;
            otgMouseInstance->hostHandle = NULL;
            EnableIRQ(usbIrq);
            if (status == kStatus_USB_Success)
            {
                usb_echo("host deinit success\r\n");
            }
            else
            {
                usb_echo("host deinit fail\r\n");
            }
            break;

        case kOtg_StackDeviceDeinit:
            DisableIRQ(usbIrq);
            status = USB_OtgDeviceMouseDeinit(otgMouseInstance->deviceHandle);
            otgMouseInstance->otgMouseState = kState_None;
            otgMouseInstance->deviceHandle = NULL;
            EnableIRQ(usbIrq);
            if (status == kStatus_USB_Success)
            {
                usb_echo("device deinit success\r\n");
            }
            else
            {
                usb_echo("device deinit fail\r\n");
            }
            break;

        default:
            break;
    }
}

void USB_OtgHidMouseApplicationTask(void *param)
{
    usb_otg_mouse_instance_t *otgMouseInstance = (usb_otg_mouse_instance_t *)param;
    char uartData;

    switch (otgMouseInstance->runState)
    {
        case kUSB_HostOtgRunIdle:
            if (DbgConsole_TryGetchar(&uartData) == kStatus_USB_Success)
            {
                if (uartData == 'p')
                {
                    USB_OtgMousePrintMenu(otgMouseInstance);
                }
                else
                {
                    USB_OtgMouseProcessMenu(otgMouseInstance, uartData);
                }
            }

            if (otgMouseInstance->otgMouseState == kState_Host)
            {
                USB_HostHidMouseTask();
            }
            break;

        default:
            break;
    }
}

static void USB_OtgCallback(void *param, uint8_t eventType, uint32_t eventValue)
{
    usb_otg_mouse_instance_t *otgMouseInstance = (usb_otg_mouse_instance_t *)param;

    if (eventType == kOtg_EventStateChange)
    {
        otgMouseInstance->otgStateMachine = eventValue;
        USB_OtgAppPrintState(eventValue);
    }
    else if (eventType == kOtg_EventStackInit)
    {
        USB_OtgMouseProcessStack(otgMouseInstance, eventValue);
    }
    else
    {
    }
}

static void USB_OtgApplicationInit(void)
{
    usb_status_t status;

#if ((defined USB_OTG_KHCI_PERIPHERAL_ENABLE) && (USB_OTG_KHCI_PERIPHERAL_ENABLE))
    /* initialize otg peripheral */
    USB_OtgMax3353Init();
#endif

    g_OtgMouseInstance.hostHandle = NULL;
    g_OtgMouseInstance.deviceHandle = NULL;
    g_OtgMouseInstance.runState = kUSB_HostOtgRunIdle;
    g_OtgMouseInstance.otgStateMachine = kOtg_State_Start;
    g_OtgMouseInstance.otgMouseState = kState_None;
    g_OtgMouseInstance.aSetBHNPEnable = 0U;

    /* initialize khci clock */
    USB_OtgClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    /* initialize otg stack */
    status = USB_OtgInit(kUSB_ControllerKhci0, &g_OtgHandle, USB_OtgCallback, &g_OtgMouseInstance);
    if (status != kStatus_USB_Success)
    {
        usb_echo("usb otg stack init error\r\n");
        return;
    }

    USB_OtgIsrEnable();

    usb_echo("usb otg stack init done\r\n");
}

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    USB_OtgApplicationInit();

    while (1)
    {
        USB_OtgTaskFunction(g_OtgHandle);
        USB_OtgHidMouseApplicationTask(&g_OtgMouseInstance);

        if (g_OtgMouseInstance.otgMouseState == kState_Device)
        {
#if USB_DEVICE_CONFIG_USE_TASK
            USB_DeviceKhciTaskFunction(g_OtgMouseInstance.deviceHandle);
#endif
        }
        else if (g_OtgMouseInstance.otgMouseState == kState_Host)
        {
            USB_HostKhciTaskFunction(g_OtgMouseInstance.hostHandle);
        }
        else
        {
        }
    }
}
