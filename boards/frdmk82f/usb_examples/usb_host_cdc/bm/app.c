/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "usb_host_cdc.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "usb_uart_drv.h"
#include "host_cdc.h"
#include "fsl_common.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#include "usb_phy.h"
#endif

#if ((!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI))
#error Please enable USB_HOST_CONFIG_KHCI or USB_HOST_CONFIG_EHCI in file usb_host_config.
#endif

#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USB_HOST_INTERRUPT_PRIORITY (3U)

/*******************************************************************************
  * Prototypes
  ******************************************************************************/
void BOARD_InitHardware(void);
extern void UART_UserCallback(USB_UartType *base, usb_uart_handle_t *handle, status_t status, void *param);
extern void USB_UartIRQHandler(USB_UartType *base, usb_uart_handle_t *handle);
/*******************************************************************************
 * Variables
 ******************************************************************************/

usb_host_handle g_hostHandle;
volatile uint8_t g_AttachFlag;

usb_uart_handle_t g_UartHandle;

extern char usbRecvUart[USB_HOST_CDC_UART_RX_MAX_LEN];
usb_xfer_t g_xfer;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief USB isr function.
 */
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
void USB0_IRQHandler(void)
{
    USB_HostKhciIsrFunction(g_hostHandle);
}
#endif
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
void USBHS_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_hostHandle);
}
#endif

/*!
 * @brief host callback function.
 *
 * device attach/detach callback function.
 *
 * @param deviceHandle           device handle.
 * @param configurationHandle attached device's configuration descriptor information.
 * @param event_code           callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The application don't support the configuration.
 */
usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                           usb_host_configuration_handle configurationHandle,
                           uint32_t event_code)
{
    usb_status_t status;
    status = kStatus_USB_Success;

    switch (event_code)
    {
        case kUSB_HostEventAttach:
            status = USB_HostCdcEvent(deviceHandle, configurationHandle, event_code);
            break;
        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostCdcEvent(deviceHandle, configurationHandle, event_code);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostCdcEvent(deviceHandle, configurationHandle, event_code);
            break;

        default:
            break;
    }
    return status;
}

/*!
 * @brief app initialization.
 */
void APP_init(void)
{
    usb_status_t status = kStatus_USB_Success;
    IRQn_Type usb_irq;
    usb_uartConfiguration uartConfiguration;

    g_xfer.buffer = (uint8_t *)&usbRecvUart[0];
    g_xfer.size = USB_HOST_CDC_UART_RX_MAX_LEN;
    USB_UartGetDefaultConfiguratoion(&uartConfiguration);
    uartConfiguration.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    uartConfiguration.enableTx = true;
    uartConfiguration.enableRx = true;
    USB_UartInit((USB_UartType *)BOARD_DEBUG_UART_BASEADDR, &uartConfiguration, CLOCK_GetFreq(BOARD_DEBUG_UART_CLKSRC));
    USB_UartCreateHandle((USB_UartType *)BOARD_DEBUG_UART_BASEADDR, &g_UartHandle, UART_UserCallback, NULL);

    USB_UartReceiveDataIRQ((USB_UartType *)BOARD_DEBUG_UART_BASEADDR, &g_UartHandle, &g_xfer, NULL);

    g_AttachFlag = 0;

    USB_HostCdcInitBuffer();

#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
    IRQn_Type usb_fs_irqs[] = USB_IRQS;
    usb_irq = usb_fs_irqs[CONTROLLER_ID - kUSB_ControllerKhci0];
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
#endif
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
    IRQn_Type usb_hs_irqs[] = USBHS_IRQS;
    usb_irq = usb_hs_irqs[CONTROLLER_ID - kUSB_ControllerEhci0];
    CLOCK_EnableUsbhs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if ((defined FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

    status = USB_HostInit(CONTROLLER_ID, &g_hostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host init error\r\n");
        return;
    }
    NVIC_SetPriority(usb_irq, USB_HOST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(usb_irq);

    usb_echo("host init done\r\n");
    usb_echo("This example requires that the CDC device uses Hardware flow\r\n");
    usb_echo(
        "if the device does't support it, please set USB_HOST_UART_SUPPORT_HW_FLOW to zero and rebuild this "
        "project\r\n");
    usb_echo("Type strings, then the string\r\n");
    usb_echo("will be echoed back from the device\r\n");
}

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    APP_init();

    while (1)
    {
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
        USB_HostKhciTaskFunction(g_hostHandle);
#endif
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
        USB_HostEhciTaskFunction(g_hostHandle);
#endif

        USB_HosCdcTask(&g_cdc);
    }
}
