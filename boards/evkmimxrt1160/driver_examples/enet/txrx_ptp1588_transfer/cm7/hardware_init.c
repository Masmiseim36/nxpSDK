/*
 * Copyright 2021-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_enet.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
phy_ksz8081_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);
    clock_root_config_t rootCfg = {.mux = 4, .div = 10}; /* Generate 50M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg);

    /* Generate 24M PTP clock. */
    rootCfg.mux = 0;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Enet_Timer1, &rootCfg);
}

void IOMUXC_SelectENETClock(void)
{
    IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_SelectENETClock();

    /* Errata ERR050396: For some bus masters, writing access to CM7 TCM is handled by a NIC301 block which does
     * not support sparse write conversion. It results in a data corruption when sparse writing to CM7 TCM happens.
     * For ENET: If CM7 TCM is the destination for writing, IOMUXC_GPR_GPR28[CACHE_ENET] should be cleared. If
     * IOMUXC_GPR_GPR28[CACHE_ENET] is set, write buffers must be placed in OCRAM or external RAM. */
    IOMUXC_GPR->GPR28 &= (~IOMUXC_GPR_GPR28_CACHE_ENET_MASK);

    /* Hardware reset PHY. */
    BOARD_ENET_PHY0_RESET;

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}
/*${function:end}*/
