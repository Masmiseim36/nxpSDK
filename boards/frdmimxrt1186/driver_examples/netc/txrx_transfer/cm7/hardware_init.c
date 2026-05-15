/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
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
static netc_mdio_handle_t s_emdio_handle;
static phy_yt8521_resource_t s_phy_resource[5];
static phy_handle_t s_phy_handle[5];
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitNETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    BOARD_NETC_Init();
}

status_t APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = EXAMPLE_NETC_FREQ,
    };

    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    status_t result = kStatus_Success;
    uint16_t regValue;

    s_phy_resource[port].write = APP_EMDIOWrite;
    s_phy_resource[port].read  = APP_EMDIORead;

    result = PHY_Init(&s_phy_handle[port], phyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    PHY_Write(&s_phy_handle[port], PHY_EXT_ADDR_REG, PHY_RGMII_CONFIG1_REG);
    PHY_Read(&s_phy_handle[port], PHY_EXT_DATA_REG, &regValue);
    regValue &= ~PHY_RGMII_CONFIG1_TXDLY_MASK;
    /* 150ps per step. */
    regValue |= 0x2;
    PHY_Write(&s_phy_handle[port], PHY_EXT_DATA_REG, regValue);

    return PHY_EnableLoopback(&s_phy_handle[port], kPHY_LocalLoop, phyConfig->speed, true);
}

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phyyt8521Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed1000M,
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

    /* Initialize PHY for switch port2. */
    phyyt8521Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT2];
    phyyt8521Config.phyAddr  = BOARD_SWT_PORT2_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT2, &phyyt8521Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link)
{
    return PHY_GetLinkStatus(&s_phy_handle[port], link);
}

status_t APP_PHY_GetLinkModeSpeedDuplex(uint32_t port,
                                        netc_hw_mii_mode_t *mode,
                                        netc_hw_mii_speed_t *speed,
                                        netc_hw_mii_duplex_t *duplex)
{
    switch (port)
    {
        case EXAMPLE_SWT_PORT0:
            *mode = kNETC_RgmiiMode;
            break;
        case EXAMPLE_SWT_PORT2:
            *mode = kNETC_RgmiiMode;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&s_phy_handle[port], (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}
/*${function:end}*/
