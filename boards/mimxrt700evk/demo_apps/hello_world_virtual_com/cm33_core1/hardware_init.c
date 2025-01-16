/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "fsl_component_serial_port_usb.h"
#include "pin_mux.h"
#include "board.h"
#include "usb_phy.h"
#include <stdbool.h>
#include "fsl_power.h"
#include "usb_device_config.h"
#include "fsl_debug_console.h"
/*${header:end}*/
/*${prototype:start}*/
void USB_DeviceClockInit(void);
/*${prototype:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_BootClockRUN();
    USB_DeviceClockInit();
    DbgConsole_Init((uint8_t)kSerialManager_UsbControllerEhci0, (uint32_t)NULL, kSerialPort_UsbCdc, (uint32_t)NULL);
}

void USB_DeviceClockInit(void)
{
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Power on COM VDDN domain for USB or eUSB */
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);    
        
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
    CLOCK_AttachClk(k32KHZ_WAKE_to_USB);
    CLOCK_AttachClk(kOSC_CLK_to_USB_24MHZ);
    CLOCK_EnableClock(kCLOCK_Usb0);
    CLOCK_EnableClock(kCLOCK_UsbphyRef);
    RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSBPHY0_RST_SHIFT_RSTn);
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    USB_EhciPhyInit((uint8_t)kSerialManager_UsbControllerEhci0, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);
}
/*${function:end}*/
