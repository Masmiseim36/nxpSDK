/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include "central_hpc.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_adapter_uart.h"
#include "controller.h"
#include "host_msd_fatfs.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

int USB_HostGetConfiguration(usb_host_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->controllerId = kUSB_HostInstanceEhci0;
    return 0;
}

int USB_HostPhyGetConfiguration(usb_host_instance_t instance, usb_host_phy_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->D_CAL     = BOARD_USB_PHY_D_CAL;
    config->TXCAL45DM = BOARD_USB_PHY_TXCAL45DM;
    config->TXCAL45DP = BOARD_USB_PHY_TXCAL45DP;
    return 0;
}

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    SCB_DisableDCache();

    if (xTaskCreate(central_hpc_task, "central_hpc_task", configMINIMAL_STACK_SIZE * 8, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("central hpc task creation failed!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
