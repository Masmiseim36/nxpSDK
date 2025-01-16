/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_lpspi.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void LPSPI_Init(void)
{
    lpspi_master_config_t masterConfig;

    /* Master config */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate                      = 500000U;
    masterConfig.bitsPerFrame                  = 8;
    masterConfig.cpol                          = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha                          = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction                     = kLPSPI_MsbFirst;
    masterConfig.whichPcs                      = BOARD_LPSPI_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow            = kLPSPI_PcsActiveLow;
    masterConfig.pinCfg                        = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig                 = kLpspiDataOutTristate;
    masterConfig.pcsToSckDelayInNanoSec        = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.enableInputDelay              = false;
    
    LPSPI_MasterInit(BOARD_EEPROM_LPSPI_BASEADDR, &masterConfig, BOARD_LPSPI_CLK_FREQ);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t i;
    lpspi_transfer_t masterXfer;
    uint8_t masterRxData[LPSPI_TRANSFER_SIZE] = {0};
    uint8_t masterTxData[LPSPI_TRANSFER_SIZE] = {0};

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\n***LPSPI Loopback Demo***\r\n");
    LPSPI_Init();

    for (i = 0; i < LPSPI_TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i;
    }
    /*Start master transfer*/
    masterXfer.txData      = masterTxData;
    masterXfer.rxData      = masterRxData;
    masterXfer.dataSize    = LPSPI_TRANSFER_SIZE;
    masterXfer.configFlags = BOARD_LPSPI_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous;
    LPSPI_MasterTransferBlocking(BOARD_EEPROM_LPSPI_BASEADDR, &masterXfer);

    /* Compare Tx and Rx data. */
    for (i = 0; i < LPSPI_TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != masterRxData[i])
        {
            break;
        }
    }

    if (LPSPI_TRANSFER_SIZE == i)
    {
        PRINTF("LPSPI loopback test pass!!!");
    }
    else
    {
        PRINTF("LPSPI loopback test fail!!!");
    }

    while (1)
    {
        __NOP();
    }
}
