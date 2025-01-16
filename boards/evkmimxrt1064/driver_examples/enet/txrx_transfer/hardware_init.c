/*
 * Copyright 2018-2023 NXP
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
#if (defined(EXAMPLE_PHY_LINK_INTR_SUPPORT) && (EXAMPLE_PHY_LINK_INTR_SUPPORT))
extern void PHY_LinkStatusChange(void);
#endif
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitModuleClock(void)
{
    /* Set 50MHz output clock required by PHY. */
    const clock_enet_pll_config_t config = {
        .enableClkOutput    = true,
        .enableClkOutput25M = false,
        .loopDivider        = 1,
    };
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

#if (defined(EXAMPLE_PHY_LINK_INTR_SUPPORT) && (EXAMPLE_PHY_LINK_INTR_SUPPORT))
    IRQ_ClearPendingIRQ(GPIO1_Combined_0_15_IRQn);
    EnableIRQ(GPIO1_Combined_0_15_IRQn);
#endif

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}

#if (defined(EXAMPLE_PHY_LINK_INTR_SUPPORT) && (EXAMPLE_PHY_LINK_INTR_SUPPORT))
void GPIO_EnableLinkIntr(void)
{
    GPIO_EnableInterrupts(GPIO1, BOARD_INITENETPINS_PHY_INTR_GPIO_PIN_MASK);
}

void GPIO1_Combined_0_15_IRQHandler(void)
{
    if (0U != (GPIO_GetPinsInterruptFlags(GPIO1) & BOARD_INITENETPINS_PHY_INTR_GPIO_PIN_MASK))
    {
        PHY_LinkStatusChange();
        GPIO_DisableInterrupts(GPIO1, BOARD_INITENETPINS_PHY_INTR_GPIO_PIN_MASK);
        GPIO_ClearPinsInterruptFlags(GPIO1, BOARD_INITENETPINS_PHY_INTR_GPIO_PIN_MASK);
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif
/*${function:end}*/
