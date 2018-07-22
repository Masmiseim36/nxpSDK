/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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

#include "usb_host_config.h"
#include "usb_device_config.h"
#include "usb_host.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_descriptor.h"
#include "fsl_device_registers.h"
#include "usb_host_hid.h"
#include "board.h"
#include "host_keyboard.h"
#include "device_mouse.h"
#include "usb_phy.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

#if ((!defined USB_HOST_CONFIG_KHCI) || (!USB_HOST_CONFIG_KHCI))
#error Please enable USB_HOST_CONFIG_KHCI in file usb_host_config.
#endif
#if ((!defined USB_DEVICE_CONFIG_EHCI) || (!USB_DEVICE_CONFIG_EHCI))
#error Please enable USB_DEVICE_CONFIG_EHCI in file usb_device_config.
#endif

#include "fsl_gpio.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB clock source and frequency*/
#define USB_FS_CLK_SRC kCLOCK_UsbSrcPll0
#define USB_FS_CLK_FREQ CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define USB_HS_PHY_CLK_SRC kCLOCK_UsbPhySrcExt
#define USB_HS_PHY_CLK_FREQ BOARD_XTAL0_CLK_HZ
#define USB_HS_CLK_SRC kCLOCK_UsbSrcUnused
#define USB_HS_CLK_FREQ 0U

#define USB_HOST_INTERRUPT_PRIORITY (3U)
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief host callback function.
 *
 * device attach/detach callback function.
 *
 * @param deviceHandle          device handle.
 * @param configurationHandle   attached device's configuration descriptor information.
 * @param eventCode             callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The application don't support the configuration.
 */
static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode);

/*!
 * @brief application initialization.
 */
static void USB_HostApplicationInit(void);

static void USB_DeviceApplicationInit(void);

void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern usb_host_keyboard_instance_t g_HostHidKeyboard;
extern usb_device_class_config_list_struct_t g_UsbDeviceHidConfigList;
usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
usb_host_handle g_HostHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief USB host isr function.
 */
void USB0_IRQHandler(void)
{
    USB_HostKhciIsrFunction(g_HostHandle);
}

/*!
 * @brief USB device isr function.
 */
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}

static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
    return USB_HostHidKeyboardEvent(deviceHandle, configurationHandle, eventCode);
}

static void USB_HostApplicationInit(void)
{
    usb_status_t status = kStatus_USB_Success;
    IRQn_Type usbIrq;

    IRQn_Type usbFsIrqs[] = USB_IRQS;
    usbIrq = usbFsIrqs[0]; /* kUSB_ControllerKhci0 */
    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);

    status = USB_HostInit(kUSB_ControllerKhci0, &g_HostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host init error\r\n");
        return;
    }
    NVIC_SetPriority(usbIrq, USB_HOST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(usbIrq);

    usb_echo("USB KHCI initialize as host, USB EHCI initialize as device\r\n");

    usb_echo("w: move up\r\n");
    usb_echo("s: move down\r\n");
    usb_echo("a: move left\r\n");
    usb_echo("d: move right\r\n");
}

static void USB_DeviceApplicationInit(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[0]; /* kUSB_ControllerEhci0 */
    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    USB_EhciPhyInit(kUSB_ControllerEhci0, BOARD_XTAL0_CLK_HZ);

    /* Set HID mouse to default state */
    g_UsbDeviceHidMouse.speed = USB_SPEED_FULL;
    g_UsbDeviceHidMouse.attach = 0U;
    g_UsbDeviceHidMouse.hidHandle = (class_handle_t)NULL;
    g_UsbDeviceHidMouse.deviceHandle = NULL;

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success !=
        USB_DeviceClassInit(kUSB_ControllerEhci0, &g_UsbDeviceHidConfigList, &g_UsbDeviceHidMouse.deviceHandle))
    {
        usb_echo("USB device mouse failed\r\n");
        return;
    }
    else
    {
        /* Get the HID mouse class handle */
        g_UsbDeviceHidMouse.hidHandle = g_UsbDeviceHidConfigList.config->classHandle;
    }

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irqNumber);

    /* Start USB device HID mouse */
    USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
}

int main(void)
{
    gpio_pin_config_t pinConfig;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* enable usb host vbus */
    pinConfig.pinDirection = kGPIO_DigitalOutput;
    pinConfig.outputLogic = 1U;

    GPIO_PinInit(PTD, 8U, &pinConfig);

#if ((defined FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
    USB_DeviceApplicationInit();
    USB_HostApplicationInit();

    while (1)
    {
        USB_HostKhciTaskFunction(g_HostHandle);
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceEhciTaskFunction(g_UsbDeviceHidMouse.deviceHandle);
#endif
        USB_HostHidKeyboardTask(&g_HostHidKeyboard);
    }
}
