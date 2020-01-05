/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file sensor_io_spi.c
 * @brief The sensor_io_spi.c file contains definitions for low-level interface functions
 * for reading and writing data from/to sensor using CMSIS APIs.
 */

#include "Driver_SPI.h"
#include "sensor_drv.h"
#include "systick_utils.h"
#include "sensor_io_spi.h"

/*******************************************************************************
 * Functions
 ******************************************************************************/
/*! The interface function to write register data to a sensor. */
int32_t Sensor_SPI_BlockWrite(ARM_DRIVER_SPI *pCommDrv,
                              registerDeviceInfo_t *devInfo,
                              void *pWriteParams,
                              const registercommandlist_t *pCommandList,
                              uint8_t error_mask)
{
    int32_t status;
    bool commandComplete;
    uint8_t cocoBuffer[2] = {0};

    /*! Validate for the correct handle.*/
    if ((pCommDrv == NULL) || (pCommandList == NULL))
    {
        return SENSOR_ERROR_BAD_ADDRESS;
    }

    const registercommandlist_t *pCmd = pCommandList;
    /*! Update register values based on register write list unless the next Cmd is the list terminator */
    do
    { /*! Write the command based on the values in the command and value pair.*/
        status =
            Register_SPI_BlockWrite(pCommDrv, devInfo, pWriteParams, pCmd->writeTo, pCmd->pWriteBuffer, pCmd->numBytes);
        if (ARM_DRIVER_OK != status)
        {
            return SENSOR_ERROR_WRITE;
        }

        do
        { /*! Wait for Command Completion. */
            BOARD_DELAY_ms(1);
            status = Register_SPI_Read(pCommDrv, devInfo, pWriteParams, 0, sizeof(cocoBuffer), cocoBuffer);
            if (ARM_DRIVER_OK != status)
            {
                return SENSOR_ERROR_READ;
            }
            commandComplete = cocoBuffer[1] & 0x80;
            if (commandComplete && cocoBuffer[1] & error_mask)
            {
                return SENSOR_ERROR_WRITE;
            }
        } while (!commandComplete);
    } while ((++pCmd)->writeTo != 0xFFFF);

    return SENSOR_ERROR_NONE;
}

/*! The interface function to write register data to a sensor. */
int32_t Sensor_SPI_Write(ARM_DRIVER_SPI *pCommDrv,
                         registerDeviceInfo_t *devInfo,
                         void *pWriteParams,
                         const registerwritelist_t *pRegWriteList)
{
    int32_t status;
    bool endDataMarkAwaited;

    /*! Validate for the correct handle.*/
    if ((pCommDrv == NULL) || (pRegWriteList == NULL))
    {
        return SENSOR_ERROR_BAD_ADDRESS;
    }

    const registerwritelist_t *pCmd = pRegWriteList;

    /*! Update register values based on register write list unless the next Cmd is the list terminator */
    do
    {
        endDataMarkAwaited = (pCmd + 1)->writeTo != 0xFFFF;
        /*! Set the register based on the values in the register value pair.*/
        status = Register_SPI_Write(pCommDrv, devInfo, pWriteParams, pCmd->writeTo, pCmd->value, pCmd->mask);
        if (ARM_DRIVER_OK != status)
        {
            return SENSOR_ERROR_WRITE;
        }
        ++pCmd;
    } while (endDataMarkAwaited);

    return SENSOR_ERROR_NONE;
}

/*! The interface function to read register data from a sensor. */
int32_t Sensor_SPI_Read(ARM_DRIVER_SPI *pCommDrv,
                        registerDeviceInfo_t *devInfo,
                        void *pReadParams,
                        const registerreadlist_t *pReadList,
                        uint8_t *pOutBuffer)
{
    int32_t status;
    uint8_t *pBuf;

    /*! Validate for the correct handle.*/
    if (pCommDrv == NULL || pReadList == NULL || pOutBuffer == NULL)
    {
        return SENSOR_ERROR_BAD_ADDRESS;
    }

    const registerreadlist_t *pCmd = pReadList;

    /*! Traverse the read list and read the registers one by one unless the register read list numBytes is zero*/
    for (pBuf = pOutBuffer; pCmd->numBytes != 0; pCmd++)
    {
        status = Register_SPI_Read(pCommDrv, devInfo, pReadParams, pCmd->readFrom, pCmd->numBytes, pBuf);
        if (ARM_DRIVER_OK != status)
        {
            return SENSOR_ERROR_READ;
        }
        pBuf += pCmd->numBytes;
    }

    return SENSOR_ERROR_NONE;
}
