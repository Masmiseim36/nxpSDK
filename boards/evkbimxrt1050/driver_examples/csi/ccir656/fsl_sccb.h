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

#ifndef _FSL_SCCB_H_
#define _FSL_SCCB_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef status_t (*sccb_i2c_send_func_t)(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
typedef status_t (*sccb_i2c_receive_func_t)(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

/*! @brief SCCB register address type. */
typedef enum _sccb_reg_addr
{
    kSCCB_RegAddr8Bit = 1,  /*!< 8-bit register address. */
    kSCCB_RegAddr16Bit = 2, /*!< 16-bit register address. */
} sccb_reg_addr_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Write value to the SCCB register.
 *
 * @param i2cAddr SCCB I2C address.
 * @param addrType SCCB register address type.
 * @param reg The register to write.
 * @param value The value to write.
 * @param i2cSendFunc The actual I2C send function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t SCCB_WriteReg(
    uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t value, sccb_i2c_send_func_t i2cSendFunc);

/*!
 * @brief Write values to multiple SCCB registers.
 *
 * @param i2cAddr SCCB I2C address.
 * @param addrType SCCB register address type.
 * @param startReg The start register to write.
 * @param value Pointer to the value to write.
 * @param len Length of the value to write.
 * @param i2cSendFunc The actual I2C send function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t SCCB_WriteMultiRegs(uint8_t i2cAddr,
                             sccb_reg_addr_t addrType,
                             uint32_t startReg,
                             const uint8_t *value,
                             uint32_t len,
                             sccb_i2c_send_func_t i2cSendFunc);

/*!
 * @brief Read the SCCB register value.
 *
 * @param i2cAddr SCCB I2C address.
 * @param addrType SCCB register address type.
 * @param reg The register to read.
 * @param value The value read out.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t SCCB_ReadReg(
    uint8_t i2cAddr, sccb_reg_addr_t addrType, uint32_t reg, uint8_t *value, sccb_i2c_receive_func_t i2cReceiveFunc);

/*!
 * @brief Modify the SCCB register value.
 *
 * This function modifies some bit fields of a register.
 *
 * @param i2cAddr SCCB I2C address.
 * @param addrType SCCB register address type.
 * @param reg The register to modify.
 * @param clrMask The mask value to clear.
 * @param value The value to set.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @param i2cSendFunc The actual I2C send function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t SCCB_ModifyReg(uint8_t i2cAddr,
                        sccb_reg_addr_t addrType,
                        uint32_t reg,
                        uint8_t clrMask,
                        uint8_t value,
                        sccb_i2c_receive_func_t i2cReceiveFunc,
                        sccb_i2c_send_func_t i2cSendFunc);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_SCCB_H_ */
