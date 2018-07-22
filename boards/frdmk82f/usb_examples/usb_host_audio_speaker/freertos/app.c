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
#include "usb_host_hid.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "audio_speaker.h"
#include "fsl_common.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#include "usb_phy.h"
#endif /* USB_HOST_CONFIG_EHCI */

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif /* USB_HOST_CONFIG_KHCI */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif /* USB_HOST_CONFIG_EHCI */

#define USB_HOST_INTERRUPT_PRIORITY (3U)
/*******************************************************************************
 * Variables
 ******************************************************************************/
usb_host_handle g_hostHandle;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
extern void USB_AudioTask(void *arg);
extern void USB_KeypadTask(void *arg);
extern usb_status_t USB_HostAudioEvent(usb_device_handle deviceHandle,
                                       usb_host_configuration_handle configurationHandle,
                                       uint32_t eventCode);
extern usb_status_t USB_HostKeypadEvent(usb_device_handle deviceHandle,
                                        usb_host_configuration_handle configurationHandle,
                                        uint32_t eventCode);

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

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief host callback function.
 *
 * device attach/detach callback function.
 *
 * @param deviceHandle           device handle.
 * @param configurationHandle attached device's configuration descriptor information.
 * @param eventCode           callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The application don't support the configuration.
 */
usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                           usb_host_configuration_handle configurationHandle,
                           uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            USB_HostKeypadEvent(deviceHandle, configurationHandle, eventCode);
            status = USB_HostAudioEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            USB_HostKeypadEvent(deviceHandle, configurationHandle, eventCode);
            status = USB_HostAudioEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            USB_HostKeypadEvent(deviceHandle, configurationHandle, eventCode);
            status = USB_HostAudioEvent(deviceHandle, configurationHandle, eventCode);
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
}

/*!
 * @brief usb host controller driver task.
 */
void USB_HostTask(void *hostHandle)
{
    while (1)
    {
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
        USB_HostEhciTaskFunction(g_hostHandle);
#endif
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
        USB_HostKhciTaskFunction(g_hostHandle);
#endif
    }
}

/*!
 * @brief keypad task.
 */
void Keypad_AppTask(void *param)
{
    while (1)
    {
        USB_KeypadTask(param);
        vTaskDelay(1);
    }
}

/*!
 * @brief audio task.
 */
void Audio_AppTask(void *param)
{
    while (1)
    {
        USB_AudioTask(param);
        vTaskDelay(1);
    }
}

/*!
 * @brief main function.
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    APP_init();

    if (xTaskCreate(USB_HostTask, "usb host task", 2000L / sizeof(portSTACK_TYPE), g_hostHandle, 4U, NULL) != pdPASS)
    {
        usb_echo("create host task error\r\n");
    }
    if (xTaskCreate(Audio_AppTask, "app task", 2000L / sizeof(portSTACK_TYPE), NULL, 3U, NULL) != pdPASS)
    {
        usb_echo("create audio task error\r\n");
    }
    if (xTaskCreate(Keypad_AppTask, "app task", 2000L / sizeof(portSTACK_TYPE), NULL, 3U, NULL) != pdPASS)
    {
        usb_echo("create keypad task error\r\n");
    }
    vTaskStartScheduler();

    while (1)
    {
        ;
    }
}
