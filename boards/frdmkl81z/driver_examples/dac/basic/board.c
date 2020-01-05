/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_i2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#include "fsl_debug_console.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    /* SIM_SOPT2[27:26]:
     *  00: Clock Disabled
     *  01: MCGFLLCLK, or MCGPLLCLK, or IRC48M
     *  10: OSCERCLK
     *  11: MCGIRCCLK
     */
    CLOCK_SetLpuartClock(2);

    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_I2C_Init(I2C_Type *base, uint32_t clkSrc_Hz)
{
    i2c_master_config_t i2cConfig = {0};

    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(base, &i2cConfig, clkSrc_Hz);
}

status_t BOARD_I2C_Send(I2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data = txBuff;
    masterXfer.dataSize = txBuffSize;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(base, &masterXfer);
}

status_t BOARD_I2C_Receive(I2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress = deviceAddress;
    masterXfer.subaddress = subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data = rxBuff;
    masterXfer.dataSize = rxBuffSize;
    masterXfer.direction = kI2C_Read;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(base, &masterXfer);
}

status_t BOARD_I2C_SendSCCB(I2C_Type *base,
                            uint8_t deviceAddress,
                            uint32_t subAddress,
                            uint8_t subaddressSize,
                            uint8_t *txBuff,
                            uint8_t txBuffSize)
{
    return BOARD_I2C_Send(base, deviceAddress, subAddress, subaddressSize, txBuff, txBuffSize);
}

/*
 * For SCCB receive, the workflow is Start -> Write -> Stop -> Start -> Read -> Stop,
 * Repeat start is not supported.
 */
status_t BOARD_I2C_ReceiveSCCB(I2C_Type *base,
                               uint8_t deviceAddress,
                               uint32_t subAddress,
                               uint8_t subaddressSize,
                               uint8_t *rxBuff,
                               uint8_t rxBuffSize)
{
    status_t status = kStatus_Success;
    while (1)
    {
        I2C_MasterStart(base, deviceAddress, kI2C_Write);
        while (!((I2C_MasterGetStatusFlags(base) & kI2C_IntPendingFlag)))
        {
        }

        I2C_MasterClearStatusFlags(base, kI2C_IntPendingFlag);

        if (I2C_MasterGetStatusFlags(base) & kI2C_ReceiveNakFlag)
        {
            I2C_MasterStop(base);
        }
        else
        {
            break;
        }
    }

    status = I2C_MasterWriteBlocking(base, (const uint8_t *)&subAddress, subaddressSize, kI2C_TransferDefaultFlag);

    if (kStatus_Success != status)
    {
        return status;
    }

    I2C_MasterStart(base, deviceAddress, kI2C_Read);

    /* Wait until data transfer complete. */
    while (!((I2C_MasterGetStatusFlags(base) & kI2C_IntPendingFlag)))
    {
    }

    I2C_MasterClearStatusFlags(base, kI2C_IntPendingFlag);

    return I2C_MasterReadBlocking(base, rxBuff, rxBuffSize, kI2C_TransferDefaultFlag);
}

void BOARD_Accel_I2C_Init(void)
{
    BOARD_I2C_Init(BOARD_ACCEL_I2C_BASEADDR, BOARD_ACCEL_I2C_CLOCK_FREQ);
}

status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff)
{
    uint8_t data = (uint8_t)txBuff;

    return BOARD_I2C_Send(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1);
}

status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_I2C_Receive(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}

void BOARD_Camera_I2C_Init(void)
{
    BOARD_I2C_Init(BOARD_CAMERA_I2C_BASEADDR, BOARD_CAMERA_I2C_CLOCK_FREQ);
}

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_I2C_SendSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, (uint8_t *)txBuff,
                              txBuffSize);
}

status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_I2C_ReceiveSCCB(BOARD_CAMERA_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff,
                                 rxBuffSize);
}
#endif /* SDK_I2C_BASED_COMPONENT_USED */
