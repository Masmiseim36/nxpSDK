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
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "disk.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "usb_phy.h"
#include "fsl_power.h"
#include "usb_device_config.h"
/*${header:end}*/
extern usb_msc_struct_t g_msc;
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_msc.deviceHandle);
}

void USB_DeviceClockInit(void)
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
    CLOCK_EnableUsbHs0DeviceClock(kOSC_CLK_to_USB_CLK, usbClockDiv);
    /* save usb ip clock freq*/
    usbClockFreq = g_xtalFreq / usbClockDiv;
    CLOCK_SetClkDiv(kCLOCK_DivPfc1Clk, 4);
    /* enable usb ram clock */
    CLOCK_EnableClock(kCLOCK_UsbhsSram);
    /* enable USB PHY PLL clock, the phy bus clock (480MHz) source is same with USB IP */
    CLOCK_EnableUsbHs0PhyPllClock(kOSC_CLK_to_USB_CLK, usbClockFreq);

    /* USB PHY initialization */
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);

#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif

    /* the following code should run after phy initialization and should wait some microseconds to make sure utmi clock
     * valid */
    /* enable usb1 host clock */
    CLOCK_EnableClock(kCLOCK_UsbhsHost);
    /*  Wait until host_needclk de-asserts */
    while (SYSCTL0->USB0CLKSTAT & SYSCTL0_USB0CLKSTAT_HOST_NEED_CLKST_MASK)
    {
        __ASM("nop");
    }
    /*According to reference mannual, device mode setting has to be set by access usb host register */
    USBHSH->PORTMODE |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
    /* disable usb1 host clock */
    CLOCK_DisableClock(kCLOCK_UsbhsHost);
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
    USB_DeviceLpcIp3511TaskFunction(deviceHandle);
}
#endif
/*${function:end}*/
