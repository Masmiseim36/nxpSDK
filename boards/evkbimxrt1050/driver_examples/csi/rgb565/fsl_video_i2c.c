/*
 * The Clear BSD License
 * Copyright (c) 2017-2018, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
#include "fsl_video_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t VIDEO_I2C_WriteReg(uint8_t i2cAddr,
                            video_reg_addr_t addrType,
                            uint32_t reg,
                            video_reg_width_t regWidth,
                            uint32_t value,
                            video_i2c_send_func_t i2cSendFunc)
{
    uint8_t data[4];
    uint8_t i;
    uint32_t regTmp;

    if (kVIDEO_RegAddr16Bit == addrType)
    {
        /* Byte swap. */
        regTmp = ((reg >> 8U) & 0xFF) | ((reg & 0xFFU) << 8U);
        reg = regTmp;
    }

    i = regWidth;
    while (i--)
    {
        data[i] = (uint8_t)value;
        value >>= 8;
    }

    return i2cSendFunc(i2cAddr, reg, addrType, data, regWidth);
}

/*!
 * @brief Read the register value.
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to read.
 * @param regWidth The width of the register.
 * @param value The value read out.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t VIDEO_I2C_ReadReg(uint8_t i2cAddr,
                           video_reg_addr_t addrType,
                           uint32_t reg,
                           video_reg_width_t regWidth,
                           void *value,
                           video_i2c_receive_func_t i2cReceiveFunc)
{
    uint8_t data[4];
    uint8_t i = 0;
    uint32_t regTmp;
    status_t status;

    if (kVIDEO_RegAddr16Bit == addrType)
    {
        /* Byte swap. */
        regTmp = ((reg >> 8U) & 0xFF) | ((reg & 0xFFU) << 8U);
        reg = regTmp;
    }

    status = i2cReceiveFunc(i2cAddr, reg, addrType, data, regWidth);

    if (kStatus_Success == status)
    {
        while (regWidth--)
        {
            ((uint8_t *)value)[i++] = data[regWidth];
        }
    }

    return status;
}

status_t VIDEO_I2C_ModifyReg(uint8_t i2cAddr,
                             video_reg_addr_t addrType,
                             uint32_t reg,
                             video_reg_width_t regWidth,
                             uint32_t clrMask,
                             uint32_t value,
                             video_i2c_receive_func_t i2cReceiveFunc,
                             video_i2c_send_func_t i2cSendFunc)
{
    status_t status;
    uint32_t regVal;

    status = VIDEO_I2C_ReadReg(i2cAddr, addrType, reg, regWidth, &regVal, i2cReceiveFunc);

    if (kStatus_Success != status)
    {
        return status;
    }

    regVal = (regVal & ~(clrMask)) | (value & clrMask);

    return VIDEO_I2C_WriteReg(i2cAddr, addrType, reg, regWidth, regVal, i2cSendFunc);
}
