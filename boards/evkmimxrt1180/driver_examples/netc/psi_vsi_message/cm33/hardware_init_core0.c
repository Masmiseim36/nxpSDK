/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app_core0.h"
#include "fsl_debug_console.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_netc_mdio.h"
/*${header:end}*/

/*${macro:start}*/
#define EXAMPLE_SWT_PORT1_PHY_RESET_PIN RGPIO6, 13
/*${macro:end}*/

/*${variable:start}*/
/* PHY operation. */
static netc_mdio_handle_t s_emdio_handle;
static phy_rtl8211f_resource_t s_phy_resource;
static phy_handle_t s_phy_handle;
/*${variable:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_NETC_Init();
}

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)__section_begin("__core1_image");
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif

status_t APP_MDIO_Init(void)
{
    netc_mdio_config_t mdioConfig = {
        .mdio.type         = kNETC_EMdio,
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = EXAMPLE_NETC_FREQ,
    };

    return NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
}

static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phy8211Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };
    rgpio_pin_config_t pinConfig = {.pinDirection = kRGPIO_DigitalOutput, .outputLogic = 0};

    /* Reset PHY8211 for ETH1(Switch port1). Reset 10ms, wait 72ms. */
    RGPIO_PinInit(EXAMPLE_SWT_PORT1_PHY_RESET_PIN, &pinConfig);
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    RGPIO_PinWrite(EXAMPLE_SWT_PORT1_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY for switch port1. */
    s_phy_resource.write   = APP_EMDIOWrite;
    s_phy_resource.read    = APP_EMDIORead;
    phy8211Config.resource = &s_phy_resource;
    phy8211Config.phyAddr  = EXAMPLE_SWT_PORT1_PHY_ADDR;

    result = PHY_Init(&s_phy_handle, &phy8211Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return PHY_EnableLoopback(&s_phy_handle, kPHY_LocalLoop, kPHY_Speed100M, true);
}

status_t APP_PHY_GetLinkStatus(bool *link)
{
    return PHY_GetLinkStatus(&s_phy_handle, link);
}

void SystemInitHook(void)
{
    Prepare_CM7(CORE1_KICKOFF_ADDRESS);

    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}
/*${function:end}*/
