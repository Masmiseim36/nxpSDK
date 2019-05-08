/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "serial_port_usb.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void USB_DeviceClockInit(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
typedef struct _clock_config
{
    mcg_config_t mcgConfig;       /*!< MCG configuration.      */
    sim_clock_config_t simConfig; /*!< SIM configuration.      */
    uint32_t coreClock;           /*!< core clock frequency.   */
} clock_config_t;
void App_BootClockRUN(void)
{
    const clock_config_t appClockConfigRun = {
        .mcgConfig =
            {
                .mcgMode = kMCG_ModeBLPE,            /* Work in BLPE mode. */
                .irclkEnableMode = kMCG_IrclkEnable, /* MCGIRCLK enable. */
                .ircs = kMCG_IrcSlow,                /* Select IRC32k. */
                .fcrdiv = 0U,                        /* FCRDIV is 0. */
                .frdiv = 4U,
                .drs = kMCG_DrsLow,         /* Low frequency range */
                .dmx32 = kMCG_Dmx32Default, /* DCO has a default range of 25%. */
                .oscsel = kMCG_OscselIrc,   /* Select IRC. */
                .pll0Config =
                    {
                        .enableMode = 0U, .prdiv = 0x00U, .vdiv = 0x00U,
                    },
            },
        .simConfig =
            {
                .pllFllSel = 3U,        /* PLLFLLSEL select IRC48M. */
                .pllFllDiv = 0U,        /* PLLFLLSEL clock divider divisor. */
                .pllFllFrac = 0U,       /* PLLFLLSEL clock divider fraction. */
                .er32kSrc = 2U,         /* ERCLK32K selection, use RTC. */
                .clkdiv1 = 0x01010000U, /* SIM_CLKDIV1. */
            },
        .coreClock = 48000000U, /* Core clock frequency */
    };
    CLOCK_SetSimSafeDivs();
    CLOCK_BootToBlpeMode(appClockConfigRun.mcgConfig.oscsel);
    CLOCK_SetInternalRefClkConfig(appClockConfigRun.mcgConfig.irclkEnableMode, appClockConfigRun.mcgConfig.ircs,
                                  appClockConfigRun.mcgConfig.fcrdiv);
    CLOCK_SetSimConfig(&appClockConfigRun.simConfig);
    SystemCoreClock = appClockConfigRun.coreClock;
}


void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    SystemCoreClockUpdate();
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
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
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    App_BootClockRUN();
    USB_DeviceClockInit();
    DbgConsole_Init((uint8_t)kSerialManager_UsbControllerKhci0, (uint32_t)NULL, kSerialPort_UsbCdc, (uint32_t)NULL);
    PMC->REGSC |= PMC_REGSC_BGEN_MASK | PMC_REGSC_VLPO_MASK;

    PRINTF("hello world.\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
