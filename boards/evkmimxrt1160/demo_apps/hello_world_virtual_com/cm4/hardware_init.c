/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_device_registers.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "fsl_debug_console.h"
#include "fsl_component_serial_port_usb.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/
/*${prototype:start}*/
void USB_DeviceClockInit(void);
/*${prototype:end}*/

/*${macro:start}*/
#define CONTROLLER_ID kSerialManager_UsbControllerEhci0
/*${macro:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();
    USB_DeviceClockInit();
    DbgConsole_Init((uint8_t)CONTROLLER_ID, (uint32_t)NULL, kSerialPort_UsbCdc, (uint32_t)NULL);
}

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    if (CONTROLLER_ID == kSerialManager_UsbControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 24000000U);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 24000000U);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, 24000000U);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, 24000000U);
    }
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
}

/*${function:end}*/
