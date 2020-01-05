/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
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
 * @file sensor_io_i2c.h
 * @brief The sensor_io_i2c.h file declares low-level interface functions for reading
 *  and writing sensor registers.
 */

#ifndef __SENSOR_IO_I2C_H
#define __SENSOR_IO_I2C_H

#include <stdint.h>
#include <stdbool.h>
#include "Driver_I2C.h"
#include "register_io_i2c.h"

/*******************************************************************************
 * API
 ******************************************************************************/
/*! @brief       Write commands to a sensor

 *  @param[in]   pCommDrv      pointer to the I2C ARM driver to use
 *  @param[in]   devInfo       The I2C device number and idle function.
 *  @param[in]   slaveAddress  the I2C slave address to write to
 *  @param[in]   pCommandList  a list of one or more commands to write
 *  @param[in]   error_mask    The mask for Error Code Bits in CoCo Byte
 *
 *  @return      returns the execution status of the operation using ::ESensorErrors
 */
int32_t Sensor_I2C_BlockWrite(ARM_DRIVER_I2C *pCommDrv,
                              registerDeviceInfo_t *devInfo,
                              uint16_t slaveAddress,
                              const registercommandlist_t *pCommandList,
                              uint8_t error_mask);

/*! @brief       Write register data to a sensor

 *  @param[in]   pCommDrv      pointer to the I2C ARM driver to use
 *  @param[in]   devInfo       The I2C device number and idle function.
 *  @param[in]   slaveAddress  the I2C slave address to write to
 *  @param[in]   pRegWriteList a list of one or more register/value pairs to write
 *
 *  @return      returns the execution status of the operation using ::ESensorErrors
 */
int32_t Sensor_I2C_Write(ARM_DRIVER_I2C *pCommDrv,
                         registerDeviceInfo_t *devInfo,
                         uint16_t slaveAddress,
                         const registerwritelist_t *pRegWriteList);

/*! @brief       Read register data from a sensor

 *  @param[in]   pCommDrv      pointer to the I2C ARM driver to use
 *  @param[in]   devInfo       The I2C device number and idle function.
 *  @param[in]   slaveAddress  the I2C slave address to read from
 *  @param[in]   pReadList     a list of one or more register addresses and lengths to read
 *  @param[in]   pOutBuffer    a pointer of sufficient size to contain the requested read data
 *
 *
 *  @return      returns the execution status of the operation using ::ESensorErrors
 */
int32_t Sensor_I2C_Read(ARM_DRIVER_I2C *pCommDrv,
                        registerDeviceInfo_t *devInfo,
                        uint16_t slaveAddress,
                        const registerreadlist_t *pReadList,
                        uint8_t *pOutBuffer);

#endif /* __SENSOR_IO_I2C_H */
