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
#include "fsl_sccb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t SCCB_WriteReg(
    uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t value, sccb_i2c_send_func_t i2cSendFunc)
{
    uint32_t regTmp;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        /* Byte swap. */
        regTmp = ((reg >> 8U) & 0xFF) | ((reg & 0xFFU) << 8U);
        reg = regTmp;
    }

    return i2cSendFunc(i2cAddr, reg, addrType, &value, 1);
}

status_t SCCB_WriteMultiRegs(uint8_t i2cAddr,
                             sccb_reg_addr_t addrType,
                             uint32_t startReg,
                             const uint8_t *value,
                             uint32_t len,
                             sccb_i2c_send_func_t i2cSendFunc)
{
    uint32_t regTmp;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        /* Byte swap. */
        regTmp = ((startReg >> 8U) & 0xFF) | ((startReg & 0xFFU) << 8U);
        startReg = regTmp;
    }

    return i2cSendFunc(i2cAddr, startReg, addrType, value, len);
}

status_t SCCB_ReadReg(
    uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t *value, sccb_i2c_receive_func_t i2cReceiveFunc)
{
    uint32_t regTmp;

    if (kSCCB_RegAddr16Bit == addrType)
    {
        /* Byte swap. */
        regTmp = ((reg >> 8U) & 0xFF) | ((reg & 0xFFU) << 8U);
        reg = regTmp;
    }

    return i2cReceiveFunc(i2cAddr, reg, addrType, value, 1);
}

status_t SCCB_ModifyReg(uint8_t i2cAddr,
                        sccb_reg_addr_t addrType,
                        uint32_t reg,
                        uint8_t clrMask,
                        uint8_t value,
                        sccb_i2c_receive_func_t i2cReceiveFunc,
                        sccb_i2c_send_func_t i2cSendFunc)
{
    status_t status;
    uint8_t regVal;

    status = SCCB_ReadReg(i2cAddr, addrType, reg, &regVal, i2cReceiveFunc);

    if (kStatus_Success != status)
    {
        return status;
    }

    regVal = (regVal & ~((uint32_t)clrMask)) | (value & clrMask);

    return SCCB_WriteReg(i2cAddr, addrType, reg, regVal, i2cSendFunc);
}
