/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "usb_timer.h"
#include "fsl_uart.h"
#include "lpm.h"
#include "fsl_gpc.h"
#include "fsl_iomuxc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_WAKEUP_BUTTON_GPIO_PIN 11U
#define BOARD_WAKEUP_BUTTON_GPIO GPIO3
#define BOARD_WAKEUP_BUTTON_NAME "SW601-3"
#define APP_WAKEUP_BUTTON_IRQ GPIO3_Combined_0_15_IRQn

#define EXAMPLE_EPIT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (CLOCK_GetDiv(kCLOCK_PerclkDiv) + 1U))

/* USB clock source and frequency*/
#define USB_HS_PHY_CLK_SRC kCLOCK_Usbphy480M
#define USB_HS_PHY_CLK_FREQ 480000000U
#define USB_HS_CLK_SRC kCLOCK_Usb480M
#define USB_HS_CLK_FREQ 480000000U
#define BOARD_XTAL0_CLK_HZ 0

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void USB_WaitClockLocked(void);
void EPIT1_IRQHandler(void);
void USBHS_IRQHandler(void);
void BOARD_InitHardware(void);
static usb_status_t USB_DeviceHidMouseAction(void);
static usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                             usb_device_endpoint_callback_message_struct_t *event,
                                             void *arg);
static void USB_DeviceApplicationInit(void);

void BOARD_InitPins(void);
void BOARD_DeinitPins(void);
void SW_IntControl(uint8_t enable);
char *SW_GetName(void);
void HW_TimerControl(uint8_t enable);
void USB_LowpowerModeInit(void);
void USB_PreLowpowerMode(void);
uint8_t USB_EnterLowpowerMode(void);
void USB_PostLowpowerMode(void);
void USB_ControllerSuspended(void);
void USB_WaitClockLocked(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

USB_DATA_ALIGNMENT static uint8_t s_MouseBuffer[USB_HID_MOUSE_REPORT_LENGTH];
usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

extern uint8_t g_UsbDeviceCurrentConfigure;
extern uint8_t g_UsbDeviceInterface[USB_HID_MOUSE_INTERFACE_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/
void WAKEUP_SW_IRQ_HANDLER(uint32_t giccIar, void *param)
{
    if ((1U << BOARD_WAKEUP_BUTTON_GPIO_PIN) & GPIO_GetPinsInterruptFlags(BOARD_WAKEUP_BUTTON_GPIO))
    {
        /* Disable interrupt. */
        GPIO_DisableInterrupts(BOARD_WAKEUP_BUTTON_GPIO, 1U << BOARD_WAKEUP_BUTTON_GPIO_PIN);
        GPIO_ClearPinsInterruptFlags(BOARD_WAKEUP_BUTTON_GPIO, 1U << BOARD_WAKEUP_BUTTON_GPIO_PIN);
        LPM_DisableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
        g_UsbDeviceHidMouse.selfWakeup = 1U;
    }
}


/*!
 * @brief De-initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_DeinitPins(void)
{
    /*
    * Set pin for current leakage.
    * Debug console RX pin: Set to pinmux to GPIO input.
    * Debug console TX pin: Don't need to change.
    */
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_GPIO1_IO17, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_GPIO1_IO17,
                        IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(2) | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
}

void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        g_UsbDeviceHidMouse.selfWakeup = 0U;
        GPIO_ClearPinsInterruptFlags(BOARD_WAKEUP_BUTTON_GPIO, 1U << BOARD_WAKEUP_BUTTON_GPIO_PIN);
        /* Enable GPIO pin interrupt */
        GPIO_EnableInterrupts(BOARD_WAKEUP_BUTTON_GPIO, 1U << BOARD_WAKEUP_BUTTON_GPIO_PIN);
        /* Enable GPC interrupt */
        LPM_EnableWakeupSource(APP_WAKEUP_BUTTON_IRQ);
    }
    else
    {
        GPIO_DisableInterrupts(BOARD_WAKEUP_BUTTON_GPIO, 1U << BOARD_WAKEUP_BUTTON_GPIO_PIN);
    }
}

void SW_Init(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t swConfig = {
        kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge,
    };

    /* Init input switch GPIO. */
    GPIO_PinInit(BOARD_WAKEUP_BUTTON_GPIO, BOARD_WAKEUP_BUTTON_GPIO_PIN, &swConfig);
}

char *SW_GetName(void)
{
    return BOARD_WAKEUP_BUTTON_NAME;
}

void HW_TimerCallback(void)
{
    g_UsbDeviceHidMouse.hwTick++;
    USB_DeviceUpdateHwTick(g_UsbDeviceHidMouse.deviceHandle, g_UsbDeviceHidMouse.hwTick);
}

void HW_TimerInit(void)
{
    USB_TimerInit(1, 1000U, EXAMPLE_EPIT_CLK_FREQ, HW_TimerCallback);
}

void HW_TimerControl(uint8_t enable)
{
    USB_TimerInt(1, enable);
}

void USB_LowpowerModeInit(void)
{
#if (defined(FSL_FEATURE_SIM_OPT_HAS_USB_PHY) && (FSL_FEATURE_SIM_OPT_HAS_USB_PHY > 0))
    SIM->SOPT2 |= SIM_SOPT2_USBSLSRC_MASK;
#endif
    SW_Init();
    HW_TimerInit();
}

void USB_PreLowpowerMode(void)
{
}

/*
 * Enter the LowPower mode.
 */
void APP_LowPower_EnterLowPower(void)
{
}

void APP_PowerPreSwitchHook()
{
    DbgConsole_Deinit();

    /*
     * Set pin for current leakage.
     * Debug console RX pin: Set to pinmux to disable.
     * Debug console TX pin: Don't need to change.
     */
    // PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
}

void USB_PostLowpowerMode(void)
{
    USB_WaitClockLocked();
}

void USB_ControllerSuspended(void)
{
}

void USB_WaitClockLocked(void)
{
}

uint8_t USB_EnterLowpowerMode(void)
{
    /* flush debug console */
    DbgConsole_Flush();
    LPM_SetPowerMode(LPM_PowerModeDsm);
    APP_PowerPreSwitchHook();
    LPM_EnableWakeupSource(USB_OTG1_IRQn);
    LPM_WaitForInterrupt(4294952554);
    return 0;
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
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
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
                usb_echo("USB device start suspend\r\n");
                USB_ControllerSuspended();
                g_UsbDeviceHidMouse.startTick = g_UsbDeviceHidMouse.hwTick;
                g_UsbDeviceHidMouse.suspend = kStatus_MouseStartSuspend;
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
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidMouse.deviceHandle);
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

    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
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
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);

    /* Start USB device HID mouse */
    USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
}

void USB_PowerPreSwitchHook(void)
{
    HW_TimerControl(0U);

    DbgConsole_Deinit();

    BOARD_DeinitPins();

    USB_PreLowpowerMode();
}

void USB_PowerPostSwitchHook(void)
{
    USB_WaitClockLocked();
    USB_PostLowpowerMode();
    BOARD_InitPins();
    BOARD_InitDebugConsole();
    HW_TimerControl(1U);
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
                usb_echo("Please Press wakeup switch(%s) to remote wakeup the host.\r\n", SW_GetName());
            }
            g_UsbDeviceHidMouse.suspend = kStatus_MouseSuspended;
            break;
        case kStatus_MouseSuspended:
            USB_PowerPreSwitchHook();
            if (g_UsbDeviceHidMouse.remoteWakeup)
            {
                SW_IntControl(1);
            }

            USB_DeviceSetStatus(g_UsbDeviceHidMouse.deviceHandle, kUSB_DeviceStatusBusSuspend, NULL);
            if (kStatus_Success != USB_EnterLowpowerMode())
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
                SW_IntControl(0);
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
            if (g_UsbDeviceHidMouse.attach)
            {
                USB_DeviceHidMouseAction();
            }
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    /*  BOARD_InitMemory();  */ /*USB code doesn't fully support cached in this release */
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(USB_OTG1_IRQn, (system_irq_handler_t)(uint32_t)USBHS_IRQHandler, NULL);
    SystemInstallIrqHandler(USB_OTG2_IRQn, (system_irq_handler_t)(uint32_t)USBHS_IRQHandler, NULL);
    SystemInstallIrqHandler(EPIT1_IRQn, (system_irq_handler_t)(uint32_t)EPIT1_IRQHandler, NULL);

    SystemInstallIrqHandler(APP_WAKEUP_BUTTON_IRQ, WAKEUP_SW_IRQ_HANDLER, NULL);
    EnableIRQ(APP_WAKEUP_BUTTON_IRQ);

    LPM_Init();

#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
    USB_LowpowerModeInit();
#endif

    USB_DeviceApplicationInit();

#if ((defined(USB_DEVICE_CONFIG_LOW_POWER_MODE)) && (USB_DEVICE_CONFIG_LOW_POWER_MODE > 0U))
    HW_TimerControl(1);
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
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_UsbDeviceHidMouse.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_UsbDeviceHidMouse.deviceHandle);
#endif
#endif
        USB_DeviceSuspendResumeTask();
    }
}
