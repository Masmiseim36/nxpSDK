/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${standard_header:start}*/
#include <stdbool.h>
/*${standard_header:end}*/
/*${header:start}*/
#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "usb_host_hid.h"
#include "host_mouse.h"

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "app.h"

#include "pmic_support.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pca9420.h"
#include "usb_phy.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "fsl_power.h"
#include "fsl_adapter_timer.h"

/*${header:end}*/

/*${macro:start}*/
#define APP_USER_WAKEUP_KEY_GPIO         GPIO
#define APP_USER_WAKEUP_KEY_PORT         0
#define APP_USER_WAKEUP_KEY_PIN          10
#define APP_USER_WAKEUP_KEY_INPUTMUX_SEL kINPUTMUX_GpioPort0Pin10ToPintsel
#define APP_DEEPSLEEP_RUNCFG0 \
    (SYSCTL0_PDSLEEPCFG0_LPOSC_PD_MASK | SYSCTL0_PDSLEEPCFG0_SYSXTAL_PD_MASK | SYSCTL0_PDSLEEPCFG0_RBB_PD_MASK)
#define APP_DEEPSLEEP_RUNCFG1                                                                  \
    (SYSCTL0_PDSLEEPCFG1_FLEXSPI0_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI1_SRAM_APD_MASK | \
     SYSCTL0_PDSLEEPCFG1_FLEXSPI0_SRAM_PPD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI1_SRAM_PPD_MASK | \
     SYSCTL0_PDSLEEPCFG1_USBHS_SRAM_PPD_MASK | SYSCTL0_PDSLEEPCFG1_USBHS_SRAM_APD_MASK)
#define APP_DEEPSLEEP_RAM_APD 0xFFC00000U /* 0x280000 - 0x4FFFFF keep powered */
#define APP_DEEPSLEEP_RAM_PPD 0xFFC00000U
#define APP_EXCLUDE_FROM_DEEPSLEEP \
    (((const uint32_t[]){APP_DEEPSLEEP_RUNCFG0, APP_DEEPSLEEP_RUNCFG1, APP_DEEPSLEEP_RAM_APD, APP_DEEPSLEEP_RAM_PPD}))
/*${macro:end}*/

/*${variable:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
extern usb_host_mouse_instance_t g_HostHidMouse;
extern usb_host_handle g_HostHandle;
static uint32_t systemTickControl;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
/*${variable:end}*/

/*${function:start}*/
static void BOARD_ConfigPMICModes(pca9420_modecfg_t *cfg, uint32_t num)
{
    assert(cfg);
    /* Configuration PMIC mode to align with power lib like below:
     *  0b00    run mode, no special.
     *  0b01    deep sleep mode, no special.
     *  0b10    deep powerdown mode, vddcore off.
     *  0b11    full deep powerdown mode vdd1v8 and vddcore off. */

    cfg[1].sw1OutVolt = kPCA9420_Sw1OutVolt0V600;
    /* Mode 2: VDDCORE off. */
    cfg[2].enableSw1Out = false;

    /* Mode 3: VDDCORE, VDD1V8 and VDDIO off. */
    cfg[3].enableSw1Out  = false;
    cfg[3].enableSw2Out  = false;
    cfg[3].enableLdo2Out = false;
}

/*!
 * @brief Call back for PINT Pin interrupt 0-7.
 */
void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    g_HostHidMouse.selfWakeup = 1U;
    /* Disable callbacks for PINT */
    PINT_DisableCallback(PINT);
}

/*
 * Setup a GPIO input pin as wakeup source.
 */
void APP_InitWakeupPin(void)
{
    gpio_pin_config_t gpioPinConfigStruct;
    /* Set SW pin as GPIO input. */
    gpioPinConfigStruct.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(APP_USER_WAKEUP_KEY_GPIO, APP_USER_WAKEUP_KEY_PORT, APP_USER_WAKEUP_KEY_PIN, &gpioPinConfigStruct);

    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, APP_USER_WAKEUP_KEY_INPUTMUX_SEL);

    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    g_HostHidMouse.selfWakeup = 0U;

    /* Initialize PINT */
    PINT_Init(PINT);

    /* Setup Pin Interrupt 1 for falling edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge, pint_intr_callback);

    /* Enable callbacks for PINT */
    PINT_EnableCallback(PINT);

    EnableDeepSleepIRQ(PIN_INT0_IRQn);
}

void BOARD_InitHardware(void)
{
    pca9420_modecfg_t pca9420ModeCfg[4];
    uint32_t i;

    /* BE CAUTIOUS TO SET CORRECT VOLTAGE RANGE ACCORDING TO YOUR BOARD/APPLICATION. PAD SUPPLY BEYOND THE RANGE DO
       HARM TO THE SILICON. */
    power_pad_vrange_t vrange = {.Vdde0Range = kPadVol_171_198,
                                 .Vdde1Range = kPadVol_171_198,
                                 /* SD0 voltage is switchable, but in power_manager demo, it's fixed 3.3V. */
                                 .Vdde2Range = kPadVol_300_360,
                                 .Vdde3Range = kPadVol_300_360,
                                 .Vdde4Range = kPadVol_171_198};

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* PMIC PCA9420 */
    BOARD_InitPmic();
    for (i = 0; i < ARRAY_SIZE(pca9420ModeCfg); i++)
    {
        PCA9420_GetDefaultModeConfig(&pca9420ModeCfg[i]);
    }
    BOARD_ConfigPMICModes(pca9420ModeCfg, ARRAY_SIZE(pca9420ModeCfg));
    PCA9420_WriteModeConfigs(&pca9420Handle, kPCA9420_Mode0, &pca9420ModeCfg[0], ARRAY_SIZE(pca9420ModeCfg));

    /* Configure PMIC Vddcore value according to main clock. DSP not used in this demo. */
    BOARD_SetPmicVoltageForFreq(CLOCK_GetFreq(kCLOCK_CoreSysClk), 0U);

    /* Indicate to power library that PMIC is used. */
    POWER_UpdatePmicRecoveryTime(1);

    POWER_SetPadVolRange(&vrange);

    APP_InitWakeupPin();
}
void BOARD_DeinitPins(void)
{
}
void SW_IntControl(uint8_t enable)
{
    if (enable)
    {
        g_HostHidMouse.selfWakeup = 0U;
        PINT_EnableCallback(PINT);
    }
    else
    {
        PINT_DisableCallback(PINT);
    }
}

char *SW_GetName(void)
{
    return "SW2";
}

void HW_TimerCallback(void *param)
{
    g_HostHidMouse.hwTick++;
    USB_HostUpdateHwTick(g_HostHandle, g_HostHidMouse.hwTick);
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
    HW_TimerInit();
}
void USB_PreLowpowerMode(void)
{
    if (SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)
    {
        systemTickControl = SysTick->CTRL;
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
    }
    __disable_irq();
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
    EnableIRQ(USB0_NEEDCLK_IRQn);
    SYSCTL0->STARTEN1 |= SYSCTL0_STARTEN1_USB0_NEEDCLK(1);
}

uint8_t USB_EnterLowpowerMode(void)
{
    /* Enter Deep Sleep mode */
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
    return kStatus_Success;
}

void USB_PostLowpowerMode(void)
{
    __enable_irq();
    SysTick->CTRL = systemTickControl;
    SYSCTL0->STARTEN1 &= ~SYSCTL0_STARTEN1_USB0_NEEDCLK(1);
    USB_LowpowerModeInit();
    DisableIRQ(USB0_NEEDCLK_IRQn);
}

void USB_ControllerSuspended(void)
{
    while (SYSCTL0->USB0CLKSTAT & (SYSCTL0_USB0CLKSTAT_HOST_NEED_CLKST_MASK))
    {
        __ASM("nop");
    }
    SYSCTL0->USB0CLKCTRL |= SYSCTL0_USB0CLKCTRL_POL_FS_HOST_CLK_MASK;
    SYSCTL0->USB0CLKCTRL &= ~SYSCTL0_USB0CLKCTRL_HS_DEV_WAKEUP_N_MASK;
}

void USB0_NEEDCLK_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(USB0_NEEDCLK_IRQn);
}

void USB_WaitClockLocked(void)
{
}

void USB0_IRQHandler(void)
{
    USB_HostIp3516HsIsrFunction(g_HostHandle);
}

void USB_HostClockInit(void)
{
    uint8_t usbClockDiv = 1;
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Make sure USDHC ram buffer and usb1 phy has power up */
    POWER_DisablePD(kPDRUNCFG_APD_USBHS_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USBHS_SRAM);
    POWER_ApplyPD();

    RESET_PeripheralReset(kUSBHS_PHY_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_DEVICE_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_HOST_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBHS_SRAM_RST_SHIFT_RSTn);

    /* enable usb ip clock */
    CLOCK_EnableUsbHs0HostClock(kOSC_CLK_to_USB_CLK, usbClockDiv);
    /* save usb ip clock freq*/
    usbClockFreq = g_xtalFreq / usbClockDiv;
    CLOCK_SetClkDiv(kCLOCK_DivPfc1Clk, 4);
    /* enable usb ram clock */
    CLOCK_EnableClock(kCLOCK_UsbhsSram);
    /* enable USB PHY PLL clock, the phy bus clock (480MHz) source is same with USB IP */
    CLOCK_EnableUsbHs0PhyPllClock(kOSC_CLK_to_USB_CLK, usbClockFreq);

    /* USB PHY initialization */
    USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);

#if ((defined FSL_FEATURE_USBHSH_USB_RAM) && (FSL_FEATURE_USBHSH_USB_RAM > 0U))
    for (int i = 0; i < (FSL_FEATURE_USBHSH_USB_RAM >> 2); i++)
    {
        ((uint32_t *)FSL_FEATURE_USBHSH_USB_RAM_BASE_ADDRESS)[i] = 0U;
    }
#endif
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHSH_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerIp3516Hs0];
    /* USB_HOST_CONFIG_EHCI */

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);

    EnableIRQ((IRQn_Type)irqNumber);
}

void USB_HostTaskFn(void *param)
{
#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
    USB_HostIp3516HsTaskFunction(param);
#endif /* USB_HOST_CONFIG_IP3516HS */
}
/*${function:end}*/
