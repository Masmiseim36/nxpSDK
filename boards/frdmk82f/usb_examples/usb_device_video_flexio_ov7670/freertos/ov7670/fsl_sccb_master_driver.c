/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#include "fsl_sccb_master_driver.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

ov7670_status_t I2C_Read_OV7670_Reg(
    I2C_Type *base, uint8_t device_addr, uint8_t regAddr, uint8_t *rxBuff, uint32_t rxSize)
{
    status_t status;

    while (1)
    {
        I2C_MasterStart(base, device_addr, kI2C_Write);
        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)))
        {
        }

        if (I2C_MasterGetStatusFlags(base) & kI2C_ReceiveNakFlag)
        {
            I2C_MasterStop(base);
        }
        else
        {
            break;
        }
    }

    status = I2C_MasterWriteWithStop(base, &regAddr, 1);

    if (status != kStatus_Success)
    {
        return kStatus_OV7670_I2CFail;
    }
    I2C_MasterStart(base, device_addr, kI2C_Read);

    /* Wait until data transfer complete. */
    while ((!(base->S & kI2C_IntPendingFlag)))
    {
    }

    status = I2C_MasterReadBlocking(base, rxBuff, rxSize);

    if (status == kStatus_Success)
    {
        return kStatus_OV7670_Success;
    }
    else
    {
        return kStatus_OV7670_I2CFail;
    }
}

ov7670_status_t I2C_Write_OV7670_Reg(I2C_Type *base, uint8_t device_addr, uint8_t regAddr, uint8_t val)
{
    status_t status;

    while (1)
    {
        I2C_MasterStart(base, device_addr, kI2C_Write);
        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)))
        {
        }

        if (I2C_MasterGetStatusFlags(base) & kI2C_ReceiveNakFlag)
        {
            I2C_MasterStop(base);
        }
        else
        {
            break;
        }
    }

    status = I2C_MasterWriteBlocking(base, &regAddr, 1);
    if (status != kStatus_Success)
    {
        I2C_MasterStop(base);
        return kStatus_OV7670_I2CFail;
    }
    status = I2C_MasterWriteBlocking(base, &val, 1);
    I2C_MasterStop(base);

    if (status == kStatus_Success)
    {
        return kStatus_OV7670_Success;
    }
    else
    {
        return kStatus_OV7670_I2CFail;
    }
}

status_t I2C_MasterWriteWithStop(I2C_Type *base, const uint8_t *txBuff, size_t txSize)
{
    status_t result = kStatus_Success;
    uint8_t statusFlags = 0;
    uint16_t timeout = UINT16_MAX;

    /* Wait until the data register is ready for transmit. */
    while ((!(base->S & kI2C_TransferCompleteFlag)) && (timeout--))
    {
    }

    /* Failed to complete the transfer. */
    if (timeout == 0)
    {
        return kStatus_I2C_Timeout;
    }

    /* Clear the IICIF flag. */
    base->S = kI2C_IntPendingFlag;

    /* Setup the I2C peripheral to transmit data. */
    base->C1 |= I2C_C1_TX_MASK;

    while (txSize--)
    {
        /* Send a byte of data. */
        base->D = *txBuff++;

        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)) && (timeout--))
        {
        }

        /* Failed to complete the transfer. */
        if (timeout == 0)
        {
            result = kStatus_I2C_Timeout;
        }

        statusFlags = base->S;

        /* Clear the IICIF flag. */
        base->S = kI2C_IntPendingFlag;

        /* Check if arbitration lost or no acknowledgement (NAK), return failure status. */
        if (statusFlags & kI2C_ArbitrationLostFlag)
        {
            base->S = kI2C_ArbitrationLostFlag;
            result = kStatus_I2C_ArbitrationLost;
        }

        if (statusFlags & kI2C_ReceiveNakFlag)
        {
            base->S = kI2C_ReceiveNakFlag;
            result = kStatus_I2C_Nak;
        }
        /* Issue the STOP command on the bus. */
        base->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
        /* Wait until data transfer complete. */
        while ((base->S & kI2C_BusBusyFlag) && (timeout--))
        {
        }

        if (timeout == 0)
        {
            result = kStatus_I2C_Timeout;
        }

        if (result != kStatus_Success)
        {
            /* Breaking out of the send loop. */
            break;
        }
    }

    return result;
}
