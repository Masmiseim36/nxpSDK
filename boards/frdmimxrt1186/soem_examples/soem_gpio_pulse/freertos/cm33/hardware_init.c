/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${macro:start}*/
/*!< PHY reset pins. */
#define EXAMPLE_SWT_PORT0_PHY_RESET_PIN RGPIO1, 15
#define EXAMPLE_SWT_PORT2_PHY_RESET_PIN RGPIO1, 20

#define PHY_EXT_ADDR_REG             0x1EU
#define PHY_EXT_DATA_REG             0x1FU
#define PHY_RGMII_CONFIG1_REG        0xA003U
#define PHY_RGMII_CONFIG1_TXDLY_MASK 0xFU
/*${macro:end}*/

/*${variable:start}*/
/* PHY operation. */
struct netc_swt_if_port if_port;
static phy_yt8521_resource_t s_phy_resource[5];
/*${variable:end}*/

status_t NETC_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = NETC_FREQ,
    };

    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&if_port.s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

static status_t NETC_EP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&if_port.s_emdio_handle, phyAddr, regAddr, data);
}

static status_t NETC_EP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&if_port.s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    status_t result = kStatus_Success;
    uint16_t regValue;

    s_phy_resource[port].write = NETC_EP_EMDIOWrite;
    s_phy_resource[port].read  = NETC_EP_EMDIORead;

    result = PHY_Init(&if_port.phy_handle, phyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    PHY_Write(&if_port.phy_handle, PHY_EXT_ADDR_REG, PHY_RGMII_CONFIG1_REG);
    PHY_Read(&if_port.phy_handle, PHY_EXT_DATA_REG, &regValue);
    regValue &= ~PHY_RGMII_CONFIG1_TXDLY_MASK;
    /* 150ps per step. */
    regValue |= 0x2;
    PHY_Write(&if_port.phy_handle, PHY_EXT_DATA_REG, regValue);

    return result;
}

status_t NETC_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phyyt8521Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyyt8521_ops,
    };

    /* Reset all PHYs even some are not used in case unstable status has effect on other PHYs. */
    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    /* Reset PHY8211 for ETH1(Switch port1), ETH2(Switch port2), ETH3(Switch port3). Reset 10ms, wait 30ms. */
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 0);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 1);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));

    /* Initialize PHY for switch port0. */
    phyyt8521Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT0];
    phyyt8521Config.phyAddr  = BOARD_SWT_PORT0_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT0, &phyyt8521Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

status_t BOARD_InitHardware(void)
{
    bool link;
    status_t result = kStatus_Success;
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitNETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_NETC_Init();

    PRINTF("Init BOARD_InitHardware success...\r\n");

    result = NETC_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = NETC_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    do
    {
		PHY_GetLinkStatus(&if_port.phy_handle, &link);
    } while (!link);

    return result;
}