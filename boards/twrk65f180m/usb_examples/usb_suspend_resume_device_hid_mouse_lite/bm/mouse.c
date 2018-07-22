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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_hid.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "mouse.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "fsl_pit.h"

#include "fsl_port.h"

#include "fsl_smc.h"

#include "fsl_gpio.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB clock source and frequency*/
#define USB_FS_CLK_SRC kCLOCK_UsbSrcIrc48M
#define USB_FS_CLK_FREQ 48000000U
#define USB_HS_PHY_CLK_SRC kCLOCK_UsbPhySrcExt
#define USB_HS_PHY_CLK_FREQ BOARD_XTAL0_CLK_HZ
#define USB_HS_CLK_SRC kCLOCK_UsbSrcUnused
#define USB_HS_CLK_FREQ 0U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void HW_TIMER_IRQ_HANDLER(void);
void BOARD_InitHardware(void);
static usb_status_t USB_DeviceHidMouseAction(void);
static usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                             usb_device_endpoint_callback_message_struct_t *event,
                                             void *arg);
static void USB_DeviceApplicationInit(void);

void BOARD_InitPins(void);
void BOARD_DeinitPins(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DATA_ALIGNMENT static uint8_t s_MouseBuffer[USB_HID_MOUSE_REPORT_LENGTH];
usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

extern uint8_t g_UsbDeviceCurrentConfigure;
extern uint8_t g_UsbDeviceInterface[USB_HID_MOUSE_INTERFACE_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

void PIT0_IRQHandler(void)
{
    HW_TIMER_IRQ_HANDLER();
}

/*!
 * @brief De-initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_DeinitPins(void)
{
    /* Affects PORTE_PCR16 register */
    PORT_SetPinMux(PORTE, 16u, kPORT_PinDisabledOrAnalog);
    /* Affects PORTE_PCR17 register */
    PORT_SetPinMux(PORTE, 17u, kPORT_PinDisabledOrAnalog);

    /* Gate the port clock */
    CLOCK_DisableClock(kCLOCK_PortE);
}

/* Update mouse pointer location. Draw a rectangular rotation*/
static usb_status_t USB_DeviceHidMouseAction(void)
{
    static int8_t x = 0U;
    static int8_t y = 0U;
    enum
    {
        RIGHT,
        DOWN,
        LEFT,
        UP
    };
    static uint8_t dir = RIGHT;

    switch (dir)
    {
        case RIGHT:
            /* Move right. Increase X value. */
            g_UsbDeviceHidMouse.buffer[1] = 2U;
            g_UsbDeviceHidMouse.buffer[2] = 0U;
            x++;
            if (x > 99U)
            {
                dir++;
            }
            break;
        case DOWN:
            /* Move down. Increase Y value. */
            g_UsbDeviceHidMouse.buffer[1] = 0U;
            g_UsbDeviceHidMouse.buffer[2] = 2U;
            y++;
            if (y > 99U)
            {
                dir++;
            }
            break;
        case LEFT:
            /* Move left. Discrease X value. */
            g_UsbDeviceHidMouse.buffer[1] = (uint8_t)(-2);
            g_UsbDeviceHidMouse.buffer[2] = 0U;
            x--;
            if (x < 2U)
            {
                dir++;
            }
            break;
        case UP:
            /* Move up. Discrease Y value. */
            g_UsbDeviceHidMouse.buffer[1] = 0U;
            g_UsbDeviceHidMouse.buffer[2] = (uint8_t)(-2);
            y--;
            if (y < 2U)
            {
                dir = RIGHT;
            }
            break;
        default:
            break;
    }
    /* Send mouse report to the host */
    return USB_DeviceSendRequest(g_UsbDeviceHidMouse.deviceHandle, USB_HID_MOUSE_ENDPOINT_IN,
                                 g_UsbDeviceHidMouse.buffer, USB_HID_MOUSE_REPORT_LENGTH);
}

/* HID mouse interrupt IN pipe callback */
static usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                             usb_device_endpoint_callback_message_struct_t *event,
                                             void *arg)
{
    /* Resport sent */
    if (g_UsbDeviceHidMouse.attach)
    {
        if ((NULL != event) && (event->length == USB_UNINITIALIZED_VAL_32))
        {
            return kStatus_USB_Error;
        }
        return USB_DeviceHidMouseAction();
    }

    return kStatus_USB_Error;
}

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* USB bus reset signal detected */
            /* Initialize the control IN and OUT pipes */
            USB_DeviceControlPipeInit(g_UsbDeviceHidMouse.deviceHandle);
            g_UsbDeviceHidMouse.attach = 0U;
            g_UsbDeviceHidMouse.remoteWakeup = 0U;
            g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
            g_UsbDeviceHidMouse.isResume = 0U;
            error = kStatus_USB_Success;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceHidMouse.deviceHandle, kUSB_DeviceStatusSpeed,
                                                           &g_UsbDeviceHidMouse.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceHidMouse.speed);
            }
#endif
        }
        break;
#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE > 0U))
        case kUSB_DeviceEventAttach:
        {
            usb_echo("USB device attached.\r\n");
            USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
        }
        break;

        case kUSB_DeviceEventDetach:
        {
            usb_echo("USB device detached.\r\n");
            g_UsbDeviceHidMouse.attach = 0;
            USB_DeviceStop(g_UsbDeviceHidMouse.deviceHandle);
        }
        break;
#endif
        case kUSB_DeviceEventSuspend:
        {
            /* USB device bus suspend signal detected */
            if (g_UsbDeviceHidMouse.attach)
            {
                g_UsbDeviceHidMouse.startTick = g_UsbDeviceHidMouse.hwTick;
                g_UsbDeviceHidMouse.suspend = kStatus_MouseStartSuspend;
                usb_echo("USB device start suspend\r\n");
                error = kStatus_USB_Success;
            }
        }
        break;
        case kUSB_DeviceEventResume:
        {
            /* USB device bus resume signal detected */
            if ((g_UsbDeviceHidMouse.attach) && (kStatus_MouseIdle != g_UsbDeviceHidMouse.suspend))
            {
                g_UsbDeviceHidMouse.isResume = 1U;
                usb_echo("USB device start resume\r\n");
                error = kStatus_USB_Success;
            }
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (USB_HID_MOUSE_CONFIGURE_INDEX == (*temp8))
            {
                /* If the confguration is valid, initliaze the HID mouse interrupt IN pipe */
                usb_device_endpoint_init_struct_t epInitStruct;
                usb_device_endpoint_callback_struct_t endpointCallback;

                endpointCallback.callbackFn = USB_DeviceHidInterruptIn;
                endpointCallback.callbackParam = handle;

                epInitStruct.zlt = 0U;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress =
                    USB_HID_MOUSE_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_UsbDeviceHidMouse.speed)
                {
                    epInitStruct.maxPacketSize = HS_HID_MOUSE_INTERRUPT_IN_PACKET_SIZE;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_HID_MOUSE_INTERRUPT_IN_PACKET_SIZE;
                }

                USB_DeviceInitEndpoint(g_UsbDeviceHidMouse.deviceHandle, &epInitStruct, &endpointCallback);

                g_UsbDeviceHidMouse.attach = 1U;
                error = USB_DeviceHidMouseAction(); /* run the cursor movement code */
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
    static uint32_t hid_mouse_setup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&hid_mouse_setup;
    return kStatus_USB_Success;
}

/* Configure device remote wakeup */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    if (g_UsbDeviceHidMouse.attach)
    {
        g_UsbDeviceHidMouse.remoteWakeup = enable;
        usb_echo("USB device remote wakeup state: %d\r\n", g_UsbDeviceHidMouse.remoteWakeup);
        return kStatus_USB_Success;
    }
    return kStatus_USB_InvalidRequest;
}

/* Configure the endpoint status (idle or stall) */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if ((USB_HID_MOUSE_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if ((USB_HID_MOUSE_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
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

    if (setup->wIndex != USB_HID_MOUSE_INTERFACE_INDEX)
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

void LLWU_SW_IRQ_HANDLER(void)
{
    if ((1U << LLWU_SW_GPIO_PIN) & PORT_GetPinsInterruptFlags(LLWU_SW_PORT))
    {
        /* Disable interrupt. */
        PORT_SetPinInterruptConfig(LLWU_SW_PORT, LLWU_SW_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(LLWU_SW_PORT, (1U << LLWU_SW_GPIO_PIN));
        g_UsbDeviceHidMouse.selfWakeup = 1U;
    }
}

#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))

void HW_TIMER_IRQ_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
    g_UsbDeviceHidMouse.hwTick++;
    USB_DeviceUpdateHwTick(g_UsbDeviceHidMouse.deviceHandle, g_UsbDeviceHidMouse.hwTick);
}

#endif

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif

static void USB_DeviceApplicationInit(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

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

    /* Set HID mouse default state */
    g_UsbDeviceHidMouse.speed = USB_SPEED_FULL;
    g_UsbDeviceHidMouse.attach = 0U;
    g_UsbDeviceHidMouse.deviceHandle = NULL;
    g_UsbDeviceHidMouse.buffer = s_MouseBuffer;
    g_UsbDeviceHidMouse.remoteWakeup = 0U;
    g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
    g_UsbDeviceHidMouse.selfWakeup = 0U;
    g_UsbDeviceHidMouse.isResume = 0U;

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_UsbDeviceHidMouse.deviceHandle))
    {
        usb_echo("USB device mouse failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device HID mouse demo\r\n");
    }

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irqNumber);

    /* Start USB device HID mouse */
    USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
}

void lowPowerInit(void)
{
    /* Set to allow entering vlps mode */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeVlp);

    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
    RTC->CR |= RTC_CR_OSCE_MASK;
#if (defined(FSL_FEATURE_SIM_OPT_HAS_USB_PHY) && (FSL_FEATURE_SIM_OPT_HAS_USB_PHY > 0))
    SIM->SOPT2 |= SIM_SOPT2_USBSLSRC_MASK;
#endif
}

void USB_PowerPreSwitchHook(void)
{
#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
    PIT_StopTimer(PIT, kPIT_Chnl_0);
#endif

    DbgConsole_Deinit();

    BOARD_DeinitPins();
    SMC_PreEnterStopModes();
}

void USB_PowerPostSwitchHook(void)
{
#if (defined(FSL_FEATURE_SOC_MCG_COUNT) && (FSL_FEATURE_SOC_MCG_COUNT > 0U))
    /* Wait for PLL lock. */
    while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
    {
    }
    CLOCK_SetPeeMode();
#endif
    SMC_PostExitStopModes();
    BOARD_InitPins();
    BOARD_InitDebugConsole();
#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
    PIT_StartTimer(PIT, kPIT_Chnl_0);
#endif
}

void USB_DeviceSuspendResumeTask(void)
{
    if (g_UsbDeviceHidMouse.isResume)
    {
        g_UsbDeviceHidMouse.isResume = 0U;
        if (kStatus_MouseIdle != g_UsbDeviceHidMouse.suspend)
        {
            g_UsbDeviceHidMouse.suspend = kStatus_MouseResumed;
        }
    }

    switch (g_UsbDeviceHidMouse.suspend)
    {
        case kStatus_MouseIdle:
            break;
        case kStatus_MouseStartSuspend:
            g_UsbDeviceHidMouse.suspend = kStatus_MouseSuspending;
            break;
        case kStatus_MouseSuspending:
            usb_echo("USB device suspended.\r\n");
            if (g_UsbDeviceHidMouse.remoteWakeup)
            {
                usb_echo("Please Press wakeup switch(%s) to remote wakeup the host.\r\n", LLWU_SW_NAME);
            }
            g_UsbDeviceHidMouse.suspend = kStatus_MouseSuspended;
            break;
        case kStatus_MouseSuspended:
            USB_PowerPreSwitchHook();
            if (g_UsbDeviceHidMouse.remoteWakeup)
            {
                PORT_SetPinInterruptConfig(LLWU_SW_PORT, LLWU_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
            }

            USB_DeviceSetStatus(g_UsbDeviceHidMouse.deviceHandle, kUSB_DeviceStatusBusSuspend, NULL);
            if (kStatus_Success != SMC_SetPowerModeVlps(SMC))
            {
                g_UsbDeviceHidMouse.selfWakeup = 1U;
                USB_PowerPostSwitchHook();
                usb_echo("Enter VLPS mode failed!\r\n");
            }
            else
            {
                USB_PowerPostSwitchHook();
            }
            if (g_UsbDeviceHidMouse.remoteWakeup)
            {
                PORT_SetPinInterruptConfig(LLWU_SW_PORT, LLWU_SW_GPIO_PIN, kPORT_InterruptOrDMADisabled);
                PORT_ClearPinsInterruptFlags(LLWU_SW_PORT, (1U << LLWU_SW_GPIO_PIN));
            }

            if (g_UsbDeviceHidMouse.attach)
            {
                g_UsbDeviceHidMouse.suspend = kStatus_MouseStartResume;
            }
            else
            {
                g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
            }
            break;
        case kStatus_MouseStartResume:
            if (g_UsbDeviceHidMouse.selfWakeup)
            {
                g_UsbDeviceHidMouse.selfWakeup = 0U;
                if (g_UsbDeviceHidMouse.remoteWakeup)
                {
                    if (kStatus_USB_Success ==
                        USB_DeviceSetStatus(g_UsbDeviceHidMouse.deviceHandle, kUSB_DeviceStatusBusResume, NULL))
                    {
                        usb_echo("Remote wakeup the host.\r\n");
                        g_UsbDeviceHidMouse.suspend = kStatus_MouseResuming;
                    }
                    else
                    {
                        usb_echo("Send resume signal failed.\r\n");
                        g_UsbDeviceHidMouse.suspend = kStatus_MouseStartResume;
                    }
                }
                else
                {
                    g_UsbDeviceHidMouse.suspend = kStatus_MouseResuming;
                }
            }
            else
            {
                g_UsbDeviceHidMouse.suspend = kStatus_MouseResumed;
            }
            break;
        case kStatus_MouseResuming:
            break;
        case kStatus_MouseResumed:
            usb_echo("USB device resumed.\r\n");
            USB_DeviceHidMouseAction();
            g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
            break;
        default:
            g_UsbDeviceHidMouse.suspend = kStatus_MouseIdle;
            break;
    }
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
    /* Structure of initialize PIT */
    pit_config_t pitConfig;
    IRQn_Type pitIrq[] = PIT_IRQS;
#endif

    gpio_pin_config_t pinConfig = {kGPIO_DigitalInput, 1U};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetXtal32Freq(BOARD_XTAL32K_CLK_HZ);
    BOARD_InitDebugConsole();

    /* Set the LLWU pin */
    GPIO_PinInit(LLWU_SW_GPIO, LLWU_SW_GPIO_PIN, &pinConfig);

    lowPowerInit();

#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000U, CLOCK_GetFreq(kCLOCK_BusClk)));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
#endif

    USB_DeviceApplicationInit();

#if ((defined(USB_DEVICE_CONFIG_REMOTE_WAKEUP)) && (USB_DEVICE_CONFIG_REMOTE_WAKEUP > 0U))
    /* Enable at the NVIC */
    EnableIRQ(pitIrq[0]);

    /* Start counting */
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    NVIC_SetPriority(LLWU_SW_IRQ, 3U);
    NVIC_EnableIRQ(LLWU_SW_IRQ);
#endif

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(g_UsbDeviceHidMouse.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(g_UsbDeviceHidMouse.deviceHandle);
#endif
#endif
        USB_DeviceSuspendResumeTask();
    }
}
