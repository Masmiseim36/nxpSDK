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
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "usb_host_hid.h"
#include "board.h"
#include "host_mouse.h"
#include "pin_mux.h"
#include "fsl_common.h"
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#include "usb_phy.h"
#endif /* USB_HOST_CONFIG_EHCI */

#include "fsl_pit.h"
#include "fsl_port.h"
#include "fsl_smc.h"

#if ((!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI))
#error Please enable USB_HOST_CONFIG_KHCI or USB_HOST_CONFIG_EHCI in file usb_host_config.
#endif

#include "fsl_gpio.h"
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
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif /* USB_HOST_CONFIG_KHCI */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif /* USB_HOST_CONFIG_EHCI */

#define USB_HOST_INTERRUPT_PRIORITY (3U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void HW_TIMER_IRQ_HANDLER(void);

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

/*!
 * @brief host freertos task function.
 *
 * @param g_HostHandle   host handle
 */
static void USB_HostTask(void *param);

/*!
 * @brief host mouse freertos task function.
 *
 * @param param   the host mouse instance pointer.
 */
static void USB_HostApplicationTask(void *param);

void BOARD_InitHardware(void);

status_t DbgConsole_Deinit(void);

void BOARD_InitPins(void);
void BOARD_DeinitPins(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief USB host mouse instance global variable */
extern usb_host_mouse_instance_t g_HostHidMouse;
usb_host_handle g_HostHandle;

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

void LLWU_SW_IRQ_HANDLER(void)
{
    if ((1U << LLWU_SW_GPIO_PIN) & PORT_GetPinsInterruptFlags(LLWU_SW_PORT))
    {
        /* Disable interrupt. */
        PORT_SetPinInterruptConfig(LLWU_SW_PORT, LLWU_SW_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(LLWU_SW_PORT, (1U << LLWU_SW_GPIO_PIN));
        g_HostHidMouse.selfWakeup = 1U;
    }
}

#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))

void HW_TIMER_IRQ_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
    g_HostHidMouse.hwTick++;
    USB_HostUpdateHwTick(g_HostHandle, g_HostHidMouse.hwTick);
}

#endif

/*!
 * @brief USB isr function.
 */
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
void USB0_IRQHandler(void)
{
    USB_HostKhciIsrFunction(g_HostHandle);
}
#endif /* USB_HOST_CONFIG_KHCI */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
void USBHS_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_HostHandle);
}
#endif /* USB_HOST_CONFIG_EHCI */

static void USB_HostRemoteWarkupCallback(void *param, usb_host_transfer_t *transfer, usb_status_t status)
{
    if (NULL == param)
    {
        return;
    }
    USB_HostFreeTransfer(param, transfer);

    if (kStatus_USB_Success == status)
    {
        if (kStatus_SuspendWaitClearRemoteWakeup == g_HostHidMouse.suspendResumeState)
        {
            usb_echo("Remote wakeup feature cleared.\r\n");
            g_HostHidMouse.isSetRemoteWakeup = 0U;
            g_HostHidMouse.suspendResumeState = kStatus_Suspending;
        }
        else if (kStatus_SuspendWaitSetRemoteWakeup == g_HostHidMouse.suspendResumeState)
        {
            usb_echo("Remote wakeup feature set.\r\n");
            g_HostHidMouse.isSetRemoteWakeup = 1U;
            g_HostHidMouse.suspendResumeState = kStatus_Suspending;
        }
        else
        {
        }
    }
    else
    {
        g_HostHidMouse.suspendResumeState = kStatus_SuspendFailRemoteWakeup;
        usb_echo(
            "\tSend clear remote wakeup feature request failed. \r\nWhether need to continue? "
            "Please ENTER y(es) or n(o): ");
    }
}

usb_status_t USB_HostControlRemoteWakeup(usb_host_handle hostHandle,
                                         usb_device_handle deviceHandle,
                                         host_inner_transfer_callback_t callbackFn,
                                         void *callbackParam,
                                         uint8_t enable)
{
    usb_host_transfer_t *transfer;
    uint32_t infoValue;

    if (hostHandle == NULL)
    {
        return kStatus_USB_InvalidHandle;
    }

    /* malloc one transfer */
    if (USB_HostMallocTransfer(hostHandle, &transfer) != kStatus_USB_Success)
    {
#ifdef HOST_ECHO
        usb_echo("error to get transfer\r\n");
#endif
        return kStatus_USB_Busy;
    }
    /* initialize transfer */
    transfer->transferBuffer = NULL;
    transfer->transferLength = 0;
    transfer->callbackFn = callbackFn;
    transfer->callbackParam = callbackParam;
    transfer->setupPacket.bmRequestType =
        USB_REQUEST_TYPE_RECIPIENT_DEVICE | USB_REQUEST_TYPE_DIR_OUT | USB_REQUEST_TYPE_TYPE_STANDARD;
    transfer->setupPacket.bRequest = (enable ? USB_REQUEST_STANDARD_SET_FEATURE : USB_REQUEST_STANDARD_CLEAR_FEATURE);
    transfer->setupPacket.wValue = (USB_REQUEST_STANDARD_FEATURE_SELECTOR_DEVICE_REMOTE_WAKEUP);
    transfer->setupPacket.wIndex = USB_SHORT_TO_LITTLE_ENDIAN(0x00U);
    transfer->setupPacket.wLength = USB_SHORT_TO_LITTLE_ENDIAN(0x00U);

    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceControlPipe, &infoValue);

    if (USB_HostSendSetup(hostHandle, (usb_host_pipe_handle)infoValue, transfer) !=
        kStatus_USB_Success) /* call host driver api */
    {
#ifdef HOST_ECHO
        usb_echo("failed for USB_HostControlRemoteWakeup\r\n");
#endif
        USB_HostFreeTransfer(hostHandle, transfer);
        return kStatus_USB_Error;
    }
    return kStatus_USB_Success;
}

static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;

    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;
        case kUSB_HostEventNotSuspended:
            if (kStatus_Idle != g_HostHidMouse.suspendResumeState)
            {
                if (g_HostHidMouse.suspendBus)
                {
                    usb_echo("Suspend BUS failed.\r\n");
                }
                else
                {
                    usb_echo("Suspend device failed.\r\n");
                }
            }
            g_HostHidMouse.suspendResumeState = kStatus_Idle;
            break;
        case kUSB_HostEventSuspended:
            if (kStatus_SuspendRequest == g_HostHidMouse.suspendResumeState)
            {
                g_HostHidMouse.suspendResumeState = kStatus_Suspended;
            }
            else
            {
                g_HostHidMouse.suspendResumeState = kStatus_Idle;
            }
            break;
        case kUSB_HostEventDetectResume:
            if (kStatus_Idle != g_HostHidMouse.suspendResumeState)
            {
                /* Wait for PLL lock. */
                while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
                {
                }
                CLOCK_SetPeeMode();
            }
            break;
        case kUSB_HostEventResumed:
            if (kStatus_Idle != g_HostHidMouse.suspendResumeState)
            {
                if (g_HostHidMouse.suspendBus)
                {
                    usb_echo("BUS has been resumed.\r\n");
                }
                else
                {
                    usb_echo("Device has been resumed.\r\n");
                }
            }
            g_HostHidMouse.suspendResumeState = kStatus_Idle;
            break;
        default:
            break;
    }
    return status;
}

static void USB_HostApplicationInit(void)
{
    usb_status_t status = kStatus_USB_Success;
    IRQn_Type usbIrq;

#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
    IRQn_Type usbFsIrqs[] = USB_IRQS;
    usbIrq = usbFsIrqs[CONTROLLER_ID - kUSB_ControllerKhci0];
    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif /* USB_HOST_CONFIG_KHCI */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
    IRQn_Type usbHsIrqs[] = USBHS_IRQS;
    usbIrq = usbHsIrqs[CONTROLLER_ID - kUSB_ControllerEhci0];
    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif /* USB_HOST_CONFIG_EHCI */
#if ((defined FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

    status = USB_HostInit(CONTROLLER_ID, &g_HostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host init error\r\n");
        return;
    }
    NVIC_SetPriority(usbIrq, USB_HOST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(usbIrq);

    usb_echo("host init done\r\n");
}

#ifdef BOARD_DEBUG_UART_TYPE

usb_status_t getCharFormDebugConsle(uint8_t *c)
{
    if (NULL == c)
    {
        return kStatus_USB_Error;
    }
#if (BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_UART)

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    if (((UART_Type *)BOARD_DEBUG_UART_BASEADDR)->RCFIFO)
#else
    if (((UART_Type *)BOARD_DEBUG_UART_BASEADDR)->S1 & UART_S1_RDRF_MASK)
#endif
    {
        *(c) = ((UART_Type *)BOARD_DEBUG_UART_BASEADDR)->D;
        return kStatus_USB_Success;
    }

#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPUART)

#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    if ((((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT)
#else
    if (((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)->STAT & LPUART_STAT_RDRF_MASK)
#endif
    {
        *(c) = ((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)->DATA;
        return kStatus_USB_Success;
    }

#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_LPSCI)

#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    if ((((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT)
#else
    if (((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)->STAT & LPUART_STAT_RDRF_MASK)
#endif
    {
        *(c) = ((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)->DATA;
        return kStatus_USB_Success;
    }

#elif(BOARD_DEBUG_UART_TYPE == DEBUG_CONSOLE_DEVICE_TYPE_USBCDC)
#error This example can not support debug consle type : USBCDC.
#else
#endif
    return kStatus_USB_Error;
}

#endif

void USB_PowerPreSwitchHook(void)
{
    PIT_StopTimer(PIT, kPIT_Chnl_0);

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
    PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void USB_HostSuspendResumeTask(void)
{
    usb_status_t usb_error;
    uint8_t command;

    if (kStatus_USB_Success != getCharFormDebugConsle(&command))
    {
        command = 0;
    }
    switch (g_HostHidMouse.suspendResumeState)
    {
        case kStatus_Idle:
            if ('s' == command)
            {
                g_HostHidMouse.suspendResumeState = kStatus_SartSuspend;
                usb_echo("Start suspend USB BUS...\r\n");
            }
            else
            {
                if (command)
                {
                    usb_echo("Please Enter 's' to start suspend test\r\n");
                }
            }
            break;
        case kStatus_SartSuspend:
            g_HostHidMouse.suspendBus = 1;
            if (g_HostHidMouse.supportRemoteWakeup)
            {
                usb_echo(
                    "\r\nPlease Enter: \r\n\t1. Enable remote wakeup feature.\r\n\t2. Disable remote wakeup "
                    "feature.\r\n");
                g_HostHidMouse.suspendResumeState = kStatus_SuspendSetRemoteWakeup;
            }
            else
            {
                g_HostHidMouse.suspendResumeState = kStatus_Suspending;
            }
            break;
        case kStatus_SuspendSetRemoteWakeup:
            if ('1' == command)
            {
                usb_echo("1");
                usb_error = USB_HostControlRemoteWakeup(g_HostHandle, g_HostHidMouse.deviceHandle,
                                                        USB_HostRemoteWarkupCallback, g_HostHandle, 1);
                if (kStatus_USB_Success == usb_error)
                {
                    g_HostHidMouse.suspendResumeState = kStatus_SuspendWaitSetRemoteWakeup;
                }
                else
                {
                    g_HostHidMouse.suspendResumeState = kStatus_SuspendFailRemoteWakeup;
                    usb_echo(
                        "\tSend set remote wakeup feature request failed. \r\nWhether need to continue? "
                        "Please ENTER y(es) or n(o): ");
                }
            }
            else if ('2' == command)
            {
                usb_echo("2");
                usb_error = USB_HostControlRemoteWakeup(g_HostHandle, g_HostHidMouse.deviceHandle,
                                                        USB_HostRemoteWarkupCallback, g_HostHandle, 0);
                if (kStatus_USB_Success == usb_error)
                {
                    g_HostHidMouse.suspendResumeState = kStatus_SuspendWaitClearRemoteWakeup;
                }
                else
                {
                    g_HostHidMouse.suspendResumeState = kStatus_SuspendFailRemoteWakeup;
                    usb_echo(
                        "\tSend clear remote wakeup feature request failed. \r\nWhether need to continue? "
                        "Please ENTER y(es) or n(o): ");
                }
            }
            else
            {
            }
            break;
        case kStatus_SuspendWaitSetRemoteWakeup:
        case kStatus_SuspendWaitClearRemoteWakeup:
            break;
        case kStatus_SuspendFailRemoteWakeup:
            if ('y' == command)
            {
                usb_echo("y");
                g_HostHidMouse.suspendResumeState = kStatus_Suspending;
            }
            else if ('n' == command)
            {
                usb_echo("n");
                g_HostHidMouse.suspendResumeState = kStatus_Idle;
            }
            else
            {
            }
            break;
        case kStatus_Suspending:
            if (kStatus_USB_Success ==
                USB_HostSuspendDeviceResquest(g_HostHandle,
                                              g_HostHidMouse.suspendBus ? NULL : g_HostHidMouse.deviceHandle))
            {
                g_HostHidMouse.suspendResumeState = kStatus_SuspendRequest;
            }
            else
            {
                usb_echo("Send suspend request failed.\r\n");
                g_HostHidMouse.suspendResumeState = kStatus_Idle;
            }
            break;
        case kStatus_SuspendRequest:
            break;
        case kStatus_Suspended:
            if (g_HostHidMouse.suspendBus)
            {
                usb_echo("BUS has been suspended.\r\n");
            }
            else
            {
                usb_echo("Device has been suspended.\r\n");
            }
            usb_echo("Please Press wakeup switch(%s) to start resume test.\r\n", LLWU_SW_NAME);
            if (g_HostHidMouse.isSetRemoteWakeup)
            {
                usb_echo("Or, wait for device sends resume signal.\r\n");
            }
            USB_PowerPreSwitchHook();
            PORT_SetPinInterruptConfig(LLWU_SW_PORT, LLWU_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
            SIM->SOPT1 |= SIM_SOPT1_USBSSTBY_MASK;
            if (kStatus_Success != SMC_SetPowerModeVlps(SMC))
            {
                g_HostHidMouse.selfWakeup = 1U;
                USB_PowerPostSwitchHook();
                usb_echo("Enter VLPS mode failed!\r\n");
            }
            else
            {
                USB_PowerPostSwitchHook();
            }

            if (g_HostHidMouse.isSetRemoteWakeup)
            {
            }
            g_HostHidMouse.suspendResumeState = kStatus_WaitResume;
            break;
        case kStatus_WaitResume:
            if (g_HostHidMouse.selfWakeup)
            {
                g_HostHidMouse.selfWakeup = 0U;
                usb_echo("Start resume the device.\r\n");
                if (kStatus_USB_Success ==
                    USB_HostResumeDeviceResquest(g_HostHandle,
                                                 g_HostHidMouse.suspendBus ? NULL : g_HostHidMouse.deviceHandle))
                {
                    g_HostHidMouse.suspendResumeState = kStatus_ResumeRequest;
                }
                else
                {
                    usb_echo("Send resume signal failed.\r\n");
                }
            }
            break;
        case kStatus_ResumeRequest:
            break;
        default:
            break;
    }
    command = 0;
}

static void USB_HostTask(void *param)
{
    while (1)
    {
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
        USB_HostKhciTaskFunction(param);
#endif /* USB_HOST_CONFIG_KHCI */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
        USB_HostEhciTaskFunction(param);
#endif /* USB_HOST_CONFIG_EHCI */
    }
}

static void USB_HostSuspendResume(void *param)
{
    while (1)
    {
        USB_HostSuspendResumeTask();
        vTaskDelay(1);
    }
}

static void USB_HostApplicationTask(void *param)
{
#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
    /* Structure of initialize PIT */
    pit_config_t pitConfig;
#endif

    /* Set to allow entering vlps mode */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeVlp);

    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
    RTC->CR |= RTC_CR_OSCE_MASK;
#if (defined(FSL_FEATURE_SIM_OPT_HAS_USB_PHY) && (FSL_FEATURE_SIM_OPT_HAS_USB_PHY > 0))
    SIM->SOPT2 |= SIM_SOPT2_USBSLSRC_MASK;
#endif

#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
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

    USB_HostApplicationInit();

#if ((defined(USB_HOST_CONFIG_LOW_POWER_MODE)) && (USB_HOST_CONFIG_LOW_POWER_MODE > 0U))
    /* Enable at the NVIC */
    EnableIRQ(PIT0_IRQn);

    /* Start counting */
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    NVIC_SetPriority(LLWU_SW_IRQ, 1U);
    NVIC_EnableIRQ(LLWU_SW_IRQ);

    usb_echo("Please Enter 's' to start suspend test\r\n");
#endif

    if (xTaskCreate(USB_HostTask, "usb host task", 2000L / sizeof(portSTACK_TYPE), g_HostHandle, 4, NULL) != pdPASS)
    {
        usb_echo("usb host task create failed!\r\n");
        return;
    }

    if (xTaskCreate(USB_HostSuspendResume, "host suspend resume task", 2000L / sizeof(portSTACK_TYPE), param, 4,
                    NULL) != pdPASS)
    {
        usb_echo("usb host suspend/resume task create failed!\r\n");
        return;
    }

    while (1)
    {
        USB_HostHidMouseTask(param);
    }
}

int main(void)
{
    gpio_pin_config_t pinConfig = {kGPIO_DigitalInput, 1U};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetXtal32Freq(BOARD_XTAL32K_CLK_HZ);
    BOARD_InitDebugConsole();

    /* Set the LLWU pin */
    GPIO_PinInit(LLWU_SW_GPIO, LLWU_SW_GPIO_PIN, &pinConfig);

    /* enable usb host vbus */
    pinConfig.pinDirection = kGPIO_DigitalOutput;
    pinConfig.outputLogic = 1U;

    GPIO_PinInit(PTD, 8U, &pinConfig);

    if (xTaskCreate(USB_HostApplicationTask, "app task", 2000L / sizeof(portSTACK_TYPE), &g_HostHidMouse, 3, NULL) !=
        pdPASS)
    {
        usb_echo("create mouse task error\r\n");
    }

    vTaskStartScheduler();

    while (1)
    {
        ;
    }
}
