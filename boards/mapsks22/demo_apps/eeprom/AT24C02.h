/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
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

#ifndef _AT24C02_H_
#define _AT24C02_H_

#include "fsl_common.h"
#include "fsl_lpi2c.h"

/*!
 * @addtogroup AT24C02
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AT24C02_I2C_BASE LPI2C0
#define AT24C02_I2C_ADDR 0x50
#define AT24C02_I2C_BAUDRATE 100000U

/*! @brief WM8960 return status. */
typedef enum _at24c02_status
{
    kStatus_AT24C02_Success = 0x0,
    kStatus_AT24C02_I2CFail = 0x1,
    kStatus_AT24C02_Fail = 0x2
} at24c02_status_t;

/*! @brief AT24C02 configure definition. */
typedef struct at24c02_handler
{
    /* I2C relevant definition. */
    LPI2C_Type *i2cBase;    /*!< I2C base pointer. */
    uint8_t device_addr;    /*!< I2C device address. */
    uint32_t baudRate_kbps; /*!< I2C baud rate. */
} at24c02_handler_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the I2C module in AT24C02.
 *
 * EEPROM AT24C02 uses i2c to write/read the registers in it.
 * @param handler AT24C02 handler structure.
 */
at24c02_status_t AT24C02_I2CInit(at24c02_handler_t *handler);

/*!
 * @brief Byte Write to AT24C02.
 *
 * @param handler AT24C02 handler structure.
 * @param address data word address
 * @param data    write data
 */
at24c02_status_t AT24C02_ByteWrite(at24c02_handler_t *handler, uint8_t address, uint8_t data);

/*!
 * @brief ACK polling for AT24C02.
 *
 * @param handler AT24C02 handler structure.
 */
at24c02_status_t AT24C02_ACKPoll(at24c02_handler_t *handler);

/*!
 * @brief Byte Read from AT24C02.
 *
 * @param handler AT24C02 handler structure.
 * @param address data word address
 * @param data    write data
 */
at24c02_status_t AT24C02_ByteRead(at24c02_handler_t *handler, uint8_t address, uint8_t *data);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _AT24C02_H_ */
