/*
 * Copyright 2020, 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_component_i3c.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1A

/* Device 48-bit Provisioned ID. */
#define I3C_NXP_VENDOR_ID              0x11BU
#define I3C_DEVICE_PROVISION_ID        0x1U
#define I3C_DEVICE_REJOIN_PROVISION_ID 0x3U

/* Generic Device. */
#if defined(FSL_FEATURE_I3C_HAS_IBI_PAYLOAD_SIZE_OPTIONAL_BYTE) && FSL_FEATURE_I3C_HAS_IBI_PAYLOAD_SIZE_OPTIONAL_BYTE
#define EXAMPLE_I3C_BCR I3C_BUS_DEV_BCR_DEV_ROLE(I3C_BUS_DEV_BCR_DEV_MASTER) | I3C_BUS_DEV_BCR_MODE_MASK | I3C_BUS_DEV_BCR_IBI_PAYLOAD_MASK
#else
#define EXAMPLE_I3C_BCR I3C_BUS_DEV_BCR_DEV_ROLE(I3C_BUS_DEV_BCR_DEV_MASTER) | I3C_BUS_DEV_BCR_MODE_MASK
#endif
#define EXAMPLE_I3C_DCR 0U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static i3c_bus_t demo_i3cBus;
static i3c_device_t demo_masterDev;

extern i3c_device_control_info_t i3cMasterCtlInfo;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if !defined(EXAMPLE_HAS_NO_HJ_SUPPORT)
status_t APP_GetTargetDynmicAddr(i3c_bus_t *demo_i3cBus, uint16_t vendorID, uint32_t provisionedID, uint8_t *addr)
{
    for (list_element_handle_t listItem = demo_i3cBus->i3cDevList.head; listItem != NULL; listItem = listItem->next)
    {
        if ((i3c_device_t *)listItem == &demo_masterDev)
        {
            continue;
        }
        else
        {
            /* Find the connected I3C slave on the other board. */
            if ((((i3c_device_t *)listItem)->info.vendorID == vendorID) && (((i3c_device_t *)listItem)->info.partNumber == provisionedID))
            {
                *addr = ((i3c_device_t *)listItem)->info.dynamicAddr;

                return kStatus_Success;
            }
        }
    }

    return kStatus_Fail;
}
#endif

void APP_PrintDeviceInfo(i3c_bus_t *demo_i3cBus)
{
    i3c_device_information_t devInfo;
    status_t result;
    uint32_t i = 0;

    PRINTF(">> Current I3C devices on the bus:\r\n");
    for (list_element_handle_t listItem = demo_i3cBus->i3cDevList.head; listItem != NULL; listItem = listItem->next)
    {
        if ((i3c_device_t *)listItem == &demo_masterDev)
        {
            continue;
        }
        else
        {
            devInfo = ((i3c_device_t *)listItem)->info;

            result = I3C_BusMasterGetDeviceInfo(&demo_masterDev, devInfo.dynamicAddr, &devInfo);
            if (result == kStatus_Success)
            {
                PRINTF("Device %u:\r\n", i);
                PRINTF("Vendor ID - 0x%X. Part Number - 0x%X\r\n", devInfo.vendorID, devInfo.partNumber);
                PRINTF("Dynamic Address - 0x%X. Static Address - 0x%X.\r\n", devInfo.dynamicAddr, devInfo.staticAddr);
                PRINTF("Max read length - %u. Max write length - %u.\r\n", devInfo.maxReadLength, devInfo.maxWriteLength);
                PRINTF("BCR - 0x%X. DCR - 0x%X.\r\n\r\n", devInfo.bcr, devInfo.dcr);
            }

            i++;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    i3c_device_information_t masterInfo;
    i3c_bus_config_t busConfig;
    status_t result;

    BOARD_InitHardware();

    PRINTF("\r\nI3C bus master example.\r\n");

    /* Create I3C bus, work as main master. */
    I3C_BusGetDefaultBusConfig(&busConfig);
    busConfig.i2cBaudRate = EXAMPLE_I2C_BAUDRATE;
    busConfig.i3cOpenDrainBaudRate = EXAMPLE_I3C_OD_BAUDRATE;
    busConfig.i3cPushPullBaudRate = EXAMPLE_I3C_PP_BAUDRATE;
    I3C_BusCreate(&demo_i3cBus, &busConfig);

    memset(&masterInfo, 0, sizeof(masterInfo));
    masterInfo.staticAddr  = I3C_MASTER_SLAVE_ADDR_7BIT;
    masterInfo.bcr         = EXAMPLE_I3C_BCR;
    masterInfo.dcr         = EXAMPLE_I3C_DCR;
    masterInfo.dynamicAddr = I3C_BusGetValidAddrSlot(&demo_i3cBus, 0);
    masterInfo.vendorID    = I3C_NXP_VENDOR_ID;
    masterInfo.partNumber  = I3C_DEVICE_PROVISION_ID;
    i3cMasterCtlInfo.isSecondary = false;

    result = I3C_BusMasterCreate(&demo_masterDev, &demo_i3cBus, &masterInfo, &i3cMasterCtlInfo);
    if (result != kStatus_Success)
    {
        PRINTF("I3C_BusMasterCreate fails with error %u.\r\n", result);
        return result;
    }
    PRINTF("I3C bus master creates.\r\n");

#if !defined(EXAMPLE_HAS_NO_HJ_SUPPORT)
    /* Disable Controller Request in case there's master operation is not over. */
    result = I3C_BusMasterDisableEvents(&demo_masterDev, I3C_BUS_BROADCAST_ADDR, (uint8_t)kI3C_EventHJ);
    if (result != kStatus_Success)
    {
        PRINTF("I3C_BusMasterDisableEvents fails with error %u.\r\n", result);
        return result;
    }

    APP_PrintDeviceInfo(&demo_i3cBus);

    /* Enable Hot-join. */
    result = I3C_BusMasterEnableEvents(&demo_masterDev, I3C_BUS_BROADCAST_ADDR, (uint8_t)kI3C_EventHJ);
    if (result != kStatus_Success)
    {
        PRINTF("I3C_BusMasterEnableEvents Hot-join fails with error %u.\r\n", result);
        return result;
    }

    uint8_t slaveAddr;
    while (APP_GetTargetDynmicAddr(&demo_i3cBus, I3C_NXP_VENDOR_ID, I3C_DEVICE_REJOIN_PROVISION_ID, &slaveAddr) != kStatus_Success);
    PRINTF("Hot-join occur.\r\n");
#endif

    result = I3C_BusMasterDisableEvents(&demo_masterDev, I3C_BUS_BROADCAST_ADDR, (uint8_t)kI3C_EventMR);
    if (result != kStatus_Success)
    {
        PRINTF("I3C_BusMasterDisableEvents fails with error %u.\r\n", result);
        return result;
    }
    APP_PrintDeviceInfo(&demo_i3cBus);

    /* Enable Controller Request. */
    result = I3C_BusMasterEnableEvents(&demo_masterDev, I3C_BUS_BROADCAST_ADDR, (uint8_t)kI3C_EventMR);
    if (result != kStatus_Success)
    {
        PRINTF("I3C_BusMasterEnableEvents master request fails with error %u.\r\n", result);
        return result;
    }

    /* Wait for mastership handoff. */
    while (demo_i3cBus.currentMaster == &demo_masterDev)
    {
    }

    PRINTF("I3C bus mastership handoff.\r\n");

    while (1)
    {
    }
}
