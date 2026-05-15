/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "usb_device_config.h"
#include "usb.h"
#include "usb_phy.h"
#include "usb_device.h"
#include "usb_device_ch9.h"
#include "usb_device_cdc_rndis.h"
#include "usb_device_descriptor.h"
#include "virtual_nic_enetif.h"
#include "virtual_nic.h"

#include "fsl_netc_mdio.h"

/*!< PHY reset pins. */
#define EXAMPLE_SWT_PORT0_PHY_RESET_PIN RGPIO1, 15
#define EXAMPLE_SWT_PORT2_PHY_RESET_PIN RGPIO1, 20

extern usb_cdc_vnic_t g_cdcVnic;
extern netc_mdio_handle_t s_mdio_handle;
uint8_t BOARD_SwitchPortNum = 0;
uint32_t BOARD_SwtichFrameFID = 1;

status_t APP_MDIO_Init(void)
{
    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz = CLOCK_GetRootClockFreq(kCLOCK_Root_Netc),
    };

    mdioConfig.mdio.type = kNETC_EMdio;

    return NETC_MDIOInit(&s_mdio_handle, &mdioConfig);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitNETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    BOARD_NETC_Init();
    APP_MDIO_Init();

    /* Reset all PHYs even some are not used in case unstable status has effect on other PHYs. */
    /* Reset YT8521 for ETH0, ETH2. Reset 10ms, wait 150ms. */
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 0);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 1);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));
}

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_cdcVnic.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
    usbClockFreq = 24000000;
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
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
