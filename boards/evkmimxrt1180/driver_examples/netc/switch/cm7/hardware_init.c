/*
 * Copyright 2024 NXP
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
#define EXAMPLE_EP0_PORT_PHY_RESET_PIN  RGPIO4, 13
#define EXAMPLE_SWT_PORT0_PHY_RESET_PIN RGPIO4, 25
#define EXAMPLE_SWT_PORT1_PHY_RESET_PIN RGPIO6, 13
#define EXAMPLE_SWT_PORT2_PHY_RESET_PIN RGPIO4, 28
#define EXAMPLE_SWT_PORT3_PHY_RESET_PIN RGPIO6, 15

#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
/*${macro:end}*/

/*${variable:start}*/
/* PHY operation. */
static netc_mdio_handle_t s_emdio_handle;
static phy_rtl8211f_resource_t s_phy_resource[5];
static phy_handle_t s_phy_handle[5];
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    if (((1U << 2) & EXAMPLE_SWT_USED_PORT_BITMAP) != 0U)
    {
        BOARD_InitSwtPort2Pins();
    }
    if (((1U << 3) & EXAMPLE_SWT_USED_PORT_BITMAP) != 0U)
    {
        BOARD_InitSwtPort3Pins();
    }
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

static status_t APP_Phy8201SetUp(phy_handle_t *handle)
{
    status_t result;
    uint16_t data;

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 7);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Read(handle, 16, &data);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* CRS/DV pin is RXDV signal. */
    data |= (1U << 2);
    result = PHY_Write(handle, 16, data);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 0);

    return result;
}

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    status_t result = kStatus_Success;

    s_phy_resource[port].write = APP_EMDIOWrite;
    s_phy_resource[port].read  = APP_EMDIORead;

    result = PHY_Init(&s_phy_handle[port], phyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return PHY_EnableLoopback(&s_phy_handle[port], kPHY_LocalLoop, phyConfig->speed, true);
}

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phy8211Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };
    phy_config_t phy8201Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8201_ops,
    };

    /* Reset all PHYs even some are not used in case unstable status has effect on other PHYs. */
    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    /* Reset PHY8211 for ETH1(Switch port1), ETH2(Switch port2), ETH3(Switch port3). Reset 10ms, wait 30ms. */
    RGPIO_PinWrite(EXAMPLE_EP0_PORT_PHY_RESET_PIN, 0);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 0);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT1_PHY_RESET_PIN, 0);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 0);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT3_PHY_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    RGPIO_PinWrite(EXAMPLE_EP0_PORT_PHY_RESET_PIN, 1);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 1);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT1_PHY_RESET_PIN, 1);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT2_PHY_RESET_PIN, 1);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT3_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));

    /* Initialize PHY for EP. */
    phy8201Config.resource = &s_phy_resource[EXAMPLE_EP0_PORT];
    phy8201Config.phyAddr  = BOARD_EP0_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_EP0_PORT, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = APP_Phy8201SetUp(&s_phy_handle[EXAMPLE_EP0_PORT]);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY for switch port0. */
    phy8201Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT0];
    phy8201Config.phyAddr  = BOARD_SWT_PORT0_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT0, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = APP_Phy8201SetUp(&s_phy_handle[EXAMPLE_SWT_PORT0]);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY for switch port1. */
    phy8211Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT1];
    phy8211Config.phyAddr  = BOARD_SWT_PORT1_PHY_ADDR;
    result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT1, &phy8211Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    if (((1U << 2) & EXAMPLE_SWT_USED_PORT_BITMAP) != 0U)
    {
        /* Initialize PHY for switch port2. */
        phy8211Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT2];
        phy8211Config.phyAddr  = BOARD_SWT_PORT2_PHY_ADDR;
        result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT2, &phy8211Config);
        if (result != kStatus_Success)
        {
            return result;
        }
    }

    if (((1U << 3) & EXAMPLE_SWT_USED_PORT_BITMAP) != 0U)
    {
        /* Initialize PHY for switch port3. */
        phy8211Config.resource = &s_phy_resource[EXAMPLE_SWT_PORT3];
        phy8211Config.phyAddr  = BOARD_SWT_PORT3_PHY_ADDR;
        result                 = APP_PHY_SetPort(EXAMPLE_SWT_PORT3, &phy8211Config);
        if (result != kStatus_Success)
        {
            return result;
        }
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
        case EXAMPLE_EP0_PORT:
            *mode = kNETC_RmiiMode;
            break;
        case EXAMPLE_SWT_PORT0:
            *mode = kNETC_RmiiMode;
            break;
        case EXAMPLE_SWT_PORT1:
            *mode = kNETC_RgmiiMode;
            break;
        case EXAMPLE_SWT_PORT2:
            *mode = kNETC_RgmiiMode;
            break;
        case EXAMPLE_SWT_PORT3:
            *mode = kNETC_RgmiiMode;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&s_phy_handle[port], (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}

status_t APP_PHY_EnableLoopback(uint32_t port, bool enable)
{
    phy_speed_t speed = kPHY_Speed100M;

    switch (port)
    {
        case EXAMPLE_EP0_PORT:
        case EXAMPLE_SWT_PORT0:
            /* kPHY_Speed100M */
            break;
        case EXAMPLE_SWT_PORT1:
        case EXAMPLE_SWT_PORT2:
        case EXAMPLE_SWT_PORT3:
            speed = kPHY_Speed1000M;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_EnableLoopback(&s_phy_handle[port], kPHY_LocalLoop, speed, enable);
}
/*${function:end}*/
