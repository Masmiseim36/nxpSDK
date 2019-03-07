/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_printer.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "device_printer.h"
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if ((defined USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI))
#include "usb_phy.h"
#endif /* USB_HOST_CONFIG_EHCI */

#include "pin_mux.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB clock source and frequency*/
#define USB_HS_PHY_CLK_SRC kCLOCK_Usbphy480M
#define USB_HS_PHY_CLK_FREQ 480000000U
#define USB_HS_CLK_SRC kCLOCK_Usb480M
#define USB_HS_CLK_FREQ 480000000U
#define BOARD_XTAL0_CLK_HZ 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* The printer class callback */
static usb_status_t USB_DevicePrinterAppCallback(class_handle_t classHandle, uint32_t event, void *param);
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DATA_ALIGNMENT uint8_t s_PrinterBuffer[USB_PRINTER_BUFFER_SIZE];
/* printer device id for interface zero */
uint8_t g_PrinterID[] = "xxMFG:NXP;MDL:ksdk printer demo;CMD:POSTSCRIPT";

usb_device_printer_app_t g_DevicePrinterApp;

extern usb_device_class_struct_t g_UsbDevicePrinterClass;

/* set class configurations */
usb_device_class_config_struct_t g_UsbDevicePrinterClassConfig[1] = {{
    USB_DevicePrinterAppCallback, /* printer class callback pointer */
    (class_handle_t)NULL,         /* The printer class handle, This field is set by USB_DeviceClassInit */
    &g_UsbDevicePrinterClass,     /* The printer configuration, including class code, subclass code, and protocol, class
                           type,
                           transfer type, endpoint address, max packet size, etc.*/
}};

/* set class configuration list */
usb_device_class_config_list_struct_t g_UsbDevicePrinterConfigList = {
    g_UsbDevicePrinterClassConfig, /* Class configurations */
    USB_DeviceCallback,            /* Device callback pointer */
    1U,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* ksdk debug console must have been initialized. */
static void USB_PrinterPrintData(uint8_t *data, uint32_t length)
{
    while (length--)
    {
        PUTCHAR(*(data++));
    }
}

static usb_status_t USB_DevicePrinterAppCallback(class_handle_t classHandle, uint32_t event, void *param)
{
    usb_status_t status = kStatus_USB_Error;
    usb_device_printer_class_request_t *classRequest;
    usb_device_endpoint_callback_message_struct_t *message;

    uint32_t len;

    switch (event)
    {
        case kUSB_DevicePrinterEventGetDeviceId:
            classRequest = (usb_device_printer_class_request_t *)param;
            if ((classRequest->configIndex == 0U) && (classRequest->interface == USB_PRINTER_INTERFACE_INDEX) &&
                (classRequest->alternateSetting == 0))
            {
                len = sizeof(g_PrinterID) - 1;
                g_PrinterID[0] = ((uint8_t)(len >> 8));
                g_PrinterID[1] = (uint8_t)len;
                classRequest->buffer = g_PrinterID;
                classRequest->length = len;
            }
            else
            {
                classRequest->buffer = NULL;
                classRequest->length = 0U;
            }
            break;

        case kUSB_DevicePrinterEventGetPortStatus:
            classRequest = (usb_device_printer_class_request_t *)param;
            classRequest->buffer = &(g_DevicePrinterApp.printerPortStatus);
            classRequest->length = 1U;
            break;

        case kUSB_DevicePrinterEventSoftReset:
            g_DevicePrinterApp.printerState = kPrinter_Idle;
            break;

        case kUSB_DevicePrinterEventRecvResponse:
            message = (usb_device_endpoint_callback_message_struct_t *)param;
            if ((g_DevicePrinterApp.attach) && (g_DevicePrinterApp.printerState == kPrinter_Receiving))
            {
                if ((message != NULL) && (message->length != USB_UNINITIALIZED_VAL_32))
                {
                    g_DevicePrinterApp.printerState = kPrinter_Received;
                    g_DevicePrinterApp.dataReceiveLength = message->length;
                }
                else
                {
                    g_DevicePrinterApp.printerState = kPrinter_Idle;
                    status = USB_DevicePrinterRecv(g_DevicePrinterApp.classHandle, USB_PRINTER_BULK_ENDPOINT_OUT,
                                                   g_DevicePrinterApp.printerBuffer, USB_PRINTER_BUFFER_SIZE);
                    if (status == kStatus_USB_Success)
                    {
                        g_DevicePrinterApp.printerState = kPrinter_Receiving;
                    }
                }
            }
            break;

        case kUSB_DevicePrinterEventSendResponse:
            USB_DevicePrinterSend(g_DevicePrinterApp.classHandle, USB_PRINTER_BULK_ENDPOINT_IN,
                                  g_DevicePrinterApp.sendBuffer, g_DevicePrinterApp.sendLength);
            break;

        default:
            break;
    }

    return status;
}

static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t status = kStatus_USB_Error;
    uint8_t *param8p = (uint8_t *)param;
    uint16_t *param16p = (uint16_t *)param;
    uint8_t interface;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
            g_DevicePrinterApp.attach = 0U;
            g_DevicePrinterApp.printerState = kPrinter_Idle;
            g_DevicePrinterApp.sendBuffer = NULL;
            g_DevicePrinterApp.sendLength = 0;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &g_DevicePrinterApp.speed))
            {
                USB_DeviceSetSpeed(handle, g_DevicePrinterApp.speed);
            }
#endif
            status = kStatus_USB_Success;
            break;

#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE > 0U))

        case kUSB_DeviceEventAttach:
            usb_echo("USB device attached.\r\n");
            USB_DeviceRun(g_DevicePrinterApp.deviceHandle);
            break;

        case kUSB_DeviceEventDetach:
            usb_echo("USB device detached.\r\n");
            g_DevicePrinterApp.attach = 0;
            USB_DeviceStop(g_DevicePrinterApp.deviceHandle);
            break;
#endif

        case kUSB_DeviceEventSetConfiguration:
            if (param)
            {
                /* Set device configuration request */
                g_DevicePrinterApp.attach = 1U;
                g_DevicePrinterApp.printerState = kPrinter_Idle;
                g_DevicePrinterApp.currentConfiguration = *param8p;
                if (USB_PRINTER_CONFIGURE_INDEX == *param8p)
                {
                    /* demo run */
                    status = USB_DevicePrinterRecv(g_DevicePrinterApp.classHandle, USB_PRINTER_BULK_ENDPOINT_OUT,
                                                   g_DevicePrinterApp.printerBuffer, USB_PRINTER_BUFFER_SIZE);
                    if (status == kStatus_USB_Success)
                    {
                        g_DevicePrinterApp.printerState = kPrinter_Receiving;
                    }

                    USB_DevicePrinterSend(g_DevicePrinterApp.classHandle, USB_PRINTER_BULK_ENDPOINT_IN,
                                          g_DevicePrinterApp.sendBuffer, g_DevicePrinterApp.sendLength);
                }
            }
            break;

        case kUSB_DeviceEventSetInterface:
            if (g_DevicePrinterApp.attach)
            {
                g_DevicePrinterApp.printerState = kPrinter_Idle;
                /* Set device interface request */
                interface = (uint8_t)((*param16p & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*param16p & 0x00FFU);

                if (interface < USB_PRINTER_INTERFACE_COUNT)
                {
                    g_DevicePrinterApp.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    /* demo run */
                    if (alternateSetting == 0U)
                    {
                        status = USB_DevicePrinterRecv(g_DevicePrinterApp.classHandle, USB_PRINTER_BULK_ENDPOINT_OUT,
                                                       g_DevicePrinterApp.printerBuffer, USB_PRINTER_BUFFER_SIZE);
                        if (status == kStatus_USB_Success)
                        {
                            g_DevicePrinterApp.printerState = kPrinter_Receiving;
                        }

                        USB_DevicePrinterSend(g_DevicePrinterApp.classHandle, USB_PRINTER_BULK_ENDPOINT_IN,
                                              g_DevicePrinterApp.sendBuffer, g_DevicePrinterApp.sendLength);
                    }
                    status = kStatus_USB_Success;
                }
            }
            break;

        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                /* Get current configuration request */
                *param8p = g_DevicePrinterApp.currentConfiguration;
                status = kStatus_USB_Success;
            }
            break;

        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                /* Get current alternate setting of the interface request */
                interface = (uint8_t)((*param16p & 0xFF00U) >> 0x08U);
                if (interface < USB_PRINTER_INTERFACE_COUNT)
                {
                    *param16p = (*param16p & 0xFF00U) | g_DevicePrinterApp.currentInterfaceAlternateSetting[interface];
                    status = kStatus_USB_Success;
                }
                else
                {
                    status = kStatus_USB_InvalidRequest;
                }
            }
            break;

        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                /* Get device descriptor request */
                status = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;

        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                /* Get device configuration descriptor request */
                status = USB_DeviceGetConfigurationDescriptor(
                    handle, (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;

        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                /* Get device string descriptor request */
                status = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;

        default:
            break;
    }

    return status;
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_DevicePrinterApp.deviceHandle);
}
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
#if defined(FSL_FEATURE_USBHS_EHCI_COUNT) && (FSL_FEATURE_USBHS_EHCI_COUNT > 1U)
void USB1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_DevicePrinterApp.deviceHandle);
}
#endif
#endif
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_DevicePrinterApp.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_DevicePrinterApp.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_DevicePrinterApp.deviceHandle);
}
#endif

static void USB_DeviceApplicationInit(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL, BOARD_USB_PHY_TXCAL45DP, BOARD_USB_PHY_TXCAL45DM,
    };

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs1Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
#else
    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];

    /* enable USB IP clock */
    CLOCK_EnableUsbfs0DeviceClock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

#if (((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)) || \
     ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)))
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
#endif

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

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

    /* set printer app to default state */
    g_DevicePrinterApp.printerPortStatus = USB_DEVICE_PRINTER_PORT_STATUS_DEFAULT_VALUE;
    g_DevicePrinterApp.printerState = kPrinter_Idle;
    g_DevicePrinterApp.speed = USB_SPEED_FULL;
    g_DevicePrinterApp.attach = 0U;
    g_DevicePrinterApp.classHandle = (uint32_t)NULL;
    g_DevicePrinterApp.deviceHandle = NULL;
    g_DevicePrinterApp.printerBuffer = s_PrinterBuffer;

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success !=
        USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDevicePrinterConfigList, &g_DevicePrinterApp.deviceHandle))
    {
        usb_echo("USB device printer fail\r\n");
        return;
    }
    else
    {
        usb_echo("USB device printer demo\r\n");
        /* Get the printer class handle */
        g_DevicePrinterApp.classHandle = g_UsbDevicePrinterConfigList.config->classHandle;
    }

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);

    /* Start USB printer demo */
    USB_DeviceRun(g_DevicePrinterApp.deviceHandle);
}

void USB_DevicePrinterAppTask(void *parameter)
{
    usb_device_printer_app_t *printerApp = (usb_device_printer_app_t *)parameter;
    usb_status_t status = kStatus_USB_Error;

    if (printerApp->attach)
    {
        if (printerApp->printerState == kPrinter_Received)
        {
            USB_PrinterPrintData(printerApp->printerBuffer, printerApp->dataReceiveLength);
            printerApp->printerState = kPrinter_Idle;
        }

        if (printerApp->printerState == kPrinter_Idle)
        {
            status = USB_DevicePrinterRecv(printerApp->classHandle, USB_PRINTER_BULK_ENDPOINT_OUT,
                                           printerApp->printerBuffer, USB_PRINTER_BUFFER_SIZE);

            if (status == kStatus_USB_Success)
            {
                printerApp->printerState = kPrinter_Receiving;
            }
        }
    }
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    /*  BOARD_InitMemory();  */ /*USB code doesn't fully support cached in this release */
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(USB_OTG1_IRQn, (system_irq_handler_t)(uint32_t)USBHS_IRQHandler, NULL);

    USB_DeviceApplicationInit();

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(g_DevicePrinterApp.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(g_DevicePrinterApp.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_DevicePrinterApp.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_DevicePrinterApp.deviceHandle);
#endif
#endif

        USB_DevicePrinterAppTask(&g_DevicePrinterApp);
    }
}
