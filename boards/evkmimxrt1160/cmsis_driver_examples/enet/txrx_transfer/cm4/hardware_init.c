/*
 * Copyright 2021-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_enet_phy_cmsis.h"
#include "fsl_enet_cmsis.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_iomuxc.h"
#include "fsl_enet.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
cmsis_enet_mac_resource_t ENET0_Resource;
cmsis_enet_phy_resource_t ENETPHY0_Resource;
static phy_rtl8211f_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
uint32_t ENET_GetFreq(void)
{
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Bus);
}

void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);
    clock_root_config_t rootCfg = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET_1G)]);
    ENET_SetSMI(ENET_1G, ENET_GetFreq(), false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET_1G, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET_1G, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    /* Hardware Initialization. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK; /* bit1:iomuxc_gpr_enet_clk_dir
                                                                 bit0:GPR_ENET_TX_CLK_SEL(internal or OSC) */
    /* Hardware reset PHY. */
    BOARD_ENET_PHY1_RESET;

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);

    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    ENET0_Resource.base           = ENET_1G;
    ENET0_Resource.GetFreq        = ENET_GetFreq;
    ENETPHY0_Resource.phyAddr     = RTE_ENET_PHY_ADDRESS;
    ENETPHY0_Resource.ops         = &phyrtl8211f_ops;
    ENETPHY0_Resource.opsResource = &g_phy_resource;

    MDIO_Init();
}
/*${function:end}*/
