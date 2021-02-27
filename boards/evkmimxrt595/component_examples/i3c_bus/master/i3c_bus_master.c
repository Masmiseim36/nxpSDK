/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_component_i3c.h"
#include "fsl_i3c.h"

#include "fsl_component_i3c_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_MASTER             I3C0
#define EXAMPLE_I2C_BAUDRATE       400000
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq()
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1A
#define WAIT_TIME                  1000
#define I3C_DATA_LENGTH            33

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_master_txBuff[I3C_DATA_LENGTH];
uint8_t g_master_rxBuff[I3C_DATA_LENGTH];
i3c_bus_t i3cBus;
i3c_device_t masterDev;
/*******************************************************************************
 * Code
 ******************************************************************************/
i3c_master_adapter_resource_t masterResource = {.base = EXAMPLE_MASTER, .transMode = kI3C_MasterTransferInterruptMode};

extern i3c_device_hw_ops_t master_ops;
i3c_device_control_info_t i3cMasterCtlInfo = {.funcs = &master_ops, .resource = &masterResource, .isSecondary = false};


/*!
 * @brief Main function
 */
int main(void)
{
    /* Attach main clock to I3C, 396MHz / 4 = 99MHz. */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 4);

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    masterResource.clockInHz = I3C_MASTER_CLOCK_FREQUENCY;

    PRINTF("\r\nI3C bus master example.\r\n");

    /* Create I3C bus. */
    i3c_bus_config_t busConfig;
    I3C_BusGetDefaultBusConfig(&busConfig);
    I3C_BusCreate(&i3cBus, &busConfig);
    i3c_device_information_t masterInfo;
    memset(&masterInfo, 0, sizeof(masterInfo));
    masterInfo.staticAddr  = I3C_MASTER_SLAVE_ADDR_7BIT;
    masterInfo.bcr         = 0x60U;
    masterInfo.dcr         = 0x00U;
    masterInfo.dynamicAddr = I3C_BusGetValidAddrSlot(&i3cBus, 0);
    masterInfo.vendorID    = 0x345U;

    extern i3c_device_control_info_t i3cMasterCtlInfo;
    I3C_BusMasterCreate(&masterDev, &i3cBus, &masterInfo, &i3cMasterCtlInfo);

    PRINTF("\r\nI3C bus master creates.\r\n");

    i3c_device_information_t devInfo;
    uint8_t slaveAddr = 0x0U;

    for (list_element_handle_t listItem = i3cBus.i3cDevList.head; listItem != NULL; listItem = listItem->next)
    {
        if ((i3c_device_t *)listItem == &masterDev)
        {
            continue;
        }
        else
        {
            slaveAddr = ((i3c_device_t *)listItem)->info.dynamicAddr;
            break;
        }
    }
    I3C_BusMasterGetDeviceInfo(&masterDev, slaveAddr, &devInfo);

    /* Wait for mastership handoff. */
    while (i3cBus.currentMaster == &masterDev)
        ;

    PRINTF("\r\nI3C bus mastership handoff.\r\n");

    while (1)
    {
    }
}
