/*
 * Copyright 2024 NXP
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
#include "usb_device_class.h"
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
#include "fsl_adapter_timer.h"
/*${header:end}*/
/*${variable:start}*/
extern const clock_main_pll_config_t g_mainPllConfig_BOARD_BootClockRUN;
extern const clock_audio_pll_config_t g_audioPllConfig_BOARD_BootClockRUN;
uint32_t g_halTimerHandle[(HAL_TIMER_HANDLE_SIZE + 3) / 4];
/*${variable:end}*/
/*${macro:start}*/
#define TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define APP_DEEPSLEEP_SLEEPCFG    (SLEEPCON0_SLEEPCFG_LPOSC_PD_MASK | SLEEPCON0_SLEEPCFG_COMP_MAINCLK_SHUTOFF_MASK | SLEEPCON0_SLEEPCFG_FRO1_PD_MASK | SLEEPCON0_SLEEPCFG_FRO0_PD_MASK | SLEEPCON0_SLEEPCFG_FRO0_GATE_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG0 (PMC_PDSLEEPCFG0_VNCOM_DSR_MASK |  PMC_PDSLEEPCFG0_V2NMED_DSR_MASK)
#define APP_DEEPSLEEP_RAM_APD     (0x3F00U) /* PMC->PDSLEEPCFG2, PT8-PT13 powered, the unused PT can be power off. */
#define APP_DEEPSLEEP_RAM_PPD     (0U)
#define APP_DEEPSLEEP_PDSLEEPCFG4 \
    (PMC_PDSLEEPCFG4_CPU0_CCACHE_MASK | PMC_PDSLEEPCFG4_CPU0_SCACHE_MASK | PMC_PDSLEEPCFG4_OCOTP_MASK | PMC_PDSLEEPCFG4_USB0_SRAM_MASK | PMC_PDSLEEPCFG4_USB1_SRAM_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG5 (PMC_PDSLEEPCFG5_USB0_SRAM_MASK | PMC_PDSLEEPCFG5_USB1_SRAM_MASK)

#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, APP_DEEPSLEEP_PDSLEEPCFG4, APP_DEEPSLEEP_PDSLEEPCFG5}))
/*${macro:end}*/
/*${prototype:start}*/
void USB_WaitClockLocked(void);
/*${prototype:end}*/
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();

    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    
    /* Init input switch GPIO. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) || \
    (!defined(FSL_FEATURE_SOC_PORT_COUNT))
    GPIO_SetPinInterruptConfig(BOARD_SW5_GPIO, BOARD_SW5_GPIO_PIN, kGPIO_InterruptFallingEdge);
#else
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW5_GPIO_PIN, kPORT_InterruptFallingEdge);
#endif
     EnableDeepSleepIRQ(GPIO00_IRQn);
     GPIO_PinInit(BOARD_SW5_GPIO, BOARD_SW5_GPIO_PIN, &sw_config);
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

void GPIO00_IRQHandler(void)
{
    POWER_ModuleExitLPRequest(kPower_GPIO0_LPREQ); /* Clear the low power request before accessing the GPIO. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) || \
    (!defined(FSL_FEATURE_SOC_PORT_COUNT))
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW5_GPIO, 1U << BOARD_SW5_GPIO_PIN);
#else
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW5_GPIO, 1U << BOARD_SW5_GPIO_PIN);
#endif
    g_UsbDeviceHidMouse.selfWakeup = 1U;
    SDK_ISR_EXIT_BARRIER;
}

void SW_IntControl(uint8_t enable)
{
}

void SW_Init(void)
{
    NVIC_SetPriority(GPIO00_IRQn, 1U);
    NVIC_EnableIRQ(GPIO00_IRQn);
}

char *SW_GetName(void)
{
    return "SW5";
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
    SW_Init();
    HW_TimerInit();
}

void USB_PreLowpowerMode(void)
{
    /* Low power handshake to for async interrupt. */
    if (POWER_ModuleEnterLPRequest(kPower_GPIO0_LPREQ) != kStatus_Success)
    {
        assert(false);
    }
    EnableDeepSleepIRQ(USB0_IRQn);
}

#if 0
void BOARD_DisablePll(void)
{
    /* Special sequence is needed for the PLL power up/initialization. The application should manually handle the states
   changes for the PLL if the PLL power states configuration are different in Active mode and Deep Sleep mode. */

    /* Disable the PFD clock output first. */
    CLOCK_DeinitMainPfd(kCLOCK_Pfd0);
    CLOCK_DeinitMainPfd(kCLOCK_Pfd1);
    CLOCK_DeinitMainPfd(kCLOCK_Pfd2);
    CLOCK_DeinitMainPfd(kCLOCK_Pfd3);

    CLOCK_DeinitAudioPfd(kCLOCK_Pfd1);
    CLOCK_DeinitAudioPfd(kCLOCK_Pfd3);
    /* Disable PLL. */
    CLOCK_DeinitMainPll();
    CLOCK_DeinitAudioPll();
}

void BOARD_RestorePll(void)
{
    /*Restore PLL*/
    CLOCK_InitMainPll(&g_mainPllConfig_BOARD_BootClockRUN);
    CLOCK_InitAudioPll(&g_audioPllConfig_BOARD_BootClockRUN);
    /*Restore PFD*/
    CLOCK_InitMainPfd(kCLOCK_Pfd0, 20U);
    CLOCK_InitMainPfd(kCLOCK_Pfd1, 24U);
    CLOCK_InitMainPfd(kCLOCK_Pfd2, 18U);
    CLOCK_InitMainPfd(kCLOCK_Pfd3, 19U);

    CLOCK_InitAudioPfd(kCLOCK_Pfd1, 24U);
    CLOCK_InitAudioPfd(kCLOCK_Pfd3, 26U);
}

/* Change main clock to a safe source. */
void BOARD_ClockSafeConfig(void)
{
    /* Switch to FRO1 for safe configure. */
    CLOCK_AttachClk(kFRO1_DIV1_to_COMPUTE_BASE);
    CLOCK_AttachClk(kCOMPUTE_BASE_to_COMPUTE_MAIN);
    CLOCK_SetClkDiv(kCLOCK_DivCmptMainClk, 1U);
    CLOCK_SetClkDiv(kCLOCK_DivComputeRamClk, 1U);
    CLOCK_AttachClk(kFRO1_DIV1_to_RAM);
    CLOCK_AttachClk(kFRO1_DIV1_to_COMMON_BASE);
    CLOCK_AttachClk(kCOMMON_BASE_to_COMMON_VDDN);
    CLOCK_SetClkDiv(kCLOCK_DivCommonVddnClk, 1U);

    CLOCK_AttachClk(kFRO1_DIV3_to_MEDIA_VDD2_BASE);
    CLOCK_AttachClk(kFRO1_DIV1_to_MEDIA_VDDN_BASE);
    CLOCK_AttachClk(kMEDIA_VDD2_BASE_to_MEDIA_MAIN);
    CLOCK_AttachClk(kMEDIA_VDDN_BASE_to_MEDIA_VDDN);

    BOARD_XspiClockSafeConfig(); /*Change to common_base. */

    BOARD_DisablePll();
}

/* Restore main clock. */
void BOARD_RestoreClockConfig(void)
{
    BOARD_RestorePll();

    CLOCK_SetClkDiv(kCLOCK_DivCmptMainClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_COMPUTE_MAIN); /* Switch to PLL 230MHZ */
    CLOCK_SetClkDiv(kCLOCK_DivMediaMainClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_MEDIA_MAIN);   /* Switch to PLL 230MHZ */
    CLOCK_SetClkDiv(kCLOCK_DivMediaVddnClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_MEDIA_VDDN);   /* Switch to PLL 230MHZ */
    CLOCK_SetClkDiv(kCLOCK_DivComputeRamClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD0_to_RAM);          /* Switch to PLL 230MHZ */
    CLOCK_SetClkDiv(kCLOCK_DivCommonVddnClk, 2U);
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_COMMON_VDDN);  /* Switch to 250MHZ */

    BOARD_SetXspiClock(XSPI0, 3U, 1U);               /* Main PLL PDF1 DIV2. */
}
#endif

/*
 * Enter the LowPower mode.
 */
void APP_LowPower_EnterLowPower(void)
{
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U); /* Select lowest voltage when DS. */
    POWER_EnterDeepSleep(APP_EXCLUDE_FROM_DEEPSLEEP);
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

void USB0_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}

void USB1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Power on COM VDDN domain for USB */
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);    
        
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    { 
        /* Power on usb ram araay as need, powered USB0RAM array*/
        POWER_DisablePD(kPDRUNCFG_APD_USB0_SRAM);
        POWER_DisablePD(kPDRUNCFG_PPD_USB0_SRAM);
        /* Apply the config */
        POWER_ApplyPD();
        /* disable the read and write gate */
        SYSCON4->USB0_MEM_CTRL |= (SYSCON4_USB0_MEM_CTRL_MEM_WIG_MASK | SYSCON4_USB0_MEM_CTRL_MEM_RIG_MASK |
                                     SYSCON4_USB0_MEM_CTRL_MEM_STDBY_MASK);
        /* Enable the USBPHY0 CLOCK */
        SYSCON4->USBPHY0_CLK_ACTIVE |= SYSCON4_USBPHY0_CLK_ACTIVE_IPG_CLK_ACTIVE_MASK;
        CLOCK_AttachClk(kUSB_24MHZ_to_USB);
        CLOCK_AttachClk(kOSC_CLK_to_USB_24MHZ);
        CLOCK_EnableClock(kCLOCK_Usb0);
        CLOCK_EnableClock(kCLOCK_UsbphyRef);
        RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
        RESET_PeripheralReset(kUSBPHY0_RST_SHIFT_RSTn);
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
        USB_EhciLowPowerPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);
        /* Make USB phy pll disabled in suspend state */
        USBPHY->PLL_SIC |= USBPHY_PLL_SIC_PLL_MISC2_CONTROL0_MASK;
    }

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
