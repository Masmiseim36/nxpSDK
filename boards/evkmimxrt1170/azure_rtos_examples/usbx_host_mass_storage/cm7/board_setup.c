
#include <stdio.h>
#include "fsl_common.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "usb.h"
#include "usb_phy.h"
#include "board.h"

void board_setup(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

static void usb_setup(void)
{
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    USB_EhciPhyInit(kUSB_ControllerEhci0, BOARD_XTAL0_CLK_HZ, &phyConfig);
}

unsigned long usb_host_base(void)
{
    /* For EHCI core.  */
    return (USB_OTG1_BASE + 0x100);
}

void usb_host_setup(void)
{
    usb_setup();
}

void usb_host_interrupt_setup(int interrupt_priority)
{
    NVIC_SetPriority(USB_OTG1_IRQn, interrupt_priority);
    NVIC_EnableIRQ(USB_OTG1_IRQn);
}
