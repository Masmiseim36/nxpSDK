/*! *********************************************************************************
/*
 * \file controller_feature.c
 * Description : This file includes API implementation for feature specific to Deep-sleep and and IR
 * for BT controller modules.
 *
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#include <stdio.h>
#include "BT_common.h"
#include "fsl_common.h"
#include "controller_features.h"
#ifdef PCAL6408A_IO_EXP_ENABLE
#include "fsl_adapter_i2c.h"
#endif /*#PCAL6408A_IO_EXP_ENABLE*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PCAL6408A_IO_EXP_ENABLE
/*PCAL6408A I2C Slave Address, 7-bit */
#define PCAL6408A_I2C_ADDR 				(0x20U)
/*PCAL6408A I2C baudrate*/
#define PCAL6408A_I2C_BAUDRATE 			(400000U)
//#define PCAL6408A_I2C_FAST_BAUDRATE 	(400000U)
#define PCAL6408A_I2C_INSTANCE 			(5U)
//#define PCAL6408AC_I2C_CLOCK_FREQ  		(24000000U)
#define PCAL6408AC_I2C_CLOCK_FREQ 		(CLOCK_GetFreq(kCLOCK_OscRc48MDiv2))

#define PCAL6408A_CHECK_RET(x, status)  \
    (status) = (x);                  \
    if ((status) != kStatus_Success) \
    {                                \
        return (status);             \
    }
#endif /*#PCAL6408A_IO_EXP_ENABLE*/
/*******************************************************************************
 * Variables
 ******************************************************************************/
#ifdef PCAL6408A_IO_EXP_ENABLE
static hal_i2c_master_handle_t handle;
#endif /*PCAL6408A_IO_EXP_ENABLE*/
/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
#ifdef PCAL6408A_IO_EXP_ENABLE
static status_t IOEXP_I2C_Init(void *handle, uint32_t i2cInstance, uint32_t i2cBaudrate, uint32_t i2cSourceClockHz);
static status_t IOEXP_I2C_Deinit(void *handle);
static status_t IOEXP_I2C_Send(void *handle,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize);
static status_t IOEXP_I2C_Receive(void *handle,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize);

static status_t PCAL6408A_WriteReg(void *handle, uint8_t reg, uint8_t val);
static status_t PCAL6408A_ReadReg(void *handle, uint8_t reg, uint8_t *val);
static status_t PCAL6408A_readAllConfig(pcal6408a_pins_cfg_t *config);
static status_t PCAL6408A_ModifyReg(void *handle, uint8_t reg, uint8_t mask, uint8_t val);
#endif /*PCAL6408A_IO_EXP_ENABLE*/
/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef PCAL6408A_IO_EXP_ENABLE
/*!
 * brief IOEXP i2c bus initilization.
 *
 * param handle i2c master handle.
 * param i2CInstance instance number of the i2c bus, such as 0 is corresponding to I2C0.
 * param i2cBaudrate i2c baudrate.
 * param i2cSourceClockHz i2c source clock frequency.
 * return kStatus_HAL_I2cSuccess is success, else initial failed.
 */
static status_t IOEXP_I2C_Init(void *handle, uint32_t i2cInstance, uint32_t i2cBaudrate, uint32_t i2cSourceClockHz)
{
    hal_i2c_master_config_t masterConfig;

    masterConfig.enableMaster = true;
    masterConfig.baudRate_Bps = i2cBaudrate;
    masterConfig.srcClock_Hz  = i2cSourceClockHz;
    masterConfig.instance     = (uint8_t)i2cInstance;

    return (status_t)HAL_I2cMasterInit((hal_i2c_master_handle_t *)handle, &masterConfig);
}

/*!
 * brief IOEXP i2c de-initilization.
 *
 * param handle i2c master handle.
 * return kStatus_HAL_I2cSuccess is success, else deinitial failed.
 */
static status_t IOEXP_I2C_Deinit(void *handle)
{
    return (status_t)HAL_I2cMasterDeinit((hal_i2c_master_handle_t *)handle);
}

/*!
 * brief IOEXP i2c send function.
 *
 * param handle i2c master handle.
 * param deviceAddress IOEXP device address.
 * param subAddress register address.
 * param subaddressSize register address width.
 * param txBuff tx buffer pointer.
 * param txBuffSize tx buffer size.
 * return kStatus_HAL_I2cSuccess is success, else send failed.
 */
static status_t IOEXP_I2C_Send(void *handle,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize)
{
    hal_i2c_master_transfer_t masterXfer;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kHAL_I2cWrite;
    masterXfer.subaddress     = (uint32_t)subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data           = txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = (uint32_t)kHAL_I2cTransferDefaultFlag;

    return (status_t)HAL_I2cMasterTransferBlocking((hal_i2c_master_handle_t *)handle, &masterXfer);
}

/*!
 * brief IOEXP i2c receive function.
 *
 * param handle i2c master handle.
 * param deviceAddress IOEXP device address.
 * param subAddress register address.
 * param subaddressSize register address width.
 * param rxBuff rx buffer pointer.
 * param rxBuffSize rx buffer size.
 * return kStatus_HAL_I2cSuccess is success, else receive failed.
 */
static status_t IOEXP_I2C_Receive(void *handle,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize)
{
    hal_i2c_master_transfer_t masterXfer;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kHAL_I2cRead;
    masterXfer.subaddress     = (uint32_t)subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = (size_t)rxBuffSize;
    masterXfer.flags          = (uint32_t)kHAL_I2cTransferDefaultFlag;

    return (status_t)HAL_I2cMasterTransferBlocking((hal_i2c_master_handle_t *)handle, &masterXfer);
}


status_t PCAL6408A_Init(pcal6408a_pins_cfg_t *config)
{
    status_t ret = kStatus_Success;

    printf ("calling PCAL6408A_Init ()\n");
    if (IOEXP_I2C_Init(&handle, PCAL6408A_I2C_INSTANCE, PCAL6408A_I2C_BAUDRATE,PCAL6408AC_I2C_CLOCK_FREQ) != (status_t)kStatus_HAL_I2cSuccess)
    {
        return kStatus_Fail;
    }

    //PCAL6408A_CHECK_RET(PCAL6408A_WriteReg(&handle, PCAL6408A_REG_PULL_UP_DOWN_ENABLE, 0x08), ret);
    //PCAL6408A_CHECK_RET(PCAL6408A_WriteReg(&handle, PCAL6408A_REG_PULL_UP_DOWN_SELECT, 0x00), ret);
    PCAL6408A_CHECK_RET(PCAL6408A_WriteReg(&handle, PCAL6408A_REG_OUTPUT_PORT_CONFIGURATION, 0x08), ret);
    /*making p0, p3 as output*/
    PCAL6408A_CHECK_RET(PCAL6408A_WriteReg(&handle, PCAL6408A_REG_CONFIGURATION, 0xF6), ret);
    PCAL6408A_CHECK_RET(PCAL6408A_WriteReg(&handle, PCAL6408A_REG_OUTPUT_PORT, 0x00), ret);
    ret = PCAL6408A_readAllConfig (config);

    return ret;
}

static status_t PCAL6408A_WriteReg(void *handle, uint8_t reg, uint8_t val)
{
    return IOEXP_I2C_Send(handle, PCAL6408A_I2C_ADDR, reg, 1U, (uint8_t *)&val, 1U);
}

static status_t PCAL6408A_ReadReg(void *handle, uint8_t reg, uint8_t *val)
{
    return IOEXP_I2C_Receive(handle, PCAL6408A_I2C_ADDR, reg, 1U, val, 1U);
}

static status_t PCAL6408A_ModifyReg(void *handle, uint8_t reg, uint8_t mask, uint8_t val)
{
    uint8_t reg_val = 0;
    status_t retval = PCAL6408A_ReadReg(handle, reg, &reg_val);

    if (retval != kStatus_Success)
    {
        return kStatus_Fail;
    }
    reg_val &= ~mask;
    reg_val |= val;
    printf ("writing %02X on IO Port\n",reg_val);
    retval = PCAL6408A_WriteReg(handle, reg, reg_val);
    if (retval != kStatus_Success)
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}

static status_t PCAL6408A_readAllConfig(pcal6408a_pins_cfg_t *config)
{
	status_t ret = kStatus_Success;

    memset (config, 0, sizeof (*config));
    uint8_t value = 0;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_CONFIGURATION, &value), ret);
    config->configured = value;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_OUTPUT_PORT, &value), ret);
    config->output_port_value = value;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_INPUT_PORT, &value), ret);
    config->input_port_value = value;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_OUTPUT_PORT_CONFIGURATION, &value), ret);
    config->output_port_config = value;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_PULL_UP_DOWN_ENABLE, &value), ret);
    config->pulls_enabled = value;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_POLARITY_INVERSION, &value), ret);
    config->polarity = value;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_OUTPUT_DRIVE_STRENGTH_0, &value), ret);
    config->ouput_drive_strength1 = value;
    PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_OUTPUT_DRIVE_STRENGTH_1, &value), ret);
    config->ouput_drive_strength2 = value;

    return ret;
}

status_t PCAL6408A_control_op_port(pcal6408a_op_port_e port_num, uint8_t signal)
{
	status_t ret = kStatus_Success;
	PCAL6408A_CHECK_RET(PCAL6408A_ModifyReg(&handle, PCAL6408A_REG_OUTPUT_PORT, (1U << port_num), (signal << port_num)), ret);
	return ret;
}

status_t PCAL6408A_read_op_port(pcal6408a_op_port_e port_num, uint8_t* value)
{
	status_t ret = kStatus_Success;
    uint8_t reg_val = 0;

	PCAL6408A_CHECK_RET(PCAL6408A_ReadReg(&handle, PCAL6408A_REG_OUTPUT_PORT, &reg_val), ret);
	*value = (reg_val >> port_num) & 1U;

	return ret;
}
#endif /*PCAL6408A_IO_EXP_ENABLE*/