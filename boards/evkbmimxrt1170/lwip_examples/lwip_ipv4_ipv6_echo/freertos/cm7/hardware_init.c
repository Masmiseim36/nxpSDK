/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_enet.h"
#include "ethernetif.h"
#include "ethernetif_priv.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
#if BOARD_NETWORK_USE_100M_ENET_PORT
#error "BOARD_NETWORK_USE_100M_ENET_PORT is set: this demo initializes both ENETs on evkbmimxrt1170"
#endif

phy_rtl8201_resource_t g_phy_resource;
phy_rtl8211f_resource_t g_phy1_resource;

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);

    clock_root_config_t rootCfg0 = {.mux = 4, .div = 10}; /* Generate 50M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg0);

    clock_root_config_t rootCfg1 = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg1);
}

void IOMUXC_SelectENETClock(void)
{
    IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK;  /* bit1:iomuxc_gpr_enet_clk_dir
                                                                  bit0:GPR_ENET_TX_CLK_SEL(internal or OSC) */
}

void BOARD_ENETFlexibleConfigure(enet_config_t *config)
{
    phy_handle_t *phy_ = ethernetif_get_phy((struct netif *)config->userData);

    if (phy_->phyAddr == BOARD_ENET0_PHY_ADDRESS)
    {
        config->miiMode = kENET_RmiiMode;
    }
    else if (phy_->phyAddr == BOARD_ENET1_PHY_ADDRESS)
    {
        config->miiMode = kENET_RgmiiMode;
    }
    else
    {
        // Unknown PHY addr
    }
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}

static void MDIO_Init_1G(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET_1G)]);
    ENET_SetSMI(EXAMPLE_ENET_1G, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write_1G(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET_1G, phyAddr, regAddr, data);
}

static status_t MDIO_Read_1G(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET_1G, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_SelectENETClock();

    BOARD_InitEnetPins();
    GPIO_PinInit(GPIO12, 12, &gpio_config);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO12, 12, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));

    BOARD_InitEnet1GPins();
    GPIO_PinInit(GPIO11, 14, &gpio_config);
    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO11, 14, 1);
    SDK_DelayAtLeastUs(30000, CLOCK_GetFreq(kCLOCK_CpuClk));

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    MDIO_Init_1G();
    g_phy1_resource.read  = MDIO_Read_1G;
    g_phy1_resource.write = MDIO_Write_1G;
}
/*${function:end}*/
