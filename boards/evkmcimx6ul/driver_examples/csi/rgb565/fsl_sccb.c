/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
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

#include "fsl_common.h"
#include "fsl_sccb.h"
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
#include "fsl_lpi2c.h"
#else
#include "fsl_i2c.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t SCCB_WriteReg(sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t value)
{
    uint8_t data[3];
    uint8_t size = 0;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        data[size++] = (reg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)(reg);
    data[size++] = value;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    status_t status;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    return LPI2C_MasterStop(i2c);
#else
    while (1)
    {
        I2C_MasterStart(i2c, i2cAddr, kI2C_Write);
        while (!((I2C_MasterGetStatusFlags(i2c) & kI2C_IntPendingFlag)))
        {
        }

        I2C_MasterClearStatusFlags(i2c, kI2C_IntPendingFlag);

        if (I2C_MasterGetStatusFlags(i2c) & kI2C_ReceiveNakFlag)
        {
            I2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    return I2C_MasterWriteBlocking(i2c, data, size, kI2C_TransferDefaultFlag);
#endif
}

status_t SCCB_WriteMultiRegs(
    sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t startReg, const uint8_t *value, uint32_t len)
{
    uint8_t data[2];
    uint8_t size = 0;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        data[size++] = (startReg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)(startReg);

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    status_t status;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    LPI2C_MasterSend(i2c, value, len);

    return LPI2C_MasterStop(i2c);
#else
    while (1)
    {
        I2C_MasterStart(i2c, i2cAddr, kI2C_Write);
        while (!((I2C_MasterGetStatusFlags(i2c) & kI2C_IntPendingFlag)))
        {
        }

        I2C_MasterClearStatusFlags(i2c, kI2C_IntPendingFlag);

        if (I2C_MasterGetStatusFlags(i2c) & kI2C_ReceiveNakFlag)
        {
            I2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    I2C_MasterWriteBlocking(i2c, data, size, kI2C_TransferNoStopFlag);

    return I2C_MasterWriteBlocking(i2c, value, len, kI2C_TransferDefaultFlag);
#endif
}

status_t SCCB_ReadReg(sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t *value)
{
    status_t status;
    uint8_t data[2];
    uint8_t size = 0;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        data[size++] = (reg >> 8U) & 0xFFU;
    }

    data[size++] = (uint8_t)reg;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    status_t status;

    while (1)
    {
        status = LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Write);

        if (kStatus_Success != status)
        {
            LPI2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    LPI2C_MasterSend(i2c, data, size);

    LPI2C_MasterStop(i2c);

    LPI2C_MasterStart(i2c, i2cAddr, kLPI2C_Read);

    LPI2C_MasterReceive(i2c, value, 1);

    return LPI2C_MasterStop(i2c);

#else
    while (1)
    {
        status = I2C_MasterStart(i2c, i2cAddr, kI2C_Write);
        while (!((I2C_MasterGetStatusFlags(i2c) & kI2C_IntPendingFlag)))
        {
        }

        I2C_MasterClearStatusFlags(i2c, kI2C_IntPendingFlag);

        if (I2C_MasterGetStatusFlags(i2c) & kI2C_ReceiveNakFlag)
        {
            I2C_MasterStop(i2c);
        }
        else
        {
            break;
        }
    }

    status = I2C_MasterWriteBlocking(i2c, data, size, kI2C_TransferDefaultFlag);

    if (kStatus_Success != status)
    {
        return status;
    }

    I2C_MasterStart(i2c, i2cAddr, kI2C_Read);

    /* Wait until data transfer complete. */
    while (!((I2C_MasterGetStatusFlags(i2c) & kI2C_IntPendingFlag)))
    {
    }

    I2C_MasterClearStatusFlags(i2c, kI2C_IntPendingFlag);

    return I2C_MasterReadBlocking(i2c, value, 1, kI2C_TransferDefaultFlag);
#endif
}

status_t SCCB_ModifyReg(
    sccb_i2c_t i2c, uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t clrMask, uint8_t value)
{
    status_t status;

    uint8_t regVal;

    status = SCCB_ReadReg(i2c, i2cAddr, addrType, reg, &regVal);

    if (kStatus_Success != status)
    {
        return status;
    }

    regVal = (regVal & ~(clrMask)) | value;

    return SCCB_WriteReg(i2c, i2cAddr, addrType, reg, regVal);
}
