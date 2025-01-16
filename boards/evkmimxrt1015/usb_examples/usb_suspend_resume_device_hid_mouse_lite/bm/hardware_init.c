/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/
/*${header:start}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "fsl_device_registers.h"
#include "mouse.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "mouse.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"
#include "fsl_gpc.h"

#include "fsl_adapter_timer.h"
/*${header:end}*/
/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
#if ((defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE)) && (FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE > 0U))
static uint32_t g_savedPrimask;
#endif
/*${variable:end}*/
/*${prototype:start}*/
void USB_WaitClockLocked(void);
/*${prototype:end}*/
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

    /* Define the initialization structure for the input switch pin. */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
        kGPIO_IntRisingEdge,
    };
    /* Initialize input switch GPIO. */
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &sw_config);

    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(BOARD_USER_BUTTON_GPIO, 1U << BOARD_USER_BUTTON_GPIO_PIN);
    EnableIRQ(BOARD_USER_BUTTON_IRQ);

    GPC_EnableIRQ(GPC, BOARD_USER_BUTTON_IRQ);
}

/*!
 * @brief De-initialize all pins used in this example
 *
 * @param disablePortClockAfterInit disable port clock after pin
 * initialization or not.
 */
void BOARD_DeinitPins(void)
{
}
void BOARD_USER_BUTTON_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_USER_BUTTON_GPIO, 1U << BOARD_USER_BUTTON_GPIO_PIN);
    g_UsbDeviceHidMouse.selfWakeup = 1U;
}
void SW_IntControl(uint8_t enable)
{
}
void SW_Callback(void *param)
{
    g_UsbDeviceHidMouse.selfWakeup = 1U;
    SW_IntControl(0);
}
void SW_Init(void)
{
    NVIC_SetPriority(BOARD_USER_BUTTON_IRQ, 1U);
    NVIC_EnableIRQ(BOARD_USER_BUTTON_IRQ);
}
char *SW_GetName(void)
{
    return BOARD_USER_BUTTON_NAME;
}
void HW_TimerCallback(void *param)
{
    g_UsbDeviceHidMouse.hwTick++;
    USB_DeviceUpdateHwTick(g_UsbDeviceHidMouse.deviceHandle, g_UsbDeviceHidMouse.hwTick);
}
void HW_TimerInit(void)
{
    hal_timer_config_t halTimerConfig;
    halTimerConfig.timeout            = 1000;
    halTimerConfig.srcClock_Hz        = TIMER_SOURCE_CLOCK;
    halTimerConfig.instance           = 0U;
    hal_timer_handle_t halTimerHandle = &g_halTimerHandle[0];
    HAL_TimerInit(halTimerHandle, &halTimerConfig);
    HAL_TimerInstallCallback(halTimerHandle, HW_TimerCallback, NULL);
}
void HW_TimerControl(uint8_t enable)
{
    if (enable)
    {
        HAL_TimerEnable(g_halTimerHandle);
    }
    else
    {
        HAL_TimerDisable(g_halTimerHandle);
    }
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
 * Execute the instrument to enter low power.
 */
#if ((defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE)) && (FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE > 0U))
AT_QUICKACCESS_SECTION_CODE(void stop(void));
#endif
void stop(void)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
#if ((defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE)) && (FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE > 0U))
    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();
#endif
    __asm("WFI");
#if ((defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE)) && (FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE > 0U))
    CCM_ANALOG->PFD_480 |= CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;
    CCM_ANALOG->PFD_480 &= ~CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;
    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
#endif
}
/*
 * Enter the LowPower mode.
 */
void APP_LowPower_EnterLowPower(void)
{
    CLOCK_SetMode(kCLOCK_ModeStop);
    stop();
}
uint8_t USB_EnterLowpowerMode(void)
{
    APP_LowPower_EnterLowPower();
    return 0;
}
void USB_WaitClockLocked(void)
{
}
void USB_PostLowpowerMode(void)
{
    USB_WaitClockLocked();
}
void USB_ControllerSuspended(void)
{
}

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

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
/*${function:end}*/
