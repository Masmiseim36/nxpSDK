/*
 * Copyright 2021-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_iomuxc.h"
#include "fsl_enet.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
phy_ksz8081_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitModuleClock(void)
{
    /* Set 50MHz output clock required by PHY, set PTP clock 25MHz. */
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = true, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);

    /* Output 50MHz clock to PHY. */
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);
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

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    /* Hardware reset PHY. */
    BOARD_ENET_PHY_RESET;

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}

/*${function:end}*/
