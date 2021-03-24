
#include "fsl_common.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "usb.h"
#include "usb_phy.h"
#include "board.h"

#include "tx_api.h"

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
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
    USB_EhciPhyInit(kUSB_ControllerEhci0, BOARD_XTAL0_CLK_HZ, &phyConfig);
}

ULONG usb_device_base(void)
{
    return USB_BASE;
}

void usb_device_setup(void)
{
    usb_setup();
}

void usb_device_interrupt_setup(int interrupt_priority)
{
    NVIC_SetPriority(USB_OTG1_IRQn, interrupt_priority);
    NVIC_EnableIRQ(USB_OTG1_IRQn);
}
