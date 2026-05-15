/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "network_cfg.h"
#include "app.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyyt8521.h"
#include "fsl_msgintr.h"
/*${header:end}*/

/*${macro:start}*/
/*!< PHY reset pins. */
#define EXAMPLE_SWT_PORT0_PHY_RESET_PIN RGPIO1, 15

#define PHY_EXT_ADDR_REG             0x1EU
#define PHY_EXT_DATA_REG             0x1FU
#define PHY_RGMII_CONFIG1_REG        0xA003U
#define PHY_RGMII_CONFIG1_TXDLY_MASK 0xFU
/*${macro:end}*/

/*${variable:start}*/
/* PHY operation. */
static netc_mdio_handle_t s_emdio_handle;

static status_t APP_PHY_YT8521_Init(phy_handle_t *handle, const phy_config_t *config); // TODO move

const phy_operations_t g_app_phyyt8521_ops = {.phyInit             = APP_PHY_YT8521_Init,
                                              .phyWrite            = PHY_YT8521_Write,
                                              .phyRead             = PHY_YT8521_Read,
                                              .getAutoNegoStatus   = PHY_YT8521_GetAutoNegotiationStatus,
                                              .getLinkStatus       = PHY_YT8521_GetLinkStatus,
                                              .getLinkSpeedDuplex  = PHY_YT8521_GetLinkSpeedDuplex,
                                              .setLinkSpeedDuplex  = PHY_YT8521_SetLinkSpeedDuplex,
                                              .enableLoopback      = PHY_YT8521_EnableLoopback,
                                              .enableLinkInterrupt = PHY_YT8521_EnableLinkInterrupt,
                                              .clearInterrupt      = PHY_YT8521_ClearInterrupt};

phy_yt8521_resource_t g_phy_yt8521_resource;
/*${variable:end}*/

/*${function:start}*/
#include "fsl_debug_console.h" // TODO remove
static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t APP_PHY_YT8521_Init(phy_handle_t *handle, const phy_config_t *config)
{
    phy_config_t phyyt8521Config = {
        .phyAddr   = config->phyAddr,
        .autoNeg   = true,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = handle->ops,
        .resource  = &g_phy_yt8521_resource,
    };
    uint16_t regValue;
    status_t result;

    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));

    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));

    g_phy_yt8521_resource.write = APP_EMDIOWrite;
    g_phy_yt8521_resource.read  = APP_EMDIORead;

    result = PHY_YT8521_Init(handle, &phyyt8521Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    PHY_Write(handle, PHY_EXT_ADDR_REG, PHY_RGMII_CONFIG1_REG);
    PHY_Read(handle, PHY_EXT_DATA_REG, &regValue);
    regValue &= ~PHY_RGMII_CONFIG1_TXDLY_MASK;
    /* 150ps per step. */
    regValue |= 0x2;
    PHY_Write(handle, PHY_EXT_DATA_REG, regValue);

    return result;
}

static void APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .mdio =
            {
                .type = kNETC_EMdio,
            },
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = EXAMPLE_CLOCK_FREQ,
    };

    mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)kNETC_ENETC0EthPort;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    while (result != kStatus_Success)
    {
        // failed
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitNETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    BOARD_NETC_Init();
    APP_MDIO_Init();
}

/*${function:end}*/
