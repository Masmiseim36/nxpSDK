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

#include "fsl_component_i3c_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
//#define EXAMPLE_SLAVE              I3C0
//#define I3C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetLpOscFreq()
//#define I3C_TIME_OUT_INDEX         100000000
//#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1EU
//#define I3C_DATA_LENGTH            34U
#define EXAMPLE_MASTER             I3C0
#define EXAMPLE_I2C_BAUDRATE       400000
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq()
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1E
#define WAIT_TIME                  1000
#define I3C_DATA_LENGTH            33

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
i3c_bus_t i3cBus;
i3c_device_t slaveDev;
uint8_t g_master_txBuff[I3C_DATA_LENGTH];
uint8_t g_master_rxBuff[I3C_DATA_LENGTH];
/*******************************************************************************
 * Code
 ******************************************************************************/
i3c_master_adapter_resource_t masterResource = {.base = EXAMPLE_MASTER, .transMode = kI3C_MasterTransferInterruptMode};

extern i3c_device_hw_ops_t master_ops;
i3c_device_control_info_t i3cMasterCtlInfo = {.funcs = &master_ops, .resource = &masterResource, .isSecondary = true};

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

    PRINTF("\r\nI3C bus slave example.\r\n");

    /* Create I3C bus, work as secondary master. */
    i3c_bus_config_t busConfig;
    I3C_BusGetDefaultBusConfig(&busConfig);
    I3C_BusCreate(&i3cBus, &busConfig);
    i3c_device_information_t masterInfo;
    memset(&masterInfo, 0, sizeof(masterInfo));
    masterInfo.staticAddr = I3C_MASTER_SLAVE_ADDR_7BIT;
    masterInfo.bcr        = 0x60U;
    masterInfo.dcr        = 0x00U;
    masterInfo.vendorID   = 0x346U;

    extern i3c_device_control_info_t i3cMasterCtlInfo;
    I3C_BusMasterCreate(&slaveDev, &i3cBus, &masterInfo, &i3cMasterCtlInfo);

    PRINTF("\r\nI3C bus secondary master creates.\r\n");

    /* Wait for address assignment finish. */
    while (EXAMPLE_MASTER->SDYNADDR == 0U)
        ;
    /* Request mastership. */
    I3C_BusSlaveRequestMasterShip(&slaveDev);

    /* Wait for mastership takeover. */
    while (i3cBus.currentMaster != &slaveDev)
        ;

    PRINTF("\r\nI3C bus mastership takeover.\r\n");

    i3c_device_information_t devInfo;
    uint8_t slaveAddr = 0xFF;

    for (list_element_handle_t listItem = i3cBus.i3cDevList.head; listItem != NULL; listItem = listItem->next)
    {
        if ((i3c_device_t *)listItem == &slaveDev)
        {
            continue;
        }
        else
        {
            slaveAddr = ((i3c_device_t *)listItem)->info.dynamicAddr;
            break;
        }
    }
    I3C_BusMasterGetDeviceInfo(&slaveDev, slaveAddr, &devInfo);

    while (1)
    {
    }
}
