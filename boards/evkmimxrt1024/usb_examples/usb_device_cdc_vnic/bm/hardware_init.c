/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "usb_device_cdc_acm.h"
#include "usb_device_cdc_rndis.h"
#include "virtual_nic_enetif.h"
#include "virtual_nic.h"
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_iomuxc.h"
#include "fsl_phyksz8081.h"
#include "fsl_phy.h"
/*${header:end}*/

/*${variable:start}*/
phy_ksz8081_resource_t g_phy_resource;
extern usb_cdc_vnic_t g_cdcVnic;
/*${variable:end}*/

/*${function:start}*/
ENET_Type *BOARD_GetExampleEnetBase(void)
{
    return ENET;
}

uint32_t BOARD_GetPhySysClock(void)
{
    return CLOCK_GetFreq(kCLOCK_IpgClk);
}

const phy_operations_t *BOARD_GetPhyOps(void)
{
    return &phyksz8081_ops;
}

void *BOARD_GetPhyResource(void)
{
    return (void *)&g_phy_resource;
}

void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {
        .enableClkOutput = true, .enableClkOutput500M = false, .enableClkOutput25M = true, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET)]);
    ENET_SetSMI(ENET, CLOCK_GetFreq(kCLOCK_IpgClk), false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 4, &gpio_config);
    GPIO_WritePinOutput(GPIO1, 4, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO1, 4, 1);
    SDK_DelayAtLeastUs(6, CLOCK_GetFreq(kCLOCK_CpuClk));

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_cdcVnic.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
    CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);
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
/*${function:end}*/
