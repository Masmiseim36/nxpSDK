/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "app.h"

#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

extern usb_host_handle g_hostHandle;

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();
}

void USB0_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_hostHandle);
}

void USB1_IRQHandler(void)
{
    USB_HostEhciIsrFunction(g_hostHandle);
}

void USB_HostClockInit(void)
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
        CLOCK_AttachClk(k32KHZ_WAKE_to_USB);
        CLOCK_AttachClk(kOSC_CLK_to_USB_24MHZ);
        CLOCK_EnableClock(kCLOCK_Usb0);
        CLOCK_EnableClock(kCLOCK_UsbphyRef);
        RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
        RESET_PeripheralReset(kUSBPHY0_RST_SHIFT_RSTn);
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
        USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);
    }

}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
/* USB_HOST_CONFIG_EHCI */

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

void USB_HostTaskFn(void *param)
{
    USB_HostEhciTaskFunction(param);
}
/*${function:end}*/