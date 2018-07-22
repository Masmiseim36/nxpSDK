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

#include "usb_otg_config.h"
#include "usb_host_config.h"
#include "usb_device_config.h"
#include "usb_otg.h"
#include "usb_host.h"
#include "usb_device.h"
#include "usb_otg_mouse.h"
#include "board.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if ((defined USB_OTG_KHCI_PERIPHERAL_ENABLE) && (USB_OTG_KHCI_PERIPHERAL_ENABLE))
#include "usb_otg_max3353.h"
#endif
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define GPIO_IRQHandler PORTE_IRQHandler
#define BOARD_MAX3353_GPIO_PIN (28)
#define BOARD_MAX3353_GPIO_PORT (PORTE)
#define BOARD_MAX3353_GPIO (GPIOE)
#define BOARD_MAX3353_GPIO_IRQ (PORTE_IRQn)

#define BOARD_MAX3353_I2C (I2C0)
#define BOARD_MAX3353_I2C_CLK_SRC (kCLOCK_BusClk)
#define BOARD_MAX3353_I2C_SLAVE_ADDR_7BIT (0x2CU)
#define BOARD_MAX3353_I2C_BAUDRATE (100U)

/* USB clock source and frequency*/
#define USB_FS_CLK_SRC kCLOCK_UsbSrcPll0
#define USB_FS_CLK_FREQ CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define USB_HS_PHY_CLK_SRC kCLOCK_UsbPhySrcExt
#define USB_HS_PHY_CLK_FREQ BOARD_XTAL0_CLK_HZ
#define USB_HS_CLK_SRC kCLOCK_UsbSrcUnused
#define USB_HS_CLK_FREQ 0U

#define USB_HOST_INTERRUPT_PRIORITY (3U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern usb_status_t USB_OtgHostMouseInit(usb_host_handle *hostHandle);
extern usb_status_t USB_OtgHostMouseDeinit(usb_host_handle hostHandle);
extern usb_status_t USB_OtgDeviceMouseInit(usb_device_handle *deviceHandle);
extern usb_status_t USB_OtgDeviceMouseDeinit(usb_device_handle deviceHandle);
extern void USB_HostHidMouseTask(void);

void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

usb_otg_handle g_OtgHandle;
usb_otg_mouse_instance_t g_OtgMouseInstance;

/*******************************************************************************
 * Code
 ******************************************************************************/

usb_status_t USB_UartTryReadChar(char *value)
{
#if (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART)
    UART_Type *base = (UART_Type *)BOARD_DEBUG_UART_BASEADDR;
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    if (base->RCFIFO)
#else
    if (base->S1 & UART_S1_RDRF_MASK)
#endif
    {
        *value = (char)base->D;
        return kStatus_USB_Success;
    }
    else
    {
        return kStatus_USB_Error;
    }

#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)
    LPUART_Type *base = (LPUART_Type *)BOARD_DEBUG_UART_BASEADDR;
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    if ((base->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT)
#else
    if (base->STAT & LPUART_STAT_RDRF_MASK)
#endif
    {
        *value = (char)base->DATA;
        return kStatus_USB_Success;
    }
    else
    {
        return kStatus_USB_Error;
    }

#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)
    UART0_Type *base = (UART0_Type *)BOARD_DEBUG_UART_BASEADDR;
#if defined(FSL_FEATURE_LPSCI_HAS_FIFO) && FSL_FEATURE_LPSCI_HAS_FIFO
    if (base->RCFIFO)
#else
    if (base->S1 & UART0_S1_RDRF_MASK)
#endif
    {
        *value = (char)base->DATA;
        return kStatus_USB_Success;
    }
    else
    {
        return kStatus_USB_Error;
    }
#endif
}

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
                NVIC_DisableIRQ(usbIrq);
                status = USB_OtgDeviceMouseDeinit(otgMouseInstance->deviceHandle);
                otgMouseInstance->otgMouseState = kState_None;
                otgMouseInstance->deviceHandle = NULL;
                NVIC_EnableIRQ(usbIrq);
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
            NVIC_DisableIRQ(usbIrq);
            otgMouseInstance->otgMouseState = kState_Host;
            otgMouseInstance->aSetBHNPEnable = 0U;
            status = USB_OtgHostMouseInit(&otgMouseInstance->hostHandle);
            NVIC_EnableIRQ(usbIrq);
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
                NVIC_DisableIRQ(usbIrq);
                status = USB_OtgHostMouseDeinit(otgMouseInstance->hostHandle);
                otgMouseInstance->otgMouseState = kState_None;
                otgMouseInstance->hostHandle = NULL;
                NVIC_EnableIRQ(usbIrq);
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
            NVIC_DisableIRQ(usbIrq);
            otgMouseInstance->otgMouseState = kState_Device;
            status = USB_OtgDeviceMouseInit(&otgMouseInstance->deviceHandle);
            NVIC_EnableIRQ(usbIrq);
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
            NVIC_DisableIRQ(usbIrq);
            status = USB_OtgHostMouseDeinit(otgMouseInstance->hostHandle);
            otgMouseInstance->otgMouseState = kState_None;
            otgMouseInstance->hostHandle = NULL;
            NVIC_EnableIRQ(usbIrq);
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
            NVIC_DisableIRQ(usbIrq);
            status = USB_OtgDeviceMouseDeinit(otgMouseInstance->deviceHandle);
            otgMouseInstance->otgMouseState = kState_None;
            otgMouseInstance->deviceHandle = NULL;
            NVIC_EnableIRQ(usbIrq);
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
        case kOtgRunIdle:
            if (USB_UartTryReadChar(&uartData) == kStatus_USB_Success)
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
    IRQn_Type usbIrq;
    IRQn_Type usbFsIrqs[] = USB_IRQS;

#if ((defined USB_OTG_KHCI_PERIPHERAL_ENABLE) && (USB_OTG_KHCI_PERIPHERAL_ENABLE))
    /* initialize otg peripheral */
    USB_OtgMax3353Init();
#endif

    g_OtgMouseInstance.hostHandle = NULL;
    g_OtgMouseInstance.deviceHandle = NULL;
    g_OtgMouseInstance.runState = kOtgRunIdle;
    g_OtgMouseInstance.otgStateMachine = kOtg_State_Start;
    g_OtgMouseInstance.otgMouseState = kState_None;
    g_OtgMouseInstance.aSetBHNPEnable = 0U;

    /* initialize khci clock */
    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

    /* initialize otg stack */
    status = USB_OtgInit(kUSB_ControllerKhci0, &g_OtgHandle, USB_OtgCallback, &g_OtgMouseInstance);
    if (status != kStatus_USB_Success)
    {
        usb_echo("usb otg stack init error\r\n");
        return;
    }

    /* initialize usb interrupt */
    usbIrq = usbFsIrqs[kUSB_ControllerKhci0 - kUSB_ControllerKhci0];
    NVIC_SetPriority(usbIrq, USB_HOST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(usbIrq);

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
